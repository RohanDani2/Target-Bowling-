#include <ti/grlib/grlib.h>
#include "graphics_hal.h"
#include "ADC_HAL.h"
#include "math.h"
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "ButtonLED_HAL.h"
#include "sound.h"
#include "song.h"
#define val33 (33)
#define val117 (117)
#define val82 (82)
#define val53 (53)
#define val50 (50)
#define val73 (73)
#define val93 (93)
#define val102 (102)
#define val105 (105)
#define val125 (125)
#define val121 (121)

#define BALL_Y_STEP 1 //Ball Y coord step
#define smalltimer 80000
#define mediumtimer 100000
#define hightimer 300000

#define BALLSmallStep smalltimer

#define BALLMediumStep mediumtimer

#define BALLHighStep hightimer

#define PRESSED 0 // When a button is pressed, it is grounded (logic 0)
#define RELEASED 1
#define xArrow (3)
extern int firstPinCount = 0;
extern HWTimer_t timer0, timer1;

extern const Graphics_Image terrycrews8BPP_UNCOMP; //first screen image
extern const Graphics_Image bowling8BPP_UNCOMP; //second screen image

#define UP_THRESHOLD 14000 //joystick up
#define DOWN_THRESHOLD 2000 //joystick down
#define LEFT_THRESHOLD 2000 //joystick left
#define RIGHT_THRESHOLD 14000 //joystick right

extern HWTimer_t timer0, timer1;

bool joyStickUp(unsigned int vy) //checks for joystick up
{
    return (vy > UP_THRESHOLD);
}

bool joyStickDown(unsigned int vy) //checks for joystick down
{
    return (vy < DOWN_THRESHOLD);
}

bool joyStickLeft(unsigned int vx) //checks for joystick left
{
    return (vx < LEFT_THRESHOLD);
}

bool joyStickRight(unsigned int vx) //checks for joystick right
{
    return (vx > RIGHT_THRESHOLD);
}

char SwitchStatus_JoyStick_Button() //HAL function to trigger joystick button
{
    return (P4IN & JOYSTICK_BUTTON );
}

char SwitchStatus_Booster_Button1() //HAL function to trigger booster top button
{
    return (P5IN & BOOSTER_BUTTON_1 );
}

char SwitchStatus_Booster_Button2() //HAL function to trigger booster bottom button
{
    return (P3IN & BOOSTER_BUTTON_2 );
}

void InitGraphics(Graphics_Context *g_sContext_p)
{
    Crystalfontz128x128_Init(); //initialize the start up screen and setup the config
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(g_sContext_p, &g_sFontCmtt16);
    Graphics_clearDisplay(g_sContext_p);
}

void LCDClearDisplay(Graphics_Context *g_sContext_p)
{
    Graphics_clearDisplay(g_sContext_p);
}

void startingScreen(Graphics_Context *g_sContext_p)
{
    int i;
    for (i = 0; i < 5; i++) //prints splash screen
    {
        Graphics_drawImage(g_sContext_p, &terrycrews8BPP_UNCOMP, 0, 0);
    }
}

void secondScreen(Graphics_Context *g_sContext_p) //sets the second menu screen
{
    Graphics_drawImage(g_sContext_p, &bowling8BPP_UNCOMP, val50, 0);
    int8_t playString[5] = "Play";  //displaying the menu choices
    int8_t highString[12] = "High Scores";
    int8_t howToString[12] = "How To Play";
    int8_t arrowString[2] = ">";

    Graphics_drawString(g_sContext_p, playString, -1, 15, val50, true); //write the menu choices in
    Graphics_drawString(g_sContext_p, arrowString, -1, 3, val50, true); //graphics
    Graphics_drawString(g_sContext_p, highString, -1, 15, 70, true);
    Graphics_drawString(g_sContext_p, howToString, -1, 15, 90, true);
}

