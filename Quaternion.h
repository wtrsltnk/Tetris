/* 
 * File:   Quaternion.h
 * Author: wouter
 *
 * Created on April 9, 2010, 4:30 PM
 */

#ifndef _QUATERNION_H
#define	_QUATERNION_H

#include "Vector3.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z);
	Quaternion(float w, const Vector3& v);
	Quaternion(float w, float x, float y, float z);
	Quaternion(float q[4]);
	Quaternion(const Quaternion& orig);
	virtual ~Quaternion();

	Vector3 rotatePoint(float point[3]);
	Quaternion multiplyVector(const Vector3& v);
	Quaternion multiplyQuaternion(const Quaternion& q);
	Quaternion inverse();
	void normalize();
	float dotProduct(const Quaternion q);
	Quaternion slerp(const Quaternion& qb, float t);

    float x() const { return mQuat[X]; }
    void x(float x) { mQuat[X] = x; }

    float y() const { return mQuat[Y]; }
    void y(float y) { mQuat[Y] = y; }

    float z() const { return mQuat[Z]; }
    void z(float z) { mQuat[Z] = z; }

    float w() const { return mQuat[W]; }
    void w(float w) { mQuat[W] = w; }

    Vector3 v() const { return Vector3(mQuat[1], mQuat[2], mQuat[3]); }
    void v(const Vector3& v) { mQuat[1] = v.x(); mQuat[2] = v.y(); mQuat[3] = v.z(); }

    const Quaternion& operator = (const Quaternion& q);
	float operator [] (int index) const;
	
    float* copyTo(float q[4]) const;
	float* toMatrix(float m[4][4]);
	void print() const;

public:
	static Quaternion fromAngleAxis(float angle, const Vector3& axis);
	static Quaternion fromMatrix(float m[4][4]);
	
private:
	float mQuat[4];

};

#endif	/* _QUATERNION_H */

