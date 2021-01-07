from flask import Flask, redirect, render_template, request, session
from flask_session import Session

app = Flask(__name__)
app.config["SESSION_PERMERNANT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

@app.route("/")
def index():
    if not session.get("name"):
        return redirect("/login")
    return render_template("index.html", session=session)

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        # Remember the user is logged in
        name = request.form.get("name")
        session["name"] = name
        # Redirect to /
        return redirect("/")

    if request.method == "GET":
        return render_template("login.html")

@app.route("/logout")
def logout():
    session["name"] = None
    return redirect("/")