#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            RGBTRIPLE *pixel = &image[i][j];

            // Calculate shade of gray
            WORD sum = pixel -> rgbtRed + pixel -> rgbtGreen + pixel -> rgbtBlue;
            BYTE average = round(sum / 3.0);

            // Update pixel
            pixel -> rgbtRed = average;
            pixel -> rgbtBlue = average;
            pixel -> rgbtGreen = average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width / 2; j ++)
        {
            // Swap left and right
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // offsets of 9 adjacent pixels
    int offsets[9][2] =
    {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 0}, {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    RGBTRIPLE temp_image[height][width];

    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            int count = 0;
            for (int k = 0; k < 9; k ++)
            {
                int x = i + offsets[k][0];
                int y = j + offsets[k][1];
                if (x >= 0 && x < height && y >= 0 && y < width)
                {
                    RGBTRIPLE pixel = image[x][y];
                    sum_r += pixel.rgbtRed;
                    sum_g += pixel.rgbtGreen;
                    sum_b += pixel.rgbtBlue;
                    count ++;
                }
            }
            temp_image[i][j].rgbtRed = round((float) sum_r / count);
            temp_image[i][j].rgbtGreen = round((float) sum_g / count);
            temp_image[i][j].rgbtBlue = round((float) sum_b / count);
        }
    }

    // Update original image
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            image[i][j] = temp_image[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Kernels
    int kernel_x[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int kernel_y[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    RGBTRIPLE temp_image[height][width];

    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            // Gx and Gy
            int Gx[3] = {0, 0, 0}; // RGB
            int Gy[3] = {0, 0, 0}; // RGB

            // Calculate
            for (int offset_x = -1; offset_x <= 1; offset_x ++)
            {
                for (int offset_y = -1; offset_y <= 1; offset_y ++)
                {
                    int x = i + offset_x, y = j + offset_y;

                    if (x >= 0 && x < height && y >= 0 && y < width)
                    {
                        // RGB values
                        BYTE rgb_values[3] = {image[x][y].rgbtRed, image[x][y].rgbtGreen, image[x][y].rgbtBlue};

                        // Get Gx and Gy
                        for (int k = 0; k < 3; k ++)
                        {
                            Gx[k] += kernel_x[offset_x + 1][offset_y + 1] * rgb_values[k];
                            Gy[k] += kernel_y[offset_x + 1][offset_y + 1] * rgb_values[k];
                        }
                    }
                }
            }
            // Bug 0 - Gx and Gy are too big

            // Final value
            int r_value = round(sqrt(pow(Gx[0], 2) + pow(Gy[0], 2)));
            int g_value = round(sqrt(pow(Gx[1], 2) + pow(Gy[1], 2)));
            int b_value = round(sqrt(pow(Gx[2], 2) + pow(Gy[2], 2)));

            // Update pixel
            temp_image[i][j].rgbtRed = r_value > 255 ? 255 : r_value;
            temp_image[i][j].rgbtGreen = g_value > 255 ? 255 : g_value;
            temp_image[i][j].rgbtBlue = b_value > 255 ? 255 : b_value;
        }
    }

    // Copy final results to image
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            image[i][j] = temp_image[i][j];
        }
    }

    return;
}
