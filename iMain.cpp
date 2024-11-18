# include "iGraphics.h"
#include "C:\\MinGW\\include\\GL\\glut.h"
/*
	function iDraw() is called again and again by the system.

	*/
bool leftMouseDown = false;
bool rightMouseDown = false;

int mouseX;
int mouseY;

int rectX = mouseX;
int rectY = mouseY;


void iDraw() {
	//place your drawing codes here
	iClear();
	iSetColor(240, 100, 235);
	iRectangle(20, 20, 640, 640);

	for(int i = 20; i <= (660-64); i += 64)
	{
		iSetColor(240, 100, 235);
		iLine(i, 20, i, (20 + 640));
	}

	for(int i = 20; i <= (660-64); i += 64)
	{
		iSetColor(240, 100, 235);
		iLine(20, i, 20+640, i);
	}

	iSetColor(220, 240, 0);
	iFilledRectangle(20, 20, rectX, rectY);
	
	//iFilledRectangle(10, 30, 20, 20);
	iSetColor(20, 200, 0);
	iText(20, 10, "Hi, I am iGraphics");
}

/*
	function iMouseMove() is called when the user presses and	 the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	// printf("x = %d, y= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON) {
		//place your codes here
		mouseX = mx;
		mouseY = my;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_CURSOR_WAIT) {
		//place your codes here
		rightMouseDown = (state == GLUT_DOWN);
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
	
	iInitialize(680, 680, "demo");
	glutMotionFunc(motion);
	return 0;
}
