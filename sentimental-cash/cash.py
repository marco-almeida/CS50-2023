from cs50 import get_float


def main():
    # user input
    money = get_money()

    # change we could use
    AVAILABLE_CHANGE = [0.25, 0.10, 0.05, 0.01]

    coins = 0

    while round(money, 2) > 0:
        # check for higher values first
        for i in AVAILABLE_CHANGE:
            while round(money, 2) >= i:
                money -= i
                coins += 1
    print(coins)


def get_money():
    # user input
    while True:
        money = get_float("Change owed: ")
        if money >= 0:
            return money


main()