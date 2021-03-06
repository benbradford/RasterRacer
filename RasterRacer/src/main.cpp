#include <SFML/Graphics.hpp>
using namespace sf;
#include <math.h>

/*
class Road {
public:

	struct Segment {

		float x;
		float y;
		float z;
		float curve;
		float hill;

	};
};


class PlayerCar {

};


class RoadUpdater {

public:

private:

	Road& mRoad;
	PlayerCar& mCar;
};

class RoadRenderer {

public:

	void RenderSegment(int zScan, float roadWorldX, float roadWorldY, float roadWorldWidth, float roadPreviousX, float roadPreviousY, float roadPreviousWidth);
private:
};
*/
int width = 550;
int height = 1000;
int roadW = 5000;
int segL = 200; //segment length
float camD = 0.84; //camera depth

void drawQuad(RenderWindow &w, Color c, int x1,int y1,int w1,int x2,int y2,int w2)
{
	ConvexShape shape(4);
	shape.setFillColor(c);
	shape.setPoint(0, Vector2f(x1-w1,y1));
	shape.setPoint(1, Vector2f(x2-w2,y2));
	shape.setPoint(2, Vector2f(x2+w2,y2));
	shape.setPoint(3, Vector2f(x1+w1,y1));
	w.draw(shape);
}

struct Line
{
  float x,y,z; //3d center of line
  float X,Y,W; //screen coord
  float hill;
  float curve,spriteX,clip,scale;
  Sprite sprite;
   Line()
  {spriteX=curve=x=y=z=hill=X=Y=W=scale=0;}

  void project(int camX,int camY,int camZ)
  {
	scale = camD/(z-camZ);
	X = (1 + scale*(x - camX)) * width/2;
	Y = (1 - scale*(y - camY)) * height/3;
	W = scale * roadW  * width/2;
  }

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
};


