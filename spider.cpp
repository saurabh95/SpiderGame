#include<stdio.h>
#include <iostream>
#include <cmath>
#include<vector>
#include <GL/glut.h>
#include<time.h>
#include<string.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
typedef struct beams{
     float x,y,theta;
}beams;
vector <beams> beam;
// Function Declarations
typedef struct spiders{
     float x,y,color;
}spiders;
vector<spiders> spider;
void drawScene();
time_t t1=0,t2=0;
char s[10];
int end = 0;
void update(int value);
void drawBox(float len);
void drawBall(float rad);
void drawTriangle();
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
int selected=0;
int isdropped[10000];
int final_score = 0,dropscore = 0;
//selected=0 cannon selected=1 red cylinder selected=2 green cylinder
// Global Variables
float ball_x = 1.0f;
float ball_y = 0.0f;
float ball_velx = 0.01f;
float ball_vely = 0.02f;
float ball_rad = 0.2f;
float box_len = 4.0f;
float tri_x = 0.0f;
float tri_y = 0.0f;
float tri_rad = 0.2f;
float theta = 0.0f;
float tank_x=0.0f;
float tank_y=-1.5f;
float theta_rotation=90;
int  pause1=0;
float x1=-0.05f,x2=0.05f,y11=0.6f,y2=0.05f;
float pistol_length=0.8;
float beam_x1=0,beam_y1=0,beam_length=0.8f,beam_x2=0,beam_y2=0;
int number_of_beams=0;
float red_cylinder=1.0f,green_cylinder=-1.0f;
time_t canon_t1=0,canon_t2=0;
float canon_right_limit,canon_left_limit,red_left_limit,red_right_limit,green_left_limit,green_right_limit;
int red_score=0,green_score=0;


int rotate = 0;
void handleMouseclick(int button, int state, int x, int y) {

     if(pause1)
	  return;
     float mousex=((float)x-455)/125;
     float mousey=(-(float)y+255)/125;
     if (state == GLUT_DOWN)
     {
	  if (button == GLUT_LEFT_BUTTON)
	  {
	       if(mousey>=-1.7&&mousey<=-1.0&&abs(tank_x-mousex)<0.4)
	       {
		    selected=0;
	       }
	       else if(mousey>=-1.7&&mousey<=-1.0&&abs(mousex-red_cylinder)<0.4)
	       {
		    selected=1;
	       }
	       else if(mousey>=-1.7&&mousey<=-1.0&&abs(mousex-green_cylinder)<0.4)
	       {
		    selected=2;
	       }
	       else
	       {
	       }

	  }
	  if(button == GLUT_RIGHT_BUTTON)
	  {

	       if(mousey>=-1.7&&mousey<=-1.0&&abs(tank_x-mousex)<0.4)
	       {
		    rotate=1;
	       }
	       else
		    rotate=0;
	  }

     }

     glutPostRedisplay();
}


void dragwithmouse(int x,int y)
{
     if(pause1==1)
	  return;
     float mousex=((float)x-455)/125;
     //    float mousey=(-(float)y+255)/125;
     if(selected==1&&abs(red_cylinder-mousex)<=0.4)
     {
	  if(mousex>=red_left_limit&&mousex<=red_right_limit)
	       red_cylinder=mousex;
     }
     else if(selected==2&&abs(green_cylinder-mousex)<0.4)
     {
	  if(mousex>=green_left_limit&&mousex<=green_right_limit)
	       green_cylinder=mousex;
     }
     else if(rotate==0&&selected==0&&abs(tank_x-mousex)<0.4)
     {
	  if(mousex>=canon_left_limit&&mousex<=canon_right_limit)
	       tank_x=mousex;
     }
     if(rotate==1 && abs(tank_x-mousex)<0.4)
     {
	  if(tank_x-mousex>0)
	  {
	       if(theta_rotation + 5<180)
		    theta_rotation += 5;
	  }
	  else
	       if(theta_rotation - 7> 0)
		    theta_rotation -=5;
     }

}





// Maximum data buffers we will need.
#define NUM_BUFFERS 3

// Maximum emissions we will need.
#define NUM_SOURCES 3

// These index the buffers and sources.
// Buffers hold sound data.
ALuint Buffers[NUM_BUFFERS];

