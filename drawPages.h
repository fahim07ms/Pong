#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#ifndef windows_h
#define windows_h
#endif	

#define PI 3.1416
int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

int menuButtonHovered = -1;
int playerButtonHovered = -1;
int maxScore = 5;
int prevPage = -1;
int controlButtonHovered = -1;
int player1ControlIsMouse = 0;

double verticalCollisionDistance = 0;

struct Balls {
    double x;
    double y;
    double velocity;
    double angle;
    double radius;
    double dx;
    double dy;
};

struct Balls ball1 = {.x = 768,
                      .y = 400,
                      .velocity = 12,
                      .angle = 0,
                      .radius = 12
                    //   .dx = -5,
                    //   .dy = 5
            };

int borderBridth = 20;

struct Players {
    int playerNo;
    int isComputer;
    int width;
    int height;
    int barX;
    int barY;
    int barDY;
    int barMoveState;
    int score;
    double maxSpeed;
};

struct Players player1 = { .playerNo = 1, 
                           .isComputer = 0, 
                           .width = 16, 
                           .height = 100,  
                           .barX = 1536 - 235, 
                           .barY = 350, 
                           .barDY = 30, 
                           .barMoveState = 0,
                           .score = 0,
                           .maxSpeed = 2.0
                           };

struct Players player2 = {
                           .playerNo = 2, 
                           .isComputer = 1, 
                           .width = 16, 
                           .height = 100,  
                           .barX = 220, 
                           .barY = 350, 
                           .barDY = 30, 
                           .barMoveState = 0,
                           .score = 0
                           };

// Colors
int violet[3] = {124, 23, 255};
int black[3] = {4, 4, 4};

// If there is any game to resume or not
int haveResume = 0;

// Prototypes
void ballHitWallSound();
void hitSound();

double responsiveX(double x)
{
    return x * (screenWidth * 1.0 / 1536);
}

double responsiveY(double y)
{
    return y * (screenHeight * 1.0 / 840);
}

int menuTopMargin = 680;
int menuLeftMargin = 1000;

// Menu Options Image
char menuOptions[5][100] = {".\\assets\\images\\resume.bmp", ".\\assets\\images\\newGame.bmp", ".\\assets\\images\\settings.bmp", ".\\assets\\images\\instructions.bmp", ".\\assets\\images\\exit.bmp"};

void drawHomePage()
{

    // double sideMargin = responsiveX(60);
    // double topMargin = responsiveY(100);
    // double lineHeight = 32;

    // iSetColor(black[0], black[1], black[2]);
    // iFilledRectangle(0, 0, screenWidth, screenHeight);

    iShowBMP(0, 0, ".\\assets\\images\\logo.bmp");

    

    int i = (haveResume) ? 0 : 1;
    for (; i < 5; i++) 
    {
        if (menuButtonHovered == i)
        {
            iSetColor(124, 23, 255);
            iFilledRectangle(menuLeftMargin - 20, menuTopMargin- 100*i + 10, 470, 70);
            iShowBMP2(menuLeftMargin,  menuTopMargin - 100*i, menuOptions[i], 0);
        }
        else iShowBMP2(menuLeftMargin,  menuTopMargin - 100*i, menuOptions[i], 0);
    }

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

bool checkGameEnd(int score)
{
    return score == maxScore;
}

void gameEnd(int playerNo)
{
    ball1.radius = 0;
    ball1.angle = 0;
    ball1.velocity = 0;
}


void drawGamePage()
{
    srand(time(NULL));
    // Drawing the scores
    char scoreImg1[100];
    char scoreImg2[100];
    sprintf(scoreImg1, ".\\assets\\scores\\%d.bmp", player1.score);
    sprintf(scoreImg2, ".\\assets\\scores\\%d.bmp", player2.score);

    iShowBMP2(560, screenHeight - 220, scoreImg1, 0);
    iShowBMP2(788, screenHeight - 220, scoreImg2, 0);


    iSetColor(violet[0], violet[1], violet[2]);
    iFilledRectangle(200, 0, screenWidth-400, borderBridth); // Bottom border
    iFilledRectangle(200, screenHeight - borderBridth, screenWidth-400, borderBridth); // Top border
    for (int i = 0; i < 20; i++) iFilledRectangle(768, borderBridth + i*40, 20, 20); // Middle point indicators

    // Ball
    iSetColor(violet[0], violet[1],violet[2]);
    iFilledCircle(ball1.x, ball1.y, ball1.radius);
    // iShowBMP2(ball1.x, ball1.y, ".\\assets\\ball.bmp", 0);

    // Setup for if the ball goes away
    iSetColor(4, 4, 4);
    iFilledRectangle(0, 0, 200, screenHeight);
    iFilledRectangle(screenWidth - 200, 0, 200, screenHeight);
    iFilledRectangle(200, 20, 20,  screenHeight - 40);
    iFilledRectangle(screenWidth - 220, 20, 20, screenHeight - 40);

    // Drawing the bars
    iSetColor(255, 255, 255);
    iFilledRectangle(player1.barX, player1.barY, player1.width, player1.height);
    iFilledRectangle(player2.barX, player2.barY, player2.width, player2.height);
    

    

}


void drawSettingsPage()
{

}

void instructionPage()
{

}


void clickSound()
{
    PlaySound(TEXT("./sounds/click003.wav"), NULL, SND_ASYNC | SND_ALIAS);
}

void hitSound()
{
    PlaySound(TEXT(".\\sounds\\paddleHit002.wav"), NULL, SND_FILENAME | SND_ASYNC);
    // Sleep(10);
    // PlaySound(NULL, 0, 0);
}