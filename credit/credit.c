#include <cs50.h>
#include <stdio.h>

int getChecksum(long number);
int getNumberLength(long number);
string getCompanyName(int size, long number);
int getFirstTwoDigits(long number);

int main(void)
{
    // main
    long number = get_long("Number: ");
    int checksum = getChecksum(number);

    // ends program here
    if (checksum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    int length = getNumberLength(number);
    printf("%s", getCompanyName(length, number));
}

int getChecksum(long number)
{
    // iterating from last digit
    int totalSum;
    for (totalSum = 0; number != 0; number /= 10)
    {
        totalSum += number % 10;
        number /= 10;
        long currentNumber = (number % 10) * 2;
        totalSum += currentNumber % 10;
        currentNumber /= 10;
        totalSum += currentNumber % 10;
    }
    return totalSum;
}

int getNumberLength(long number)
{
    // get length of credit card number
    int size;
    for (size = 0; number != 0; size++)
    {
        number /= 10;
    }

    return size;
}

int getFirstTwoDigits(long number)
{
    // returns first two digits of long number
    while (number >= 100)
    {
        number /= 10;
    }
    return (int)number;
}

string getCompanyName(int length, long number)
{
    switch (length)
    {
        case 13:
            // VISA and so must start with 4
            if (getFirstTwoDigits(number) / 10 == 4)
            {
                return "VISA\n";
            }
            return "INVALID\n";
        case 15:
            // AMEX and so must start with 34 or 37
            if (0)
            {
                printf("ASD\n");
            }
            int firstTwoDigits1 = getFirstTwoDigits(number);
            if (firstTwoDigits1 == 34 || firstTwoDigits1 == 37)
            {
                return "AMEX\n";
            }
            return "INVALID\n";
        case 16:
            // VISA OR MASTERCARD
            // VISA starts with 4
            if (0)
            {
                printf("ASD\n");
            }
            int firstTwoDigits = getFirstTwoDigits(number);
            int firstDigit = firstTwoDigits / 10;

            if (firstDigit == 4)
            {
                return "VISA\n";
            }
            // MASTERCARD starts with 51,52,53,54 or 55
            if (firstTwoDigits == 51 || firstTwoDigits == 52 || firstTwoDigits == 53 || firstTwoDigits == 54 || firstTwoDigits == 55)
            {
                return "MASTERCARD\n";
            }

            return "INVALID\n";
        default:
            return "INVALID\n";
    }
}
