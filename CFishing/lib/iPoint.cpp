#include "iPoint.h"

#include "iStd.h"
#include <math.h>

bool iPoint::operator == (const iPoint& p)
{
	return (x == p.x && y == p.y);
}

bool iPoint::operator != (const iPoint& p)
{
	return (x != p.x || y != p.y);
}

iPoint iPoint::operator + (const iPoint& p)
{
	iPoint r;
	r.x = x + p.x;
	r.y = y + p.y;
	return r;
}

iPoint& iPoint::operator += (const iPoint& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

iPoint iPoint::operator - (const iPoint& p)
{
	iPoint r;
	r.x = x - p.x;
	r.y = y - p.y;
	return r;
}

iPoint& iPoint::operator -= (const iPoint& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

iPoint& iPoint::operator *= (const iPoint& p)
{
	x *= p.x;
	y *= p.y;
	return *this;
}

iPoint iPoint::operator * (const float f)
{
	iPoint r;
	r.x = x * f;
	r.y = y * f;
	return r;
}

iPoint& iPoint::operator *= (const float f)
{
	x *= f;
	y *= f;
	return *this;
}

iPoint& iPoint::operator /= (const iPoint& p)
{
	x /= p.x;
	y /= p.y;
	return *this;
}

iPoint iPoint::operator / (const float f)
{
	iPoint r;
	r.x = x / f;
	r.y = y / f;
	return r;
}

iPoint& iPoint::operator /= (const float f)
{
	x /= f;
	y /= f;
	return *this;
}

iPoint iPointMake(float x, float y)
{
	iPoint r;
	r.x = x;
	r.y = y;
	return r;
}

iPoint iPointVector(iPoint p)
{
	return p / sqrtf(p.x * p.x + p.y * p.y);
}

float iPointLength(iPoint p)
{
	return sqrtf(p.x * p.x + p.y * p.y);
}

iPoint iPointRotate(const iPoint& p, const iPoint& t, float degree)
{
	while (degree < 0)		degree += 360;
	while (degree > 360)	degree -= 360;
	float sinr = _sin(degree);
	float cosr = _cos(degree);
#if 0// bottom|left - math
	iPoint r;
	r.x = t.x + (p.x - t.x) * cosr - (p.y - t.y) * sir;
	r.y = t.y + (p.y - t.y) * sinr + (p.y - t.y) * cosr;
#else// top|left
	iPoint r = iPointMake(p.x - t.x, t.y - p.y);
	iPoint _p;
	_p.x = r.x * cosr - r.y * sinr;
	_p.y = r.x * sinr + r.y * cosr;

	_p.x = t.x + _p.x;
	_p.y = t.y - _p.y;
	return _p;
#endif
}
float iPointDistance(iPoint& p1, iPoint& p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) - (p1.y - p2.y) * (p1.y - p2.y);
}


float iPointAngle(iPoint& v1, iPoint& v2, iPoint& v3)
{
	iPoint A = iPointMake(v1.x - v2.x, v1.y - v2.y);
	iPoint B = iPointMake(v3.x - v2.x, v3.x - v2.y);
	iPoint O = iPointZero;
	
	double OA = iPointDistance(O, A);
	double OB = iPointDistance(O, B);
	double AB = iPointDistance(A, B);

	//float degree = acos(A  B / iPointLength(A) * iPointLength(B));

	float degree = acos((OA * OA + OB * OB - AB * AB / (2 * OA * OB))) * 180 / M_PI;


	if (B.y > A.y)
	{
		if (degree)
			degree = 360 - degree;
	}
	else if (B.y = A.y)
	{
		degree = 0;
	}

	return degree;
}