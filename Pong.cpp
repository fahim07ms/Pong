#include "drawPages.h"
#include <windows.h>
#include <mmsystem.h>
#include <winuser.h>

#ifndef iGraphics.h
#define iGraphics.h
#endif	

/********* All Global and Constant Variables *********/
// Screen sizes
// int screenWidth = 1536;
// int screenHeight = 841;
int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

// Mouse Positions
int mouseX = 0;
int mouseY = 0; 

// Pages
int currentPage = -1;

void iDraw() {
	//place your drawing codes here
	iClear();
	
	if (currentPage == -1)
	{
		drawHomePage();
	}
	else if (currentPage == 1)
	{
		drawGamePage();
	}
	
}

/*
	function iMouseMove() is called when the user presses and	 the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (currentPage == -1)
		{
			clickSound();
			if ((mx >= 1000  && mx <= 1260) && (my >= 680 && my <= 740))
			{
				currentPage = 0;
			}
			if ((mx >= 1000 && mx <= 1305) && (my >= 580 && my <= 640))
			{
				currentPage = 1;
			}
			if ((mx >= 1000 && mx <= 1280) && (my >= 480 && my <= 540))
			{
				currentPage = 2;
			}
			if ((mx >= 1000 && mx <= 1420) && (my >= 380 && my <= 440))
			{
				currentPage = 3;
			}
			if ((mx >= 1000 && mx <= 1140) && (my >= 280 && my <= 340))
			{
				exit(0);
			}

		}
		
	}
	
	
	
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if (key == 'q') {
		exit(0);
	}
	//place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
		exit(0);
	}
	//place your codes for other keys here
}


int main() {
	//place your own initialization codes here.
	iInitialize(screenWidth, screenHeight, "The Pong Game");
	iSetTimer(10, drawGamePage);
	
	return 0;
}

