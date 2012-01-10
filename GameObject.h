/* 
 * File:   GameObject.h
 * Author: wouter
 *
 * Created on December 29, 2011, 3:04 PM
 */

#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H

#include "Matrix4x4.h"

class GameObject
{
public:
	GameObject();
	GameObject(const GameObject& obj);
	virtual ~GameObject();
	
	void render();
	
	void setMatrix(const Matrix4x4& m);
	void move(const Matrix4x4& m);

protected:
	void preRender();
	virtual void doRender() = 0;
	void postRender();
	
public:
	Matrix4x4 mLocal;

};

#endif	/* GAMEOBJECT_H */

