#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);
int *decimal_to_binary(int dec);

int main(void)
{
    // input
    string text = get_string("Message: ");

    // iterate word, char by char
    for (int i = 0; text[i] != '\0'; i++)
    {
        // get char bits
        int *letter = decimal_to_binary(text[i]);

        // print each bit of character
        for (int j = 0; j < BITS_IN_BYTE; j++)
        {
            print_bulb(letter[j]);
        }
        printf("\n");
    }

}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}

int *decimal_to_binary(int dec)
{
    // init size 8 array
    static int output[BITS_IN_BYTE];

    for (int i = BITS_IN_BYTE - 1; i >= 0; i--, dec /= 2)
    {
        if (dec % 2 == 0)
        {
            output[i] = 0;
        }
        else
        {
            output[i] = 1;
        }
    }

    return output;
}