#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

class Animation {
private:
	double m_x, m_y, m_angle;
	int m_frameCounter;
	bool m_done;
	struct action {
		double x, y, angle, numFrames;
	};
	
public:
	Animation(double, double, double);
	~Animation();
	double getX();
	double getY();
	double getAngle();
	bool actionPerFrame();
	void setAction(double, double, double);
};

#endif