// Sources are points of emitting sound.
ALuint Sources[NUM_SOURCES];



// Position of the source sounds.
ALfloat SourcesPos[NUM_SOURCES][3];

// Velocity of the source sounds.
ALfloat SourcesVel[NUM_SOURCES][3];



// Position of the listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

// Velocity of the listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };




/*
 * ALboolean LoadALData()
 *
 *	This function will load our sample data from the disk using the alut
 *	utility and send the data into OpenAL as a buffer. A source is then
 *	also created to play that buffer.
 */
ALboolean LoadALData()
{
     // Variables to load into.

     ALenum format;
     ALsizei size;
     ALvoid* data;
     ALsizei freq;
     ALboolean loop;

     // Load wav data into buffers.

     alGenBuffers(NUM_BUFFERS, Buffers);

     if(alGetError() != AL_NO_ERROR)
	  return AL_FALSE;

     alutLoadWAVFile((ALbyte*)"audio_files/Background.wav", &format, &data, &size, &freq, &loop);
     alBufferData(Buffers[0], format, data, size, freq);
     alutUnloadWAV(format, data, size, freq);

     alutLoadWAVFile((ALbyte*)"audio_files/Gun.wav", &format, &data, &size, &freq, &loop);
     alBufferData(Buffers[1], format, data, size, freq);
     alutUnloadWAV(format, data, size, freq);

     alutLoadWAVFile((ALbyte*)"audio_files/collect.wav", &format, &data, &size, &freq, &loop);
     alBufferData(Buffers[2], format, data, size, freq);
     alutUnloadWAV(format, data, size, freq);

     // Bind buffers into audio sources.

     alGenSources(NUM_SOURCES, Sources);

     if(alGetError() != AL_NO_ERROR)
	  return AL_FALSE;

     alSourcei (Sources[0], AL_BUFFER,   Buffers[0]   );
     alSourcef (Sources[0], AL_PITCH,    1.0f              );
     alSourcef (Sources[0], AL_GAIN,     1.0f              );
     alSourcefv(Sources[0], AL_POSITION, SourcesPos[0]);
     alSourcefv(Sources[0], AL_VELOCITY, SourcesVel[0]);
     alSourcei (Sources[0], AL_LOOPING,  AL_TRUE           );
     alSourcei (Sources[1], AL_BUFFER,   Buffers[1]   );
     alSourcef (Sources[1], AL_PITCH,    1.0f            );
     alSourcef (Sources[1], AL_GAIN,     1.0f            );
     alSourcefv(Sources[1], AL_POSITION, SourcesPos[1]);
     alSourcefv(Sources[1], AL_VELOCITY, SourcesVel[1]);
     alSourcei (Sources[1], AL_LOOPING,  AL_FALSE        );

     alSourcei (Sources[2], AL_BUFFER,   Buffers[2]   );
     alSourcef (Sources[2], AL_PITCH,    1.0f            );
     alSourcef (Sources[2], AL_GAIN,     1.0f            );
     alSourcefv(Sources[2], AL_POSITION, SourcesPos[2]);
     alSourcefv(Sources[2], AL_VELOCITY, SourcesVel[2]);
     alSourcei (Sources[2], AL_LOOPING,  AL_FALSE        );

     // Do another error check and return.

     if(alGetError() != AL_NO_ERROR)
	  return AL_FALSE;

     return AL_TRUE;
}



/*
 * void SetListenerValues()
 *
 *	We already defined certain values for the listener, but we need
 *	to tell OpenAL to use that data. This function does just that.
 */
void SetListenerValues()
{
     alListenerfv(AL_POSITION,    ListenerPos);
     alListenerfv(AL_VELOCITY,    ListenerVel);
     alListenerfv(AL_ORIENTATION, ListenerOri);
}



/*
 * void KillALData()
 *
 *	We have allocated memory for our buffers and sources which needs
 *	to be returned to the system. This function frees that memory.
 */
void KillALData()
{
     alDeleteBuffers(NUM_BUFFERS, Buffers);
     alDeleteSources(NUM_SOURCES, Sources);
     alutExit();
}