void MoveArrow(Graphics_Context *g_sContext_p, bool movetoUp, bool movetoDown,
               int state, int pastState)
{
    int8_t arrowString[2] = ">";
    if (movetoUp || movetoDown) //move the arrow up and down
    {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK); //moves arrow based off prev input
        if (pastState == 1)
        {
            Graphics_drawString(g_sContext_p, arrowString, -1, xArrow, val50,
                                true);
        }
        else if (pastState == 2)
        {
            Graphics_drawString(g_sContext_p, arrowString, -1, xArrow, 70,
                                true);
        }
        else if (pastState == xArrow)
        {
            Graphics_drawString(g_sContext_p, arrowString, -1, xArrow, 90,
                                true);
        }

        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        if (state == 1)
        {
            Graphics_drawString(g_sContext_p, arrowString, -1, xArrow, val50,
                                true);
        }
        else if (state == 2)
        {
            Graphics_drawString(g_sContext_p, arrowString, -1, xArrow, 70,
                                true);
        }
        else if (state == 3)
        {
            Graphics_drawString(g_sContext_p, arrowString, -1, xArrow, 90,
                                true);
        }

    }

}

void instructionScreen1(Graphics_Context *g_sContext_p, bool s1Pushed) //display the first instruction screen
{
    if (s1Pushed)
    {
        GrContextFontSet(g_sContext_p, &g_sFontCmtt12);

        int8_t string1[13] = "How to Play:"; //prints out instruction screen
        int8_t string2[20] = "First Throwing";
        int8_t string3[18] = "mode Go joystick";
        int8_t string4[16] = "Up/Down to pick";
        int8_t string5[16] = "High/Medium/Low";
        int8_t string6[6] = "Level";
        int8_t string7[14] = "S1: Change to";
        int8_t string8[10] = "Directing";
        int8_t string9[14] = "S2: Change to";
        int8_t string10[12] = "Positioning";

        Graphics_drawString(g_sContext_p, string1, -1, 3, 5, true);
        Graphics_drawString(g_sContext_p, string2, -1, 1, 21, true);
        Graphics_drawString(g_sContext_p, string3, -1, 1, val33, true);
        Graphics_drawString(g_sContext_p, string4, -1, 1, 45, true);
        Graphics_drawString(g_sContext_p, string5, -1, 1, 57, true);
        Graphics_drawString(g_sContext_p, string6, -1, 1, 69, true);
        Graphics_drawString(g_sContext_p, string7, -1, 1, 81, true);
        Graphics_drawString(g_sContext_p, string8, -1, 20, 93, true);
        Graphics_drawString(g_sContext_p, string9, -1, 1, val105, true);
        Graphics_drawString(g_sContext_p, string10, -1, 20, val117, true);
    }
    GrContextFontSet(g_sContext_p, &g_sFontCmtt16);
}

