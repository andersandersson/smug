#include "rectangle.h"

Rectangle Rectangle_(double x, double y, double w, double h)
{
	Rectangle r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

Rectangle* Rectangle_new(double x, double y, double w, double h)
{
	Rectangle* r = malloc(sizeof(Rectangle));
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
	return r;
}
