import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Read parameters
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        # Check parameters
        if not name or not month or not day:
            return redirect("/")
        
        # Month and day should be integers
        try:
            month = int(month)
            day = int(day)
        except Exception as e:
            return redirect("/")

        if (not(month >=1 and month <= 12)) or (not(day >= 1 and day<= 31)):
            return redirect("/")

        # Add the user's entry into the database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)

        return redirect("/")

    else:
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays)


