/* 
 * File:   main.cpp
 * Author: wouter
 *
 * Created on December 27, 2011, 12:03 AM
 */

#include <cstdlib>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <SFML/Window.hpp>

#include "Field.h"
#include "Particle.h"
#include "uiFont.h"
#include "Block.h"

using namespace std;

#define UPLEVEL_COUNT 20
#define UPLEVEL_SPEEDINCREASE 0.9f

static int windowWidth = 600;
static int windowHeight = 700;

static Block* currentBlock = 0;
static Block* nextBlock = 0;
static Field* field = 0;
static float speed = 0.5f;
static int score = 0;
static ui::Font scoreFont;
static string scoreText;
static int level = 0;
static int levelCounter = 0;
static string levelText;
static vector<Particle*> particles;

static ui::Font pauseFont;
bool gameInPause = true;
bool gameIsDead = false;
static string deadText;

static ui::Font headerFont;

void setScore(int s)
{
	score = s;
	stringstream ss;
	ss << "Score: " << s;
	scoreText = ss.str();
}

void setLevel(int l)
{
	level = l;
	stringstream ss;
	ss << "Level: " << l;
	levelText = ss.str();
}

void onResize(int w, int h)
{
	if (h <= 0)
		h = 1;
	
	glViewport(0, 0, w, h);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, float(w) / float(h), 1.0f, 500.0f);
}

void onInitialize()
{
	scoreFont.initializeFont("ASTRONAU.TTF", 32);
	pauseFont.initializeFont("ASTRONAU.TTF", 56);
	headerFont.initializeFont("ASTRONAU.TTF", 72);
	srand(time(NULL));
	
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	field = new Field();
	currentBlock = Block::randomBlock(field);
	nextBlock = Block::randomBlock(field);
	
	setScore(0);
	setLevel(1);
}

void doStep()
{
	if (currentBlock != 0)
	{
		if (currentBlock->canStep(0, -1))
		{
			currentBlock->step();
		}
		else
		{
			bool deleteAll = false;
			for (vector<Particle*>::iterator i = particles.begin(); i != particles.end(); ++i)
				if ((*i)->mLocal.m[3][1] < 0)
					deleteAll = true;

			if (deleteAll == true)
			{
				while (particles.empty() == false)
				{
					Particle* tmp = particles.back();
					particles.pop_back();
					delete tmp;
				}
				cout << particles.size() << endl;
			}
			
			int fullRowCount = field->addBlocks(currentBlock, particles);
			if (fullRowCount == -1)
			{
				gameInPause = true;
				gameIsDead = true;
				stringstream ss;
				ss << "You're score: " << score;
				deadText = ss.str();
			}
			else if (fullRowCount == 1)
				setScore(score + 10 * level);
			else if (fullRowCount == 2)
				setScore(score + 25 * level);
			else if (fullRowCount == 3)
				setScore(score + 50 * level);
			else if (fullRowCount == 4)
				setScore(score + 100 * level);
			delete currentBlock;
			currentBlock = nextBlock;
			nextBlock = Block::randomBlock(field);
			levelCounter++;
			if (levelCounter > UPLEVEL_COUNT)
			{
				setLevel(level+1);
				levelCounter = 0;
				speed *= UPLEVEL_SPEEDINCREASE;
				setScore(score + 10);
			}
		}
	}
}

void onRender(float elapsed)
{
	static float prevElapsed = 0;
	
	for (vector<Particle*>::iterator i = particles.begin(); i != particles.end(); ++i)
		(*i)->update(elapsed-prevElapsed);
	
	if (elapsed - prevElapsed > speed)
	{
		if (gameInPause == false)
			doStep();
		prevElapsed = elapsed;
	}
	
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Apply some transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -55.0f);
	glScalef(10, 10, 10);
//	glRotatef(elapsed * 50, 1.0f, 0.0f, 0.0f);
	glRotatef(elapsed * 30, 0.0f, 1.0f, 0.0f);
	glColor3f(0, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glTranslatef(-2.0f, -2.0f, 0.0f);
	nextBlock->renderBlock(true);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
	
	glTranslatef(-10.0f, -20.0f, -25.0f);
	field->render();
	currentBlock->render();
	
	for (vector<Particle*>::iterator i = particles.begin(); i != particles.end(); ++i)
		(*i)->render();
	
	// Rendering 2D stuff
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(-viewport[2]/2, viewport[2]/2, -viewport[3]/2, viewport[3]/2, -10.0f, 10.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	float length = headerFont.getTextLength("Tetris");
	headerFont.drawText(-length/2, viewport[3]/2-48, "Tetris", ui::RGBA(0, 155, 255));
	
	scoreFont.drawText(-viewport[2]/2+10, -viewport[3]/2+30, levelText.c_str(), ui::RGBA(255, 255, 255, 255));
	scoreFont.drawText(-viewport[2]/2+10, -viewport[3]/2+10, scoreText.c_str(), ui::RGBA(255, 255, 255, 255));
	
	if (gameIsDead)
	{
		length = pauseFont.getTextLength(deadText.c_str());
		pauseFont.drawText(-length/2, 0, deadText.c_str(), ui::RGBA(0, 255, 0));
	}
	else if (gameInPause)
	{
		length = pauseFont.getTextLength("Press space to start");
		pauseFont.drawText(-length/2, 0, "Press space to start", ui::RGBA(0, 255, 0));
	}
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void onKeyPress(int key)
{
	if (gameInPause == false)
	{
		if (key == sf::Key::Left)
			currentBlock->moveLeft();

		if (key == sf::Key::Right)
			currentBlock->moveRight();

		if (key == sf::Key::Down)
		{
			currentBlock->moveDown();
		}

		if (key == sf::Key::Up)
			if (currentBlock->canRotate())
				currentBlock->rotate();
	}
	
	if (key == sf::Key::Space)
	{
		gameInPause = !gameInPause;
		if (gameInPause == false)
		{
			if (gameIsDead)
				field->reset();
			gameIsDead = false;
		}
	}
}

void onKeyReleased(int key)
{
	
}

int main(int argc, char** argv)
{
	sf::WindowSettings settings;
	settings.DepthBits         = 24; // Request a 24 bits depth buffer
	settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
	settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing
	sf::Window app(sf::VideoMode(windowWidth, windowHeight, 32), "TETRIS", sf::Style::Close, settings);
	
	app.SetFramerateLimit(60);

    // Create a clock for measuring time elapsed
    sf::Clock clock;

	onResize(windowWidth, windowHeight);
	onInitialize();
	
	while (app.IsOpened())
	{
		sf::Event Event;
		while (app.GetEvent(Event))
		{
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                app.Close();

            // Escape key : exit
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                app.Close();
            
			if (Event.Type == sf::Event::KeyPressed)
				onKeyPress(Event.Key.Code);
            
			if (Event.Type == sf::Event::KeyReleased)
				onKeyReleased(Event.Key.Code);

            // Resize event : adjust viewport
			if (Event.Type == sf::Event::Resized)
				onResize(Event.Size.Width, Event.Size.Height);
		}

		// Set the active window before using OpenGL commands
		// It's useless here because active window is always the same,
		// but don't forget it if you use multiple windows or controls
		app.SetActive();

		onRender(clock.GetElapsedTime());
		
		// Finally, display rendered frame on screen
		app.Display();
	}
	
	// Cleanup
	while (particles.empty() == false)
	{
		Particle* tmp = particles.back();
		particles.pop_back();
		delete tmp;
	}
	delete currentBlock;
	delete nextBlock;
	delete field;
	
	return 0;
}