void instructionScreen2(Graphics_Context *g_sContext_p, bool s2Pushed)
{
    if (s2Pushed)
    {
        GrContextFontSet(g_sContext_p, &g_sFontCmtt12);

        int8_t string_1[13] = "How to Play:";
        int8_t string_2[18] = "In Position Mode:";
        int8_t string_3[17] = "Go JoyStick Left";
        int8_t string_4[19] = "/Right to pick";
        int8_t string_5[16] = "ball's starting";
        int8_t string_6[9] = "position";
        int8_t string_7[14] = "S1: Change to";
        int8_t string_8[10] = "Directing";
        int8_t string_9[14] = "S2: Change to";
        int8_t string_10[12] = "Positioning";

        Graphics_drawString(g_sContext_p, string_1, -1, 3, 5, true);
        Graphics_drawString(g_sContext_p, string_2, -1, 1, 21, true);
        Graphics_drawString(g_sContext_p, string_3, -1, 1, val33, true);
        Graphics_drawString(g_sContext_p, string_4, -1, 1, 45, true);
        Graphics_drawString(g_sContext_p, string_5, -1, 1, 57, true);
        Graphics_drawString(g_sContext_p, string_6, -1, 1, 69, true);
        Graphics_drawString(g_sContext_p, string_7, -1, 1, 81, true);
        Graphics_drawString(g_sContext_p, string_8, -1, 20, 93, true);
        Graphics_drawString(g_sContext_p, string_9, -1, 1, val105, true);
        Graphics_drawString(g_sContext_p, string_10, -1, 20, val117, true);
    }
    GrContextFontSet(g_sContext_p, &g_sFontCmtt16);
}
void bowlGameGraphics(Graphics_Context *g_sContext_p, int state)
{
    if (state == 1)
    {
        Graphics_Rectangle R; //prints out the game graphics
        R.xMin = 80;
        R.xMax = 100;
        R.yMin = 10;
        R.yMax = 30;

        Graphics_Rectangle R2;
        R2.xMin = 100;
        R2.xMax = 120;
        R2.yMin = 10;
        R2.yMax = 30;

        Graphics_Rectangle R3;
        R3.xMin = 80;
        R3.xMax = 100;
        R3.yMin = 30;
        R3.yMax = val50;

        Graphics_Rectangle R4;
        R4.xMin = 100;
        R4.xMax = 120;
        R4.yMin = 30;
        R4.yMax = val50;

        Graphics_Rectangle R5;
        R5.xMin = 80;
        R5.xMax = 100;
        R5.yMin = val50;
        R5.yMax = 70;

        Graphics_Rectangle R6;
        R6.xMin = 100;
        R6.xMax = 120;
        R6.yMin = val50;
        R6.yMax = 70;

        Graphics_Rectangle R7;
        R7.xMin = 80;
        R7.xMax = 100;
        R7.yMin = 70;
        R7.yMax = 90;

        Graphics_Rectangle R8;
        R8.xMin = 100;
        R8.xMax = 120;
        R8.yMin = 70;
        R8.yMax = 90;

        Graphics_Rectangle R9; //5th block on left
        R9.xMin = 80;
        R9.xMax = 100;
        R9.yMin = 90;
        R9.yMax = 110;

        Graphics_Rectangle R10; //5th block on right
        R10.xMin = 100;
        R10.xMax = 120;
        R10.yMin = 90;
        R10.yMax = 110;

        Graphics_drawRectangle(g_sContext_p, &R);
        Graphics_drawRectangle(g_sContext_p, &R2);
        Graphics_drawRectangle(g_sContext_p, &R3);
        Graphics_drawRectangle(g_sContext_p, &R4);
        Graphics_drawRectangle(g_sContext_p, &R5);
        Graphics_drawRectangle(g_sContext_p, &R6);
        Graphics_drawRectangle(g_sContext_p, &R7);
        Graphics_drawRectangle(g_sContext_p, &R8);
        Graphics_drawRectangle(g_sContext_p, &R9);
        Graphics_drawRectangle(g_sContext_p, &R10);
        Graphics_drawLine(g_sContext_p, 30, 128, 30, 0);
        Graphics_drawLine(g_sContext_p, 70, 128, 70, 0);
        Graphics_fillCircle(g_sContext_p, val50, 122, 3);
        int8_t string1[2] = "1";
        Graphics_drawString(g_sContext_p, string1, -1, 45, 98, true);
    }
}

void highScoreGraphics(Graphics_Context *g_sContext_p, int state)
{
    if (state == 2)
    {
        int8_t string1[13] = "HIGH SCORES:";
        int8_t string2[13] = "00";
        int8_t string3[13] = "00";
        int8_t string4[13] = "00";
        Graphics_drawString(g_sContext_p, string1, -1, 10, 25, true);
        Graphics_drawString(g_sContext_p, string2, -1, 10, 45, true);
        Graphics_drawString(g_sContext_p, string3, -1, 10, 65, true);
        Graphics_drawString(g_sContext_p, string4, -1, 10, 85, true);
    }
}

