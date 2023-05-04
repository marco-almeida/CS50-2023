#include <cs50.h>
#include <stdio.h>

int getStartSize(void);
int getEndSize(int startSize);
int getYearsThreshold(int startSize, int endSize);

int main(void)
{
    // main function where we get the year threshold
    int startSize = getStartSize();

    int endSize = getEndSize(startSize);

    int years = getYearsThreshold(startSize, endSize);
    // formatted print
    printf("Years: %i\n", years);
}

int getStartSize(void)
{
    // get start size of population in a range
    int size;

    do
    {
        size = get_int("Start size: ");
    }
    while (size < 9);

    return size;
}

int getEndSize(int startSize)
{
    // get end size of population in a range
    int size;

    do
    {
        size = get_int("End size: ");
    }
    while (size < startSize);

    return size;
}

int getYearsThreshold(int startSize, int endSize)
{
    // get threshold range of population given startsize and endsize
    int years;
    for (years = 0; startSize < endSize; years++)
    {
        int newborns = startSize / 3;
        int deaths = startSize / 4;
        startSize += (newborns - deaths);
    }

    return years;
}
