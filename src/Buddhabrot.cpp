#include "Buddhabrot.h"
using namespace std;

void Buddhabrot::gen_fractal()
{
	// Real (-2.5, 1)
	// Imaginary (-1, 1)
	int k;
    int h;
    // Initialize a bucket array (one integer for each pixel) (this is the outer bucket array)
	int *outer = (int *)malloc(m_height*m_width*sizeof(int));
	for (int i = 0; i < m_width * m_height; i++)
		outer[i] = 0;
	// iterate over the following several thousand times (at least more times than # of pixels)
  	for( int j = 0; j < m_width * m_height * 5; j++ )
  	{
        // Create a temporary bucket array (one integer for each pixel)
        int *temp = (int *)malloc(m_height*m_width*sizeof(int));
		for (int i = 0; i < m_width * m_height; i++)
			temp[i] = 0;
     
        // Let C be a random point in the complex plane
		double c_r = rand() * ( 1.0 + 2.5 ) / RAND_MAX + -2.5;
		double c_i = rand() * ( 1.0 + 1.0 ) / RAND_MAX + -1.0;
        
		double z_r = 0, z_i = 0;
	    double z_temp;

		int i = 0;

        // Trace the orbit of C, incrementing the temporary bucket that z falls in for each iteration
        for( ; i < 1000; i++ )
		{
	        z_temp = pow(z_r,2) - pow(z_i,2) + c_r;
	       	z_i = z_r*z_i*2 + c_i;
	       	z_r = z_temp;
	       	int pz_i = (1 + z_i) * m_height / 2;
	       	int pz_r = (2.5 + z_r) * m_width / 3.5;
			k = pz_i;
			h = pz_r;
			if (pz_r < m_width && pz_i < m_height) // make sure it's within bounds
	       		temp[pz_i*m_width + pz_r] += 1;
			
       		if( pow(z_r,2) + pow(z_i,2) >= 4 )
       			break;
  		}
		// If Z is in the mandelbrot set, discard the temporary bucket
		// Else, merge the temporary bucket with the outer bucket array
		if (i < 1000)
		{
			for (int i = 0; i < m_width * m_height; i++)
        		outer[i] += temp[i];
		}
		delete [] temp;
  	}
    // Normalize the global bucket array by dividing each value by the maximum value
    int largest = 0;
    for( int i = 0; i < m_width * m_height; i++)
    {
		//cout << outer[i] << endl;
        if( outer[i] > largest )
        	largest = outer[i];
    }
    /*for( int b = 0; b < sizeof(outer); b++ )
        outer[b] = outer[b] / largest;*/
     // Color each pixel however you wish
     for (int i = 0; i < m_width * m_height; i++)
	 {
		 m_bitmap[i * 4] = 255.0 / largest * outer[i];
		 m_bitmap[i * 4 + 1] = 0;
		 m_bitmap[i * 4 + 2] = 0;
		 m_bitmap[i * 4 + 3] = 255;
	 }

     // Parallelizing this function is tricky. It helps to have a list of temporary bucket arrays
     // Which are merged after the computation has finished.
     
     // Parallelizing is not required, but will save you a lot of time.
     
	delete [] outer;
}
    