void howToPlayGraphics(Graphics_Context *g_sContext_p, int state)
{
    if (state == 3)
    {
        instructionScreen1(g_sContext_p, true); //plays how to play graphics
    }
}

bool bowlingGame(Graphics_Context *g_sContext_p, int angle, bool s1ButtonPushed,
                 bool s2ButtonPushed)
{
    static GameModes gamemode = throwing;
    static GameModes prevGamemode = throwing;
    pin pinArray[10] = { { 45, 17 }, { 55, 6 }, { 65, 8 }, { 45, 2 },
                         { 50, 12 }, { 60, 22 }, { 35, 12 }, { 55, 15 }, { 65,
                                                                           25 },
                         { 60, 12 } };
    static int scoreCount = 0;
    static int bowlCount = 0;
    song_t enter_sandman;
    InitSongList_Sandman(&enter_sandman);
    static bool joyStickPushedtoUp = false; //bools for joysticks u/d/l/r
    static bool joyStickPushedtoDown = false;
    static bool joyStickPushedtoLeft = false;
    static bool joyStickPushedtoRight = false;
    static bool joyStickPushedtoLeftDir = false;
    static bool joyStickPushedtoRightDir = false;
    static bool onUp = false; //FSM bools
    static bool onDown = false;
    static bool onLeft = false;
    static bool onRight = false;
    static bool onLeftDir = false;
    static bool onRightDir = false;
    static int arrowState = 1;
    static int ballXState = val50;
    static int ballYState = 122;
    static int arrowXState = 45;
    static int arrowYState = 98;
    static int lineState = 0;
    static bool lineFirsttime = true;
    static bool pinTouched = false;
    static int ballX1DirState = 50;
    static int ballX2DirState = 50;
    static bool pressed = false;
    unsigned vx, vy;
    getSampleJoyStick(&vx, &vy);
    static OneShotSWTimer_t yMoveTimer;
    static bool init = true;
    static bool ballThrown = false;
    int XORval = (vx ^ vy) / 1000;
    if (firstPinCount == 0)
    {
        Graphics_fillCircle(g_sContext_p, pinArray[XORval].x,
                            pinArray[XORval].y, 1);
        firstPinCount++;
    }

    if (gamemode == throwing)
    {
        if (init)
        {
            if (arrowState == 1) //sets the timer for the speed for the ball
            {
                InitOneShotSWTimer(&yMoveTimer, &timer0, BALLSmallStep);
                StartOneShotSWTimer(&yMoveTimer);
            }
            else if (arrowState == 2)
            {
                InitOneShotSWTimer(&yMoveTimer, &timer0, BALLMediumStep);
                StartOneShotSWTimer(&yMoveTimer);
            }
            else if (arrowState == 3)
            {
                InitOneShotSWTimer(&yMoveTimer, &timer0, BALLHighStep);
                StartOneShotSWTimer(&yMoveTimer);
            }
            init = false;
        }

        if (OneShotSWTimerExpired(&yMoveTimer))
        {
            StartOneShotSWTimer(&yMoveTimer);
            if (SwitchStatus_JoyStick_Button() == PRESSED)
            {
                if (arrowState == 1)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    int8_t string1prev[2] = "1";
                    Graphics_drawString(g_sContext_p, string1prev, -1,
                                        arrowXState, arrowYState, true);
                }
                else if (arrowState == 2)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    int8_t string2prev[2] = "2";
                    Graphics_drawString(g_sContext_p, string2prev, -1,
                                        arrowXState, arrowYState, true);
                }
                else if (arrowState == 3)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    int8_t string3prev[2] = "3";
                    Graphics_drawString(g_sContext_p, string3prev, -1,
                                        arrowXState, arrowYState, true);
                }
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_WHITE);
                pressed = true;
            }
            if (pressed)
            {
                if (ballYState > 1 && !pinTouched)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_fillCircle(g_sContext_p, ballXState, ballYState,
                                        3);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballYState = ballYState - BALL_Y_STEP;
                    Graphics_fillCircle(g_sContext_p, ballXState, ballYState,
                                        3);
                    if (ballXState == pinArray[XORval].x
                            && ballYState == pinArray[XORval].y)
                    {
                        pinTouched = true;
                    }
                }
                else
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_fillCircle(g_sContext_p, ballXState, ballYState,
                                        3);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballThrown = true;
                    pressed = false;
                }
            }
        }
    }
    switch (gamemode)
    {
    case throwing:
        if (ballThrown) //if the ball has already been thrown then it resets
        {
            bowlCount++;
            GrContextFontSet(g_sContext_p, &g_sFontCmtt12);
            int8_t string2prev[4] = "Hit"; //tells whether it has hit or miss
            int8_t string3prev[5] = "Miss";
            if ((ballXState == pinArray[XORval].x
                    && ballYState == pinArray[XORval].y))
            {
                scoreCount++;
                PlaySong_blocking(enter_sandman);
                int i = 0;
                while (i < 40)
                {
                    Graphics_drawString(g_sContext_p, string2prev, -1, val33,
                    val50,
                                        true);
                    i++;
                }
            }
            else
            {
                int j = 0;
                while (j < 40)
                {
                    Graphics_drawString(g_sContext_p, string3prev, -1, val33,
                    val50,
                                        true);
                    j++;
                }
            }
            GrContextFontSet(g_sContext_p, &g_sFontCmtt16);
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            Graphics_drawString(g_sContext_p, string2prev, -1, val33, val50,
                                true);
            Graphics_drawString(g_sContext_p, string3prev, -1, val33,
            val50,
                                true);
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_WHITE);
            char string[2]; //prints the score
            string[0] = ((scoreCount % 100) / 10) + '0';
            string[1] = ((scoreCount % 10) / 1) + '0';
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_WHITE);
            GrContextFontSet(g_sContext_p, &g_sFontCmtt16);
            if (bowlCount == 1) //prints the score in the location whether hit or miss
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val82,
                                    13, true);
            }
            else if (bowlCount == 2)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val102,
                                    13, true);
            }
            else if (bowlCount == 3)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val82,
                                    33, true);
            }
            else if (bowlCount == 4)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val102,
                                    33, true);
            }
            else if (bowlCount == 5)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val82,
                val53,
                                    true);
            }
            else if (bowlCount == 6)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val102,
                val53,
                                    true);
            }
            else if (bowlCount == 7)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val82,
                val73,
                                    true);
            }
            else if (bowlCount == 8)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val102,
                val73,
                                    true);
            }
            else if (bowlCount == 9)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val82,
                val93,
                                    true);
            }
            else if (bowlCount == 10)
            {
                Graphics_drawString(g_sContext_p, (int8_t*) string, -1, val102,
                val93,
                                    true);
            }
            GrContextFontSet(g_sContext_p, &g_sFontCmtt16);
            ballXState = val50; //resets the game assets
            ballYState = 122;
            arrowXState = 45;
            arrowYState = 98;
            ballX1DirState = 50;
            ballX2DirState = 50;
            lineFirsttime = true;
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_WHITE);
            Graphics_fillCircle(g_sContext_p, ballXState, ballYState, 3);
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(g_sContext_p, pinArray[XORval].x,
                                pinArray[XORval].y, 1);
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_WHITE);
            XORval = (vx ^ vy) / 1000;
            Graphics_fillCircle(g_sContext_p, pinArray[XORval].x,
                                pinArray[XORval].y, 1);
            ballThrown = false;
            pinTouched = false;
        }

        if (bowlCount == 10) //if the last ball is reached then the game is over
        {
            LCDClearDisplay(g_sContext_p);
            int i;
            for (i = 0; i < 10; i++)
            {
                GameOverScreen(g_sContext_p, scoreCount);
            }
            bowlCount = 0;
            scoreCount = 0;
            return true;
        }

        if (SwitchStatus_Booster_Button1() == PRESSED)
        {
            prevGamemode = gamemode;
            gamemode = position; //changes modes if the button is pressed
        }
        else if (SwitchStatus_Booster_Button2() == PRESSED)
        {
            prevGamemode = gamemode;
            gamemode = direction;
        }

        if (gamemode == throwing)
        {
            if (onDown)
            {
                joyStickPushedtoDown = false; //FSM for moving arrow up and down
                if (!joyStickDown(vy))
                {
                    onDown = false;
                }
            }
            else if (onUp)
            {
                joyStickPushedtoUp = false;
                if (!joyStickUp(vy))
                {
                    onUp = false;
                }
            }
            else
            {
                if (joyStickDown(vy))
                {
                    joyStickPushedtoDown = true;
                    onDown = true;
                }
                else if (joyStickUp(vy))
                {
                    joyStickPushedtoUp = true;
                    onUp = true;
                }
            }

            if (joyStickPushedtoUp)
            {
                if (arrowState == 2)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    int8_t string2prev[2] = "2";
                    Graphics_drawString(g_sContext_p, string2prev, -1,
                                        arrowXState, arrowYState, true);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    int8_t string3prev[2] = "3";
                    Graphics_drawString(g_sContext_p, string3prev, -1,
                                        arrowXState, arrowYState, true);
                    arrowState = 3;
                }
                if (arrowState == 1)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    int8_t string1prev[2] = "1";
                    Graphics_drawString(g_sContext_p, string1prev, -1,
                                        arrowXState, arrowYState, true);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    int8_t string2prev[2] = "2";
                    Graphics_drawString(g_sContext_p, string2prev, -1,
                                        arrowXState, arrowYState, true);
                    arrowState = 2;
                }
            }
            else if (joyStickPushedtoDown)
            {
                if (arrowState == 2)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    int8_t string2prev[2] = "2";
                    Graphics_drawString(g_sContext_p, string2prev, -1,
                                        arrowXState, arrowYState, true);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    int8_t string1prev[2] = "1";
                    Graphics_drawString(g_sContext_p, string1prev, -1,
                                        arrowXState, arrowYState, true);
                    arrowState = 1;
                }
                else if (arrowState == 3)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    int8_t string3prev[2] = "3";
                    Graphics_drawString(g_sContext_p, string3prev, -1,
                                        arrowXState, arrowYState, true);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    int8_t string2prev[2] = "2";
                    Graphics_drawString(g_sContext_p, string2prev, -1,
                                        arrowXState, arrowYState, true);
                    arrowState = 2;
                }
            }
        }
        break;
    case position:
        if (arrowState == 1)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            int8_t string1prev[2] = "1";
            Graphics_drawString(g_sContext_p, string1prev, -1, arrowXState,
                                arrowYState, true);
        }
        else if (arrowState == 2)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            int8_t string2prev[2] = "2";
            Graphics_drawString(g_sContext_p, string2prev, -1, arrowXState,
                                arrowYState, true);
        }
        else if (arrowState == 3)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            int8_t string3prev[2] = "3";
            Graphics_drawString(g_sContext_p, string3prev, -1, arrowXState,
                                arrowYState, true);
        }
        Graphics_setForegroundColor(g_sContext_p,
        GRAPHICS_COLOR_WHITE);

        if (SwitchStatus_Booster_Button1() == PRESSED)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_WHITE);
            int8_t string1prev[2] = "1";
            Graphics_drawString(g_sContext_p, string1prev, -1, arrowXState,
                                arrowYState, true);
            arrowState = 1;
            prevGamemode = gamemode;
            gamemode = throwing;
        }
        else if (SwitchStatus_Booster_Button2() == PRESSED)
        {
            prevGamemode = gamemode;
            gamemode = direction;
        }

        if (gamemode == position)
        {
            if (onLeft)
            {
                joyStickPushedtoLeft = false;
                if (!joyStickLeft(vx))
                {
                    onLeft = false;
                }
            }
            else if (onRight)
            {
                joyStickPushedtoRight = false;
                if (!joyStickRight(vx))
                {
                    onRight = false;
                }
            }
            else
            {
                if (joyStickLeft(vx))
                {
                    joyStickPushedtoLeft = true;
                    onLeft = true;
                }
                else if (joyStickRight(vx))
                {
                    joyStickPushedtoRight = true;
                    onRight = true;
                }

                if (joyStickPushedtoLeft)
                {
                    if (ballXState > 35)
                    {
                        Graphics_setForegroundColor(g_sContext_p,
                        GRAPHICS_COLOR_BLACK);
                        Graphics_fillCircle(g_sContext_p, ballXState,
                                            ballYState, 3);
                        Graphics_setForegroundColor(g_sContext_p,
                        GRAPHICS_COLOR_WHITE);
                        ballXState = ballXState - 2;
                        arrowXState = arrowXState - 2;
                        Graphics_fillCircle(g_sContext_p, ballXState,
                                            ballYState, 3);
                    }
                }
                else if (joyStickPushedtoRight)
                {
                    if (ballXState < 65)
                    {
                        Graphics_setForegroundColor(g_sContext_p,
                        GRAPHICS_COLOR_BLACK);
                        Graphics_fillCircle(g_sContext_p, ballXState,
                                            ballYState, 3);
                        Graphics_setForegroundColor(g_sContext_p,
                        GRAPHICS_COLOR_WHITE);
                        ballXState = ballXState + 2;
                        arrowXState = arrowXState + 2;
                        Graphics_fillCircle(g_sContext_p, ballXState,
                                            ballYState, 3);
                    }
                }
            }
        }
        break;
    case direction:
        Graphics_setForegroundColor(g_sContext_p,
        GRAPHICS_COLOR_WHITE);
        ballX1DirState = ballXState;
        ballX2DirState = ballXState;

        Graphics_drawLine(g_sContext_p, ballX1DirState, val125, ballX2DirState,
                          0);

        if (arrowState == 1 && lineFirsttime && prevGamemode == throwing)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            int8_t string1prev[2] = "1";
            Graphics_drawString(g_sContext_p, string1prev, -1, arrowXState,
                                arrowYState, true);
        }
        else if (arrowState == 2 && lineFirsttime && prevGamemode == throwing)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            int8_t string2prev[2] = "2";
            Graphics_drawString(g_sContext_p, string2prev, -1, arrowXState,
                                arrowYState, true);
        }
        else if (arrowState == 3 && lineFirsttime && prevGamemode == throwing)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            int8_t string3prev[2] = "3";
            Graphics_drawString(g_sContext_p, string3prev, -1, arrowXState,
                                arrowYState, true);
        }
        Graphics_setForegroundColor(g_sContext_p,
        GRAPHICS_COLOR_WHITE);

        if (SwitchStatus_Booster_Button1() == PRESSED)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, ballX1DirState, 119, ballX2DirState,
                              0);
            Graphics_setForegroundColor(g_sContext_p,
                        GRAPHICS_COLOR_WHITE);
            prevGamemode = gamemode;
            gamemode = position;
        }
        else if (SwitchStatus_Booster_Button2() == PRESSED)
        {
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, ballX1DirState, 119, ballX2DirState,
                              0);
            Graphics_setForegroundColor(g_sContext_p,
            GRAPHICS_COLOR_WHITE);
            prevGamemode = gamemode;
            gamemode = throwing;
        }

        if (onLeftDir)
        {
            joyStickPushedtoLeftDir = false;
            if (!joyStickLeft(vx))
            {
                onLeftDir = false;
            }
        }
        else if (onRightDir)
        {
            joyStickPushedtoRightDir = false;
            if (!joyStickRight(vx))
            {
                onRightDir = false;
            }
        }
        else
        {
            if (joyStickLeft(vx))
            {
                joyStickPushedtoLeftDir = true;
                onLeftDir = true;
            }
            else if (joyStickRight(vx))
            {
                joyStickPushedtoRightDir = true;
                onRightDir = true;
            }
        }

        if (joyStickPushedtoLeftDir)
        {
            if (ballX2DirState > 35)
            {
                if (lineState == -3)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 40, 0);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballX2DirState = ballX2DirState - 5;
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 35, 0);
                }
                else if (lineState == -2)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 45, 0);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballX2DirState = ballX2DirState - 5;
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 40, 0);
                    lineState = -3;
                }
                else if (lineState == -1)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 50, 0);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballX2DirState = ballX2DirState - 5;
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 45, 0);
                    lineState = -2;
                }
                else if (lineState == 0)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 55, 0);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballX2DirState = ballX2DirState - 5;
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 50, 0);
                    lineState = -1;
                }
                else if (lineState == 1)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 60, 0);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballX2DirState = ballX2DirState - 5;
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 55, 0);
                    lineState = 0;
                }
                else if (lineState == 2)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 65, 0);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballX2DirState = ballX2DirState - 5;
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 60, 0);
                    lineState = 1;
                }
                else if (lineState == 3)
                {
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_BLACK);
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 70, 0);
                    Graphics_setForegroundColor(g_sContext_p,
                    GRAPHICS_COLOR_WHITE);
                    ballX2DirState = ballX2DirState - 5;
                    Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 65, 0);
                    lineState = 2;
                }
            }
        }
        else if (joyStickPushedtoRightDir)
        {
            if (lineState == 3)
            {
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_BLACK);
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 65, 0);
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_WHITE);
                ballX2DirState = ballX2DirState - 5;
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 70, 0);
            }
            if (lineState == 2)
            {
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_BLACK);
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 60, 0);
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_WHITE);
                ballX2DirState = ballX2DirState - 5;
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 65, 0);
            }
            else if (lineState == 1)
            {
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_BLACK);
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 55, 0);
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_WHITE);
                ballX2DirState = ballX2DirState - 5;
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 60, 0);
                lineState = 2;
            }
            else if (lineState == 0)
            {
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_BLACK);
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 50, 0);
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_WHITE);
                ballX2DirState = ballX2DirState - 5;
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 55, 0);
                lineState = 1;
            }
            else if (lineState == -1)
            {
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_BLACK);
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 45, 0);
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_WHITE);
                ballX2DirState = ballX2DirState - 5;
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 50, 0);
                lineState = 0;
            }
            else if (lineState == -2)
            {
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_BLACK);
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 40, 0);
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_WHITE);
                ballX2DirState = ballX2DirState - 5;
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 45, 0);
                lineState = -1;
            }
            else if (lineState == -3)
            {
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_BLACK);
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 35, 0);
                Graphics_setForegroundColor(g_sContext_p,
                GRAPHICS_COLOR_WHITE);
                ballX2DirState = ballX2DirState - 5;
                Graphics_drawLine(g_sContext_p, ballX1DirState, 119, 40, 0);
                lineState = -2;
            }
        }
    }
    return false;
}

void GameOverScreen(Graphics_Context *g_sContext_p, int scoreCount)
{
    char string[2];
    string[0] = ((scoreCount % 100) / 10) + '0';
    string[1] = ((scoreCount % 10) / 1) + '0';
    int8_t yourScore[12] = "Your Score:";
    Graphics_drawString(g_sContext_p, yourScore, -1, 0, 5, true);
    Graphics_drawString(g_sContext_p, (int8_t*) string, -1, 80, 5, true);
}

