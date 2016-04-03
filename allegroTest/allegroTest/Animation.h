#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
private:
	double m_x, m_y, m_angle;
	int m_frameCounter;
	bool m_done;
public:
	Animation(double, double, double);
	~Animation();
	double getX();
	double getY();
	double getAngle();
	bool actionPerFrame();
};

#endif
