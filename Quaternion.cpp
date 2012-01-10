#include "Quaternion.h"
#include <math.h>
#include <iostream>

Quaternion::Quaternion()
{
	this->mQuat[0] = 0.0f;
	this->mQuat[1] = 0.0f;
	this->mQuat[2] = 0.0f;
	this->mQuat[3] = 1.0f;
}

Quaternion::Quaternion(float x, float y, float z)
{
	float t = 1.0f - (x * x) - (y * y) - (z * z);

	this->mQuat[X] = x;
	this->mQuat[Y] = y;
	this->mQuat[Z] = z;
	if (t < 0.0f)
		this->mQuat[W] = 0.0f;
	else
		this->mQuat[W] = -sqrt(t);
}

Quaternion::Quaternion(float s, const Vector3& v)
{
	this->mQuat[0] = s;
	this->mQuat[1] = v.x();
	this->mQuat[2] = v.y();
	this->mQuat[3] = v.z();
}

Quaternion::Quaternion(float s, float x, float y, float z)
{
	this->mQuat[0] = s;
	this->mQuat[1] = x;
	this->mQuat[2] = y;
	this->mQuat[3] = z;
}

Quaternion::Quaternion(float q[4])
{
	this->mQuat[0] = q[0];
	this->mQuat[1] = q[1];
	this->mQuat[2] = q[2];
	this->mQuat[3] = q[3];
}

Quaternion::Quaternion(const Quaternion& orig)
{
	this->mQuat[0] = orig.mQuat[0];
	this->mQuat[1] = orig.mQuat[1];
	this->mQuat[2] = orig.mQuat[2];
	this->mQuat[3] = orig.mQuat[3];
}

Quaternion::~Quaternion()
{
}

Vector3 Quaternion::rotatePoint(float point[3])
{
	Quaternion inv = this->inverse();

	inv.normalize();

	Quaternion tmp = this->multiplyVector(Vector3(point));
	Quaternion final = tmp.multiplyQuaternion(inv);

	return Vector3(final[X], final[Y], final[Z]);
}

