from cs50 import get_int


def main():
    height = get_height()

    for i in range(height):
        # print blanks
        print(" " * (height - 1 - i), end="")

        # left side
        print("#" * (i + 1), end="")

        # separate pyramids
        print("  ", end="")

        # right side and newline
        print("#" * (i + 1))


def get_height():
    # get height in range 1-8
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            return height


main()