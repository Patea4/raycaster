//
// Created by patea on 5/23/23.
//

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define PI 3.1415926535

using namespace std;

float px,py,pdx,pdy,pa; // player position and angle


// Draws player to the screen as a point with coordinates px and py
void drawPlayer() {
    glColor3f(1,1,0); // color of vertex
    glPointSize(8); // size of points
    glBegin(GL_POINTS); //  Treat vertex as points
    glVertex2i(px,py); // position for points
    glEnd();
    
    
    // Draw line showing direction
    glLineWidth(3); 
    glBegin(GL_LINES);
    glVertex2i(px,py);
    glVertex2i(px+pdx*5, py+pdy*5); 
    glEnd();
}

// passed to the glutKeyboardFunc to deal with key events, changing position of player with key presses
void buttons(unsigned char key, int x, int y) {
	// Changes the angle of the player and calculates pdx and pdy
    if (key=='a'){ pa-= 0.1; if (pa<0) {pa+=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;} 
    if (key=='d'){ pa+= 0.1; if (pa>2*PI) {pa+=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;}
    // Uses the calcualted values of pdx and pdy (trig ratios) to move the player.
    if (key=='w'){ px+=pdx; py+=pdy; }
    if (key=='s'){ px-=pdx; py-=pdy; }
    glutPostRedisplay(); // tells the window to be redrawn after running the checks
}

int mapX=8,mapY=8,mapS=64; // size of map and the size of each square in the map, so every 1 or 0 will be a square of length 64.


// Game map is represented by an array of 1 and 0s, with 1 representing a wall and 0 an empty space which will be rendered as squares.
int map[] =
        {
                1,1,1,1,1,1,1,1,
                1,0,1,0,0,0,0,1,
                1,0,1,0,0,0,0,1,
                1,0,1,0,0,0,0,1,
                1,0,0,0,0,0,0,1,
                1,0,0,0,0,1,0,1,
                1,0,0,0,0,0,0,1,
                1,1,1,1,1,1,1,1,
        };

void drawMap2D() {
    int x,y,xo,yo;

    for(y=0;y<mapY;y++) {
        for(x=0;x<mapX;x++) {
            /*
             * map[y*mapX+x] checks every position in the array
             * map[y*mapX+x] can be interpreted as a for loop for a 2d array such as map[y][x], with y*mapX being the
             * equivalent to map[y] and the +x being the [x]
             */

            if(map[y*mapX+x] == 1) {
                glColor3f(1,1,1); // walls will be white
            } else {
                glColor3f(0,0,0); // empty spaces will be black
            }
            xo=x*mapS,yo=y*mapS;
            glBegin(GL_QUADS); // treats the next 4 vertices into a quadrilateral aka the squares of the map;
            // 4 vertices = the 4 corners of the square
            // add and subtract 1 so that each side is 1 pixel "deeper" making a 1p edge around the square
            glVertex2i(xo + 1,yo + 1); // top left
            glVertex2i(xo + 1,yo+mapS - 1); // bottom left
            glVertex2i(xo+mapS - 1,yo+mapS - 1); // bottom right
            glVertex2i(xo+mapS - 1,yo + 1); // top right
            glEnd();
        }
    }
}

void drawRays3D() {
	int r,mx,my,mp,dof; float rx,ry,ra,xo,yo;
	ra=pa; // ray angle == player angle
	for (r=0;r<1;r++) {
		// Check horizontal lines
		dof = 0;
		float aTan = -1/tan(ra);
		cout << "ra" << ra << endl;
		cout << tan(ra) << endl;
		cout << aTan << endl;
		
		
		if (ra>PI) { // check if looking down
			ry = (((int)py / 64) * 64);
			//ry = (((int)py>>6)<<6)-0.0001;
			rx = (py-ry)*aTan+px;
			yo=-64;
			xo=-yo*aTan;
	
			/*
			cout << "aTan " << aTan << endl;
			cout << "ry " << ry << endl;
			cout << "rx " << rx << endl;
			cout << "ra " << ra << endl;
			cout << "pa " << pa << endl;
			cout << "reset" << endl;
			*/
		}
		if (ra<PI) { // check if looking down
			ry = (((int)py / 64) * 64) + 64;
			//ry = (((int)py>>6)<<6)-0.0001;
			rx = (py-ry)*aTan+px;
			yo= 64;
			xo=-yo*aTan;
	
			/*
			cout << "aTan " << aTan << endl;
			cout << "ry " << ry << endl;
			cout << "rx " << rx << endl;
			cout << "ra " << ra << endl;
			cout << "pa " << pa << endl;
			cout << "reset" << endl;
			*/
		}
	}
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    drawPlayer();
    drawRays3D();
    glutSwapBuffers();
}

void init() {
    glClearColor(0.3,0.3,0.3,0); // background color
    gluOrtho2D(0,1024,512,0); // window size
    px=300;
    py=300;
    pa=PI/2;
    pdx=cos(pa)*5; pdy=sin(pa)*5;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024,512);
    glutCreateWindow("Raycasting Demo");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(buttons); // set function which will process key events
    glutMainLoop();
}

