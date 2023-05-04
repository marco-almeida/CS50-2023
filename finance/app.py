import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# create if doesnt exists, new sqlite table which keeps track of users' bought stocks
db.execute("CREATE TABLE IF NOT EXISTS transactions ( id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, timestamp NOT NULL DEFAULT (datetime(CURRENT_TIMESTAMP, 'localtime')), owner_id INTEGER, shares INTEGER, share_symbol TEXT NOT NULL, price NUMERIC NOT NULL, status TEXT NOT NULL, FOREIGN KEY(owner_id) REFERENCES users(id));")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # information for each owned stock
    results = []
    money_spent = 0.0
    for stock in get_shares(session['user_id']):
        query = lookup(stock["share_symbol"])
        results.append({'symbol': stock["share_symbol"], 'name': query['name'],
                       'shares': stock["shares"], 'price': query['price']})

        money_spent += stock["shares"] * query['price']

    # get current cash
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    return render_template("portfolio.html", results=results, grand_total=user_cash + money_spent, user_cash=user_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        # get symbol data
        symbol = request.form.get("symbol")
        result = lookup(symbol)

        # validate symbol input
        if not symbol or not result or symbol == "":
            return apology("Error validating symbol", 400)

        symbol = symbol.upper()

        # validate number of shares
        shares = request.form.get("shares")
        try:
            shares = int(shares)
            if shares < 1:
                return apology("Error validating number of shares", 400)
        except:
            return apology("Error validating number of shares", 400)

        stock_price = result["price"]
        user_money = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[0]['cash']

        # check if user has enough money to buy what he wants
        if user_money < stock_price * shares:
            return apology("Can't afford", 400)

        # update users table with user current money
        spent_money = stock_price * shares
        new_money = user_money - spent_money
        update_cash(session["user_id"], new_money)

        # update new table with user data
        db.execute("INSERT INTO transactions (owner_id, shares, share_symbol, price, status) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], shares, symbol, stock_price, "purchased")

        # in the end, redirect user to home
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # get all transactions
    purchases = db.execute("SELECT * FROM transactions WHERE owner_id = ? AND status = 'purchased'", session["user_id"])
    sold_stocks = db.execute("SELECT * FROM transactions WHERE owner_id = ? AND status = 'sold'", session["user_id"])

    # differentiate sold stocks
    for it in sold_stocks:
        it["shares"] = it["shares"] * -1

    # merge lists and sort by timestamp
    purchases.extend(sold_stocks)
    purchases = sorted(purchases, key=lambda d: d['timestamp'])

    return render_template("history.html", results=purchases)


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
        # get stock symbol
        symbol = request.form.get("symbol")

        # get data from API regarding symbol
        if not symbol or symbol == "":
            return apology("Error validating symbol", 400)

        results = lookup(symbol)

        # show data to user
        if results:
            return render_template("quoted.html", results=results)

        return apology("Error validating symbol", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        name = request.form.get("username")
        password = request.form.get("password")
        confirm_password = request.form.get("confirmation")

        # query database if name already exists
        name_exists = len(db.execute("SELECT * FROM users WHERE username = ?", name)) > 0

        # ensure username was submitted and is unique
        if name == "" or not name or name_exists:
            return apology("Error validating username", 400)

        # ensure password equals confirmed password and both were submitted
        if password == "" or confirm_password == "" or not password or not confirm_password or password != confirm_password:
            return apology("Error validating password", 400)

        # user is valid, lets store its information
        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", name, hash)

        # redirect user to login page
        return redirect("/login")
    else:
        # User reached route via GET (as by clicking a link or via redirect)
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        # validate symbol
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Error validating stock")

        # get all stock
        all_owned_stock = get_shares(session["user_id"])
        amount_owned_stock = 0
        for item in all_owned_stock:
            # check if we have target stock
            if item["share_symbol"] == symbol:
                amount_owned_stock = item["shares"]

        if amount_owned_stock == 0:
            return apology("Stock not owned")

        # validate number of shares
        shares_to_sell = request.form.get("shares")
        try:
            shares_to_sell = int(shares_to_sell)
            if shares_to_sell < 1:
                return apology("Error validating number of shares", 403)

            if shares_to_sell > amount_owned_stock:
                return apology("Not enough shares")
        except:
            return apology("Error validating number of shares", 403)

        stock_info = lookup(symbol)
        winnings = stock_info["price"] * shares_to_sell

        # update cash in users table
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        update_cash(session["user_id"], current_cash + winnings)

        # update sold_stocks table with user data
        db.execute("INSERT INTO transactions (owner_id, shares, share_symbol, price, status) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], shares_to_sell, symbol, stock_info["price"], "sold")

        return redirect("/")

    # GET
    available_options = get_shares(session["user_id"])
    return render_template("sell.html", available_options=available_options)


@app.route("/add_cash", methods=["GET", "POST"])
def add_cash():

    if request.method == "POST":
        cash = request.form.get("cash")

        # validate user input
        if not cash or not cash.isnumeric():
            return apology("Error validating cash")

        cash = float(cash)

        # validate number size
        if cash <= 0:
            return apology("Submit a higher value")

        # add cash to db
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        update_cash(session["user_id"], current_cash + cash)

        return redirect("/")

    return render_template("add_cash.html")


def update_cash(user_id, cash):
    db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, user_id)


def get_shares(user_id):
    purchases = db.execute(
        "SELECT share_symbol, SUM(shares) FROM transactions WHERE owner_id = ? AND status = 'purchased' GROUP BY share_symbol", user_id)
    sold_stocks = db.execute(
        "SELECT share_symbol, SUM(shares) FROM transactions WHERE owner_id = ? AND status = 'sold' GROUP BY share_symbol", user_id)

    balance = []
    for item in purchases:
        company = item['share_symbol']
        balance.append({'shares': item['SUM(shares)'], 'share_symbol': company})
        # check if this company exists in sold_stocks
        for sold_item in sold_stocks:
            if sold_item['share_symbol'] == company:
                # exists and so we must check the balance
                new_balance = item['SUM(shares)'] - sold_item['SUM(shares)']

                # delete item if we do not own it anymore
                if new_balance == 0:
                    balance.pop(-1)
                else:
                    balance[-1]['shares'] = new_balance
                break

    return balance
