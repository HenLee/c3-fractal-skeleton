#include "Mandelbrot.h"

void Mandelbrot::gen_fractal()
{
	// Generate a random color table using RGB values
	unsigned char * color_table = new unsigned char[COLOR_QUANTITY * 3];
	for (int i = 0; i < COLOR_QUANTITY * 3; i++)
	{
		if (i > 2) // Ensure that color values adjacent on table have a value difference of at least 32
			color_table[i] = (color_table[i - 3] + rand() % 128 + 32) % 192 + 64;
		else
			color_table[i] = rand() % 192 + 64;
	}

	// Initialize subsample diameter from subsample radius
	const int SUBSAMPLE_DIAM = 2 * SUBSAMPLE_RADIUS + 1;

	// Calculate iterations taken until reaching a magnitude of 2
	#pragma omp parallel for
	for (unsigned int i = 0; i < m_width; i++)
    {
        for (unsigned int j = 0; j < m_height; j++)
        {
            // Initialize counter, real coordinate base_c_r, and imaginary coordinate base_c_i
            int iter_count;
            double base_c_r = 3.5 * i / m_width - 2.5;		// Tests real coordinates between -2.5 and 1
            double base_c_i = 2.0 * j / m_height - 1.0;		// Tests imaginary coordinates between -1 and 1

			// Initialize color sums
			int red_sum = 0, green_sum = 0, blue_sum = 0;

			// Take a sub-sample and average colors
			for (int sub_x = -1 * SUBSAMPLE_RADIUS; sub_x <= SUBSAMPLE_RADIUS; sub_x++)
			{
				for (int sub_y = -1 * SUBSAMPLE_RADIUS; sub_y <= SUBSAMPLE_RADIUS; sub_y++)
				{
					// Initialize subsample coordinates c_r and c_i
					double c_r = base_c_r + ((double)sub_x / m_width / SUBSAMPLE_DIAM);
					double c_i = base_c_i + ((double)sub_y / m_height / SUBSAMPLE_DIAM);

					// Initialize iteration intermediaries z_r (real coordinate) and z_i (imaginary coordinate)
					double z_r = 0.0, z_i = 0.0;
					double z_temp;

					// Stay in loop as long as magnitude < 4 or max iteration count (1000) has not been reached
					for (iter_count = 0; z_r * z_r + z_i * z_i < 4 && iter_count < MAX_ITER; iter_count++)
					{
						// Z = Z^2 + C
						z_temp = z_r * z_r - z_i * z_i + c_r;
						z_i = 2 * z_r * z_i + c_i;
						z_r = z_temp;
					}
					
					// If escaped to infinity, add colors to color sums
					if (iter_count < MAX_ITER)		
					{
						// Normalize iterations for better approximation of the rate at which a point reaches infinity
						double norm_iter_count = log(iter_count + 1 - log(0.5 * log(z_r * z_r + z_i * z_i)) / log(2));
						int color_index = (int)(norm_iter_count) % COLOR_QUANTITY * 3;
						int next_color_index = (int)(norm_iter_count + 1) % COLOR_QUANTITY * 3;
						// Interpolate color based on decimal portion of log(norm_iter_count)
						red_sum		+= (color_table[next_color_index + 0] - color_table[color_index + 0]) * (norm_iter_count - (int)norm_iter_count) + color_table[color_index + 0];
						green_sum	+= (color_table[next_color_index + 1] - color_table[color_index + 1]) * (norm_iter_count - (int)norm_iter_count) + color_table[color_index + 1];
						blue_sum	+= (color_table[next_color_index + 2] - color_table[color_index + 2]) * (norm_iter_count - (int)norm_iter_count) + color_table[color_index + 2];
					}
				}
			}

			// Divide sums by subsample count to get averages and enter values into table
			red_sum		/= SUBSAMPLE_DIAM * SUBSAMPLE_DIAM;
			green_sum	/= SUBSAMPLE_DIAM * SUBSAMPLE_DIAM;
			blue_sum	/= SUBSAMPLE_DIAM * SUBSAMPLE_DIAM;

			m_bitmap[(i + j * m_width) * 4 + 0] = red_sum;
			m_bitmap[(i + j * m_width) * 4 + 1] = green_sum;
			m_bitmap[(i + j * m_width) * 4 + 2] = blue_sum;
			m_bitmap[(i + j * m_width) * 4 + 3] = 255;
		}
    }

	delete [] color_table;
}