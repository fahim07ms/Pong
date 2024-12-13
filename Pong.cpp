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
extern int prevPage,currentPage, menuButtonHovered, playerButtonHovered, controlButtonHovered;
extern int player1ControlIsMouse;
extern int ballServer;
extern int backButtonHovered, pauseButtonHovered;
extern bool haveResume, gameHasEnd, gameHasStarted;

void update_bar1()
{
	if(!player1ControlIsMouse) 
	{
		if (player1.destination != player1.barY)
		{
			if (player1.barY < player1.destination) player1.barY += player1.maxSpeed;
			else player1.barY -= player1.maxSpeed;
		}
	}
}

void update_bar2()
{
	if (player2.destination != player2.barY)
	{
		if (player2.barY < player2.destination) player2.barY += player2.maxSpeed;
		else player2.barY -= player2.maxSpeed;
	}
} 

void aiMove()
{
	double vy = ball1.velocity * sin(ball1.angle);
	verticalCollisionDistance = (vy * (screenWidth - 440)) / (ball1.velocity * cos(ball1.angle));
	verticalCollisionDistance = verticalCollisionDistance > 0 ? verticalCollisionDistance : -verticalCollisionDistance;
	
	int closeBounce = (vy > 0) ? screenHeight - 20 - ball1.y : ball1.y - 20;
	if (verticalCollisionDistance > closeBounce) 
	{
		verticalCollisionDistance -= closeBounce;
		while (verticalCollisionDistance > screenHeight - 40)
		{
			verticalCollisionDistance -= (screenHeight - 40);

			vy = -vy;
		}

		player2.destination = (vy < 0) ? verticalCollisionDistance + 20 : (screenHeight - 20) - verticalCollisionDistance;

	}
	else 
	{
		player2.destination = (vy > 0) ? ball1.y + verticalCollisionDistance : ball1.y - verticalCollisionDistance;
	}

	player2.destination = player2.destination - player2.destination % player2.maxSpeed;
	if ((player2.destination < (screenHeight - borderBridth - player2.height/2) && (player2.destination > borderBridth + player2.height/2))) 
	{
		player2.destination = player2.destination - player2.height/2;
	}
}

void ballGoesOut()
{
	if ((ball1.x + ball1.radius < 200 || ball1.x - ball1.radius > (screenWidth - 200)) && (ball1.y < player1.barY - ball1.radius || ball1.y > player1.barY + 100 + ball1.radius)) 
    {
        ball1.x = 768;
        ball1.y = 400;

        if ((ball1.velocity * cos(ball1.angle)) > 0) 
        {
            player1.score++;
            if (checkGameEnd(player1.score)) gameEnd(player1.playerNo);
        }
        else 
        {
            player2.score++;
            if (checkGameEnd(player2.score)) gameEnd(player2.playerNo);
        }

		if (Setting.serveSlow) ballServed = false;
		
		int playerLost = ((ball1.velocity * cos(ball1.angle)) > 0) ? player2.playerNo : player1.playerNo;
		switch (Setting.whoServesFirst)
		{
			case 1:
				ballServer = (playerLost == 1) ? 2 : 1;
				break;
			case -1:
				ballServer = (playerLost == 1) ? 1 : 2;
				break;
			default:
				ballServer = (ballServer == 1) ? 2 : 1;
				break;
		}

		ball1.angle = (ballServer == 1) ? 0 : PI;
        player1.barY = 350;
        player2.barY = 350;
		player1.destination = player1.barY;
		player2.destination = player2.barY;
        // ball1.dy = 0;

        Sleep(3000);
    }
}

void paddleDoesNotCrossBorder()
{
	// Left side bar
    if (player2.destination >= screenHeight - borderBridth - player2.height - screenHeight%(player2.maxSpeed)) 
    {
        player2.destination = screenHeight - borderBridth - player2.height - screenHeight%(player2.maxSpeed);
        player2.barMoveState = 1;
    }
    else if (player2.destination <= borderBridth) 
    {
        player2.destination = borderBridth;
        player2.barMoveState = -1;
    }
    else 
    {
        player2.barMoveState = 0;
    }

    // Right side bar
    if (player1.destination >= (screenHeight - borderBridth - player1.height - screenHeight%(player1.maxSpeed))) 
    {
        player1.destination = screenHeight - borderBridth - player1.height - screenHeight%(player1.maxSpeed);
        player1.barMoveState = 1;
    }
    else if (player1.destination <= borderBridth) 
    {
        player1.destination = borderBridth;
        player1.barMoveState = -1;
    }
    else
    {
        player1.barMoveState = 0;
    }
}

