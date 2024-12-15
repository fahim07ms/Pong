#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#ifndef windows_h
#define windows_h
#endif	

#define PI 3.141592654
#define DEFAULT_SETTING {.maxScore = 5, .serveSlow = true, .whoServesFirst = 0, .keyboardSensitivity = 1, .soundOn = true};
#define DEFAULT_BALL {.x = 768, .y = 400, .velocity = ballVelocity[0], .angle = 0, .radius = 12};
#define DEFAULT_PLAYER1 {.playerNo = 1, .isComputer = 0, .width = 16, .height = 100, .barX = 1536 - 235, .barY = 360, .barDY = 70, .barMoveState = 0, .score = 0, .destination = 360, .maxSpeed = 10};
#define DEFAULT_PLAYER2 {.playerNo = 2, .isComputer = 1, .width = 16, .height = 100,  .barX = 220, .barY = 360, .barDY = 70, .barMoveState = 0,.score = 0,.destination = 360,.maxSpeed = 8};

int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
int menuTopMargin = 680;
int menuLeftMargin = 1000;
int borderBridth = 20;

int menuButtonHovered = -1;
int playerButtonHovered = -1;
int controlButtonHovered = -1;
int backButtonHovered = 0;
int pauseButtonHovered = 0;
int pauseMenuButtonHovered = -1;
int gameOverButtonHovered = -1;

int prevPage = -1;
int currentPage = -1;
bool introGiven = false;

int winner = 0;
int player1ControlIsMouse = 0;

// If there is any game to resume or not
bool haveResume = false;
bool gameHasEnd = false;
bool gameHasStarted = false;

// Colors
int violet[3] = {124, 23, 255};
int black[3] = {4, 4, 4};

bool ballServed = false;
double ballVelocity[3] = {8.0, 16.0, 18.0};

bool randomHit = false;
int ball2DX = 0;
int ball2DY = 0;
int ballHitted = false;

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

struct Balls ball1 = DEFAULT_BALL;
struct Balls ball2 = {.x = 200, .y = 40, .velocity = 20.0, .angle = 0, .radius = 12};

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

struct Players player1 = DEFAULT_PLAYER1;
struct Players player2 = DEFAULT_PLAYER2;

int ballServer = player1.playerNo;

// Menu Options Image
char menuOptions[5][100] = {".\\assets\\images\\resume.bmp", ".\\assets\\images\\newGame.bmp", ".\\assets\\images\\settings.bmp", ".\\assets\\images\\instructions.bmp", ".\\assets\\images\\exit.bmp"};
char settingsMaxScore[4][100] = {".\\assets\\settings\\maxscore5.bmp", ".\\assets\\settings\\maxscore10.bmp", ".\\assets\\settings\\maxscore15.bmp", ".\\assets\\settings\\maxscore20.bmp"};
char yesNo[2][100] = {".\\assets\\settings\\yes.bmp", ".\\assets\\settings\\no.bmp"};
char whoServesFirst[3][100] = {".\\assets\\settings\\scorer.bmp", ".\\assets\\settings\\alternate.bmp", ".\\assets\\settings\\loser.bmp"};
char keyboardSensitivity[2][100] = {".\\assets\\settings\\low.bmp", ".\\assets\\settings\\high.bmp"};
char pauseButtons[4][100] = {".\\assets\\pause\\resume.bmp", ".\\assets\\pause\\restart.bmp", ".\\assets\\pause\\settings.bmp", ".\\assets\\pause\\mainMenu.bmp"};

// Protypes
void backButtonClicked(int mx, int my);
void backButtonHover(int mx, int my);
void drawHomePage();
bool checkGameEnd(int score);
void gameEnd(int playerNo);
void drawGamePage();
void drawSettingsPage();
void drawInstructionPage();
void playSound(sound snd);
void gamePaused();
void gameRestart();
void hoverEffect(int imgX, int imgY, char *path, int ignColor, int rectX, int rectY, int rectW, int rectH);

void hoverEffect(int imgX, int imgY, char *path, int ignColor, int rectX, int rectY, int rectW, int rectH, int var, int expVal)
{
    if (var == expVal)
    {
        iSetColor(violet[0], violet[1], violet[2]);
        iFilledRectangle(rectX, rectY, rectW, rectH);
        iShowBMP2(imgX,  imgY, path, ignColor);
    }
    else iShowBMP2(imgX,  imgY, path, ignColor);
}

void backButtonClicked(int mx, int my)
{
    if (mx >= 10 && mx <= 150 && my >= 10-70 && my <= 80-70)
    {
        playSound(CLICK);
        currentPage = prevPage;
    }
}

void backButtonHover(int mx, int my)
{
    if (mx >= 10 && mx <= 150 && my >= 10-70 && my <= 80-70) backButtonHovered = 1;
	else backButtonHovered = 0;
}

void gamePaused()
{
    haveResume = 1;
}

void gameRestart()
{
    if (prevPage == 11 || prevPage == 21)
    {
        ball1 = DEFAULT_BALL;
        player1.barX = 1536 - 235;
        player1.barY = 360;
        player1.destination = player1.barY;
        player1.score = 0;

        player2.barX = 220;
        player2.barY = 360;
        player2.destination = player2.barY;
        player2.score = 0;

        haveResume = false;
        gameHasEnd = false;
        ballServed = false;
        ballServer = player1.playerNo;
    }
    else if (prevPage == -1)
    {
        ball1 = DEFAULT_BALL;
        player1 = DEFAULT_PLAYER1;
        player2 = DEFAULT_PLAYER2;
        gameHasStarted = false;
        haveResume = false;
        gameHasEnd = false;
        ballServed = false;
        ballServer = player1.playerNo;
    }
}

