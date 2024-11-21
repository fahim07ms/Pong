#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifndef windows.h
#define windows.h
#endif	

extern int screenWidth;
extern int screenHeight;

// Ball
int ballX = 780;
int ballY = 400;
int ballDX = -2;
int ballDY = 2;
int ballRadius = 10;

// Bars
int bar0X = screenWidth - 240;
int bar0Y = 380;
int bar1X = 220;
int bar1Y = 380;

// Colors
int violet[3] = {124, 23, 255};
int black[3] = {4, 4, 4};

int haveResume = 0;


// Prototypes
void ballHitWallSound();

double responsiveX(double x)
{
    return x * (screenWidth / 1536);
}

double responsiveY(double y)
{
    return y * (screenHeight / 841);
}

// Menu Options Image
char menuOptions[5][100] = {".\\assets\\menu\\resume.bmp", ".\\assets\\menu\\newGame.bmp", ".\\assets\\menu\\settings.bmp", ".\\assets\\menu\\instruct.bmp", ".\\assets\\menu\\exit.bmp"};

void drawHomePage()
{

    double sideMargin = responsiveX(60);
    double topMargin = responsiveY(100);
    double lineHeight = 32;

    iSetColor(black[0], black[1], black[2]);
    iFilledRectangle(0, 0, screenWidth, screenHeight);

    iShowBMP(sideMargin, topMargin, ".\\PongImg\\logo.bmp");

    int menuTopMargin = 680, menuLeftMargin = 1000;

    int i = (haveResume) ? 0 : 1;
    for (; i < 5; i++) iShowBMP(1000,  menuTopMargin - 100*i, menuOptions[i]);

    // for (int i = 0; i < screenWidth; i += 20)
    // {
    //     iSetColor(255, 0, 0);
    //     iLine(i, 0, i, screenHeight);
    //     iSetColor(255, 255, 255);
    //     char str[50];
    //     sprintf(str, "%d", i);
    //     iText(i, 0, str, GLUT_BITMAP_HELVETICA_10);
    // }
    // for (int i = 0; i < screenHeight; i += 20)
    // {
    //     iSetColor(255, 0, 0);
    //     iLine(0, i, screenWidth, i);
    //     iSetColor(255, 255, 255);
    //     char str[50];
    //     sprintf(str, "%d", i);
    //     iText(0, i, str, GLUT_BITMAP_HELVETICA_10);
    // }
}

void drawGamePage()
{
    srand(time(NULL));
    iSetColor(violet[0], violet[1], violet[2]);
    iFilledRectangle(200, 0, screenWidth-400, 20); // Bottom bar
    iFilledRectangle(200, screenHeight - 20, screenWidth-400, 20); // Top Bar
    for (int i = 0; i < 20; i++) iFilledRectangle(768, 20 + i*40, 20, 20); // Middle point indicators

    // Ball
    iFilledCircle(ballX, ballY, ballRadius);

    // Setup for if the ball goes away
    iSetColor(4, 4, 4);
    iFilledRectangle(0, 0, 200, screenHeight);
    iFilledRectangle(screenWidth - 200, 0, 200, screenHeight);
    iFilledRectangle(200, 20, 20,  screenHeight - 40);
    iFilledRectangle(screenWidth - 220, 20, 20, screenHeight - 40);

    iSetColor(violet[0], violet[1], violet[2]);
    iFilledRectangle(bar1X, bar1Y, 15, 100);
    iFilledRectangle(screenWidth - 240, 380, 15, 100);

    ballX += ballDX;
    ballY += ballDY;
    bar1Y = ballY;


    if (ballY < 20 || ballY > (screenHeight - 20)) 
    {
        ballHitWallSound();
        ballDY = -ballDY;
    }



    if (ballX < 200 || ballX > (screenWidth - 200)) 
    {
        ballX = 768;
        ballY = 400;

        Sleep(2000);
    }
    if (bar1Y > (screenHeight - 20 - 100)) 
    {
        bar1Y = screenHeight - 120;
    }
    else if (bar1Y - ballRadius < 20) bar1Y = 20;
    

}


void clickSound()
{
    PlaySoundW(L"./sounds/click003.wav", NULL, SND_ASYNC | SND_ALIAS);
}

void ballHitWallSound()
{
    PlaySoundW(L"./sounds/ballHitWall001.wav", NULL, SND_ASYNC | SND_ALIAS);
}
