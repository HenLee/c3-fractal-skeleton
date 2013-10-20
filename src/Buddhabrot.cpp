#include "Buddhabrot.h"
using namespace std;

void Buddhabrot::gen_fractal()
{

   
	// Real (-2.5, 1)
	// Imaginary (-1, 1)
    
    // Initialize a bucket array (one integer for each pixel) (this is the outer bucket array)
	int outer[sizeof(m_bitmap)];
	int n;
    // iterate over the following several thousand times (at least more times than # of pixels)
	for( int i = 0; i <= sizeof(outer); i++)
	{
        	double z_r = 0, z_i = 0;
        	double z_temp;
  		for( int j = 0; j < 100000; j++ )
  		{
        // Create a temporary bucket array (one integer for each pixel
        //
     
        // Let C be a random point in the complex plane
        //
        		double c_r = 1.01, c_i = -0.123;
        		z_temp = pow(z_r,2) - pow(z_i,2) + c_r;
        		z_i = z_r*z_i*2 + c_i;
        		z_r = z_temp;
        // Trace the orbit of C, incrementing the temporary bucket that z falls in for each iteration
        // If Z is in the mandelbrot set, discard the temporary bucket
        		if( pow(z_r,2) + pow(z_i,2) >= 4 )
        			n++;
  		}
        // Else, merge the temporary bucket with the outer bucket array
        	outer[i] = n;
	}
     // Normalize the global bucket array by dividing each value by the maximum value
     // Color each pixel however you wish
     //
     // Parallelizing this function is tricky. It helps to have a list of temporary bucket arrays
     // Which are merged after the computation has finished.
     
     // Parallelizing is not required, but will save you a lot of time.
     

}
    

