/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001


#include <GL/freeglut.h>

#include <openGL/gl.h>
#include <openGL/glu.h>
#include <stdio.h>
#include <math.h> 
#include <iostream>
#include <vector>
#include <array>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_FPS = 60;

static bool keys[255];

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
		//printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	for (int i = 0; i < 255; ++i)
		keys[i] = false;

	return true;
}


void runMainLoop(int val);

void update2();
void render2();
void init();
int window = -1;

int main(int argc, char* args[])
{
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEP);
	
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	window = glutCreateWindow("OpenGL");
	
	//Do post window/context creation initialization
	if (!initGL())
	{
		printf("Unable to initialize graphics library!\n");
		return 1;
	}

	init();

	//Set rendering function
	glutDisplayFunc(render2);

	//Set main loop
	glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);

	//Start GLUT main loop
	glutMainLoop();

	return 0;
}

void keyboardPressed(unsigned char c, int , int )
{
	if (c == 27) {

		glutDestroyWindow(window);
		exit(0);
	}
	keys[c] = true;
}

void keyboardUp(unsigned char c, int, int)
{
	keys[c] = false;
}

void runMainLoop(int val)
{
	update2();
	render2();
	//Run frame one more time
	glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
	glutKeyboardFunc(keyboardPressed);
	glutKeyboardUpFunc(keyboardUp);
}

float width = SCREEN_WIDTH;
float height = SCREEN_HEIGHT;
int roadW = 2000;
int segL = 200; //segment length
float camD = 0.84; //camera depth

struct Color { 
	float r; float g; float b;

	Color( int rr, int gg, int bb) : r(rr/255.f), g(gg/255.f), b(bb/255.f) {}
};

void drawQuad(float r, float g, float b, float x1,float y1,float w1,float x2,float y2,float w2, bool print)
{
	glColor3f(r,g,b);	

	const auto xPos1 = (1.0f * float(x1 / width));
	const auto xPos2 = (1.0f * float(x2 / width));
	const auto yPos1 = 1.0f - (2.0f * float(y1 / height));
	const auto yPos2 = 1.0f - (2.0f * float(y2 / height));
	const auto width1 = float(w1 / width);
	const auto width2 = float(w2 / width);

	//glBegin(GL_QUADS);
	//	glVertex2f(xPos1-width1, yPos1);
	//	glVertex2f(xPos2-width2, yPos2);
	//	glVertex2f(xPos2+width2, yPos2);
	//	glVertex2f(xPos1+width1, yPos1);
	//glEnd();

	glBegin(GL_QUADS);
		glVertex2f(xPos1-width1,yPos1);
		glVertex2f(xPos1+width1,yPos1);
		glVertex2f(xPos2+width2,yPos2);
		glVertex2f(xPos2-width2,yPos2);
	glEnd();

	if (print) {

		printf("%f %f %f, %f %f %f |||  %f %f %f, %f %f %f ||| %.2f %.2f %.2f\n", x1,y1,w1, x2,y2,w2, xPos1,yPos1,width1, xPos2,yPos2,width2, r,g,b);
	}
}

void drawQuad(const Color& c, float x1,float y1,float w1,float x2,float y2,float w2, bool print) {

	drawQuad(c.r,c.g,c.b, x1,y1,w1,x2,y2,w2, print);
}

struct Line
{
  float x,y,z; //3d center of line
  float X,Y,W; //screen coord
  float curve,spriteX,clip,scale;
 
  Line()
  {spriteX=curve=x=y=z=0;}

  void project(int camX,int camY,int camZ)
  {
    scale = camD/(z-camZ);
    X = (1 + scale*(x - camX)) * width/2;
    Y = (1 - scale*(y - camY)) * height/2;
    W = scale * roadW  * width/2;
  }

/*
  void drawSprite(RenderWindow &app)
  {
    Sprite s = sprite;
    int w = s.getTextureRect().width;
    int h = s.getTextureRect().height;

    float destX = X + scale * spriteX * width/2;
    float destY = Y + 4;
    float destW  = w * W / 266;
    float destH  = h * W / 266;

    destX += destW * spriteX; //offsetX
    destY += destH * (-1);    //offsetY

    float clipH = destY+destH-clip;
    if (clipH<0) clipH=0;

    if (clipH>=destH) return;
    s.setTextureRect(IntRect(0,0,w,h-h*clipH/destH));
    s.setScale(destW/w,destH/h);
    s.setPosition(destX, destY);
    app.draw(s);
    }
    */
};

