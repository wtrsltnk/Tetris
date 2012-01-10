/* 
 * File:   Block.h
 * Author: wouter
 *
 * Created on December 30, 2011, 8:33 AM
 */

#ifndef BLOCK_H
#define	BLOCK_H

#include "GameObject.h"

// This is from imgui, recast navigation code
inline unsigned int makeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255)
{
	return (r) | (g << 8) | (b << 16) | (a << 24);
}

class Field;

class Block : public GameObject
{
public:
	Block(Field* field, int size);
	Block(const Block& block);
	virtual ~Block();
	
	void rotate();
	void step();
	bool canStep(int addX, int addY);
	bool canRotate();
	
	unsigned int isBlock(int x, int y);
	void moveLeft();
	void moveRight();
	void moveDown();
	int x() const { return this->mX; }
	int y() const { return this->mY; }
	int size() const { return this->mSize; }

	virtual void doRender();
	void renderBlock(bool useColor);

protected:
	void setBlock(int x, int y, bool value = true);
	void setColor(unsigned int color);
	
private:
	Field* mField;
	int mSize;
	unsigned int **mSubBlocks;
	int mX, mY;

public:
	static Block* randomBlock(Field* field);
	
};

#endif	/* BLOCK_H */