void drawLines(void) {
     // glClear(GL_COLOR_BUFFER_BIT);  
     //    glColor3f(1.0,0.0,0.0); 
     //     glPointSize(3.0);  
     glColor3f(0.0f,0.0f,1.0f);

     glBegin(GL_LINES);
     glVertex2d(-2, -1.5);
     glVertex2d(2, -1.5);
     glEnd();
     //glFlush();
}



void printscore(float x, float y, string String)
{
     glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();
     glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();
     glPushAttrib(GL_DEPTH_TEST);
     glDisable(GL_DEPTH_TEST);
     glRasterPos2f(x,y);
     for (size_t i=0; i<String.size(); i++)
     {
	  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
     }
     glPopAttrib();
     glMatrixMode(GL_PROJECTION);
     glPopMatrix();
     glMatrixMode(GL_MODELVIEW);
     glPopMatrix();
}




void drawTank(float rad) {

     glBegin(GL_TRIANGLE_FAN);
     for(int i=0 ; i<180 ; i++) {
	  glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
     }
     glEnd();
}



int main(int argc, char **argv) {

     // Initialize GLUT
     canon_left_limit=-box_len/2;
     canon_right_limit=box_len/2;
     red_left_limit=-box_len/2;
     red_right_limit=box_len/2;
     green_left_limit=-box_len/2;
     green_right_limit=box_len/2;
     glutInit(&argc, argv);

     alutInit(NULL, 0);
     alGetError();

     // Load the wav data.

     if(LoadALData() == AL_FALSE)
	  return 0;

     SetListenerValues();

     // Setup an exit procedure.

     atexit(KillALData);



     alSourcePlay(Sources[0]);
     //intitialize the windowing system giving size of the window
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);		//used to set the display mode set initial bit masks for a display mode

     int w = glutGet(GLUT_SCREEN_WIDTH);
     int h = glutGet(GLUT_SCREEN_HEIGHT);
     int windowWidth = w * 2 / 3;
     int windowHeight = h * 2 / 3;

     glutInitWindowSize(windowWidth, windowHeight);
     glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

     glutCreateWindow("SpiderMan");  // Setup the window
     initRendering();

     // Register callbacks
     glutDisplayFunc(drawScene);
     glutIdleFunc(drawScene);
     glutKeyboardFunc(handleKeypress1);
     glutSpecialFunc(handleKeypress2);
     glutMouseFunc(handleMouseclick);
     glutMotionFunc(dragwithmouse);
     glutReshapeFunc(handleResize);
     glutTimerFunc(10, update, 0);

     glutMainLoop();
     return 0;
}

float RandomFloat(float a, float b) {
     float random = ((float) rand()) / (float) RAND_MAX;
     float diff = b - a;
     float r = random * diff;
     return a + r;
}


void CreateSpider()
{
     if(!pause1)
     {
	  spiders s;
	  s.x=RandomFloat(-box_len/2+0.25f,box_len/2-0.25f);
	  s.y=box_len/2;
	  s.color=rand()%3;
	  spider.push_back(s);
     }
     /*    glPushMatrix();
	   cout << s.x << "  " << s.y << "  " << s.color << "\n";
	   glTranslatef(0.0f, 0.0f,0.0f);
	   glColor3f(0.0f, 0.0f, 0.0f);
	   glLineWidth(15);
	   glBegin(GL_LINES); 
	   glVertex2f(-10.2f, -10.4f); 
	   glVertex2f(10.8f, 10.4f); 
     //     glVertex3f(.8, .6, 0.0); 
     //   glVertex3f(.2, .6, 0.0); 
     glEnd();
     glPopMatrix();
     glTranslatef(0.0f, 0.0f,0.0f);
     glLineWidth(1);
     //     glTranslatef(0.0f, box_len, 0.0f);
     //   glColor3f(1.0f, 0.0f, 0.0f);
     //   drawBox(box_len);
      */
}





