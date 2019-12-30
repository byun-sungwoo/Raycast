#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>
#include <sys/time.h>
#include "raycast.c"

void distances(int);
int distancecol(double);
void drawcol(int,int);

int dimr,dimc;

void distances(int col) {
	int i;
	for(i=0;i<col;i++) {
		double curang = angle-(theta/2)+(theta/col)*i;
		drawcol(distancecol(curang),i);
	}
}

int distancecol(double diff) {
	double cx,cy,p;
	double n = 0;
	while(1) {
		n += PRECISION;
		cx = sin((diff)*M_PI/180)*n;
		cy = cos((diff)*M_PI/180)*n;
		if(inrange((int)(x+cx)) && inrange((int)(y+cy))) {
			if(map[(int)(x+cx)][(int)(y+cy)] == '1') {
				//return (int)cy;
				return (int)n;
			}
		} else {
			return SIZE;
		}
	}
}

void drawcol(int dist, int col) {
	//mvprintw(0,col,"%d",dist);
	if(dist<=0) dist = 1;
	double i,j;
	for(i=0;i<dimc;i++) {
		if(i>=((double)dimr/2)-((double)dimr/(dist))&&i<((double)dimr/2)+((double)dimr/(dist))) {
			attron(COLOR_PAIR(1));
			mvprintw(i,col," ");
			attroff(COLOR_PAIR(1));
		} else {
			mvprintw(i,col," ");
		}
	}
}

void raycastloop() {
	char input;
	nodelay(stdscr,TRUE);
	char *file = "map.txt";
	loadmap(file);
	setlocation();
	while(1) {
		input = getch();
		if(input == 'q') break;
		if(input == 68) lookleft();
		if(input == 67) lookright();
		if(input == 'w') moveforward();
		if(input == 's') movebackward();
		if(input == 'a') moveleft();
		if(input == 'd') moveright();
		distances(dimc);
		//int i,j;
		//for(i=0;i<SIZE;i++) {
		//	for(j=0;j<SIZE;j++) {
		//		mvprintw(i,j,"%c",map[i][j]);
		//		if((int)x == j && (int)y == i)
		//			mvprintw(j,i,"-");
		//	}
		//}
		mvprintw(0,0,"angle:%f",angle);
		mvprintw(1,0,"pos:x:%f y:%f",x,y);
	}
}

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
	init_pair(1,COLOR_BLACK,COLOR_WHITE);
	// run
	raycastloop();
	// end ncurses
	while(1)
		if(getchar() == 'q')
			break;
	endwin();
	return 0;
}
