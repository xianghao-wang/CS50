import os
import sqlite3
from flask import Flask, redirect, render_template, request
from flask_mail import Mail, Message

app = Flask(__name__)

# Set email sender
app.config["MAIL_DEFAULT_SENDER"] = os.getenv("MAIL_DEFAULT_SENDER")
app.config["MAIL_PASSWORD"] = os.getenv("MAIL_PASSWORD")
app.config["MAIL_PORT"] = 587
app.config["MAIL_SERVER"] = "smtp.office365.com"
app.config["MAIL_USE_TLS"] = True
app.config["MAIL_USERNAME"] = os.getenv("MAIL_USERNAME")

mail = Mail(app)

SPORTS = [
    "Baseball",
    "Basketball",
    "Soccer",
    "Tennis",
    "Swimming"
]

db = sqlite3.connect("froshims.db", check_same_thread=False)

# Python decorator
@app.route("/", methods=["GET", "POST"])
def index():
    return render_template("index.html", sports=SPORTS)

@app.route("/register", methods=["POST"])
def register():
    email = request.form.get("email")
    if not email:
        return render_template("error.html", message="Missing email")
    
    sport = request.form.get("sport")
    if not sport:
        return render_template("error.html", message="Missing sport")
    if sport not in SPORTS:
        return render_template("error.html", message="Invalid sport")

    db.execute("INSERT INTO registrants (email, sport) VALUES (?, ?)", (email, sport))
    
    message = Message("You are registered", recipients=[email])
    mail.send(message)

    return redirect("/registrants")

@app.route("/registrants")
def registrants():
    registrants = db.execute("SELECT * FROM registrants")
    return render_template("registrants.html", registrants=registrants)
