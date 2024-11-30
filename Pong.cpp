#include "drawPages.h"
#include <windows.h>
#include <mmsystem.h>
#include <winuser.h>
#include "GL/glut.h"

#ifndef iGraphics.h
#define iGraphics.h
#endif	

/********* All Global and Constant Variables *********/
// Screen sizes
// int screenWidth = 1536;
// int screenHeight = 793;

extern int screenWidth;
extern int screenHeight;
extern int menuTopMargin;
extern int menuLeftMargin;
extern int prevPage, menuButtonHovered, playerButtonHovered, controlButtonHovered;
extern int player1ControlIsMouse;

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
	else if (currentPage == 11)
	{
		iSetColor(4, 4, 4);
		iFilledRectangle(0, 0, screenWidth, screenHeight);

		if (playerButtonHovered == 1)
        {
            iSetColor(124, 23, 255);
            iFilledRectangle(450, 430, 620, 100);
            iShowBMP2(450, 430, ".\\assets\\menu\\oneplayer.bmp", 0);
        }
        else iShowBMP2(450, 430, ".\\assets\\menu\\oneplayer.bmp", 0);

		if (playerButtonHovered == 2)
        {
            iSetColor(124, 23, 255);
            iFilledRectangle(450, 270, 620, 100);
            iShowBMP2(445, 270, ".\\assets\\menu\\twoplayer.bmp", 0);
        }
        else iShowBMP2(445, 270, ".\\assets\\menu\\twoplayer.bmp", 0);
	}
	else if (currentPage == 21)
	{
		iSetColor(4, 4, 4);
		iFilledRectangle(0, 0, screenWidth, screenHeight);

		if (controlButtonHovered == 1)
        {
            iSetColor(124, 23, 255);
            iFilledRectangle(450, 430, 620, 100);
            iShowBMP2(450, 430, ".\\assets\\menu\\mouse.bmp", 0);
        }
        else iShowBMP2(450, 430, ".\\assets\\menu\\mouse.bmp", 0);

		if (controlButtonHovered == 2)
        {
            iSetColor(124, 23, 255);
            iFilledRectangle(450, 270, 620, 100);
            iShowBMP2(445, 270, ".\\assets\\menu\\keyboard.bmp", 0);
        }
        else iShowBMP2(445, 270, ".\\assets\\menu\\keyboard.bmp", 0);
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
			for (int i = 0; i < 5; i++)
			{
				if ((mx >= 980 && mx <= 1430) && (my + 25 >= (menuTopMargin - 100*i) && my + 25 <= (menuTopMargin - 100*i + 80)))
				{
					if (i == 4) exit(0);
					else 
					{
						if (i == 1) currentPage = 11;
						else currentPage = i;
					}

				}
			}
			// if ((mx >= 1000  && mx <= 1260) && (my >= 680 && my <= 740))
			// {
			// 	currentPage = 0;
			// }
			// if ((mx >= 1000 && mx <= 1305) && (my >= 580 && my <= 640))
			// {
			// 	currentPage = 1;
			// 	Sleep(500);
			// }
			// if ((mx >= 1000 && mx <= 1280) && (my >= 480 && my <= 540))
			// {
			// 	currentPage = 2;
			// }
			// if ((mx >= 1000 && mx <= 1420) && (my >= 380 && my <= 440))
			// {
			// 	currentPage = 3;
			// }
			// if ((mx >= 1000 && mx <= 1140) && (my >= 280 && my <= 340))
			// {
			// 	exit(0);
			// }
		}
		else if (currentPage == 11)
		{
			if ((mx >= 450 && mx <= 1070) && (my + 25 >= 430 && my + 25 <= 530))
			{
				player2.isComputer = 1;
				currentPage = 21;
			}
			else if ((mx >= 450 && mx <= 1070) && (my + 25 >= 260 && my + 25 <= 370))
			{
				player2.isComputer = 0;
				currentPage = 1;
			}
		}
		else if (currentPage == 21)
		{
			if ((mx >= 450 && mx <= 1070) && (my + 25 >= 430 && my + 25 <= 530))
			{
				player1ControlIsMouse = 1;
				currentPage = 1;
			}
			else if ((mx >= 450 && mx <= 1070) && (my + 25 >= 260 && my + 25 <= 370))
			{
				player1ControlIsMouse = 0;
				currentPage = 1;
			}
		}
	}
	
}

void iPassiveMouseMove(int mx, int my)
{
	//printf("mx: %d, my: %d\n", mx, my);
	if (currentPage == -1)
	{
		for (int i = 0; i < 5; i++)
		{
			if ((mx >= 980 && mx <= 1430) && (my + 25 >= (menuTopMargin - 100*i) && my + 25 <= (menuTopMargin - 100*i + 80)))
			{
				menuButtonHovered = i;
				break;
			}
			else menuButtonHovered = -1;
		}
	}
	else if (currentPage == 1 && player1ControlIsMouse)
	{
		if (my >= (screenHeight - borderBridth - player1.height)) 
		{
			player1.barY = screenHeight - borderBridth - player1.height;
		}
		else if (my <= borderBridth)
		{
			player1.barY = borderBridth;
		}
		else 
		{
			player1.barY = my;
		}
	}
	else if (currentPage == 11)
	{
		if ((mx >= 450 && mx <= 1070) && (my + 25 >= 430 && my + 25 <= 530))
		{
			playerButtonHovered = 1;	
		}
		else if ((mx >= 450 && mx <= 1070) && (my + 25 >= 260 && my + 25 <= 370))
		{
			playerButtonHovered = 2;
		}
		else playerButtonHovered = -1;
	}
	else if (currentPage == 21)
	{
		if ((mx >= 450 && mx <= 1070) && (my + 25 >= 430 && my + 25 <= 530))
		{
			controlButtonHovered = 1;	
		}
		else if ((mx >= 450 && mx <= 1070) && (my + 25 >= 260 && my + 25 <= 370))
		{
			controlButtonHovered = 2;
		}
		else controlButtonHovered = -1;
	}

	
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if (key == 'q') {
		printf("%d %d\n", screenWidth, screenHeight);
		exit(0);
	}
	if (key == 'b') 
	{
		if (currentPage != -1) currentPage = -1;
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

	if (key == GLUT_KEY_END) 
	{
		exit(0);
	}
	if (key == GLUT_KEY_UP && player1.barMoveState != 1 && !player1ControlIsMouse)
	{
		player1.barY += player1.barDY;
	}
	if (key == GLUT_KEY_DOWN && player1.barMoveState != -1 && !player1ControlIsMouse)
	{
		player1.barY -= player1.barDY;
	}
	//place your codes for other keys here
}


int main() {
	//place your own initialization codes here.
	iInitialize(screenWidth, screenHeight, "The Pong Game");
	iSetTimer(10, drawGamePage);
	
	return 0;
}