// Function to draw objects on the screen
void drawScene() {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     if(end==0)
     {
	  glPushMatrix();





	  //draw spiders






	  // Draw Box
	  glTranslatef(0.0f, 0.0f, -5.0f);
	  glColor3f(1.0f, 0.0f, 0.0f);
	  drawBox(box_len);
	  glPushMatrix();
	  drawLines();
	  glPopMatrix();

	  if(t1==0)  //l1=0,l2 are time_t type variables- include ctime
	       t1=time(0)-2;
	  t2=time(0);
	  if(t2>=t1+2)
	  {						//create instance of laser	
	       CreateSpider();
	       t1=t2;						
	  }
	  for(unsigned i=0;i<spider.size();i++)
	  {
	       glPushMatrix();
	       glTranslatef(spider[i].x, spider[i].y,0.0f);
	       if(spider[i].color==0)
		    glColor3f(1.0f, 0.0f, 0.0f);
	       if(spider[i].color==1)
		    glColor3f(0.0f, 1.0f, 0.0f);
	       if(spider[i].color==2)
		    glColor3f(0.0f, 0.0f, 0.0f);
	       /*  glBegin(GL_POLYGON); 
		   glVertex2f(.2f, .2); 
		   glVertex2f(-.2f, .2); 
		   glVertex2f(-.2f, -.2); 
		   glVertex2f(.2f, -.2);
		   glEnd();
		*/

	       //draw spider
	       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	       glBegin(GL_QUADS);
	       glVertex2f(-0.05,-0.1);
	       glVertex2f(0.05,-0.1);
	       glVertex2f(0.05,0.1);
	       glVertex2f(-0.05,0.1);

	       glVertex2f(-0.05,0.1);
	       glVertex2f(0.05,0.1);
	       glVertex2f(0.02,0.15);
	       glVertex2f(-0.02,0.15);

	       glVertex2f(-0.02,-0.15);
	       glVertex2f(0.02,-0.15);
	       glVertex2f(0.05,-0.1);
	       glVertex2f(-0.05,-0.1);
	       glEnd();

	       glLineWidth(3);
	       glBegin(GL_LINES);
	       glVertex2f(0.035,0.13);
	       glVertex2f(0.035+0.12*0.707,0.17+0.12*0.707); 
	       glVertex2f(0.035+0.12*0.707,0.17+0.12*0.707); 
	       glVertex2f((0.035+0.12*0.707) + 0.02,(0.17+0.12*0.707) + 0.1); 

	       glVertex2f(0.035,-0.13);
	       glVertex2f(0.035+0.12*0.707,-0.17-0.12*0.707); 
	       glVertex2f(0.035+0.12*0.707,-0.17-0.12*0.707); 
	       glVertex2f((0.035+0.12*0.707) + 0.02,(-0.17-0.12*0.707) -0.1 );  

	       glVertex2f(-0.035,0.13);
	       glVertex2f(-0.035-0.12*0.707,0.17+0.12*0.707); 
	       glVertex2f(-0.035-0.12*0.707,0.17+0.12*0.707); 
	       glVertex2f((-0.035-0.12*0.707) -0.02 ,(0.17+0.12*0.707) +0.1 );  

	       glVertex2f(-0.035,-0.13);
	       glVertex2f(-0.035-0.12*0.707,-0.17-0.12*0.707); 
	       glVertex2f(-0.035-0.12*0.707,-0.17-0.12*0.707);
	       glVertex2f((-0.035-0.12*0.707) -0.02 ,(-0.17-0.12*0.707) -0.1);



	       glVertex2f(0.02,0.015);
	       glVertex2f(0.02+0.12,0.015+0.1);

	       glVertex2f(0.02,-0.015);
	       glVertex2f(0.02+0.12,0.015-0.1);

	       glVertex2f(-0.02,0.015);
	       glVertex2f(-0.02-0.12,0.015+0.1);

	       glVertex2f(-0.02,-0.015);
	       glVertex2f(-0.02-0.12,-0.015-0.1);

	       glEnd();
	       glPopMatrix();

	  }

	  glPushMatrix();
	  glColor3f(1.0f,0.0f,0.0f);
	  glTranslatef(red_cylinder,-.9f,0.0f);
	  GLUquadricObj *quadratic;
	  quadratic = gluNewQuadric();
	  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	  gluCylinder(quadratic,0.23f,0.23f,.6f,32,32);
	  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	  glPopMatrix();

	  glColor3f(0.0f,0.0f,1.0f);


	  glPushMatrix();
	  glColor3f(0.0f,1.0f,0.0f);
	  glTranslatef(green_cylinder,-.9f,0.0f);
	  GLUquadricObj *quadratic1;
	  quadratic1 = gluNewQuadric();
	  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	  gluCylinder(quadratic1,0.23f,0.23f,.6f,32,32);
	  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	  glPopMatrix();

	  glColor3f(0.0f,0.0f,1.0f);





	  glPushMatrix();
	  glTranslatef(tank_x,tank_y,0.0f);
	  glRotatef((90-theta_rotation),0,0,-1);
	  glBegin(GL_POLYGON);
	  glVertex2f( 0.12f, 0.12f );
	  glVertex2f( -0.12f, 0.12f );
	  glVertex2f( -0.12f, -0.12f );
	  glVertex2f( 0.12f, -0.12f );
	  glEnd();
	  glLineWidth(20);
	  glBegin(GL_LINES);
	  glColor3f(128.0f, 0.0f,0.0f);
	  glVertex2f(0.12f,0.0f);
	  glVertex2f(0.15f,0.0f);
	  glEnd();
	  glLineWidth(20);
	  glBegin(GL_LINES);
	  glColor3f(128.0f, 0.0f,0.0f);
	  glVertex2f(-0.12f,0.0f);
	  glVertex2f(-0.15f,0.0f);
	  glEnd();
	  glLineWidth(15);
	  glBegin(GL_LINES);
	  glColor3f(128.0f, 0.0f,0.0f);
	  glVertex2f(0.19f,.15f);
	  glVertex2f(0.19f,-.15f);
	  glEnd();
	  glLineWidth(15);
	  glBegin(GL_LINES);
	  glColor3f(128.0f, 0.0f,0.0f);
	  glVertex2f(-0.19f,.15f);
	  glVertex2f(-0.19f,-.15f);
	  glEnd();
	  glBegin(GL_POLYGON);
	  glVertex2f( 0.1f, 0.12f );
	  glVertex2f( -0.1f, 0.12f );
	  glVertex2f( -0.1f, 0.2f );
	  glVertex2f( 0.1f, 0.2f );
	  glEnd();

	  //     glTranslatef(tank_x, tank_y, 0.0f);
	  //   drawTank(0.3);
	  glPopMatrix();

	  glLineWidth(1);


	  glLineWidth(60);
	  glColor3f(128.0f, 0.0f,0.0f);
	  glBegin(GL_LINES);
	  glVertex2f(tank_x,tank_y);
	  glVertex2f(tank_x+pistol_length*cos(DEG2RAD(theta_rotation)),tank_y+pistol_length*sin(DEG2RAD(theta_rotation)));
	  glEnd();
	  glLineWidth(1);


	  //draw beam
	  float laser_angle;
	  float temp;
	  for(unsigned i=0;i<beam.size();i++)
	  {
	       laser_angle=180-beam[i].theta;


	       if(DEG2RAD(beam[i].theta)>=DEG2RAD(90) && beam[i].x>=-box_len/2 && -box_len/2>=(beam[i].x+(beam_length*cosf(DEG2RAD(beam[i].theta)))))
	       {
		    temp=-((-box_len/2)-beam[i].x);
		    cout<<"temp1: "<<-((-box_len/2)-beam[i].x)<<"\n";
		    float tra=180-beam[i].theta;
		    cout<<"theta : "<<tra <<"tan value: "<<(sinf(DEG2RAD(tra))/cosf(DEG2RAD(tra)))<<"\n";
		    temp*=(sinf(DEG2RAD(tra))/cosf(DEG2RAD(tra)));
		    cout<<"temp2: "<<temp<<"\n";//(sinf(DEG2RAD(17.5))/cosf(DEG2RAD(17.5)))<<"\n";
		    temp=temp+beam[i].y;
		    glLineWidth(20);
		    glColor3f(128.0f, 0.0f,0.0f);
		    // glTranslate()
		    glBegin(GL_LINES);
		    glVertex3f(beam[i].x,beam[i].y, 0.0);
		    glVertex3f((-box_len/2), temp,0.0);
		    glEnd();
		    float newline=((-box_len/2)-beam[i].x)*((-box_len/2)-beam[i].x);
		    newline+=(temp-beam[i].y)*(temp-beam[i].y);
		    newline=beam_length-sqrt(newline);
		    laser_angle=DEG2RAD(180)-DEG2RAD(beam[i].theta);
		    glBegin(GL_LINES);
		    glVertex3f((-box_len/2), temp,0.0);
		    glVertex3f((-box_len/2)+(newline*cosf(laser_angle)), temp+(newline*sinf(laser_angle)), 0.0);
		    glEnd();
		    glLineWidth(1);
	       }
	       else if(DEG2RAD(beam[i].theta)<=DEG2RAD(90) && beam[i].x<=box_len/2 && box_len/2<=(beam[i].x+(beam_length*cosf(DEG2RAD(beam[i].theta)))))
	       {
		    temp=((box_len/2)-beam[i].x);
		    cout<<"temp1: "<<((box_len/2)-beam[i].x)<<"\n";
		    float tra=beam[i].theta;
		    cout<<"theta : "<<tra <<"tan value: "<<(sinf(DEG2RAD(tra))/cosf(DEG2RAD(tra)))<<"\n";
		    temp*=(sinf(DEG2RAD(tra))/cosf(DEG2RAD(tra)));
		    cout<<"temp2: "<<temp<<"\n";//(sinf(DEG2RAD(17.5))/cosf(DEG2RAD(17.5)))<<"\n";
		    temp=temp+beam[i].y;
		    glLineWidth(20);
		    glColor3f(128.0f, 0.0f,0.0f);
		    // glTranslate()
		    glBegin(GL_LINES);
		    glVertex3f(beam[i].x,beam[i].y, 0.0);
		    glVertex3f((box_len/2), temp,0.0);
		    glEnd();
		    float newline=((box_len/2)-beam[i].x)*((box_len/2)-beam[i].x);
		    newline+=(temp-beam[i].y)*(temp-beam[i].y);
		    newline=beam_length-sqrt(newline);
		    laser_angle=DEG2RAD(180)-DEG2RAD(beam[i].theta);
		    glBegin(GL_LINES);
		    glVertex3f((box_len/2), temp,0.0);
		    glVertex3f((box_len/2)+(newline*cosf(laser_angle)), temp+(newline*sinf(laser_angle)), 0.0);
		    glEnd();
		    glLineWidth(1);
	       }

	       else if(beam[i].x<=-box_len/2)
	       {
		    laser_angle=180-beam[i].theta;
		    beam[i].y=temp;
		    beam[i].x=-box_len/2;
		    beam[i].theta=laser_angle;
		    cout << "GONE\n";
		    //  glBegin(GL_LINES);
		    //  glVertex2f(beam[i].x,beam[i].y);
		    //  glVertex2f(beam[i].x+(0.5)*cos(DEG2RAD(beam[i].theta)),beam[i].y+(0.5)*sin(DEG2RAD(beam[i].theta)));
		    //  glEnd();
	       }
	       else if(beam[i].x>=box_len/2)
	       {
		    laser_angle=180-beam[i].theta;
		    beam[i].y=temp;
		    beam[i].x=box_len/2;
		    beam[i].theta=laser_angle;
	       }
	       else
	       {
		    glLineWidth(20);
		    glColor3f(128.0f, 0.0f,0.0f);
		    glBegin(GL_LINES);
		    glVertex2f(beam[i].x,beam[i].y);
		    glVertex2f(beam[i].x+(beam_length)*cos(DEG2RAD(beam[i].theta)),beam[i].y+(beam_length)*sin(DEG2RAD(beam[i].theta)));
		    glEnd();
		    glLineWidth(1);
	       }
	  }
	  /*     glPushMatrix();
		 glColor3f(0.0f, 0.0f, 0.0f);
		 glRectf(tank_x-0.0f,tank_y+0.60f, tank_x+x2, tank_y+y2);
		 glutSwapBuffers();
		 glPopMatrix();
	   */
	  // Draw Ball
	  /*    glPushMatrix();
		glTranslatef(ball_x, ball_y, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		drawBall(ball_rad);
		glPopMatrix();
	   */
	  // Draw Triangle
	  /*   glPushMatrix();
	       glTranslatef(tri_x, tri_y, 0.0f);
	       glRotatef(theta, 0.0f, 0.0f, 1.0f);
	       glScalef(0.5f, 0.5f, 0.5f);
	       drawBall(tri_rad);
	       glPopMatrix();

	   */
	  glColor3f(0,0,0);
	  sprintf(s,"Score : %d",final_score);
	  printscore(0.3,0.8,s);
	  glPopMatrix();
	  glutSwapBuffers();
     }
     else
     {
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glColor3f(0,0,0);
	  sprintf(s,"Score : %d",final_score);
	  printscore(0.3,0.8,s);
	  string buffer="GAME HAS ENDED";
	  printscore(0,0,buffer);
	  glutSwapBuffers();
	  return;
     }
}

