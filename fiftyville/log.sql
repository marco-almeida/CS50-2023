-- Keep a log of any SQL queries you execute as you solve the mystery.
-- july 28 in humphrey street
-- who he is
-- where did he go
-- accomplice

SELECT * FROM crime_scene_reports WHERE street='Humphrey Street' AND day = 28 AND month = 7 AND year = 2021;

-- | 295 | 2021 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

SELECT * FROM interviews WHERE day = 28 AND month = 7 AND year = 2021;

-- | 161 | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

-- | 162 | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

-- | 163 | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |

SELECT license_plate FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute BETWEEN 15 AND 25;

-- +---------------+
-- | license_plate |
-- +---------------+
-- | 5P2BI95       |
-- | 94KL13X       |
-- | 6P58WS2       |
-- | 4328GD8       |
-- | G412CB7       |
-- | L93JTIZ       |
-- | 322W7JE       |
-- | 0NTHK55       |
-- +---------------+


SELECT account_number, amount FROM atm_transactions WHERE atm_location = 'Leggett Street' AND day = 28 AND month = 7 AND transaction_type='withdraw';

-- +----------------+--------+
-- | account_number | amount |
-- +----------------+--------+
-- | 28500762       | 48     |
-- | 28296815       | 20     |
-- | 76054385       | 60     |
-- | 49610011       | 50     |
-- | 16153065       | 80     |
-- | 25506511       | 20     |
-- | 81061156       | 30     |
-- | 26013199       | 35     |
-- +----------------+--------+

SELECT people.id, name, phone_number, passport_number, people.license_plate FROM people INNER JOIN bakery_security_logs ON people.license_plate=bakery_security_logs.license_plate WHERE month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute BETWEEN 15 AND 25;

-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- +--------+---------+----------------+-----------------+---------------+

SELECT caller AS criminal, receiver AS accomplice FROM phone_calls WHERE day = 28 AND month = 7 AND duration < 60;

-- +----------------+----------------+
-- |    criminal    |   accomplice   |
-- +----------------+----------------+
-- | (130) 555-0289 | (996) 555-8899 |
-- | (499) 555-9472 | (892) 555-8872 |
-- | (367) 555-5533 | (375) 555-8161 |
-- | (499) 555-9472 | (717) 555-1342 |
-- | (286) 555-6063 | (676) 555-6554 |
-- | (770) 555-1861 | (725) 555-3243 |
-- | (031) 555-6622 | (910) 555-3251 |
-- | (826) 555-1652 | (066) 555-9701 |
-- | (338) 555-6650 | (704) 555-2131 |
-- +----------------+----------------+

SELECT people.id, name, phone_number, passport_number, people.license_plate, receiver AS accomplice FROM people INNER JOIN bakery_security_logs ON people.license_plate=bakery_security_logs.license_plate INNER JOIN phone_calls ON phone_number=caller WHERE phone_calls.month = 7 AND phone_calls.day = 28 AND hour = 10 AND activity = 'exit' AND minute BETWEEN 15 AND 25 AND duration < 60;

-- +--------+--------+----------------+-----------------+---------------+----------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |   accomplice   |
-- +--------+--------+----------------+-----------------+---------------+----------------+
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | (375) 555-8161 |
-- | 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       | (996) 555-8899 |
-- | 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       | (725) 555-3243 |
-- | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       | (717) 555-1342 |
-- | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       | (892) 555-8872 |
-- +--------+--------+----------------+-----------------+---------------+----------------+

SELECT * FROM flights WHERE day = 29 AND month = 7 ORDER BY hour, minute LIMIT 1;

-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+

SELECT flight_id, passport_number FROM passengers WHERE flight_id = 36;

-- +-----------+-----------------+
-- | flight_id | passport_number |
-- +-----------+-----------------+
-- | 36        | 7214083635      |
-- | 36        | 1695452385      |
-- | 36        | 5773159633      |
-- | 36        | 1540955065      |
-- | 36        | 8294398571      |
-- | 36        | 1988161715      |
-- | 36        | 9878712108      |
-- | 36        | 8496433585      |
-- +-----------+-----------------+

SELECT people.id, name, phone_number, people.passport_number, people.license_plate, receiver AS accomplice FROM people INNER JOIN bakery_security_logs ON people.license_plate=bakery_security_logs.license_plate INNER JOIN phone_calls ON phone_number=caller INNER JOIN passengers ON passengers.passport_number=people.passport_number INNER JOIN bank_accounts ON people.id=bank_accounts.person_id WHERE phone_calls.month = 7 AND phone_calls.day = 28 AND hour = 10 AND activity = 'exit' AND minute BETWEEN 15 AND 25 AND duration < 60 AND flight_id = 36;

-- +--------+-------+----------------+-----------------+---------------+----------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |   accomplice   |
-- +--------+-------+----------------+-----------------+---------------+----------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | (375) 555-8161 |
-- +--------+-------+----------------+-----------------+---------------+----------------+

SELECT * FROM people WHERE phone_number = '(375) 555-8161';

-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+


SELECT * FROM flights WHERE id = 36;

-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+

SELECT * FROM airports WHERE id = 4;

-- +----+--------------+-------------------+---------------+
-- | id | abbreviation |     full_name     |     city      |
-- +----+--------------+-------------------+---------------+
-- | 4  | LGA          | LaGuardia Airport | New York City |
-- +----+--------------+-------------------+---------------+