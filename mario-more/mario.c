#include <cs50.h>
#include <stdio.h>

int getHeight(void);
void printPyramid(int height);

int main(void)
{
    // get pyramid height
    int height = getHeight();


    printPyramid(height);

}

int getHeight(void)
{
    // get height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    return height;
}

void printPyramid(int height)
{
    // loop height
    for (int i = 1; i <= height; i++)
    {
        // print gaps
        for (int j = 1; j <= height - i; j++)
        {
            printf(" ");
        }

        // print first pyramid
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }

        // separate pyramids
        printf("  ");

        // print second pyramid
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }

        // separate height levels
        printf("\n");
    }
}