// Function to handle all calculations in the scene
// updated evry 10 milliseconds
void update(int value) {
     if(end==0)
     {

	  // Handle ball collisions with box
	  if(!pause1)
	  {
	       if(ball_x + ball_rad > box_len/2 || ball_x - ball_rad < -box_len/2)
		    ball_velx *= -1;
	       if(ball_y + ball_rad > box_len/2 || ball_y - ball_rad < -box_len/2)
		    ball_vely *= -1;

	       // Update position of ball
	       ball_x += ball_velx;
	       ball_y += ball_vely;
	       float d=0.08f;
	       for(unsigned i=0;i<beam.size();i++)
	       {
		    beam[i].x+=d*cos(DEG2RAD(beam[i].theta));
		    beam[i].y+=d*sin(DEG2RAD(beam[i].theta));
		    if(beam[i].y>box_len/2 || beam[i].y<-box_len/2)
			 beam.erase(beam.begin()+i);
		    for(unsigned j=0;j<spider.size();j++)
		    {
			 if(beam[i].x>=(spider[j].x-0.2) && beam[i].x<=(spider[j].x+0.2) && beam[i].y>=(spider[j].y-0.25)&& beam[i].y<=(spider[j].y+0.25))
			 {
			      if(spider[j].color==0 || spider[j].color==1)
				   red_score--;
			      else
				   red_score++;
			      spider.erase(spider.begin()+j);
			      beam.erase(beam.begin()+i);
			      alSourcePlay(Sources[2]);
			      break;
			 }
		    }
		    //	  beam_x2=beam_x2+beam_length*cos(DEG2RAD(theta_rotation));
		    //	  beam_y2=beam_y2+beam_length*sin(DEG2RAD(theta_rotation));
	       }
	       dropscore = 0;
	       float spider_speed;
	       for(unsigned i=0;i<spider.size();i++)
	       {
		    spider_speed = RandomFloat(0.01f,0.02f);
		    if(spider[i].y>-(box_len/2-0.5f))
			 spider[i].y+=-spider_speed;
		    else 
		    {
			 isdropped[i]=1;
			 dropscore+=5;
			 if(tank_x>=(spider[i].x-0.2) && tank_x<=(spider[i].x+0.2))
			 {
			      end=1;
			      return;
			 }
			 if(spider[i].x<canon_right_limit && tank_x<spider[i].x)
			      canon_right_limit=spider[i].x;
			 else if(spider[i].x>canon_left_limit && tank_x>spider[i].x)
			      canon_left_limit=spider[i].x;
			 if(spider[i].x<red_right_limit && red_cylinder<spider[i].x)
			      red_right_limit=spider[i].x;
			 else if(spider[i].x>red_left_limit && red_cylinder>spider[i].x)
			      red_left_limit=spider[i].x;
			 if(spider[i].x<green_right_limit && green_cylinder<spider[i].x)
			      green_right_limit=spider[i].x;
			 else if(spider[i].x>green_left_limit && green_cylinder>spider[i].x)
			      green_left_limit=spider[i].x;
		    }
		    if(spider[i].x>=(red_cylinder-0.2) && spider[i].x<=(red_cylinder+0.2) && spider[i].y<=-0.5&& spider[i].y>=-0.6)
		    {
			 if(spider[i].color==0)
			      red_score++;
			 else
			      red_score--;
			 alSourcePlay(Sources[2]);
			 spider.erase(spider.begin()+i);
		    }
		    if(spider[i].x>=(green_cylinder-0.2) && spider[i].x<=(green_cylinder+0.2)  && spider[i].y<=-0.5&& spider[i].y>=-0.6)
		    {
			 if(spider[i].color==1)
			      green_score++;
			 else
			      green_score--;
			 alSourcePlay(Sources[2]);
			 spider.erase(spider.begin()+i);
		    }
	       }
	       final_score = red_score + green_score - dropscore;
	  }
	  glutTimerFunc(10, update, 0);
     }
     else
	  return;
}

