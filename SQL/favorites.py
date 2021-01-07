import csv
from cs50 import SQL

 # Create a empty file
open("shows.db", "w").close()

# Open db file
db = SQL("sqlite:///shows.db")

# Create shows table
db.execute("CREATE TABLE shows (id INTEGER, title TEXT, PRIMARY KEY(id))")

# Create generes table
db.execute("CREATE TABLE genres (show_id INTEGER, genre TEXT, FOREIGN KEY(show_id) REFERENCES shows(id))")

# Open csv file
with open("favorites.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        title = row["title"].strip().upper()
        # Insert row in shows
        id = db.execute("INSERT INTO shows(title) VALUES(?)", title)
        for genre in row["genres"].split(","):
            # Insert row in generes
            genre_id = db.execute("INSERT INTO genres(show_id, genre) VALUES(?, ?)", id, genre.strip().upper())