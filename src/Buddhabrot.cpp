#include "Buddhabrot.h"
using namespace std;

void Buddhabrot::gen_fractal()
{

   
	// Real (-2.5, 1)
	// Imaginary (-1, 1)
	int k;
    int h;
    // Initialize a bucket array (one integer for each pixel) (this is the outer bucket array)
	int *outer = ( int *)malloc(m_height*m_width*4*sizeof( int));
	// iterate over the following several thousand times (at least more times than # of pixels)
  	for( int j = 0; j < 10000; j++ )
  	{
        // Create a temporary bucket array (one integer for each pixel
        //
        	int *temp = ( int *)malloc(m_height*m_width*4*sizeof( int));
     
        // Let C be a random point in the complex plane
        //
		double c_r = rand() * ( 1.0 + 2.5 ) / RAND_MAX + -2.5;
		double c_i = rand() * ( 1.0 + 1.0 ) / RAND_MAX + -1.0;
        	
        // Trace the orbit of C, incrementing the temporary bucket that z falls in for each iteration
        	for( int i = 0; i < 10000; i++ )
        	{
	        	double z_r = 0, z_i = 0;
	        	double z_temp;
	        	z_temp = pow(z_r,2) - pow(z_i,2) + c_r;
	       		z_i = z_r*z_i*2 + c_i;
	       		z_r = z_temp;
	       		int pz_i = (1 + z_i) * m_height / 2;
	       		int pz_r = (2.5 + z_r) * m_width / 3.5;
				k = pz_i;
				h = pz_r;
	       		temp[pz_i*m_width + pz_r*m_height] += 1;
        // If Z is in the mandelbrot set, discard the temporary bucket
       			if( pow(z_r,2) + pow(z_i,2) >= 4 )
       			{
       				temp[pz_i*m_width + pz_r*m_height] = 0;
       				break;
       			}
  		}
        // Else, merge the temporary bucket with the outer bucket array
        	outer[k*m_width + h*m_height] = temp[k*m_width + h*m_height];
			delete [] temp;
  	}
	delete [] outer;
     // Normalize the global bucket array by dividing each value by the maximum value
     // Color each pixel however you wish
     //
     // Parallelizing this function is tricky. It helps to have a list of temporary bucket arrays
     // Which are merged after the computation has finished.
     
     // Parallelizing is not required, but will save you a lot of time.
     

}
    
