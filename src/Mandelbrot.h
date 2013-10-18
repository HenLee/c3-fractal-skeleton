#include <cmath>
#include <cstdlib>
#include "Fractal.h"

class Mandelbrot : public Fractal
{
	unsigned char *color_table;
	void gen_color_table();
	public:
	Mandelbrot(int height, int width):Fractal(height, width) {}
    Mandelbrot(double x):Fractal(x) {}
	~Mandelbrot();
	void gen_fractal();
};