void drawBox(float len) {

     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     glBegin(GL_QUADS);
     glVertex2f(-len / 2, -len / 2);
     glVertex2f(len / 2, -len / 2);
     glVertex2f(len / 2, len / 2);
     glVertex2f(-len / 2, len / 2);
     glEnd();
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBall(float rad) {

     glBegin(GL_TRIANGLE_FAN);
     for(int i=0 ; i<360 ; i++) {
	  glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
     }
     glEnd();
}

void drawTriangle() {

     glBegin(GL_TRIANGLES);
     glColor3f(0.0f, 0.0f, 1.0f);
     glVertex3f(0.0f, 1.0f, 0.0f);
     glColor3f(0.0f, 1.0f, 0.0f);
     glVertex3f(-1.0f, -1.0f, 0.0f);
     glColor3f(1.0f, 0.0f, 0.0f);
     glVertex3f(1.0f, -1.0f, 0.0f);
     glEnd();
}

// Initializing some openGL 3D rendering options
void initRendering() {

     glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
     glEnable(GL_COLOR_MATERIAL);    // Enable coloring
     glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

     glViewport(0, 0, w, h);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

     if (key == 27 || key=='q' || key=='Q') {
	  exit(0);     // escape key is pressed
     }
     if(key=='p' || key=='P')
     {
	  if(pause1==0)
	       pause1=1;
	  else
	       pause1=0;
     }
     if(key=='r' || key=='R')
	  selected=1;
     if(key=='g' || key=='G')
	  selected=2;
     if(key=='b' || key=='B')
	  selected=0;
     if(key==32)
     {
	  if(canon_t1==0)
	       canon_t1=time(0)-1;
	  canon_t2=time(0);
	  beams a;
	  if(canon_t2-canon_t1>=1 && !pause1)
	  {
	       a.x=tank_x;
	       a.y=tank_y;
	       a.theta=theta_rotation;
	       beam.push_back(a);
	       alSourcePlay(Sources[1]);
	       cout << beam.size() <<"\n";
	  }
     }
}

void handleKeypress2(int key, int x, int y) {
     if(!pause1)
     {
	  if(selected==0)
	  {
	       if (key == GLUT_KEY_LEFT)
	       {
		    if(tank_x>canon_left_limit+0.25f)
			 tank_x -= 0.1;
	       }
	       if (key == GLUT_KEY_RIGHT)
	       {
		    if(tank_x<canon_right_limit-0.25f)
			 tank_x += 0.1;
	       }
	       if (key == GLUT_KEY_UP)
		    if(theta_rotation>=0)
			 theta_rotation-=2.5;
	       if (key == GLUT_KEY_DOWN)
		    if(theta_rotation<=180)
			 theta_rotation+=2.5;
	  }
	  else if(selected==1)
	  {
	       if (key == GLUT_KEY_LEFT)
		    if(red_cylinder>red_left_limit+0.25f)
			 red_cylinder -= 0.1;
	       if (key == GLUT_KEY_RIGHT)
		    if(red_cylinder<red_right_limit-0.25f)
			 red_cylinder += 0.1;
	  }
	  else
	  {
	       if (key == GLUT_KEY_LEFT)
		    if(green_cylinder>green_left_limit+0.25f)
			 green_cylinder -= 0.1;
	       if (key == GLUT_KEY_RIGHT)
		    if(green_cylinder<green_right_limit-0.25f)
			 green_cylinder += 0.1;
	  }
     }
}

/*void handleMouseclick(int button, int state, int x, int y) {

  if (state == GLUT_DOWN)
  {
  if (button == GLUT_LEFT_BUTTON)
  theta += 15;
  else if (button == GLUT_RIGHT_BUTTON)
  theta -= 15;
  }
  }*/