Quaternion Quaternion::multiplyVector(const Vector3& v)
{
	Quaternion out;

	float w = - (this->mQuat[X] * v[X]) - (this->mQuat[Y] * v[Y]) - (this->mQuat[Z] * v[Z]);
	float x =   (this->mQuat[W] * v[X]) + (this->mQuat[Y] * v[Z]) - (this->mQuat[Z] * v[Y]);
	float y =   (this->mQuat[W] * v[Y]) + (this->mQuat[Z] * v[X]) - (this->mQuat[X] * v[Z]);
	float z =   (this->mQuat[W] * v[Z]) + (this->mQuat[X] * v[Y]) - (this->mQuat[Y] * v[X]);

	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::multiplyQuaternion(const Quaternion& q)
{
	float w = (this->mQuat[W] * q[W]) - (this->mQuat[X] * q[X]) - (this->mQuat[Y] * q[Y]) - (this->mQuat[Z] * q[Z]);
	float x = (this->mQuat[X] * q[W]) + (this->mQuat[W] * q[X]) + (this->mQuat[Y] * q[Z]) - (this->mQuat[Z] * q[Y]);
	float y = (this->mQuat[Y] * q[W]) + (this->mQuat[W] * q[Y]) + (this->mQuat[Z] * q[X]) - (this->mQuat[X] * q[Z]);
	float z = (this->mQuat[Z] * q[W]) + (this->mQuat[W] * q[Z]) + (this->mQuat[X] * q[Y]) - (this->mQuat[Y] * q[X]);

	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::inverse()
{
	return Quaternion(-this->mQuat[X], -this->mQuat[Y], -this->mQuat[Z], this->mQuat[W]);
}

void Quaternion::normalize()
{
	float mag = sqrt(
			(this->mQuat[X] * this->mQuat[X]) +
			(this->mQuat[Y] * this->mQuat[Y]) +
			(this->mQuat[Z] * this->mQuat[Z]) +
			(this->mQuat[W] * this->mQuat[W])
		);

	if (mag > 0.0f)
	{
		float oneOverMag = 1.0f / mag;
		this->mQuat[X] *= oneOverMag;
		this->mQuat[Y] *= oneOverMag;
		this->mQuat[Z] *= oneOverMag;
		this->mQuat[W] *= oneOverMag;
	}
}

float Quaternion::dotProduct(const Quaternion q)
{
  return ((this->mQuat[X] * q[X]) + (this->mQuat[Y] * q[Y]) + (this->mQuat[Z] * q[Z]) + (this->mQuat[W] * q[W]));
}

Quaternion Quaternion::slerp(const Quaternion& q, float t)
{
	/* Check for out-of range parameter and return edge points if so */
	if (t <= 0.0)
	{
		return (*this);
	}

	if (t >= 1.0)
	{
		return q;
	}

	Quaternion out;
	/* Compute "cosine of angle between quaternions" using dot product */
	float cosOmega = this->dotProduct(q);

  /* If negative dot, use -q1.  Two quaternions q and -q
	 represent the same rotation, but may produce
	 different slerp.  We chose q or -q to rotate using
	 the acute angle. */
  float q1w = q[W];
  float q1x = q[X];
  float q1y = q[Y];
  float q1z = q[Z];

  if (cosOmega < 0.0f)
	{
	  q1w = -q1w;
	  q1x = -q1x;
	  q1y = -q1y;
	  q1z = -q1z;
	  cosOmega = -cosOmega;
	}

  /* We should have two unit quaternions, so dot should be <= 1.0 */
//  assert (cosOmega < 1.1f);

  /* Compute interpolation fraction, checking for quaternions
	 almost exactly the same */
  float k0, k1;

  if (cosOmega > 0.9999f)
	{
	  /* Very close - just use linear interpolation,
	 which will protect againt a divide by zero */

	  k0 = 1.0f - t;
	  k1 = t;
	}
  else
	{
	  /* Compute the sin of the angle using the
	 trig identity sin^2(omega) + cos^2(omega) = 1 */
	  float sinOmega = sqrt (1.0f - (cosOmega * cosOmega));

	  /* Compute the angle from its sin and cosine */
	  float omega = atan2 (sinOmega, cosOmega);

	  /* Compute inverse of denominator, so we only have
	 to divide once */
	  float oneOverSinOmega = 1.0f / sinOmega;

	  /* Compute interpolation parameters */
	  k0 = sin ((1.0f - t) * omega) * oneOverSinOmega;
	  k1 = sin (t * omega) * oneOverSinOmega;
	}

  /* Interpolate and return new quaternion */
  out.w((k0 * this->mQuat[W]) + (k1 * q1w));
  out.x((k0 * this->mQuat[X]) + (k1 * q1x));
  out.y((k0 * this->mQuat[Y]) + (k1 * q1y));
  out.z((k0 * this->mQuat[Z]) + (k1 * q1z));

  return out;
}

const Quaternion& Quaternion::operator = (const Quaternion& q)
{
	for (int i = 0; i < 4; i++)
		this->mQuat[i] = q.mQuat[i];

	return (*this);
}

Quaternion Quaternion::fromAngleAxis(float angle, const Vector3& axis)
{
	float sinAngle = sin(angle / 2);
	
	return Quaternion(cos(angle / 2), axis.x() * sinAngle, axis.y() * sinAngle, axis.z() * sinAngle);
}

Quaternion Quaternion::fromMatrix(float m[4][4])
{
	float s = 0.5 * sqrt(m[0][0] + m[1][1] + m[2][2] + 1);
	
	return Quaternion(s,
				(1 / (4 * s)) * (m[2][1] - m[1][2]),
				(1 / (4 * s)) * (m[0][2] - m[2][0]),
				(1 / (4 * s)) * (m[1][0] - m[0][1])
			);
}

float Quaternion::operator [] (int index) const
{
	if (index >= 0 && index < 4)
		return this->mQuat[index];

	return 0.0f;
}

float* Quaternion::copyTo(float q[4]) const
{
    q[0] = this->mQuat[0];
    q[1] = this->mQuat[1];
    q[2] = this->mQuat[2];
    q[2] = this->mQuat[2];

    return q;
}

float* Quaternion::toMatrix(float m[4][4])
{
	float xy = this->mQuat[1] * this->mQuat[2];
	float xz = this->mQuat[1] * this->mQuat[3];
	float yz = this->mQuat[2] * this->mQuat[3];
	float sx = this->mQuat[0] * this->mQuat[1];
	float sy = this->mQuat[0] * this->mQuat[2];
	float sz = this->mQuat[0] * this->mQuat[3];
	float xsquared = this->mQuat[1] * this->mQuat[1];
	float ysquared = this->mQuat[2] * this->mQuat[2];
	float zsquared = this->mQuat[3] * this->mQuat[3];

	m[0][0] = 1 - 2 * (ysquared + zsquared);
	m[0][1] = (2 * xy) - (2 * sz);
	m[0][2] = (2 * sy) + (2 * xz);

	m[1][0] = (2 * xy) + (2 * sz);
	m[1][1] = 1 - 2 * (xsquared + zsquared);
	m[1][2] = -(2 * sx) + (2 * yz);

	m[2][0] = -(2 * sy) + (2 * xz);
	m[2][1] = (2 * sx) + (2 * yz);
	m[2][2] = 1 - 2 * (xsquared + ysquared);

	return &m[0][0];
}

void Quaternion::print() const
{
	std::cout << "q=(" << this->mQuat[0] << " " << this->mQuat[1] << " " << this->mQuat[2] << " " << this->mQuat[4] << ")" << std::endl;
}