void drawHomePage()
{
    iShowBMP(0, 0, ".\\assets\\images\\logo.bmp");

    int i = (haveResume) ? 0 : 1;
    for (; i < 5; i++) 
    {
        hoverEffect(menuLeftMargin, menuTopMargin - 100*i, menuOptions[i], 0, menuLeftMargin - 20, menuTopMargin- 100*i + 10, 470, 70, menuButtonHovered, i);
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
    return (score == Setting.maxScore);
}

void gameEnd(int playerNo)
{
    ball1.radius = 0;
    ball1.angle = 0;
    ball1.velocity = 0;
    winner = playerNo;
    gameHasEnd = true;

}


void drawGamePage()
{
    srand(time(NULL));
    // Drawing the scores
    char scoreImg1[100];
    char scoreImg2[100];
    sprintf(scoreImg1, ".\\assets\\scores\\%d.bmp", player1.score);
    sprintf(scoreImg2, ".\\assets\\scores\\%d.bmp", player2.score);

    iShowBMP2(788, screenHeight - 220, scoreImg1, 0);
    iShowBMP2(560, screenHeight - 220, scoreImg2, 0);

    iSetColor(violet[0], violet[1], violet[2]);
    iFilledRectangle(200, 0, screenWidth-400, borderBridth); // Bottom border
    iFilledRectangle(200, screenHeight - borderBridth, screenWidth-400, borderBridth); // Top border
    for (int i = 0; i < 20; i++) iFilledRectangle(768, borderBridth + i*40, 20, 20); // Middle point indicators

    // Ball1
    iSetColor(violet[0], violet[1],violet[2]);
    iFilledCircle(ball1.x, ball1.y, ball1.radius);

    // Ball2
    iSetColor(255, violet[1],violet[2]);
    iFilledCircle(ball2.x, ball2.y, ball2.radius);

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

    hoverEffect(18, screenHeight - 65, ".\\assets\\pause\\pause.bmp", 0, 18, screenHeight - 65, 60, 50, pauseButtonHovered, 1);

    if (haveResume)
    {
        iShowBMP(480, 80, ".\\assets\\pause\\pauseMenu.bmp");
        for (int i = 0; i < 4; i++)
        {
            hoverEffect(480, 460 - 80*i, pauseButtons[i], 0, 560, 460 - 80*i, 440, 65, pauseMenuButtonHovered, i);
        }
    }
    else if (gameHasEnd)
    {
        int winX, winY, pic1X, pic2X, pic1Y, pic2Y, rectX, rectY;
        pic1Y = screenHeight - 450;
        pic2Y = 240;
        winY = screenHeight - 320;
        //printf("%d\n", winner);
        if (winner == 1)
        {
            winX = screenWidth - 550;
            pic1X = screenWidth - 605;
            pic2X = screenWidth - 635;
        }
        else if (winner == 2)
        {
            winX = 400;
            pic1X = 345;
            pic2X = 315;
        }

        iShowBMP(winX, winY, ".\\assets\\gameover\\win.bmp");
        hoverEffect(pic1X, pic1Y, ".\\assets\\gameover\\restart.bmp", 0, pic1X, pic1Y, 260, 70, gameOverButtonHovered, 1);
        hoverEffect(pic2X, pic2Y, ".\\assets\\gameover\\menu.bmp", 0, pic2X, pic2Y, 315, 70, gameOverButtonHovered, 2);        
    }
    
}

void drawBackButton()
{
    hoverEffect(10, 10, ".\\assets\\settings\\back.bmp", 0, 10, 10, 140, 65, backButtonHovered, 1);
}

void drawSettingsPage()
{
    iShowBMP(0, 0, ".\\assets\\settings\\settings_menu2.bmp");

    
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
        hoverEffect(720 + 180*i, 560, settingsMaxScore[i], 0, 720 + 180*i, 560, 140, 70, Setting.maxScore, (i+1)*5);
    }

    // For slow serve
    for (int i = 0; i < 2; i++)
    {
        hoverEffect(720 + 180*i, 445, yesNo[i], 0, 720 + 180*i, 445, 140, 65, !(Setting.serveSlow), i);
    }

    // For keyboard sensitivity
    for (int i = 0; i < 2; i++)
    {
        hoverEffect(720 + 180*i, 340, keyboardSensitivity[i], 0, 720 + 180*i, 340, 140, 65, Setting.keyboardSensitivity, i);
    }

    // For who serves first
    for (int i = 0; i < 3; i++)
    {
        hoverEffect(720 + 270*i, 230, whoServesFirst[i], 0, 720 + 270*i, 230, 250, 65, Setting.whoServesFirst, i-1);
    }

    // For sound
    for (int i = 0; i < 2; i++)
    {
        hoverEffect(720 + 180*i, 130, yesNo[i], 0, 720 + 180*i, 130, 140, 65, !Setting.soundOn, i);
    }

    drawBackButton();
}

void drawInstructionPage()
{
    iShowBMP(0, 0, ".\\assets\\images\\instructionsMenu.bmp");
    drawBackButton();
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