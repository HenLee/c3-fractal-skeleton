#include "Mandelbrot.h"

void Mandelbrot::gen_fractal()
{
	// Iterate over each pixel
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
            for (iter_count = 0; iter_count < 1000; iter_count++)
            {
                // Z = Z^2 + C
                z_temp = z_r * z_r - z_i * z_i + c_r;
                z_i = 2 * z_r * z_i + c_i;
                z_r = z_temp;

                // If magnitude of Z is greater than 2, escapes to infinity and breaks out of loop
                if (z_r * z_r + z_i * z_i >= 4)
                    iter_count += 1000;
            }

			// Any value of Z > 1000 indicates that the loop was broken out of due to escaping to infinity
            // If Z escaped to infinity, input color
            if (iter_count > 1000)
            {
                // PLACEHOLDER COLORING ALGORITHM
                m_bitmap[(i + j * m_width) * 4 + 0] = (unsigned char)(255.0 * sqrt((iter_count - 1000) / 1000.0)); // R
                m_bitmap[(i + j * m_width) * 4 + 1] = (unsigned char)(255.0 * sqrt((iter_count - 1000) / 1000.0)); // G
                m_bitmap[(i + j * m_width) * 4 + 2] = (unsigned char)(255.0 * sqrt((iter_count - 1000) / 1000.0)); // B
            }
			// If Z did not escape to infinity, color black
			else
			{
				m_bitmap[(i + j * m_width) * 4 + 0] = 0;
				m_bitmap[(i + j * m_width) * 4 + 1] = 0;
				m_bitmap[(i + j * m_width) * 4 + 2] = 0;
			}
        }
    }
}
