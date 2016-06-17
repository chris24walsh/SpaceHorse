#ifndef ENUM_H
#define ENUM_H

enum class Fire //we don't want a dynamically allocated array(or vector) unless necessary, but we don't want a magic number either. So we use an enum class.
{ 
	MAXFIREBALLS = 15
};

struct Coordinates
{
	double x,
		y;
};

#endif