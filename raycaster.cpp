//
// Created by patea on 5/23/23.
//

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533 // One degree in radians

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
    if (key=='d'){ pa+= 0.1; if (pa>2*PI) {pa-=2*PI;} pdx=cos(pa)*5; pdy=sin(pa)*5;}
    // Uses the calculated values of pdx and pdy (trig ratios) to move the player.
    if (key=='w'){ px+=pdx; py+=pdy; }
    if (key=='s'){ px-=pdx; py-=pdy; }
    glutPostRedisplay(); // tells the window to be redrawn after running the checks
}


int mapX=8,mapY=8,mapS=64; // size of map and the size of each square in the map, so every 1 or 0 will be a square of length 64.
// Game map is represented by an array of 1 and 0s, with 1 representing a wall and 0 an empty space which will be rendered as squares.
int map[] =
        {
                1,1,1,1,1,1,1,1,
                1,0,1,0,0,0,0,0,
                1,0,1,0,0,0,0,1,
                1,0,1,0,0,0,0,1,
                1,0,0,0,0,0,0,1,
                1,0,0,0,0,1,0,1,
                1,0,0,0,0,0,0,1,
                1,1,1,1,1,1,1,1,
        };


// Function which generates the 2D Map on the left of the screen by turning every
// element in the array into a square with side 64
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

// Calculate distance from player until wall that ray a hits, found using pythagorean theorem
float dist(float playerx, float playery, float wallx, float wally) {
    return (sqrt((wallx - playerx) * (wallx - playerx) + (wally - playery) * (wally - playery)));
}

// Function which generates the rays that check both horizontal
// and vertical lines until a wall is found
void drawRays3D() {
	
	glColor3f(0,1,1); glBegin(GL_QUADS); glVertex2i(526,  0); glVertex2i(1006,  0); glVertex2i(1006,160); glVertex2i(526,160); glEnd();	 // half of screen is cyan
 	glColor3f(0,0,1); glBegin(GL_QUADS); glVertex2i(526,160); glVertex2i(1006,160); glVertex2i(1006,320); glVertex2i(526,320); glEnd();	 // other half is blue
	
	
	int r,mx,my,mp,dof; float rx,ry,ra,xo,yo,distT;
	ra=pa-30*DR; // ray angle == player angle * 30 DRs aka 30 Degrees

    //checking if rays after changing value have to be flipped back to 2pi or 0 to continue on 0 < ra < 2pi
    if (ra<0) {ra += 2*PI;}
    if (ra>2*PI) {ra-=2*PI;}


    int rn = 60; // Number of rays
    // For example iterating over 60 rays would be 60 degrees of field of view as pa-30deg + 60 deg would cover the area of pa-30deg to pa+30deg


    // Iterate over number of rays
	for (r=0;r<rn;r++) {

        //////// Start of Horizontal Line ////////
		dof = 0;
		float aTan = -1/tan(ra);
        float distH = 1000000; // setting up distance value, and setting it high, so it can be decreased later
        float hx=px, hy=py; // Horizontal ray x and y position

        if (ra>PI) { // check if looking up
			ry = (((int)py / 64) * 64) - 0.0001;
			rx = (py-ry)*aTan+px;
			yo=-64;
			xo=-yo*aTan;

            cout << "up" << endl;
		}

		if (ra<PI) { // check if looking down
			ry = (((int)py / 64) * 64) + 64;
			rx = (py-ry)*aTan+px;
			yo= 64;
			xo=-yo*aTan;

            cout << " down" <<endl;
		}

        // straight left or right
        if (ra==0 || ra==PI) {
            rx=px; ry=py;
            dof=8;
            cout << "straight" << endl;
        }

        while (dof<8) {
            mx = (int) rx / 64;
            my = (int) ry / 64;
            mp = my*mapX+mx;
            if (mp > 0 && mp<mapX*mapY && map[mp]==1) { // check if the ray hit a wall
                dof = 8;
                hx=rx;
                hy=ry;
                distH=dist(px,py,hx,hy); // assign values of the ray that hits a wall to hx and hy and calculate the distance to the wall
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        //////// End of Horizontal Line ////////

        //////// Start of Vertical Line ////////
        dof = 0;
        float nTan = -tan(ra);
        float distV = 1000000; // setting up distance value, and setting it high, so it can be decreased later
        float vx=px, vy=py; // Vertical ray x and y position

        if (ra>P2 && ra < P3) { // check if looking left
            rx = (((int)px / 64) * 64) - 0.0001;
            ry = (px-rx)*nTan+py;
            xo=-64;
            yo=-xo*nTan;

            cout << "up" << endl;
        }

        if (ra < P2 || ra > P3) { // check if looking right
            rx = (((int)px / 64) * 64) + 64;
            ry = (px-rx)*nTan+py;
            xo= 64;
            yo=-xo*nTan;

            cout << " down" <<endl;
        }

        // straight up or down
        if (ra==0 || ra==PI) {
            rx=px; ry=py;
            dof=8;
            cout << "straight" << endl;
        }

        while (dof<8) {
            mx = (int) rx / 64;
            my = (int) ry / 64;
            mp = my*mapX+mx;
            if (mp > 0 && mp<mapX*mapY && map[mp]==1) { // check if the ray hit a wall
                dof = 8;
                vx=rx;
                vy=ry;
                distV=dist(px,py,vx,vy); // assign values of the ray that hits a wall to hx and hy and calculate the distance to the wall
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        //////// End of Vertical Line ////////


        if (distV<distH) {rx=vx; ry=vy; distT = distV; glColor3f(1,0,0);} // if vertical ray is shorter assign ray to the vertical ray
        if (distV>distH) {rx=hx; ry=hy; distT = distH; glColor3f(0.6,0,0);} // if horizontal ray is shorter assign ray to the horizontal ray

        // render final ray
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i(px,py);
        glVertex2i(rx,ry);
        glEnd();


        ////////// Start of 3D Render ////////////
		
		float centerAngle = pa-ra; if(centerAngle<0) {centerAngle+=2*PI;} if(centerAngle>2*PI) {centerAngle -=2*PI;} // calculate the angle at the center
		distT = distT*cos(centerAngle); // fix fisheye effect
		
		
        float lineH = (mapS * 320)/distT; // line height
        if (lineH > 320) { // height limit = screen size
            lineH = 320;
        }
        float lineO = 160 - lineH/2; //offset walls to middle of screen
        

        // Draw line
        glLineWidth(8); // draw every 8th pixel, r*8 will make it so that the lines also skip 8px
        glBegin(GL_LINES);
        glVertex2i(r*8+530,lineO); // +530 to shift the window to the right side, so it doesn't overlap the 2d render
        glVertex2i(r*8+530,lineH+lineO); // top of line
        glEnd();






        ra += DR; // increase ra by one degree for next iteration
        //checking if ra after changing value have to be flipped back to 2pi or 0 to continue on 0 < ra < 2pi
        if (ra<0) {ra += 2*PI;}
        if (ra>2*PI) {ra-=2*PI;}

	}
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    drawPlayer();
    drawRays3D();
    glutSwapBuffers();
}

// Call function to always resize window back to default size
void resize(int w, int h) {
	glutReshapeWindow(1024,512);
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
    glutReshapeFunc(resize);
    glutKeyboardFunc(buttons); // set function which will process key events
    glutMainLoop();
}

// write about adding color to floor and sky
// fixing the fish eye effect
// fixing resizing
