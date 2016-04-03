#include "Animation.h"

Animation::Animation(double shipX, double shipY, double shipAngle) {
	m_frameCounter = 0;
	m_done = false;
	m_x = shipX;
	m_y = shipY;
	m_angle = shipAngle;
}

Animation::~Animation() {
}

double Animation::getX() {
	return m_x;
}

double Animation::getY() {
	return m_y;
}

double Animation::getAngle() {
	return m_angle;
}

bool Animation::actionPerFrame() {
	if (m_frameCounter<50) m_x += 1;
	else if (m_frameCounter>=50 && m_frameCounter<100) m_y += 1;
	else if (m_frameCounter>=100 && m_frameCounter<150) m_x -= 1;
	else if (m_frameCounter>=150 && m_frameCounter<200) m_y -= 1;
	else if (m_frameCounter>=200) m_done = true;
	m_frameCounter++;
	return m_done;
}