int main()
{
	RenderWindow app(VideoMode(width, height), "Outrun Racing!");
	app.setFramerateLimit(60);

	Texture t[8];
	Sprite object[50];
	for(int i=1;i<=7;i++)
	 {
	   t[i].loadFromFile("../images/"+std::to_string(i)+".png");
	   t[i].setSmooth(true);
	   object[i].setTexture(t[i]);
	 }

	Texture bg;
	bg.loadFromFile("../images/bg.png");
	bg.setRepeated(true);
	Sprite sBackground(bg);
	sBackground.setTextureRect(IntRect(-500000,0,1000000,411));
	sBackground.setPosition(-50000,0);
	sBackground.setScale(1.0f,0.75f);
	Texture carTexture;
	carTexture.loadFromFile("../images/car.png");
	carTexture.setSmooth(true);
	Sprite carObject;
	carObject.setTexture(carTexture);

	std::vector<Line> lines;

	for(int i=0;i<3800;i++)
	 {
	   Line line;
	   line.z = i*segL;

	   if (i>300 && i<700) line.curve=2.5;
	   if (i>1100 && i < 2000) line.curve=-1.8;

	   if (i<600 && i%20==0) {line.spriteX=-2.5; line.sprite=object[5];}
	   if (i%17==0)          {line.spriteX=1.2; line.sprite=object[6];}
	   if (i>600 && i%20==0) {line.spriteX=-0.7; line.sprite=object[4];}
	   if (i>1200 && i%20==0) {line.spriteX=-1.2; line.sprite=object[1];}
	   if (i==400)           {line.spriteX=-1.5; line.sprite=object[7];}

	   if (i>750 && i < 1600) line.y = sin(i/30.0)*1500;

	   if (i > 1600) {

		  if (i < 1800) {
			//line.y = i - 1800 * 20.0f;
			line.hill = 0.6f;
			
		  } else if (i < 2000) {
			//line.y = i - 2000 * 20.0f;
			line.hill = 0.6f;
			//line.curve = +3.0f;
		  }
		  else if (i < 2200) {
		  	
			//line.y = 2.f;
			line.hill = -1.5f;
			//line.curve = -3.0f;
		  } else if ( i < 2400) {

			line.hill = 1.2f;
		  }
		  else if (i > 2700) {

			  if (i < 3250) {

				line.curve = 4.0f;
			  }
			  //else if (i < 3800) {
				//line.curve = 1.2f;
			  //}

		  }
	   }
	   lines.push_back(line);
	 }

   int N = lines.size();
   float playerX = 0;
   int pos = 0;
   int H = 1500;
   int speed=0;
   float turn = 0.0f;
   float lastY = 0.0f;
   bool drifting = false;
   bool jumping = false;
   float jumpDelta = 0.0f;
   int noAccelTime = 0;
   int accelTime = 0;
   int driftTime = 0;
   const float MAX_SPEED = 600;
	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		const float autoTurn = speed > 20.0f ? 0.001f : 0.0f;
		
		const float turnSpeed = autoTurn+ (drifting ? 0.000006f : 0.0000015f) * speed*0.17f;
		if (Keyboard::isKeyPressed(Keyboard::Right) && jumping == false) {

			turn+=turnSpeed;

			if (drifting == false && accelTime < 30 && speed > 130) {

				drifting = true;
				printf("drifting from turn\n");
				driftTime = 0;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Left) && jumping == false) {

			turn-=turnSpeed;

			if (drifting == false && accelTime < 30 && speed > 130) {

				drifting = true;
				printf("drifting from turn\n");
				driftTime = 0;
			}
		}

		//if (Keyboard::isKeyPressed(Keyboard::Down) && jumping == false) speed-=1.f;

		if (Keyboard::isKeyPressed(Keyboard::Up) && jumping == false) {

			accelTime++;
			if (drifting == false && noAccelTime > 1 && speed > 130 && (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Left) )) {

				printf("drifting\n");
				driftTime = 0;
				drifting = true;
			} else if (drifting && noAccelTime > 1 && fabsf(turn) < 0.5f ){//&& Keyboard::isKeyPressed(Keyboard::Right) == false && Keyboard::isKeyPressed(Keyboard::Left) == false) {

				drifting = false;
				printf("drifting off accel out\n");
			}
			noAccelTime = 0;
			speed+=4.f;
		} else {

			accelTime = 0;
			noAccelTime++;
		}

		if (drifting && speed < 100) {

			drifting = false;
			printf("drifting off too slow\n");
		}

		//if (Keyboard::isKeyPressed(Keyboard::S)) H-=100;
		if (Keyboard::isKeyPressed(Keyboard::Escape)) app.close();
		
		
		if (jumping ) {
			speed *= 0.9999999f;
			H+= jumpDelta;
			jumpDelta -= 7.5f;

			if (H < 1500.0f) {

				H = 1500.0f;
				speed -= 2.f;
				
				jumpDelta = -jumpDelta * 0.3f;
				if (jumpDelta < 20.0f) {

					jumping = false;
					jumpDelta = 0.0f;
				}
			}
		} else {
			
			turn*= drifting? 0.99f : 0.95f;
			speed *=0.997f - turn * (drifting ? 0.05f : 0.2f);
		}
		playerX+=turn;
		playerX = playerX > 1.5f ? 1.5f : playerX;
		playerX = playerX < -1.5f ? -1.5f : playerX;
		speed = speed > MAX_SPEED ? MAX_SPEED : speed;

		pos+=speed;
		while (pos >= N*segL) pos-=N*segL;
		while (pos < 0) pos += N*segL;

		app.clear(Color(105,205,4));
		app.draw(sBackground);
		int startPos = pos/segL;
		int camH = lines[startPos].y + H;
		if (speed>0) sBackground.move(-lines[startPos].curve*2,0);
		if (speed<0) sBackground.move( lines[startPos].curve*2,0);

		int maxy = height;
		float x=0,dx=0;
		float y=0, dy=0;

		///////draw road////////
		for(int n = startPos; n<startPos+300; n++) {

			Line &l = lines[n%N];
			l.project(playerX*roadW-x, camH+y, startPos*segL - (n>=N?N*segL:0));
			x+=dx;
			dx+=l.curve;
			y+=dy;
			dy+=l.hill;

			l.clip=maxy;
			if (l.Y>=maxy) continue;
			maxy = l.Y;

			Color grass  = (n%14<7)?Color(16,200,16):Color(5,180,5);
			Color rumble = (n%14<7)?Color(225,225,225):Color(10,10,10);
			Color road   = (n%14<7)?Color(107,107,107):Color(103,103,103);

			Color line = (n%14<5) ? road : Color(200,200,200);

			Line p = lines[(n-1)%N]; //previous line

			

			drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
			drawQuad(app, rumble,p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
			drawQuad(app, road,  p.X, p.Y, p.W, l.X, l.Y, l.W);

			const float lineMin = 1.0f;
			const float lw = l.W*0.02f < lineMin ? lineMin : l.W*0.02f;
			const float pw = p.W*0.02f < lineMin ? lineMin : p.W*0.02f;
			drawQuad(app, line, p.X-p.W*0.25f,p.Y,pw, l.X-l.W*0.25f,l.Y,lw);
			drawQuad(app, line, p.X+p.W*0.25f,p.Y,pw, l.X+l.W*0.25f,l.Y,lw);
		 }

		 if (startPos > 0) {
			float jumpDiff = lastY - lines[(startPos+1)%N].Y;
			  //printf(">%.2f\n", jumpDiff);
		}

		if (jumping == false && lines[startPos%N].hill - lines[startPos+1%N].hill < -0.5f) {

			printf("line jump\n");
			jumpDelta = (-(lines[startPos%N].hill - lines[startPos+1%N].hill) * 0.55f) * (50.0f + (speed / MAX_SPEED) * 230.0f);
			jumping = true;
			turn = 0.0f;
			
		}
		lastY = lines[startPos+1%N].Y;

		if (drifting) {

			driftTime++;
			//printf ("drifting %.3f %.3f\n", fabsf(lines[startPos%N].curve), turn);

			if (driftTime > 30 && fabsf(lines[startPos%N].curve) < 1.1f && turn < 0.05f ) {

			  printf("drifting off\n");
			  drifting = false;
		  }
		}

		const float pullSpeed = lines[startPos%N].curve * speed * (drifting ? 0.00004f : 0.00002f);
		playerX -= pullSpeed;
		if (jumping == false && fabsf(playerX) > 0.97f) {
			speed *=0.98f;
			if (playerX > 0.0f) {
		 		turn -=0.001f;
			} else {
		 		turn +=0.001f;
			}
		} 

		////////draw objects////////
		for(int n=startPos+300; n>startPos; n--)
		lines[n%N].drawSprite(app);

		int texStartY = 0;
		int texEndY = 40;
		const float absTurn = fabsf(turn);
		if (absTurn > 0.006f) {

		texStartY = 45;

		} if (absTurn > 0.03f) {

		texStartY = 90;

		}
		int w = carObject.getTextureRect().width;
		int h = carObject.getTextureRect().height;

		carObject.setTextureRect(IntRect(0,texStartY,w,texEndY));

		//carObject.setOrigin(carObject.getLocalBounds().width, 0 );
		const float scale = 4.0f;
		if (turn >= 0.0f) {
			carObject.setScale(scale,scale);
			carObject.setPosition(width*0.5f-(w*scale)/2, height-300);
		} else {
		  carObject.setScale(-scale,scale);
		  carObject.setPosition(width*0.5f+(w*scale)/2, height-300);
		}
		app.draw(carObject);

		app.display();
	}

	return 0;
}
