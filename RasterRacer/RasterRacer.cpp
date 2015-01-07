/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001


#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

#include "ZMap.h"
#include <iostream>
#include <vector>
#include <array>
#include "RoadUpdater.h"
#include "RoadScanLine.h"

//Screen Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_FPS = 60;
const float HORIZON_HEIGHT = SCREEN_HEIGHT*0.50f;

float playerX = 0.0f;
float distance = 0.0f;
float speedAmount = 0.018f;
float maxSpeed = 1.0f;
float speed = 0.0f;
float roadTextureOffset = 0.0f;

void UpdateTextureOffset(float speed)
{
	roadTextureOffset += speed;
	if (roadTextureOffset > 4.0f)
	{
		roadTextureOffset -= 4.0f;
	}
}

static bool keys[255];

std::unique_ptr<RoadUpdater> roadUpdater;
void InitRacer()
{
	ZMap::Initialise(300);
	roadUpdater.reset(new RoadUpdater((int)HORIZON_HEIGHT, SCREEN_HEIGHT, SCREEN_WIDTH, ZMap::GetInstance()));
}

void TerminateRacer()
{
	ZMap::Terminate();
}

bool initGL()
{
	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.75f, 1.f);

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	for (int i = 0; i < 255; ++i)
		keys[i] = false;

	return true;
}

void update()
{
	
	if (keys['a'])
		playerX -= 5.1f;
	if (keys['d'])
		playerX += 5.1f;

	if (keys['w'])
		speed += speedAmount;
	if (keys['s'])
		speed -= (speedAmount * 1.3f);
	//else
	//	speed = 0.0f;
	
	speed -= (speedAmount * 0.1f);
	speed = std::max(speed, 0.0f);
	speed = std::min(speed, maxSpeed);
	
	roadUpdater->Update(speed, playerX);

}

void render()
{
	const float h = float(ZMap::GetInstance().mHorizonHeight);
	const float w = float(SCREEN_WIDTH);
	//Clear color buffer5
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.6f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 0.9f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();

	static const float grassColour[2][3] =
	{
		{ 0.2f, 1.0f, 0.2f }, { 0.2f, 1.0f, 0.2f }
		//{0.1f,0.96f,0.2f}
	};
	static const float roadColour[2][3] =
	{
		{ 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f },
		//{ 0.6f, 0.6f, 0.6f }
	};

	int maxScanLines = 0;
	auto roadParts = roadUpdater->GetScanLines(maxScanLines);
	for (int i = maxScanLines-1; i >= 0; --i)
	{
		if (roadParts[i].draw)
		{

			glColor3f(grassColour[roadParts[i].roadFrame][0], grassColour[roadParts[i].roadFrame][1], grassColour[roadParts[i].roadFrame][2]);


			float y = ((float(h - roadParts[i].screenYInt) / h));
			float maxY = y - ((float(roadParts[i].offsetY) / h));
			glBegin(GL_QUADS);
			glVertex2f(-1.0f, y);
			glVertex2f(1.0f, y);
			glVertex2f(1.0f, maxY);
			glVertex2f(-1.0f, maxY);
			glEnd();

			glPushMatrix();
			
			glTranslatef(-1.0f + ((roadParts[i].screenX / w) * 2.0f), 0.0f, 0.0f);
			glScalef(roadParts[i].scale, 1.0f, 1.0f);
			glColor3f(roadColour[roadParts[i].roadFrame][0], roadColour[roadParts[i].roadFrame][1], roadColour[roadParts[i].roadFrame][2]);
			
			for (int j = 1; j <= roadParts[i].offsetY; ++j)
			{

				float minX = -1.0f;
				float maxX = 1.0f;
				y = ((float(h -( float(roadParts[i].screenYInt) + (j-1))) / h));
				maxY = y - (1 / h);
				glBegin(GL_QUADS);
				glVertex2f(minX, y);
				glVertex2f(maxX, y);
				glVertex2f(maxX, maxY);
				glVertex2f(minX, maxY);
				glEnd();
			}
			
			glPopMatrix();
		}
		
	}
	
	glutSwapBuffers();
}

void runMainLoop(int val);


int main(int argc, char* args[])
{
	InitRacer();

	//Initialize FreeGLUT
	glutInit(&argc, args);

	//Create OpenGL 2.1 context
	glutInitContextVersion(2, 1);

	//Create Double Buffered Window
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("OpenGL");

	//Do post window/context creation initialization
	if (!initGL())
	{
		printf("Unable to initialize graphics library!\n");
		return 1;
	}

	//Set rendering function
	glutDisplayFunc(render);

	//Set main loop
	glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);

	//Start GLUT main loop
	glutMainLoop();

	TerminateRacer();
	return 0;
}

void keyboardPressed(unsigned char c, int , int )
{
	keys[c] = true;
}

void keyboardUp(unsigned char c, int, int)
{
	keys[c] = false;
}

void runMainLoop(int val)
{
	//Frame logic
	update();
	render();

	//Run frame one more time
	glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
	glutKeyboardFunc(keyboardPressed);
	glutKeyboardUpFunc(keyboardUp);
}
