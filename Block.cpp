/* 
 * File:   Block.cpp
 * Author: wouter
 * 
 * Created on December 30, 2011, 8:33 AM
 */

#include "Block.h"
#include "Field.h"
#include <GL/gl.h>
#include <iostream>
#include <cstdlib>

using namespace std;

Block::Block(Field* field, int size)
	: mField(field), mSize(size), mX(FIELD_WIDTH/2-1), mY(FIELD_HEIGHT-this->mSize)
{
	this->mSubBlocks = new unsigned int*[this->mSize];
	for (int x = 0; x < this->mSize; x++)
	{
		this->mSubBlocks[x] = new unsigned int[this->mSize];
		for (int y = 0; y < this->mSize; y++)
			this->mSubBlocks[x][y] = makeColor(0, 0, 0, 0);
	}

}

Block::Block(const Block& block)
	: mField(block.mField), mSize(block.mSize), mX(block.mX), mY(block.mY)
{
	this->mSubBlocks = new unsigned int*[this->mSize];
	for (int x = 0; x < this->mSize; x++)
	{
		this->mSubBlocks[x] = new unsigned int[this->mSize];
		for (int y = 0; y < this->mSize; y++)
			this->mSubBlocks[x][y] = block.mSubBlocks[x][y];
	}
}

Block::~Block()
{
}

void Block::doRender()
{
	glPushMatrix();
	glTranslatef(this->mX, this->mY, 0.0f);
	this->renderBlock(true);
	glPopMatrix();
	
	glColor3f(0, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
	glTranslatef(this->mX, this->mY, 0.0f);
	this->renderBlock(false);
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Block::renderBlock(bool useColor)
{
	float s = 0.5f;
	for (int x = 0; x < this->mSize; x++)
	{
		for (int y = 0; y < this->mSize; y++)
		{
			if (this->mSubBlocks[x][y] != 0)
			{
				if (useColor)
					glColor4ub(this->mSubBlocks[x][y]&0xff, (this->mSubBlocks[x][y]>>8)&0xff, 
						(this->mSubBlocks[x][y]>>16)&0xff, (this->mSubBlocks[x][y]>>24)&0xff);

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
				glEnd();											// Done Drawing The Quad
			}
			glTranslatef(0.0f, 1.0f, 0.0f);
		}
		glTranslatef(1.0f, -1.0f * this->mSize, 0.0f);
	}
}

void Block::rotate()
{
	unsigned int tmp[this->mSize][this->mSize];
	for (int i = this->mSize-1; i >= 0; --i)
		for (int j = 0; j < this->mSize; ++j)
			tmp[j][this->mSize-1-i] = this->mSubBlocks[i][j];
		 
	for (int x = 0; x < this->mSize; x++)
		for (int y = 0; y < this->mSize; y++)
			this->mSubBlocks[x][y] = tmp[x][y];
}

void Block::step()
{
	this->mY -= 1;
}

bool Block::canStep(int addX, int addY)
{
	for (int x = 0; x < this->mSize; x++)
	{
		for (int y = 0; y < this->mSize; y++)
		{
			if (this->mSubBlocks[x][y] != 0)
			{
				if (this->mY+y+addY < 0)
					return false;
				if (this->mY+y+addY >= FIELD_HEIGHT)
					return false;
				if (this->mX+x+addX < 0)
					return false;
				if (this->mX+x+addX >= FIELD_WIDTH)
					return false;
				if (this->mField->isBlock(this->mX+x+addX, this->mY+y+addY) != makeColor(0, 0, 0, 0))
					return false;
			}
		}
	}
	return true;
}

bool Block::canRotate()
{
	Block b(*this);
	b.rotate();
	return b.canStep(0, 0);
}

unsigned int Block::isBlock(int x, int y)
{
	return this->mSubBlocks[x][y];
}

void Block::moveLeft()
{
	if (this->canStep(-1, 0))
		this->mX -= 1;
}
void Block::moveRight()
{
	if (this->canStep(1, 0))
		this->mX += 1;
}

void Block::moveDown()
{
	if (this->canStep(0, -1))
		this->mY -= 1;
}

void Block::setBlock(int x, int y, bool value)
{
	this->mSubBlocks[x][y] = value;
}

void Block::setColor(unsigned int color)
{
	for (int x = 0; x < this->mSize; x++)
	{
		for (int y = 0; y < this->mSize; y++)
		{
			if (this->mSubBlocks[x][y] != 0)
				this->mSubBlocks[x][y] = color;
		}
	}
}

Block* Block::randomBlock(Field* field)
{
	Block* result = 0;
	switch (rand() % 7)
	{
		case 0:	// I
		{
			result = new Block(field, 4);
			result->setBlock(2, 0);
			result->setBlock(2, 1);
			result->setBlock(2, 2);
			result->setBlock(2, 3);
			result->setColor(makeColor(255, 255, 0, 0));
			break;
		}
		case 1:	// O
		{
			result = new Block(field, 4);
			result->setBlock(1, 1);
			result->setBlock(1, 2);
			result->setBlock(2, 1);
			result->setBlock(2, 2);
			result->setColor(makeColor(255, 0, 255, 0));
			break;
		}
		case 2:	// S
		{
			result = new Block(field, 3);
			result->setBlock(2, 1);
			result->setBlock(1, 1);
			result->setBlock(1, 2);
			result->setBlock(0, 2);
			result->setColor(makeColor(0, 0, 255, 0));
			break;
		}
		case 3:	// T
		{
			result = new Block(field, 3);
			result->setBlock(0, 1);
			result->setBlock(1, 1);
			result->setBlock(1, 2);
			result->setBlock(2, 1);
			result->setColor(makeColor(0, 255, 0, 0));
			break;
		}
		case 4:	// Z
		{
			result = new Block(field, 3);
			result->setBlock(0, 1);
			result->setBlock(1, 1);
			result->setBlock(1, 2);
			result->setBlock(2, 2);
			result->setColor(makeColor(0, 128, 255));
			break;
		}
		case 5:	// Mirrored L
		{
			result = new Block(field, 4);
			result->setBlock(2, 0);
			result->setBlock(2, 1);
			result->setBlock(2, 2);
			result->setBlock(1, 2);
			result->setColor(makeColor(255, 128, 0, 0));
			break;
		}
		case 6:	// L
		{
			result = new Block(field, 4);
			result->setBlock(1, 0);
			result->setBlock(1, 1);
			result->setBlock(1, 2);
			result->setBlock(2, 2);
			result->setColor(makeColor(255, 0, 0, 0));
			break;
		}
	}
	return result;
}