void update_game()
{
	if (!gameHasEnd && !haveResume && gameHasStarted)
	{
		if (!ballServed) ball1.velocity = ballVelocity[0];

		// Change the y aixs of 1st bar according to the ball
		// If the ball hits the top or bottom borders
		if (ball1.y < borderBridth + ball1.radius || ball1.y > (screenHeight - borderBridth - ball1.radius)) 
		{
			ball1.angle = -ball1.angle;
			playSound(HIT);
		}
		
		// Always change the ball's X and Y value
		ball1.x += (ball1.velocity * cos(ball1.angle));
		ball1.y += (ball1.velocity * sin(ball1.angle));

		// For the left side bar (Player 2)
		// When the ball hits in left bar area
		if ((ball1.x < (player2.barX + player2.width + ball1.radius) && ball1.x > (player2.barX + (player2.width/2)) ) && !(ball1.y < player2.barY - ball1.radius || ball1.y > player2.barY + player2.height + ball1.radius))
		{
			playSound(HIT);
			ball1.x = player2.barX + player2.width + ball1.radius;
			// printf("BallY: %lf, Player2.BarY: %lf , Player2.width: %lf\n", ball1.y, player2.barY, player2.width);
			ball1.angle = (PI/3)*((ball1.y - player2.barY - player2.height/2)/(player2.height));

			if (!ballServed && ballServer == player2.playerNo) 
			{
				ball1.velocity = (player2.isComputer) ? ballVelocity[2] : ballVelocity[1];
				ballServed = true;	
			}
		}

		//if ((ball1.velocity * cos(ball1.angle)) > 0 && player2.isComputer) player2.destination = ball1.y; 
		// When the ball hits in right bar area
		if ((ball1.x + ball1.radius > player1.barX && (ball1.x < player1.barX + player1.width / 2)) && !(ball1.y < player1.barY - ball1.radius || ball1.y > player1.barY + player1.height + ball1.radius))
		{
			playSound(HIT);
			ball1.x = player1.barX - ball1.radius;
			ball1.angle = PI - (PI/3)*((ball1.y - player1.barY - player1.height/2)/(player1.height));
			
			if (!ballServed && ballServer == player1.playerNo) 
			{
				ball1.velocity = (player2.isComputer) ? ballVelocity[2] : ballVelocity[1];
				ballServed = true;	
			}

			if (player2.isComputer)
			{
				aiMove();
			}
		}

		// If the ball goes out
		ballGoesOut();

		// Try that the bars does not go out of the screen
		paddleDoesNotCrossBorder();


		printf("BallX: %lf, BallY: %lf, Ball veclocity: %lf, Ball angle: %lf, vx: %lf, vy: %lf\n", ball1.x, ball1.y, ball1.velocity, ball1.angle, (ball1.velocity * cos(ball1.angle)), (ball1.velocity * sin(ball1.angle)));
	}
}

void iDraw() 
{
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
		drawBackButton();

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
		drawBackButton();

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
	else if (currentPage == 2)
	{
		drawSettingsPage();
	}
	
}

