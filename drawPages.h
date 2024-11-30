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
int maxScore = 10;
int prevPage = -1;
int controlButtonHovered = -1;
int player1ControlIsMouse = 0;

int verticalCollisionDistance = 0;

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
                      .velocity = 5,
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
                           .barDY = 50, 
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
                           .barDY = 35, 
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
    

    // Always change the ball's X and Y value
    ball1.x += (ball1.velocity * cos(ball1.angle));
    ball1.y += (ball1.velocity * sin(ball1.angle));
    // ball1.x += ball1.dx;
    // ball1.y += ball1.dy;

    // Change the y aixs of 1st bar according to the ball
    player2.barY = ball1.y;

    // If the ball hits the top or bottom borders
    if (ball1.y < borderBridth + ball1.radius || ball1.y > (screenHeight - borderBridth - ball1.radius)) 
    {
        ball1.angle = -ball1.angle;
        ballHitWallSound();
        // ball1.dy = -ball1.dy;
    }
    // For the left side bar (Player 2)
    // When the ball hits in left bar area
    if (ball1.x - ball1.radius - player2.width < player2.barX && !(ball1.y < player2.barY - ball1.radius || ball1.y > player2.barY + player2.height + ball1.radius))
    {
        ball1.x = player2.barX + player2.width + ball1.radius;
        // printf("BallY: %lf, Player2.BarY: %lf , Player2.width: %lf\n", ball1.y, player2.barY, player2.width);
        ball1.angle = (PI/2)*((ball1.y - player2.barY - player2.height/2)/(player2.height));
        // ball1.dx = -ball1.dx;

        // if (ball1.y == (player2.barY + player2.height/2)) 
        // {
        //     ball1.dy = 0;
        // }
        // else if (ball1.y < (player2.barY + player2.height/2)) 
        // {
        //     if (ball1.dy >= 0) ball1.dy = -2;
        // }
        // else if (ball1.y > (player2.barY + player2.height/2)) 
        // {
        //     if (ball1.dy <= 0) ball1.dy = 2;
        // }
    }
    // When the ball hits in right bar area
    if (ball1.x + ball1.radius > player1.barX && !(ball1.y < player1.barY - ball1.radius || ball1.y > player1.barY + player1.height + ball1.radius))
    {
        ball1.x = player1.barX - ball1.radius;
        ball1.angle = PI - (PI/2)*((ball1.y - player1.barY - player1.height/2)/(player1.height));
        verticalCollisionDistance = ((ball1.velocity * sin(ball1.angle)) * (screenWidth - 440)) / (ball1.velocity * cos(ball1.angle));

        int bounceDistance;
        if ((ball1.velocity * sin(ball1.angle)) < 0) bounceDistance = ball1.y - borderBridth;
        else bounceDistance = screenHeight - borderBridth - ball1.y;

        
        
        // ball1.dx = -ball1.dx;


        // if (ball1.y == (player1.barY + player1.height/2)) 
        // {
        //     ball1.dy = 0;
        // }
        // else if (ball1.y < (player1.barY + player1.height/2)) 
        // {
        //     if (ball1.dy >= 0) ball1.dy = -2;
        // }
        // else if (ball1.y > (player1.barY + player1.height/2)) 
        // {
        //     printf("Got it\n");
        //     if (ball1.dy <= 0) ball1.dy = 2;
        // }
    }
    
    
    // If the ball goes out
    if ((ball1.x < 200 || ball1.x > (screenWidth - 200)) && (ball1.y < player1.barY - ball1.radius || ball1.y > player1.barY + 100 + ball1.radius)) 
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

        ball1.angle = PI - ball1.angle;
        // ball1.dy = 0;

        Sleep(2000);
    }

    // Try that the bars does not go out of the screen
    // Left side bar
    if (player2.barY >= (screenHeight - borderBridth - player2.height)) 
    {
        player2.barY = screenHeight - borderBridth - player2.height;
    }
    else if (player2.barY <= borderBridth) player2.barY = borderBridth;

    // Right side bar
    if (player1.barY >= (screenHeight - borderBridth - player1.height)) 
    {
        player1.barY = screenHeight - borderBridth - player1.height;
        player1.barMoveState = 1;
    }
    else if (player1.barY <= borderBridth) 
    {
        player1.barY = borderBridth;
        player1.barMoveState = -1;
    }
    else
    {
        player1.barMoveState = 0;
    }

}


void drawSettingsPage()
{

}

void instructionPage()
{

}


void clickSound()
{
    PlaySoundW(L"./sounds/click003.wav", NULL, SND_ASYNC | SND_ALIAS);
}

void ballHitWallSound()
{
    PlaySoundW(L"./sounds/ballhitwall002.wav", NULL, SND_ASYNC | SND_ALIAS);
}
