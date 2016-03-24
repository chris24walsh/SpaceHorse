#include "Animation.h"

Animation::Animation() {
	frameCounter = 0;
}

Animation::~Animation() {
}

double Animation::getX() {
	return x;
}

double Animation::getY() {
	return y;
}

double Animation::getAngle() {
	return angle;
}

bool Animation::actionPerFrame(double sx, double sy, double sangle) {
	x = sx;
	y = sy;
	angle = sangle;
	if (frameCounter<5) x++;
	else if (frameCounter>=5 && frameCounter<10) y++;
	else if (frameCounter>=10 && frameCounter<15) x--;
	else if (frameCounter>=15 && frameCounter<20) y--;
	else if (frameCounter>=20) done = true;
	frameCounter++;
	return done;
}