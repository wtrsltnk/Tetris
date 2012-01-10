#include "Plane.h"
#include <math.h>

Plane::Plane()
	: mDistance(0)
{
}

Plane::Plane(float nx, float ny, float nz, float d)
	: mDistance(d)
{
	mNormal.x(nx);
	mNormal.y(ny);
	mNormal.z(nz);
}

Plane::Plane(float d, const float n[3])
	: mDistance(d)
{
	mNormal.x(n[0]);
	mNormal.y(n[1]);
	mNormal.z(n[2]);
}

Plane::Plane(float p[4])
	: mDistance(p[3])
{
	mNormal.x(p[0]);
	mNormal.y(p[1]);
	mNormal.z(p[2]);
}

Plane::~Plane()
{
}

float Plane::distance()
{
	return mDistance;
}

float Plane::distance() const
{
	return mDistance;
}

void Plane::setDistance(float d)
{
	this->mDistance = d;
}

Vector3& Plane::normal()
{
	return mNormal;
}

const Vector3& Plane::normal() const
{
	return mNormal;
}

float Plane::distanceTo(const Vector3& vec)
{
	// f(x,y,z) = Ax+Ay+Az-D
	return this->mNormal[0] * vec[0]
			   +this->mNormal[1] * vec[1]
			   +this->mNormal[2] * vec[2]
			   -this->mDistance;
}

void Plane::invert()
{
	this->mNormal[0] = -this->mNormal[0];
	this->mNormal[1] = -this->mNormal[1];
	this->mNormal[2] = -this->mNormal[2];
	this->mDistance = -this->mDistance;
}

bool Plane::intersect(const Vector3& start, const Vector3& end, Vector3& result)
{
	// Ray is ptStart*(1-t) + ptEnd*(t) for t->[0, 1]
	float fSDotN = start.dotProduct(this->mNormal);
	float fEDotN = end.dotProduct(this->mNormal);
	float fDenom = fEDotN - fSDotN;
	
	if (fabs(fDenom) < cZeroTolerance)
		return Vector3();

	float t = (this->mDistance - fSDotN) / fDenom;
	if (t >= 0.0f && t <= 1.0f)
	{
		result = (start * (1.0f - t)) + (end * t);
		return true;
	}

	return false;
}
