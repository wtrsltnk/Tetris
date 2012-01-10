#include "Matrix4x4.h"
#include <math.h>
#include <iostream>
#include <GL/gl.h>

Matrix4x4::Matrix4x4()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i == j)
				this->m[i][j] = 1.0f;
			else
				this->m[i][j] = 0.0f;
}

Matrix4x4::Matrix4x4(const Matrix4x4& matrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->m[i][j] = matrix.m[i][j];
}

Matrix4x4::Matrix4x4(float (*matrix)[4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->m[i][j] = matrix[i][j];
}

Matrix4x4::Matrix4x4(float* matrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->m[i][j] = matrix[i*4+j];
}

Matrix4x4::~Matrix4x4()
{
}

float Matrix4x4::get(int index)
{
	return m[index%4][index/4];
}

float* Matrix4x4::get()
{
	return (float*)&this->m[0][0];
}

const Vector3 Matrix4x4::forwardVector() const
{
    return Vector3(this->m[0][2], this->m[1][2], this->m[2][2]);
}

const Vector3 Matrix4x4::leftVector() const
{
    return Vector3(this->m[0][0], this->m[1][0], this->m[2][0]);
}

const Vector3 Matrix4x4::upVector() const
{
    return Vector3(this->m[0][1], this->m[1][1], this->m[2][1]);
}

Matrix4x4& Matrix4x4::operator = (const Matrix4x4& other)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->m[i][j] = other.m[i][j];
	
	return (*this);
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& second)
{
	Matrix4x4 out;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			out.m[i][j] = 0.0f;
			for(int k = 0; k < 4; k++)
			{
				out.m[i][j] += this->m[i][k] * second.m[k][j];
			}
		}
	}
	return out;
}

Matrix4x4 Matrix4x4::rotateMatrixX(float angle)
{
	Matrix4x4 out;

    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

    out.m[1][1] =  cosAngle;
    out.m[2][1] = -sinAngle;
    out.m[1][2] =  sinAngle;
    out.m[2][2] =  cosAngle;

	return out;
}

Matrix4x4 Matrix4x4::rotateMatrixY(float angle)
{
	Matrix4x4 out;

    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

    out.m[0][0] =  cosAngle;
    out.m[2][0] =  sinAngle;
    out.m[0][2] = -sinAngle;
    out.m[2][2] =  cosAngle;

	return out;
}

Matrix4x4 Matrix4x4::rotateMatrixZ(float angle)
{
	Matrix4x4 out;

    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

    out.m[0][0] =  cosAngle;
    out.m[1][0] = -sinAngle;
    out.m[0][1] =  sinAngle;
    out.m[1][1] =  cosAngle;

	return out;
}

Matrix4x4 Matrix4x4::translateMatrix(float x, float y, float z)
{
	Matrix4x4 out;

	out.m[3][0] = x;
	out.m[3][1] = y;
	out.m[3][2] = z;
	
	return out;
}

void Matrix4x4::loadIdentity()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i == j)
				this->m[i][j] = 1.0f;
			else
				this->m[i][j] = 0.0f;
}

void Matrix4x4::glMultiply() const
{
	glMultMatrixf(&this->m[0][0]);
}

// Transform a plane
Plane Matrix4x4::transform(const Plane& plane) const
{
    Plane transformed;
    transformed.normal().x(this->m[0][0] * plane.normal().x() + this->m[0][1] * plane.normal().y() + this->m[0][2] * plane.normal().z());
    transformed.normal().y(this->m[1][0] * plane.normal().x() + this->m[1][1] * plane.normal().y() + this->m[1][2] * plane.normal().z());
    transformed.normal().z(this->m[2][0] * plane.normal().x() + this->m[2][1] * plane.normal().y() + this->m[2][2] * plane.normal().z());
    transformed.setDistance(-(	(-plane.distance() * transformed.normal().x() + this->m[3][0]) * transformed.normal().x() +
                        (-plane.distance() * transformed.normal().y() + this->m[3][1]) * transformed.normal().y() +
                        (-plane.distance() * transformed.normal().z() + this->m[3][2]) * transformed.normal().z()));
    return transformed;
}

// Inverse transform a plane
Plane Matrix4x4::inverseTransform(const Plane& plane) const		
{
    return Plane(
        this->m[0][0] * plane.normal().x() + this->m[0][1] * plane.normal().y() + this->m[0][2] * plane.normal().z() + this->m[0][3] * plane.distance(),
        this->m[1][0] * plane.normal().x() + this->m[1][1] * plane.normal().y() + this->m[1][2] * plane.normal().z() + this->m[1][3] * plane.distance(),
        this->m[2][0] * plane.normal().x() + this->m[2][1] * plane.normal().y() + this->m[2][2] * plane.normal().z() + this->m[2][3] * plane.distance(),
        this->m[3][0] * plane.normal().x() + this->m[3][1] * plane.normal().y() + this->m[3][2] * plane.normal().z() + this->m[3][3] * plane.distance()
    );
}

Vector3 Matrix4x4::multiplyVector(const Vector3& v)
{
	return Vector3(
			m[0][0] * v.x() + m[0][1] * v.y() + m[0][2] * v.z() + m[0][3],
			m[1][0] * v.x() + m[1][1] * v.y() + m[1][2] * v.z() + m[1][3],
			m[2][0] * v.x() + m[2][1] * v.y() + m[2][2] * v.z() + m[2][3]
		);
}

void Matrix4x4::print() const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << this->m[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
