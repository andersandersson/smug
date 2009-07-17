#ifndef RECTANGLE_H
#define RECTANGLE_H

typedef struct Rectangle
{
	double x;
	double y;
	double w;
	double h;
} Rectangle;

Rectangle Rectangle_create(double x, double y, double w, double h);

#endif // RECTANGLE_H
