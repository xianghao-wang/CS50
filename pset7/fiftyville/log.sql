-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Time: 2020-7-28 10:15 am Place: Chamberlin Street

-- Thief: German

-- Crime scene reports
SELECT description FROM crime_scene_reports
WHERE year=2020 AND month=7 AND day=28 AND street="Chamberlin Street";

-- Interviews
SELECT name, transcript FROM interviews
WHERE year=2020 AND month=7 AND day=28;

-- Find suspects (or thief) and its destination id
-- call a phone, make a transaction,  drive car away from courthouse and have a ticket of the earlist flight from Fiftyville on 7-29
SELECT DISTINCT(name), flights.destination_airport_id FROM people
JOIN phone_calls ON people.phone_number=phone_calls.caller
JOIN bank_accounts ON people.id=bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
JOIN courthouse_security_logs ON people.license_plate=courthouse_security_logs.license_plate
JOIN passengers ON passengers.passport_number=people.passport_number
JOIN flights ON passengers.flight_id=flights.id
JOIN airports ON flights.origin_airport_id=airports.id
WHERE phone_calls.year=2020 AND phone_calls.month=7 AND phone_calls.day=28 AND duration<60
AND atm_transactions.year=2020 AND atm_transactions.month=7 AND atm_transactions.day=28 AND atm_transactions.transaction_type="withdraw"
AND courthouse_security_logs.year=2020 AND courthouse_security_logs.month=7 AND courthouse_security_logs.day=28 AND courthouse_security_logs.hour=10
AND courthouse_security_logs.minute<=25
AND flights.year=2020 AND flights.month=7 AND flights.day=29
AND airports.full_name="Fiftyville Regional Airport"
ORDER BY flights.hour,flights.minute;
LIMIT 1;

-- We get Ernest|4

-- Find the thief's destination
SELECT DISTINCT(city) FROM airports
JOIN flights ON airports.id=flights.destination_airport_id
WHERE airports.id=4;

-- We get London

-- Find Accomplice: receive a phone call less than 1 minute from Ernest
SELECT name FROM people
JOIN phone_calls ON people.phone_number=phone_calls.receiver
WHERE phone_calls.caller=(SELECT phone_number FROM people WHERE name="Ernest")
AND phone_calls.year=2020 AND phone_calls.month=7 AND phone_calls.day=28 AND duration<60;

-- We get Berthold