std::vector<Line> lines;
int N;// = lines.size();
float playerX;// = 0;
int pos;// = 0;
int H;// = 1500;
int speed;//=0;
int startPos;// = pos/segL;

void init()
{
    


    for(int i=0;i<1600;i++)
     {
       Line line;
       line.z = i*segL;

       if (i>300 && i<700) line.curve=0.5;
       if (i>1100) line.curve=-0.7;

       if (i<300 && i%20==0) {line.spriteX=-2.5;};// line.sprite=object[5];}
       if (i%17==0)          {line.spriteX=2.0;};// line.sprite=object[6];}
       if (i>300 && i%20==0) {line.spriteX=-0.7;};// line.sprite=object[4];}
       if (i>800 && i%20==0) {line.spriteX=-1.2;};// line.sprite=object[1];}
       if (i==400)           {line.spriteX=-1.2;};// line.sprite=object[7];}

       if (i>750) line.y = sin(i/30.0)*1500;

       lines.push_back(line);
     }

     N = lines.size();
	playerX = 0;
	pos = 0;
	H = 1500;
	speed=0;
	startPos = pos/segL;  
}


void update2() {

	if (keys['w']) {

		startPos++;
	} else if (keys['s']) {

		startPos--;
	}


}

void render2() {


 
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.6f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 0.9f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();

	/*glColor3f(0.0f,1.0f,0.0f);	

	glBegin(GL_QUADS);
		glVertex2f(-1.0f, 0.2f);//SCREEN_HEIGHT);
		glVertex2f(1.0f, 0.2f);//SCREEN_HEIGHT);
		glVertex2f(1.0f, -1.0f);//SCREEN_HEIGHT);
		glVertex2f(-1.0f, -1.0f);//SCREEN_HEIGHT);
	glEnd();
	*/
	//drawQuad(0.0f,1.0f,0.0f, 0,SCREEN_HEIGHT/2.0f,SCREEN_WIDTH, 0, 0, SCREEN_WIDTH);
	//drawQuad(0.0f,1.0f,0.0f, 
	//	10, 10, width - 20, 
	//	10, height - 10, width - 20);
	//glBegin(GL_QUADS);
	//	glVertex2f(-0.2f,-0.2f);
	//	glVertex2f(0.2f,-0.2f);
	//	glVertex2f(0.2f,0.2f);
	//	glVertex2f(-0.2f,0.2f);
	//glEnd();
	//glColor3f(0.5f,0.5f,0.5f);

static bool first = false;
   
	
  if (keys['d']) playerX+=0.1;
  if (keys['a']) playerX-=0.1;
  if (keys['w']) speed=200;
  if (keys['s']) speed=-200;
  if (keys['k']) speed*=3;
  if (keys['e']) H+=100;
  if (keys['q']) H-=100;
  if (keys['g']) speed = 0;
  if (keys['p']) first = true;
  
  pos+=speed;
  while (pos >= N*segL) pos-=N*segL;
  while (pos < 0) pos += N*segL;

 // app.clear(Color(105,205,4));
 // app.draw(sBackground);
  int startPos = pos/segL;
  int camH = lines[startPos].y + H;
  //if (speed>0) sBackground.move(-lines[startPos].curve*2,0);
  //if (speed<0) sBackground.move( lines[startPos].curve*2,0);

  int maxy = height;
  float x=0,dx=0;

  ///////draw road////////
   
  for(int n = startPos; n<startPos+300; n++)  
   {
    Line &l = lines[n%N];
    l.project(playerX*roadW*2-x, camH, startPos*segL - (n>=N?N*segL:0));
    x+=dx;
    dx+=l.curve;

    l.clip=maxy;
    if (l.Y>=maxy) continue;
    maxy = l.Y;

    Color grass  = (n/3)%2?Color(16,200,16):Color(0,154,0);
    Color rumble = (n/3)%2?Color(255,255,255):Color(0,0,0);
    Color road   = (n/3)%2?Color(107,107,107):Color(105,105,105);

    Line p = lines[(n-1)%N]; //previous line
    if (first) printf("%i -> ", n);
    drawQuad(grass, 0, p.Y, width, 0, l.Y, width, false);
    drawQuad(rumble,p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2, false);
    drawQuad(road,  p.X, p.Y, p.W, l.X, l.Y, l.W, first);
    
   }
   if (first) printf("\n\n\n");
	first = false;

	/*auto lastX = 0.0f;
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
	}*/


	glutSwapBuffers();

}

