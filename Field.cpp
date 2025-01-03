/*
 * File:   Field.cpp
 * Author: wouter
 *
 * Created on December 30, 2011, 5:44 PM
 */

#include "Field.h"
#include "Block.h"
#include "Particle.h"

#include <glad/glad.h>

using namespace std;

Field::Field()
{
    for (int x = 0; x < FIELD_WIDTH; x++)
        for (int y = 0; y < FIELD_HEIGHT; y++)
            this->mFieldBlocks[x][y] = makeColor(0, 0, 0, 0);
}

Field::~Field()
{
}

void Field::render()
{
    glColor3f(0, 0, 1);
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(FIELD_WIDTH - 0.5f, -0.5f, 0.5f);

    glVertex3f(FIELD_WIDTH - 0.5f, -0.5f, 0.5f);
    glVertex3f(FIELD_WIDTH - 0.5f, FIELD_HEIGHT + 0.5f, 0.5f);

    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, FIELD_HEIGHT + 0.5f, 0.5f);

    glVertex3f(-0.5f, FIELD_HEIGHT + 0.5f, 0.5f);
    glVertex3f(FIELD_WIDTH - 0.5f, FIELD_HEIGHT + 0.5f, 0.5f);
    glEnd();

    glColor3f(1, 0, 0);
    float s = 0.5f;
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT; y++)
        {
            if (this->mFieldBlocks[x][y] != 0)
            {
                glColor4ub(this->mFieldBlocks[x][y] & 0xff, (this->mFieldBlocks[x][y] >> 8) & 0xff,
                           (this->mFieldBlocks[x][y] >> 16) & 0xff, (this->mFieldBlocks[x][y] >> 24) & 0xff);

                glBegin(GL_QUADS);     // Draw A Quad
                glVertex3f(s, s, -s);  // Top Right Of The Quad (Top)
                glVertex3f(-s, s, -s); // Top Left Of The Quad (Top)
                glVertex3f(-s, s, s);  // Bottom Left Of The Quad (Top)
                glVertex3f(s, s, s);   // Bottom Right Of The Quad (Top)

                glVertex3f(s, -s, s);   // Top Right Of The Quad (Bottom)
                glVertex3f(-s, -s, s);  // Top Left Of The Quad (Bottom)
                glVertex3f(-s, -s, -s); // Bottom Left Of The Quad (Bottom)
                glVertex3f(s, -s, -s);  // Bottom Right Of The Quad (Bottom)

                glVertex3f(s, s, s);   // Top Right Of The Quad (Front)
                glVertex3f(-s, s, s);  // Top Left Of The Quad (Front)
                glVertex3f(-s, -s, s); // Bottom Left Of The Quad (Front)
                glVertex3f(s, -s, s);  // Bottom Right Of The Quad (Front)

                glVertex3f(s, -s, -s);  // Top Right Of The Quad (Back)
                glVertex3f(-s, -s, -s); // Top Left Of The Quad (Back)
                glVertex3f(-s, s, -s);  // Bottom Left Of The Quad (Back)
                glVertex3f(s, s, -s);   // Bottom Right Of The Quad (Back)

                glVertex3f(-s, s, s);   // Top Right Of The Quad (Left)
                glVertex3f(-s, s, -s);  // Top Left Of The Quad (Left)
                glVertex3f(-s, -s, -s); // Bottom Left Of The Quad (Left)
                glVertex3f(-s, -s, s);  // Bottom Right Of The Quad (Left)

                glVertex3f(s, s, -s);  // Top Right Of The Quad (Right)
                glVertex3f(s, s, s);   // Top Left Of The Quad (Right)
                glVertex3f(s, -s, s);  // Bottom Left Of The Quad (Right)
                glVertex3f(s, -s, -s); // Bottom Right Of The Quad (Right)
                glEnd();               // Done Drawing The Quad
            }
            glTranslatef(0.0f, 1.0f, 0.0f);
        }
        glTranslatef(1.0f, -1.0f * FIELD_HEIGHT, 0.0f);
    }
    glPopMatrix();
    glColor3f(0, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT; y++)
        {
            if (this->mFieldBlocks[x][y])
            {
                glBegin(GL_QUADS);     // Draw A Quad
                glVertex3f(s, s, -s);  // Top Right Of The Quad (Top)
                glVertex3f(-s, s, -s); // Top Left Of The Quad (Top)
                glVertex3f(-s, s, s);  // Bottom Left Of The Quad (Top)
                glVertex3f(s, s, s);   // Bottom Right Of The Quad (Top)

                glVertex3f(s, -s, s);   // Top Right Of The Quad (Bottom)
                glVertex3f(-s, -s, s);  // Top Left Of The Quad (Bottom)
                glVertex3f(-s, -s, -s); // Bottom Left Of The Quad (Bottom)
                glVertex3f(s, -s, -s);  // Bottom Right Of The Quad (Bottom)

                glVertex3f(s, s, s);   // Top Right Of The Quad (Front)
                glVertex3f(-s, s, s);  // Top Left Of The Quad (Front)
                glVertex3f(-s, -s, s); // Bottom Left Of The Quad (Front)
                glVertex3f(s, -s, s);  // Bottom Right Of The Quad (Front)

                glVertex3f(s, -s, -s);  // Top Right Of The Quad (Back)
                glVertex3f(-s, -s, -s); // Top Left Of The Quad (Back)
                glVertex3f(-s, s, -s);  // Bottom Left Of The Quad (Back)
                glVertex3f(s, s, -s);   // Bottom Right Of The Quad (Back)

                glVertex3f(-s, s, s);   // Top Right Of The Quad (Left)
                glVertex3f(-s, s, -s);  // Top Left Of The Quad (Left)
                glVertex3f(-s, -s, -s); // Bottom Left Of The Quad (Left)
                glVertex3f(-s, -s, s);  // Bottom Right Of The Quad (Left)

                glVertex3f(s, s, -s);  // Top Right Of The Quad (Right)
                glVertex3f(s, s, s);   // Top Left Of The Quad (Right)
                glVertex3f(s, -s, s);  // Bottom Left Of The Quad (Right)
                glVertex3f(s, -s, -s); // Bottom Right Of The Quad (Right)
                glEnd();               // Done Drawing The Quad
            }
            glTranslatef(0.0f, 1.0f, 0.0f);
        }
        glTranslatef(1.0f, -1.0f * FIELD_HEIGHT, 0.0f);
    }
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int Field::addBlocks(Block *block, vector<Particle *> &rows)
{
    for (int x = 0; x < block->size(); x++)
    {
        for (int y = 0; y < block->size(); y++)
        {
            if (block->isBlock(x, y) != makeColor(0, 0, 0, 0))
            {
                if (this->mFieldBlocks[block->x() + x][block->y() + y] != makeColor(0, 0, 0, 0))
                    return -1;

                this->mFieldBlocks[block->x() + x][block->y() + y] = block->isBlock(x, y);
            }
        }
    }

    int fullRowCount = 0;
    for (int r = 0; r < FIELD_HEIGHT; r++)
    {
        bool full = true;
        for (int c = 0; c < FIELD_WIDTH; c++)
            if (this->mFieldBlocks[c][r] == 0)
                full = false;
        if (full == true)
        {
            for (int x = 0; x < FIELD_WIDTH; x++)
            {
                rows.push_back(new Particle(x, r, this->mFieldBlocks[x][r]));
                for (int y = r; y < FIELD_HEIGHT - 1; y++)
                    this->mFieldBlocks[x][y] = this->mFieldBlocks[x][y + 1];
            }
            r--;
            fullRowCount++;
        }
    }
    return fullRowCount;
}

unsigned int Field::isBlock(int x, int y)
{
    return this->mFieldBlocks[x][y];
}

void Field::reset()
{
    for (int x = 0; x < FIELD_WIDTH; x++)
        for (int y = 0; y < FIELD_HEIGHT; y++)
            this->mFieldBlocks[x][y] = makeColor(0, 0, 0, 0);
}