/*
	function iMouseMove() is called when the user presses and	 the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my) 
{
	
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my) 
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		if (currentPage == -1)
		{
			for (int i = 0; i < 5; i++)
			{
				if ((mx >= 980 && mx <= 1430) && (my + 25 >= (menuTopMargin - 100*i) && my + 25 <= (menuTopMargin - 100*i + 80)))
				{
					prevPage = currentPage;
					playSound(CLICK);	
					switch(i)
					{
						case 0:
							if (haveResume) 
							{
								haveResume = 0;
								currentPage = 1;
								break;
							}
							else continue;
							break;
						case 1:
							if (haveResume)
							{
								gameRestart();
								haveResume = false;
							}
							currentPage = 11;
							break;
						case 2:
							currentPage = 2;
							break;
						case 3:
							currentPage = 3;
							break;
						case 4:
							exit(0);
							break;
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
		else if (currentPage == 1)
		{
			if (mx >= 18 && mx <= 78 && my >= screenHeight - 85 && my <= screenHeight - 25)
			{
				gamePaused();
			}

			if (haveResume)
			{
				for (int i = 0; i < 4; i++)
				{
					if (mx >= 560 && mx <= 1000 && my >= 460 - 80*i - 30 && my <= 525 - 80*i - 30)
					{
						switch (i)
						{
							case 0:
								haveResume = 0;
								break;
							case 1:
								gameRestart();
								break;
							case 2:
								prevPage = currentPage;
								currentPage = 2;
								break;
							case 3:
								currentPage = -1;
								break;
						}
					}
				}
			}
		}
		else if (currentPage == 11)
		{
			if ((mx >= 450 && mx <= 1070) && (my + 25 >= 430 && my + 25 <= 530))
			{
				playSound(CLICK);
				player2.isComputer = 1;
				prevPage = currentPage;
				currentPage = 21;
			}
			else if ((mx >= 450 && mx <= 1070) && (my + 25 >= 260 && my + 25 <= 370))
			{
				playSound(CLICK);
				player2.isComputer = 0;
				prevPage = currentPage;
				currentPage = 1;
				gameHasStarted = true;
				iSetTimer(10, update_game);
				iSetTimer(10, update_bar1);
				iSetTimer(10, update_bar2);
			}

			backButtonClicked(mx, my);
		}
		else if (currentPage == 21)
		{
			if ((mx >= 450 && mx <= 1070) && (my + 25 >= 430 && my + 25 <= 530))
			{
				playSound(CLICK);
				prevPage = currentPage;
				player1ControlIsMouse = 1;
				currentPage = 1;
				gameHasStarted = true;
				iSetTimer(10, update_game);
				iSetTimer(10, update_bar1);
				iSetTimer(10, update_bar2);
			}
			else if ((mx >= 450 && mx <= 1070) && (my + 25 >= 260 && my + 25 <= 370))
			{
				playSound(CLICK);
				prevPage = currentPage;
				player1ControlIsMouse = 0;
				currentPage = 1;
				gameHasStarted = true;
				iSetTimer(10, update_game);
				iSetTimer(10, update_bar1);
				iSetTimer(10, update_bar2);
			}

			backButtonClicked(mx, my);
		}
		else if (currentPage == 2)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mx >= 720 + 180*i && mx <= 860 + 180*i && my >= 560 - 20 && my <= 630 - 10)
				{
					playSound(CLICK);
					Setting.maxScore = (i+1)*5;
				}
			}

			for (int i = 0; i < 2; i++)
			{
				if (mx >= 720 + 180*i && mx <= 860 + 180*i && my >= 445 - 20 && my <= 510 - 10)
				{
					playSound(CLICK);
					Setting.serveSlow = !i;
				}
			}

			for (int i = 0; i < 2; i++)
			{
				if (mx >= 720 + 180*i  && mx <= 860 + 180*i && my >= 130 - 50 && my <= 195 - 50)
				{
					playSound(CLICK);
					Setting.soundOn = !i;
				}
			}

			for (int i = 0; i < 3; i++)
			{
				if (mx >= 720 + 270*i && mx <= 970 + 270*i && my >= 230 - 50 && my <= 295 - 50)
				{
					playSound(CLICK);
					Setting.whoServesFirst = i-1;
				}
			}
			
			for (int i = 0; i < 2; i++)
			{
				if (mx >= 720 + 180*i && mx <= 860 + 180*i && my >= 340 - 50 && my <= 405 - 50)
				{
					playSound(CLICK);
					Setting.keyboardSensitivity = i;
				}
			}

			backButtonClicked(mx, my);
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
	else if (currentPage == 1)
	{
		if (player1ControlIsMouse && !haveResume)
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
		else if (haveResume)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mx >= 560 && mx <= 1000 && my >= 460 - 80*i - 30 && my <= 525 - 80*i - 30)
				{
					pauseMenuButtonHovered = i;
				}
				else pauseButtonHovered = -1;
			}
		}

		if (mx >= 18 && mx <= 78 && my >= screenHeight - 85 && my <= screenHeight - 25) pauseButtonHovered = 1;
		else pauseButtonHovered = 0;
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

		backButtonHover(mx, my);
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

		backButtonHover(mx, my);
	}
	else if (currentPage == 2)
	{
		backButtonHover(mx, my);
	}

	
}


/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key) 
{
	if (key == 'q') {
		printf("%d %d\n", screenWidth, screenHeight);
		exit(0);
	}
	if (key == 'b') 
	{
		if (currentPage != -1) currentPage = -1;
	}
	if (key == 'p')
	{
		iPauseTimer(0);
	}
	if (!player2.isComputer)
	{
		if (key == 'w' && player2.barMoveState != 1)
		{
			player2.destination = player2.barY + player2.barDY;
		}
		if (key == 'x' && player2.barMoveState != -1)
		{
			player2.destination = player2.barY - player2.barDY;
		}
	}
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
void iSpecialKeyboard(unsigned char key) 
{
	if (key == GLUT_KEY_END) 
	{
		exit(0);
	}
	if (key == GLUT_KEY_UP && player1.barMoveState != 1 && !player1ControlIsMouse)
	{
		player1.destination = player1.barY + player1.barDY;
	}
	if (key == GLUT_KEY_DOWN && player1.barMoveState != -1 && !player1ControlIsMouse)
	{
		player1.destination = player1.barY - player1.barDY;
	}
	//place your codes for other keys here
}

int main() 
{
	//place your own initialization codes here.
	//if (currentPage == 1) iSetTimer()
	iInitialize(screenWidth, screenHeight, "The Pong Game");
	return 0;
}