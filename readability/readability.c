#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // input
    string text = get_string("Text: ");

    // get needed data
    float words = count_words(text);
    float L = count_letters(text) * (100 / words);
    float S = count_sentences(text) * (100 / words);
    float index = 0.0588 * L - 0.296 * S - 15.8;

    // right print
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16 || (int)round(index) > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)round(index));
    }
}

int count_sentences(string text)
{
    // sentences end at ., ? or !
    int sum = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sum++;
        }
    }

    return sum;
}

int count_words(string text)
{
    // new word every space
    int sum = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            sum++;
        }
    }
    // add first word always
    return sum + 1;
}

int count_letters(string text)
{
    // alphabetic words only
    int sum = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            sum++;
        }
    }

    return sum;
}
