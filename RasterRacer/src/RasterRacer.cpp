/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001


#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

//#include "ZMap.h"
#include <iostream>
#include <vector>
#include <array>
//#include "RoadUpdater.h"
//#include "RoadScanLine.h"

//Screen Constants
//

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int SCREEN_FPS = 60;
/*
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
*/
static bool keys[255];
//std::unique_ptr<ZMap> zMap;
//std::unique_ptr<RoadUpdater> roadUpdater;


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
/*	
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
*/
}

void render()
{
		/*
	const float h = float(HORIZON_HEIGHT);
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
		{ 0.7f, 0.7f, 0.7f }, { 0.7f, 0.7f, 0.7f }
		//{ 0.6f, 0.6f, 0.6f }
	};

	int maxScanLines = 0;
	auto roadParts = roadUpdater->GetScanLines(maxScanLines);
	float lastMinRoadX = 0.0f;
	float lastMaxRoadX = 0.0f;
	float lastRoadY = -1.0f;
	float lastMinLineX = 0.0f;
	float lastMaxLineX = 0.0f;
	for (int i = maxScanLines-1; i >= 0; --i)
	{
		if (roadParts[i].draw)
		{

			glColor3f(grassColour[roadParts[i].roadFrame][0], grassColour[roadParts[i].roadFrame][1], grassColour[roadParts[i].roadFrame][2]);


			float y = ((float(h - roadParts[i].screenYInt) / h));
			

			glBegin(GL_QUADS);
			glVertex2f(-1.0f, y);
			glVertex2f(1.0f, y);
			glVertex2f(1.0f, lastRoadY);
			glVertex2f(-1.0f, lastRoadY);
			glEnd();

			glColor3f(roadColour[roadParts[i].roadFrame][0], roadColour[roadParts[i].roadFrame][1], roadColour[roadParts[i].roadFrame][2]);

			const float centerX = -1.0f + ((roadParts[i].screenX / w) * 2.0f);
			const float minX = centerX - roadParts[i].scale;
			const float maxX = centerX + roadParts[i].scale;
			glBegin(GL_QUADS);
			glVertex2f(minX, y);
			glVertex2f(maxX, y);
			glVertex2f(lastMaxRoadX, lastRoadY);
			glVertex2f(lastMinRoadX, lastRoadY);
			glEnd();
			lastMinRoadX = minX;
			lastMaxRoadX = maxX;

			const float lineScale = roadParts[i].scale * 0.02f;
			const float lineminX = centerX - lineScale;
			const float linemaxX = centerX + lineScale;
			if (roadParts[i].roadFrame)
			{

				glBegin(GL_QUADS);
				
				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex2f(lineminX, y);
				glVertex2f(linemaxX, y);
				glVertex2f(lastMaxLineX, lastRoadY);
				glVertex2f(lastMinLineX, lastRoadY);
				glEnd();
			}
			lastMinLineX = lineminX;
			lastMaxLineX = linemaxX;
			lastRoadY = y;
		}
		
	}
	
	glutSwapBuffers();
*/
}

void runMainLoop(int val);


int main(int argc, char* args[])
{
//	zMap.reset(new ZMap(int(HORIZON_HEIGHT)));
//	roadUpdater.reset(new RoadUpdater((int)HORIZON_HEIGHT, SCREEN_HEIGHT, SCREEN_WIDTH, *zMap));

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

void update2();
void render2();

void runMainLoop(int val)
{
	//Frame logic
//	update();
//	render();
	update2();
	render2();
	//Run frame one more time
	glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
	glutKeyboardFunc(keyboardPressed);
	glutKeyboardUpFunc(keyboardUp);
}

	struct Line {
		float x;
		float y;
		float z;
	};
	std::vector<Line> lines;
	

	auto startPos = 0.0f;

void update2() {

	if (keys['w']) {

		startPos++;
	} else if (keys['s']) {

		startPos--;
	}
}

void render2() {


   const auto camDepth = 0.84f;
   const auto segL = 200;
   const auto roadW = 2000;
   const auto camX = 0.0f;
   const auto width = SCREEN_WIDTH;
   const auto height = SCREEN_HEIGHT;

   static bool first = true;
	if (first) {

		
		for (auto i = 0u; i < 200; ++i) {
			
				lines.push_back({0.01f,0.0f,float(i*segL)});
		}
		for (auto i = 0u; i < 300; ++i) {
			
				lines.push_back({-0.01f,0.0f,float(i*segL)});
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.6f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 0.9f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();

	glColor3f(0.0f,1.0f,0.0f);	

	glBegin(GL_QUADS);
		glVertex2f(-1.0f, 0.2f);//SCREEN_HEIGHT);
		glVertex2f(1.0f, 0.2f);//SCREEN_HEIGHT);
		glVertex2f(1.0f, -1.0f);//SCREEN_HEIGHT);
		glVertex2f(-1.0f, -1.0f);//SCREEN_HEIGHT);
	glEnd();


	glColor3f(0.5f,0.5f,0.5f);

	auto lastX = 0.0f;
	auto lastY = 0.0f;
	auto lastW = 0.0f;
	const auto camH = 1000.f;

	auto x = 0.0f;
	auto dx = 0.0f;
	for (int i = startPos; i < startPos+300; ++i) {
        
		auto& line = lines[i];
	
		x+=dx;
		dx+=line.x;
	    	
		const auto camZ = startPos*segL - (i >= lines.size() ? lines.size() * segL : 0.0f);
		const auto scale = camDepth / ( line.z-camZ);
		const auto worldX = width - (2.0f * ((1+scale * (x-camX)) * width/2));
		const auto worldY = (1+scale*(line.y-camH)) * height/2;	
		const auto worldW = scale * roadW * width/2;
		
		auto renderX = worldX;
		auto renderY =  (worldY / 1600.0f);
		auto renderW =  (worldW / 600.0f);
		if (first) {

			printf("%0.4f: %.4f, %.4f, %.4f |||| %.4f, %.4f, %.4f\n", scale, worldX, worldY, worldW, renderX, renderY, renderW);
		}



		if (i > 0) {
		
			//-1589.9999, 1680
			
			glBegin(GL_QUADS);
				glVertex2f(renderX-renderW, renderY);
				glVertex2f(renderX+renderW, renderY);
				glVertex2f(lastX+lastW, lastY);
				glVertex2f(lastX-lastW, lastY);
			glEnd();
		}

		lastX = renderX;
		lastY = renderY;
		lastW = renderW;
	}


	glutSwapBuffers();

	first = false;
}

