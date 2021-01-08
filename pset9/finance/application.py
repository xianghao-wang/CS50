import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, record

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute("SELECT * FROM stocks WHERE user_id=?", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
    total = cash

    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        stock["name"] = quote["name"]
        total += stock["price"] * stock["shares"]

    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Check parameters
        if not request.form.get("symbol") or not request.form.get("shares"):
            # Missing parameters
            return apology("Missing symbol or shares!")
        
        quote = lookup(request.form.get("symbol"))
        if not quote:
            # Symbol does not exist
            return apology("Symbol does not exist!")

        # Check shares
        try:
            shares = int(request.form.get("shares"))
        except Exception as e:
            return apology("Invalid shares!")

        if shares <= 0:
            return apology("Invalid shares!")

        # Pay
        prices = shares * quote["price"]    
        cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
        if prices > cash:
            flash("You do not have enough cash!")
            return redirect("/buy")
        db.execute("UPDATE users SET cash=? WHERE id=?", cash - prices, session["user_id"])

        # Gain stocks
        stocks = db.execute("SELECT id, shares FROM stocks WHERE user_id=? AND symbol=?", session["user_id"], request.form.get("symbol"))
        print(session["user_id"])
        if len(stocks) == 0:
            db.execute("INSERT INTO stocks (user_id, symbol, shares) VALUES (?, ?, ?)", session["user_id"], request.form.get("symbol"), shares)
        else:
            db.execute("UPDATE stocks SET shares=? WHERE id=?", shares + stocks[0]["shares"], stocks[0]["id"])

        record(db, session["user_id"], request.form.get("symbol"), shares, quote["price"])

        flash("Bought!")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    records = db.execute("SELECT * FROM history WHERE user_id=?", session["user_id"])
    return render_template("history.html", records=records)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        return render_template("quoted.html", quote=lookup(request.form.get("symbol")))
    else:
        """Show quote result"""
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Check validity of username and password
        if not request.form.get("username") or not request.form.get("password") or not request.form.get("password-again"):
            # Missing username or password
            flash("Missing username or password!")
            return redirect("register")

        if db.execute("SELECT COUNT(*) FROM users WHERE username=?", request.form.get("username"))[0]['COUNT(*)'] != 0:
            # User already exists
            flash("Username already exists!")
            return redirect("register")

        # Store user info
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        # Store cookies
        user_id = db.execute("SELECT id FROM users WHERE username=?", request.form.get("username"))[0]["id"]
        session["user_id"] = user_id

        # Redirect to index page
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Check parameters
        if not request.form.get("symbol") or not request.form.get("shares"):
            # Missing parameters
            return apology("Missing symbol or shares!")
        
        stocks = db.execute("SELECT id, shares FROM stocks WHERE user_id=? AND symbol=?", session["user_id"], request.form.get("symbol"))
        if len(stocks) != 1:
            # Symbol does not exist
            return apology("Symbol does not exist!")

        stock = stocks[0]

        # Check shares
        try:
            shares = int(request.form.get("shares"))
        except Exception as e:
            return apology("Invalid shares!")

        if shares <= 0:
            return apology("Invalid shares!")

        if shares > stock["shares"]:
            flash("You do not have enough shares")
            return redirect("/sell")

        # Sell
        if shares == stock["shares"]:
            db.execute("DELETE FROM stocks WHERE id=?", stock["id"])
        else:
            db.execute("UPDATE stocks SET shares=? WHERE id=?", stock["shares"] - shares, stock["id"])

        # Gain cash
        price = lookup(request.form.get("symbol"))["price"]
        cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
        db.execute("UPDATE users SET cash=? WHERE id=?", shares * price  + cash, session["user_id"])

        flash("Sold!")

        record(db, session["user_id"], request.form.get("symbol"), -shares, price)

        return redirect("/")
    else:
        stocks = db.execute("SELECT symbol, shares FROM stocks WHERE user_id=?", session["user_id"])
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)