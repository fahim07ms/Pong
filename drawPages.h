#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#ifndef windows_h
#define windows_h
#endif	

#define PI 3.141592654
#define DEFAULT_SETTING {.maxScore = 10, .serveSlow = true, .whoServesFirst = 0, .keyboardSensitivity = 1, .soundOn = true};

int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

int menuButtonHovered = -1;
int playerButtonHovered = -1;
int prevPage = -1;
int controlButtonHovered = -1;
int player1ControlIsMouse = 0;
int currentPage = -1;

bool ballServed = false;
double ballVelocity[2] = {8.0, 18.0};
enum sound{CLICK, HIT};

struct Settings {
    int maxScore;
    bool serveSlow;
    int whoServesFirst;
    int keyboardSensitivity;
    bool soundOn;
};

struct Settings Setting  =  DEFAULT_SETTING;

double verticalCollisionDistance = 0;

struct Balls {
    double x;
    double y;
    double velocity;
    double angle;
    double radius;
};

struct Balls ball1 = {
    .x = 768,
    .y = 400,
    .velocity = ballVelocity[0],
    .angle = 0,
    .radius = 12
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
    int destination;
    int maxSpeed;
};

struct Players player1 = { 
    .playerNo = 1, 
    .isComputer = 0, 
    .width = 16, 
    .height = 100,  
    .barX = 1536 - 235, 
    .barY = 350, 
    .barDY = 70, 
    .barMoveState = 0,
    .score = 0,
    .destination = 350,
    .maxSpeed = 10
};

struct Players player2 = {
    .playerNo = 2, 
    .isComputer = 1, 
    .width = 16, 
    .height = 100,  
    .barX = 220, 
    .barY = 350, 
    .barDY = 70, 
    .barMoveState = 0,
    .score = 0,
    .destination = 350,
    .maxSpeed = 10
};

int ballServer = player1.playerNo;

// Colors
int violet[3] = {124, 23, 255};
int black[3] = {4, 4, 4};

// If there is any game to resume or not
int haveResume = 0;

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
char settingsMaxScore[4][100] = {".\\assets\\maxscore5.bmp", ".\\assets\\maxscore10.bmp", ".\\assets\\maxscore15.bmp", ".\\assets\\maxscore20.bmp"};
char yesNo[2][100] = {".\\assets\\settings\\yes.bmp", ".\\assets\\settings\\no.bmp"};
char whoServesFirst[3][100] = {".\\assets\\settings\\scorer.bmp", ".\\assets\\settings\\alternate.bmp", ".\\assets\\settings\\loser.bmp"};
char keyboardSensitivity[2][100] = {".\\assets\\settings\\low.bmp", ".\\assets\\settings\\high.bmp"};
void drawHomePage()
{
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
    return score == Setting.maxScore;
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
    iShowBMP(0, 0, ".\\assets\\settings_menu.bmp");

    
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

    // For max score
    for (int i = 0; i < 4; i++)
    {
        if ((i + 1) * 5 == Setting.maxScore)
        {
            iSetColor(violet[0], violet[1], violet[2]);
            iFilledRectangle(720 + 180*i, 560, 140, 70);
            iShowBMP2(720 + 180*i, 560, settingsMaxScore[i], 0);
        }
        else iShowBMP(720 + 180*i, 560, settingsMaxScore[i]);
    }

    // For slow serve
    for (int i = 0; i < 2; i++)
    {
        if (!Setting.serveSlow == i)
        {
            iSetColor(violet[0], violet[1], violet[2]);
            iFilledRectangle(720 + 180*i, 445, 140, 65);
            iShowBMP2(720 + 180*i, 445, yesNo[i], 0);
        }
        else iShowBMP(720 + 180*i, 445, yesNo[i]);
    }

    // For keyboard sensitivity
    for (int i = 0; i < 2; i++)
    {
        if (Setting.keyboardSensitivity == i)
        {
            iSetColor(violet[0], violet[1], violet[2]);
            iFilledRectangle(720 + 180*i, 340, 140, 65);
            iShowBMP2(720 + 180*i, 340, keyboardSensitivity[i], 0);
        }
        else iShowBMP(720 + 180*i, 340, keyboardSensitivity[i]);
    }

    // For who serves first
    for (int i = 0; i < 3; i++)
    {
        if (Setting.whoServesFirst == (i-1))
        {
            iSetColor(violet[0], violet[1], violet[2]);
            iFilledRectangle(720 + 270*i, 230, 250, 65);
            iShowBMP2(720 + 270*i, 230, whoServesFirst[i], 0);
        }
        else iShowBMP(720 + 270*i, 230, whoServesFirst[i]);
    }

    // For sound
    for (int i = 0; i < 2; i++)
    {
        if (!Setting.soundOn == i)
        {
            iSetColor(violet[0], violet[1], violet[2]);
            iFilledRectangle(720 + 180*i, 130, 140, 65);
            iShowBMP2(720 + 180*i, 130, yesNo[i], 0);
        }
        else iShowBMP(720 + 180*i, 130, yesNo[i]);
    }

}

void drawInstructionPage()
{

}

void playSound(sound snd)
{
    if (Setting.soundOn)
    {
        switch (snd)
        {
            case CLICK:
                PlaySound(TEXT("./sounds/click0012.wav"), NULL, SND_ASYNC | SND_ALIAS);
                break;
            case HIT:
                PlaySound(TEXT(".\\sounds\\paddleHit002.wav"), NULL, SND_FILENAME | SND_ASYNC);
                break;
        }
    }
}