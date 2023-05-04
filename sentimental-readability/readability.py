from cs50 import get_string


def main():
    # input
    text = get_string("Text: ")

    # get needed data
    words = count_words(text)
    L = count_letters(text) * (100 / words)
    S = count_sentences(text) * (100 / words)
    index = 0.0588 * L - 0.296 * S - 15.8

    # right print
    if index < 1:
        print("Before Grade 1")
    elif round(index) >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(index)}")


def count_sentences(text):
    return text.count(".") + text.count("?") + text.count("!")


def count_words(text):
    # no need to account for first word
    return len(text.split(" "))


def count_letters(text):
    sum = 0
    # iterate char by char to see if element is 'A' or 'Z'
    for char in text:
        if char.isalpha():
            sum += 1

    return sum


main()