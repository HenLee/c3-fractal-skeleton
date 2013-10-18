#include "Mandelbrot.h"

Mandelbrot::~Mandelbrot()
{
	delete [] color_table;
}

void Mandelbrot::gen_fractal()
{
	// Randomly generate colors
	gen_color_table();

	double *iter_count_table = new double[m_width * m_height];

	// Run loops in parallel for faster processing
	#pragma omp parallel for

	// Calculate iterations taken until reaching a magnitude of 2
	for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            // Initialize counter, real coordinate c_r, and imaginary coordinate c_i
            int iter_count;
            double c_r = 3.5 * i / m_width - 2.5;		// Tests real coordinates between -2.5 and 1
            double c_i = 2.0 * j / m_height - 1.0;		// Tests imaginary coordinates between -1 and 1

            // Initialize iteration intermediaries z_r (real coordinate) and z_i (imaginary coordinate)
            double z_r = 0.0, z_i = 0.0;
            double z_temp;

			// Stay in loop as long as magnitude < 4 or max iteration count (1000) has not been reached
            for (iter_count = 0; z_r * z_r + z_i * z_i < 4 && iter_count < MAXITER; iter_count++)
            {
                // Z = Z^2 + C
                z_temp = z_r * z_r - z_i * z_i + c_r;
                z_i = 2 * z_r * z_i + c_i;
                z_r = z_temp;
            }

			// Normalize iterations for better approximation of the rate at which a point reaches infinity
			if (iter_count < MAXITER)		// Escaped to infinity
				iter_count_table[i + j * m_width] = iter_count + 1 - log(0.5 * log(z_r * z_r + z_i * z_i)) / log(2);
			else							// Did not escape to infinity
				iter_count_table[i + j * m_width] = MAXITER + 100.0;
		}
    }

	// Render bitmap using iteration counts
	// Iterate over all pixels
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			// If point escaped to infinity, color black
			if ((int)iter_count_table[i + j * m_width] >= MAXITER)
			{
				m_bitmap[(i + j * m_width) * 4 + 0] = 0;
				m_bitmap[(i + j * m_width) * 4 + 1] = 0;
				m_bitmap[(i + j * m_width) * 4 + 2] = 0;
				m_bitmap[(i + j * m_width) * 4 + 3] = 255;
			}
			// Otherwise, color
			else
			{
				// Find the average iteration count for all adjacent pixels, including the pixel itself (smoothens the picture)
				int sum_count = 0;
				double average = 0;
				for (int x = -1; x <= 1; x++)
					for (int y = -1; y <= 1; y++)
						if (i + x >= 0 && i + x < m_width && j + y >= 0 && j + y < m_height && iter_count_table[i + j * m_width] < 1000.0)
						{
							average += iter_count_table[i + x + (j + y) * m_width];
							sum_count++;
						}
				average /= sum_count;

				// Determine which color in the palette to use with the average iteration count
				int color_index = (int)(log(average)) % COLORQUANTITY * 3;
				int next_color_index = (int)(log(average) + 1) % COLORQUANTITY * 3;

				// Interpolate color based on the decimal portion of log(average)
				m_bitmap[(i + j * m_width) * 4 + 0] = (color_table[next_color_index] - color_table[color_index]) * (log(average) - (int)log(average)) + color_table[color_index];
				m_bitmap[(i + j * m_width) * 4 + 1] = (color_table[next_color_index + 1] - color_table[color_index + 1]) * (log(average) - (int)log(average)) + color_table[color_index + 1];
				m_bitmap[(i + j * m_width) * 4 + 2] = (color_table[next_color_index + 2] - color_table[color_index + 2]) * (log(average) - (int)log(average)) + color_table[color_index + 2];
				m_bitmap[(i + j * m_width) * 4 + 3] = 255;
			}
		}
	}

	delete [] iter_count_table;
}

void Mandelbrot::gen_color_table()
{
	// Generate a color table using RGB values
	color_table = new unsigned char[COLORQUANTITY * 3];
	for (int i = 0; i < COLORQUANTITY * 3; i++)
		color_table[i] = rand() % 128 + 64;
}