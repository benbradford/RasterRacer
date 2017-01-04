#include <SFML/Graphics.hpp>
using namespace sf;
#include <math.h>
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
  {spriteX=curve=x=y=z=hill=0;}

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

    for(int i=0;i<2400;i++)
     {
       Line line;
       line.z = i*segL;

       if (i>300 && i<700) line.curve=2.5;
       if (i>1100) line.curve=-1.8;

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
            line.curve = +3.0f;
          }
          else if (i < 2200) {
            //line.y = 2.f;
            line.hill = -1.5f;
            line.curve = -3.0f;
          } else if ( i < 2400) {

            line.hill = 1.2f;
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
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

  const float autoTurn = speed > 20.0f ? 0.0004f : 0.0f;
  if (Keyboard::isKeyPressed(Keyboard::Right)) turn+=autoTurn + 0.000005f * speed*0.3f;
  if (Keyboard::isKeyPressed(Keyboard::Left)) turn-=autoTurn + 0.000005f * speed*0.3f;
  if (Keyboard::isKeyPressed(Keyboard::Up)) speed+=5.f;
  if (Keyboard::isKeyPressed(Keyboard::Down)) speed-=1.f;
  //if (Keyboard::isKeyPressed(Keyboard::Tab)) speed*=3;
  if (Keyboard::isKeyPressed(Keyboard::W)) H+=100;
  if (Keyboard::isKeyPressed(Keyboard::S)) H-=100;
  if (Keyboard::isKeyPressed(Keyboard::Escape)) app.close();

  playerX+=turn;
  turn*=0.95f;
  speed *=0.997f - turn * 0.1f;
  speed = speed > 600 ? 600 : speed;

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
  for(int n = startPos; n<startPos+300; n++)  
   {
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

   playerX -= lines[startPos%N].curve * speed * 0.00002f;
   if (fabsf(playerX) > 0.97f) {
      speed *=0.99f;
   } 

    ////////draw objects////////
    for(int n=startPos+300; n>startPos; n--)
      lines[n%N].drawSprite(app);

    int texStartY = 0;
    int texEndY = 40;
    const float absTurn = fabsf(turn);
    if (absTurn > 0.006f) {

      texStartY = 45;

    } if (absTurn > 0.025f) {

      texStartY = 90;

    }
    int w = carObject.getTextureRect().width;
    int h = carObject.getTextureRect().height;

    carObject.setTextureRect(IntRect(0,texStartY,w,texEndY));

    //carObject.setOrigin(carObject.getLocalBounds().width, 0 );
    const float scale = 4.0f;
    if (turn >= 0.0f) {
      carObject.setScale(scale,scale);
      carObject.setPosition(width*0.5f-(w*scale)/2, height-200);
    } else {
        carObject.setScale(-scale,scale);
        carObject.setPosition(width*0.5f+(w*scale)/2, height-200);
    }
    app.draw(carObject);

    app.display();
    }

    return 0;
}
