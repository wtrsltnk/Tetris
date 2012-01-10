/* 
 * File:   Field.h
 * Author: wouter
 *
 * Created on December 30, 2011, 5:44 PM
 */

#ifndef FIELD_H
#define	FIELD_H

#include <vector>

#define FIELD_WIDTH 20
#define FIELD_HEIGHT 40

class Block;
class Particle;

class Field
{
public:
	Field();
	virtual ~Field();
	
	void render();
	
	int addBlocks(Block* block, std::vector<Particle*>& rows);
	unsigned int isBlock(int x, int y);
	void reset();
	
private:
	unsigned int mFieldBlocks[FIELD_WIDTH][FIELD_HEIGHT];

};

#endif	/* FIELD_H */

