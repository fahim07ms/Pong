#include "iGraphics.h"

/********* All Global and Constant Variables *********/
// Screen sizes
int screenWidth = 1520;
int screenHeight = 900;

// Mouse Positions
int mouseX = 0;
int mouseY = 0; 

// Color RGB values
double backgroundColor[3] = {12.0, 12.0, 12.0};
double objectColor[3] = {249, 249, 249};

// Pages
int currentPage = 0;

void passiveMotion(int x, int y)
{
	mouseX = x;
	mouseY = y;
}


void iDraw() {
	//place your drawing codes here
	iClear();
	
	if (currentPage == 0)
	{
		double sideMargin = 100;
		double topMargin = 700;
		double lineHeight = 32;

		iSetColor(objectColor[0], objectColor[1], objectColor[2]);
		iText(sideMargin, topMargin, "Start Game", GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(objectColor[0], objectColor[1], objectColor[2]);
		iText(sideMargin, topMargin - lineHeight, "Exit", GLUT_BITMAP_TIMES_ROMAN_24);
		iShowBMP(0,0, "bitmap.bmp");

		for (int i = 0; i < screenWidth; i += 20)
		{
			iSetColor(255, 0, 0);
			iLine(i, 0, i, screenHeight);
			iSetColor(255, 255, 255);
			char str[50];
			sprintf(str, "%d", i);
			iText(i, 0, str, GLUT_BITMAP_HELVETICA_10);
		}
		for (int i = 0; i < screenHeight; i += 20)
		{
			iSetColor(255, 0, 0);
			iLine(0, i, screenWidth, i);
			iSetColor(255, 255, 255);
			char str[50];
			sprintf(str, "%d", i);
			iText(0, i, str, GLUT_BITMAP_HELVETICA_10);
		}
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
	if (button == GLUT_LEFT_BUTTON) {
		//place your codes here
		
	}
	
		//place your codes here
		
	
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
	
	
	return 0;
}
