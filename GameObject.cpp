/* 
 * File:   GameObject.cpp
 * Author: wouter
 * 
 * Created on December 29, 2011, 3:04 PM
 */

#include "GameObject.h"
#include <GL/gl.h>

GameObject::GameObject()
{
}

GameObject::GameObject(const GameObject& obj)
	: mLocal(obj.mLocal)
{
}

GameObject::~GameObject()
{
}

void GameObject::render()
{
	this->preRender();
	this->doRender();
	this->postRender();
}

void GameObject::setMatrix(const Matrix4x4& m)
{
	this->mLocal = m;
}

void GameObject::move(const Matrix4x4& m)
{
	this->mLocal = this->mLocal * m;
}

void GameObject::preRender()
{
	glPushMatrix();
	this->mLocal.glMultiply();
}

void GameObject::postRender()
{
	glPopMatrix();
}
