#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>
#include <sys/time.h>
#include "raycast.h"

// constants
#define SIZE 40
#define MAXCHAR 1000
#define ANGTICK 2.5
#define POSTICK 0.1
#define PRECISION 0.05

// variables
double x = 0;
double y = 0;
double theta = 0;
double alpha = 40;
char map[SIZE][SIZE];	// map[y][x]
int dimr,dimc;

// main driver
int main() {
	// init ncurses and create stdscr
	initscr();
	noecho();
	// capture dimensions
	getmaxyx(stdscr, dimr, dimc);
	// no cursor
	curs_set(0);
	// color init
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_WHITE);
	init_pair(2,COLOR_BLUE,COLOR_BLUE);
	init_pair(8,COLOR_RED,COLOR_RED);
	init_pair(9,COLOR_YELLOW,COLOR_YELLOW);
	// run
	raycastloop();
	// end ncurses
	while(1)
		if(getchar() == 'q')
			break;
	endwin();
	return 0;
}

// loop to process raycasting
void raycastloop() {
	nodelay(stdscr,TRUE);
	char input;
	char *file = "map.txt";
	loadmap(file);
	setlocation();
	while(1) {
		// scan input
		input = getch();
		// process input
		if(input == 'q') break;
		if(input == 68) lookplayer(0);
		if(input == 67) lookplayer(1);
		if(input == 'w') moveplayer(0);
		if(input == 's') moveplayer(1);
		if(input == 'a') moveplayer(2);
		if(input == 'd') moveplayer(3);
		// var
		int i,j;
		// wolfenstein visual
		for(i=0;i<dimc;i++) {
			double tempangle = theta-(alpha/2) + (alpha/dimc)*i;
			double ray = raycast(tempangle);
			drawcol(ray,i);
		}
		// visual birdseye
		for(i=0;i<SIZE;i++) {
			for(j=0;j<SIZE;j++) {
				if(i==(int)y && j==(int)x) {
					attron(COLOR_PAIR(8));
					mvprintw(i,j*2,"--");
					attroff(COLOR_PAIR(8));
				} else {
					if(map[i][j] != '0') {
						attron(COLOR_PAIR(2));
						mvprintw(i,j*2,"[]");
						attroff(COLOR_PAIR(2));
					}
					else
						mvprintw(i,j*2,"  ");
				}
			}
		}
		int cols = 200;
		for(i=0;i<cols;i++) {
			double tempangle = theta-(alpha/2) + (alpha/cols)*i;
			double ray = raycast(tempangle);
			attron(COLOR_PAIR(9));
			mvprintw((int)(y+dsin(tempangle)*ray),(int)(x+dcos(tempangle)*ray)*2,"//");
			attroff(COLOR_PAIR(9));
		}
		//// print information
		//int from = SIZE;
		//mvprintw(from+0,0,"theta:%f",theta);
		//mvprintw(from+1,0,"alpha:%f",alpha);
		//mvprintw(from+2,0,"y:%f",y);
		//mvprintw(from+3,0,"x:%f",x);
	}
}

void drawcol(double dist, int col) {
	//mvprintw(0,col,"%d",dist);
	if(dist<=0) dist = 1;
	double i;
	for(i=0;i<dimr;i++) {
		//if(i>=((double)dimr/2)-((double)dimr/dist)&&i<((double)dimr/2)+((double)dimr/dist)) {
		if(i>=((double)dimr/2)-((double)dimr/dist)&&i<((double)dimr/2)+((double)dimr/dist)) {
			attron(COLOR_PAIR(1));
			mvprintw(i,col," ");
			attroff(COLOR_PAIR(1));
		} else {
			mvprintw(i,col," ");
		}
	}
}

double nextx(double x, double angle) {
	if(dcos(angle) >= 0) return ceil(x) - x;
	return x - floor(x);
}

double nexty(double y, double angle) {
	if(dsin(angle) >= 0) return ceil(y) - y;
	return y - floor(y);
}

// return the distance it takes to reach the nearest wall
// from the given angle with respect to player position
double raycast(double angle) {
	//double dx,dy;
	//dx = nextx(x,angle);
	//dy = nexty(y,angle);
	//double ray = sqrt(pow(x+dx,2)+pow(y+dy,2));
	//int xfirst = dx < dy;
	//while(inrange(y+dsin(angle)*ray,x+dcos(angle)*ray)) {
	//	dx = nextx(x,angle);
	//	dy = nexty(y,angle);
	//	if(xfirst) {
	//		dy = dtan(angle)*dx;
	//		xfirst = 0;
	//	} else {
	//		dx = dtan(angle)*dy;
	//		xfirst = 1;
	//	}
	//	ray = sqrt(pow(x+dx,2)+pow(y+dy,2));
	//	attron(COLOR_PAIR(9));
	//	mvprintw((int)(y+dsin(angle)*ray),(int)(x+dcos(angle)*ray)*2,"//");
	//	attroff(COLOR_PAIR(9));
	//}
	//return ray;
	double ray = PRECISION;
	while(inrange(y+dsin(angle)*ray,x+dcos(angle)*ray)) {
		ray+=PRECISION;
		//attron(COLOR_PAIR(9));
		//mvprintw((int)(y+dsin(angle)*ray),(int)(x+dcos(angle)*ray)*2,"//");
		//attroff(COLOR_PAIR(9));
	}
	return ray;
}

// 0- left, 1-right
void lookplayer(int i) {
	theta += i ? ANGTICK : -ANGTICK;
	if(theta >= 360) theta = theta-360;
	if(theta < 0) theta = 360+theta;
}

// 0-forward, 1-back, 2-left, 3-right
void moveplayer(int i) {
	double cx = dcos(theta)*POSTICK;
	double cy = dsin(theta)*POSTICK;
	if(i==1) {
		cx *= -1;
		cy *= -1;
	} else if(i==2) {
		double hold = cx;
		cx = cy;
		cy = -hold;
	} else if(i==3) {
		double hold = cx;
		cx = -cy;
		cy = hold;
	}
	if(inrange(y+cy,x+cx)) {
		x += cx;
		y += cy;
	}
}

// checks if the current position given is valid
// only valid if not out of bounds and not a wall
int inrange(double y, double x) {
	int iy = (int)y;
	int ix = (int)x;
	if(iy<0 || ix<0 || iy >= SIZE || ix >= SIZE) return 0;
	if(map[iy][ix] != '0') return 0;
	return 1;
}

// places player in a location that is not a wall
void setlocation() {
	int i,j;
	for(i=0;i<SIZE;i++)
		for(j=0;j<SIZE;j++)
			if(map[i][j] == '0') {
				x = i;
				y = j;
				return;
			}
}

// sets map equal to file
void loadmap(char *mapname) {
	FILE *fp;
	char str[MAXCHAR];
	fp = fopen(mapname, "r");
	if (fp == NULL){
		printf("Could not open file %s\n",mapname);
		return;
	}
	int c = 0;
	while(fgets(str,MAXCHAR,fp) != NULL) {
		memcpy(map[c],str,sizeof(char)*SIZE);
		c++;
	}
	fclose(fp);
}

// sin in degrees
double dsin(double x) {
	return sin(x*M_PI/180);
}

// cos in degrees
double dcos(double x) {
	return cos(x*M_PI/180);
}

// tan in degrees
double dtan(double x) {
	return tan(x*M_PI/180);
}
