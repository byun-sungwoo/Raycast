#include <stdio.h>
#include <string.h>
#include <math.h>
#include "raycast.h"
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>
#include <sys/time.h>

#define SIZE 100
#define MAXCHAR 1000
#define POSTICK 1
#define ANGTICK 1
#define PRECISION 0.1

// player position
double x = 0;
double y = 0;
double a = 0;
double theta = 50;	// viewing angle
double angle = 0;	// current angle

// map
char map[SIZE][SIZE];

//// main
//int main() {
//	char *mapname = "map.txt";
//	loadmap(mapname);
//	printmap();
//	setlocation();
//	printf("x:%f y:%f\n",x,y);
//	printf("angle:%f\n",angle);
//	distances(30);
//	
//	angle+=45;
//	printf("x:%f y:%f\n",x,y);
//	printf("angle:%f\n",angle);
//	distances(30);
//	movebackward();
//	
//	angle+=45;
//	printf("x:%f y:%f\n",x,y);
//	printf("angle:%f\n",angle);
//	distances(30);
//}
//
//void distances(int col) {
//	int i;
//	for(i=0;i<col;i++) {
//		double curang = angle-(theta/2)+(theta/col)*i;
//		//printf("%.2f ",curang);
//		drawcol(distancecol(curang),i);
//	}
//	printf("\n");
//}
//
//int distancecol(double diff) {
//	double cx,cy,p;
//	double n = 0;
//	while(1) {
//		n += PRECISION;
//		cx = sin((diff)*M_PI/180)*n;
//		cy = cos((diff)*M_PI/180)*n;
//		if(inrange((int)(x+cx)) && inrange((int)(y+cy))) {
//			if(map[(int)(x+cx)][(int)(y+cy)] == '1') {
//				//return (int)cy;
//				return (int)n;
//			}
//		} else {
//			return SIZE;
//		}
//	}
//}
//
//void drawcol(int dist, int col) {
//	printf("%d ",dist);
//}

void moveforward() {
	double schange = sin(angle*M_PI/180)*POSTICK;
	double cchange = cos(angle*M_PI/180)*POSTICK;
	if(inrange(x+schange) && map[(int)(x+schange)][(int)y] != 1)
		x += schange;
	if(inrange(y+cchange) && map[(int)x][(int)(y+cchange)] != 1)
		y += cchange;
}

void movebackward() {
	angle += 180;
	double schange = sin(angle*M_PI/180)*POSTICK;
	double cchange = cos(angle*M_PI/180)*POSTICK;
	if(inrange(x+schange) && map[(int)(x+schange)][(int)y] != 1)
		x += schange;
	if(inrange(y+cchange) && map[(int)x][(int)(y+cchange)] != 1)
		y += cchange;
	angle -= 180;
}

void moveleft() {
	angle += 270;
	double schange = sin(angle*M_PI/180)*POSTICK;
	double cchange = cos(angle*M_PI/180)*POSTICK;
	if(inrange(x+schange) && map[(int)(x+schange)][(int)y] != 1)
		x += schange;
	if(inrange(y+cchange) && map[(int)x][(int)(y+cchange)] != 1)
		y += cchange;
	angle -= 270;
}

void moveright() {
	angle += 90;
	double schange = sin(angle*M_PI/180)*POSTICK;
	double cchange = cos(angle*M_PI/180)*POSTICK;
	if(inrange(x+schange) && map[(int)(x+schange)][(int)y] != 1)
		x += schange;
	if(inrange(y+cchange) && map[(int)x][(int)(y+cchange)] != 1)
		y += cchange;
	angle -= 90;
}

void lookleft() {
	angle -= ANGTICK;
	if(angle < 0)
		angle = 360+angle;
}

void lookright() {
	angle += ANGTICK;
	if(angle >= 360)
		angle = angle-360;
}

int inrange(double n) {
	return n>=0&&n<SIZE;
}

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

void printmap() {
	int i,j;
	for(i=0;i<SIZE;i++) {
		for(j=0;j<SIZE;j++)
			printf("%c",map[i][j]);
		printf("\n");
	}
}
