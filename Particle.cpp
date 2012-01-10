/* 
 * File:   Particle.cpp
 * Author: wouter
 * 
 * Created on January 2, 2012, 6:28 PM
 */

#include "Particle.h"
#include "Field.h"
#include <GL/gl.h>
#include <cstdlib>
#include <iostream>

using namespace std;

Particle::Particle()
	: mColor(0)
{
}

Particle::Particle(const Particle& particle)
	: GameObject(particle), mColor(particle.mColor), mVelocity(particle.mVelocity)
{
}

Particle::Particle(int x, int y, unsigned int color)
	: mColor(color)
{
	Matrix4x4 m;
	this->setMatrix(m.translateMatrix(x, y, 0));
	float vx = float((rand()%128 - 64) / 255.0f);
	float vy = float((rand()%128) / 255.0f);
	this->mVelocity = (Vector3(vx, vy, 0.0f)).unit();
}

Particle::~Particle()
{
}

void Particle::update(float elapsed)
{
	Vector3 gravity(0, -0.02f, 0.0f);
	this->mVelocity += gravity;
	this->mVelocity = this->mVelocity.unit();
	this->mLocal.m[3][0] += (this->mVelocity.x()*0.15f);
	this->mLocal.m[3][1] += (this->mVelocity.y()*0.15f);
	this->mLocal.m[3][2] += (this->mVelocity.z()*0.15f);
}

void Particle::doRender()
{
	glColor4ub(this->mColor&0xff, (this->mColor>>8)&0xff, 
			(this->mColor>>16)&0xff, (this->mColor>>24)&0xff);

	float s = 0.5f;
	glBegin(GL_QUADS);							// Draw A Quad
		glVertex3f( s, s,-s);					// Top Right Of The Quad (Top)
		glVertex3f(-s, s,-s);					// Top Left Of The Quad (Top)
		glVertex3f(-s, s, s);					// Bottom Left Of The Quad (Top)
		glVertex3f( s, s, s);					// Bottom Right Of The Quad (Top)

		glVertex3f( s,-s, s);					// Top Right Of The Quad (Bottom)
		glVertex3f(-s,-s, s);					// Top Left Of The Quad (Bottom)
		glVertex3f(-s,-s,-s);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( s,-s,-s);					// Bottom Right Of The Quad (Bottom)

		glVertex3f( s, s, s);					// Top Right Of The Quad (Front)
		glVertex3f(-s, s, s);					// Top Left Of The Quad (Front)
		glVertex3f(-s,-s, s);					// Bottom Left Of The Quad (Front)
		glVertex3f( s,-s, s);					// Bottom Right Of The Quad (Front)

		glVertex3f( s,-s,-s);					// Top Right Of The Quad (Back)
		glVertex3f(-s,-s,-s);					// Top Left Of The Quad (Back)
		glVertex3f(-s, s,-s);					// Bottom Left Of The Quad (Back)
		glVertex3f( s, s,-s);					// Bottom Right Of The Quad (Back)

		glVertex3f(-s, s, s);					// Top Right Of The Quad (Left)
		glVertex3f(-s, s,-s);					// Top Left Of The Quad (Left)
		glVertex3f(-s,-s,-s);					// Bottom Left Of The Quad (Left)
		glVertex3f(-s,-s, s);					// Bottom Right Of The Quad (Left)

		glVertex3f( s, s,-s);					// Top Right Of The Quad (Right)
		glVertex3f( s, s, s);					// Top Left Of The Quad (Right)
		glVertex3f( s,-s, s);					// Bottom Left Of The Quad (Right)
		glVertex3f( s,-s,-s);					// Bottom Right Of The Quad (Right)
	glEnd();									// Done Drawing The Quad
}

void Particle::set(int x, int y, unsigned int color)
{
	Matrix4x4 m;
	m.translateMatrix(x, y, 0);
	this->setMatrix(m);
	this->mColor = color;
	this->mVelocity = (Vector3(float(rand()%255), float(rand()%255), 0.0f)).unit();
}
