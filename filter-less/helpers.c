#include "helpers.h"
#include <math.h>
#include <stdio.h>

void swapPixels(RGBTRIPLE *a, RGBTRIPLE *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// Convert image to sepia // WELL DONE
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sepiaRed = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue;
            float sepiaGreen = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue;
            float sepiaBlue = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue;
            image[i][j].rgbtRed = sepiaRed > 255 ? image[i][j].rgbtRed = 255 : round(sepiaRed);
            image[i][j].rgbtGreen = sepiaGreen > 255 ? image[i][j].rgbtGreen = 255 : round(sepiaGreen);
            image[i][j].rgbtBlue = sepiaBlue > 255 ? image[i][j].rgbtBlue = 255 : round(sepiaBlue);
        }
    }
}

// Reflect image horizontally // WELL DONE
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swapPixels(&image[i][j], &image[i][width - j - 1]);
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // populate auxilliary image
    RGBTRIPLE tmp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    // populate image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // init/reset values
            int pixelsFound = 0;
            float sumRed = 0;
            float sumGreen = 0;
            float sumBlue = 0;

            // select current pixel and iterate height - 1 till height + 1
            for (int k = i - 1, limit_height = k + 3; k < limit_height; k++)
            {
                // account for border
                if (k < 0 || k > height - 1)
                {
                    continue;
                }

                for (int m = j - 1, limit_width = m + 3; m < limit_width; m++)
                {
                    // account for border
                    if (m < 0 || m > width - 1)
                    {
                        continue;
                    }

                    // account for found pixels
                    pixelsFound++;
                    sumRed += tmp[k][m].rgbtRed;
                    sumGreen += tmp[k][m].rgbtGreen;
                    sumBlue += tmp[k][m].rgbtBlue;
                }

            }
            // compute averages
            BYTE averageRed = round(sumRed / pixelsFound);
            BYTE averageGreen = round(sumGreen / pixelsFound);
            BYTE averageBlue = round(sumBlue / pixelsFound);
            image[i][j].rgbtRed = averageRed;
            image[i][j].rgbtGreen = averageGreen;
            image[i][j].rgbtBlue = averageBlue;

        }
    }

}

void swapPixels(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    *a = *b;
    *b = temp;
}
