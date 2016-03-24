#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
private:
	double x, y, angle;
	int frameCounter;
	bool done;
public:
	Animation();
	~Animation();
	double getX();
	double getY();
	double getAngle();
	bool actionPerFrame(double, double, double);
};

#endif
