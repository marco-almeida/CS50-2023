import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print(f"Usage: python {sys.argv[0]} file.csv file.txt")
        sys.exit(1)

    # Read database file into a variable
    csv_database = read_database(sys.argv[1])

    # Read DNA sequence file into a variable
    dna_sequence = read_sequence(sys.argv[2])

    # List of STRs in csv file
    KNOWN_STRS = list(csv_database[0].keys())[1:]

    # Find longest match of each STR in DNA sequence
    count = {}

    for strs in KNOWN_STRS:
        count[strs] = longest_match(dna_sequence, strs)

    # typecast values to strings
    for key in count:
        count[key] = str(count[key])

    # Check database for matching profiles
    for person in csv_database:
        name = person["name"]
        person.pop('name', None)
        if person == count:
            print(name)
            sys.exit(0)

    print("No match")


def read_sequence(filename):
    # return file contents as string
    with open(filename, 'r') as file:
        return file.read()


def read_database(filename):
    # Read database file into a variable
    csv_data = []
    with open(filename, 'r') as file:
        reader = csv.DictReader(file)
        # append each item
        csv_data = [item for item in reader]
 s
    return csv_data


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
