/* 
 * File:   Particle.h
 * Author: wouter
 *
 * Created on January 2, 2012, 6:28 PM
 */

#ifndef PARTICLE_H
#define	PARTICLE_H

#include "GameObject.h"
#include "Vector3.h"

class Particle : public GameObject
{
public:
	Particle();
	Particle(const Particle& particle);
	Particle(int x, int y, unsigned int color);
	virtual ~Particle();

	void update(float elapsed);
	virtual void doRender();
	void set(int x, int y, unsigned int color);
	
private:
	unsigned int mColor;
	Vector3 mVelocity;

};

#endif	/* PARTICLE_H */

