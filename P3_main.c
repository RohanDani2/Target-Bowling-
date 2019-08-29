#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "graphics_hal.h"
#include "Timer32_HAL.h"
#include "ButtonLED_HAL.h"
#include "ADC_HAL.h"
#include "sound.h"
#include "song.h"
extern HWTimer_t timer0, timer1;

void InitGPIO(); // This function initializes all the needed GPIO pins
void initialize(Graphics_Context *g_sContext); //initialization function

int main(void)
{
    Graphics_Context g_sContext;
    initialize(&g_sContext);
    startingScreen(&g_sContext); //puts splash screen
    button_t joystickButton; //initializes all the buttons
    initButton(&joystickButton, GPIO_PORT_P4, GPIO_PIN1, &timer0);
    button_t S1_Button;
    initButton(&S1_Button, GPIO_PORT_P5, GPIO_PIN1, &timer0);
    button_t S2_Button;
    initButton(&S2_Button, GPIO_PORT_P3, GPIO_PIN5, &timer0);
    song_t hokie_fight;
    InitSound();
    InitSongList_Fight(&hokie_fight);
    unsigned vx, vy;
    int state = 1;
    int pastState;
    bool joyStickPushedtoUp = false; //sets all the bools for the FSM
    bool joyStickPushedtoDown = false;
    bool onUp = false;
    bool onDown = false;
    bool onFirst = false;
    bool onSecond = false;
    bool inFirstScreen = true;
    bool firstScreenPrinted = false;
    bool inPlay = false;
    bool inHighScore = false;
    bool inHowToPlay = false;
    bool s1Pushed = false;
    bool s2Pushed = false;
    bool gameDone = false;
    int angle = 0; //gives in angle that direction mode starts in
    PlaySong_blocking(hokie_fight);
    while (1) {
        pastState = state;
        bool joystickPushed = ButtonPushed(&joystickButton); //checks for button input
        bool s1ButtonPushed = ButtonPushed(&S1_Button);
        bool s2ButtonPushed = ButtonPushed(&S2_Button);

        getSampleJoyStick(&vx, &vy);
        if (inFirstScreen) //only when on main menu screen
        {
            if (!firstScreenPrinted) {
                LCDClearDisplay(&g_sContext); //prints first screen if not printed
                secondScreen(&g_sContext);
                firstScreenPrinted = true;
                state = 1;
            }
            if (onDown) //joystick logic
            {
                joyStickPushedtoDown = false;
                if (!joyStickDown(vy)) {
                    onDown = false;
                }
            }
            else if (onUp) {
                joyStickPushedtoUp = false;
                if (!joyStickUp(vy)) {
                    onUp = false;
                }
            }
            else {
                if (joyStickDown(vy)) {
                    joyStickPushedtoDown = true;
                    onDown = true;
                    if (state == 1) //changes arrow based off menu choice
                    {
                        state = 2;
                    }
                    else if (state == 2) {
                        state = 3;
                    }
                }
                else if (joyStickUp(vy)) {
                    joyStickPushedtoUp = true;
                    onUp = true;
                    if (state == 2) {
                        state = 1;
                    }
                    else if (state == 3) {
                        state = 2;
                    }
                }
            }
            MoveArrow(&g_sContext, joyStickPushedtoUp, joyStickPushedtoDown,
                      state, pastState); //move arrow based on menu screen
            if (joystickPushed) {
                if (state == 1) {
                    inFirstScreen = false; //sets bool based off what screen chosen
                    inPlay = true;
                }
                else if (state == 2) {
                    inFirstScreen = false;
                    inHighScore = true;
                }
                else if (state == 3) {
                    inFirstScreen = false;
                    inHowToPlay = true;
                }
            }
        }
        else if (inPlay) //when in play mode
        {
            static bool bowlingGameScreen = false;
            if (!bowlingGameScreen) {
                LCDClearDisplay(&g_sContext);
                bowlGameGraphics(&g_sContext, state); //sets the bowling game graphics
                bowlingGameScreen = true;
            }
            gameDone = bowlingGame(&g_sContext, angle, s1ButtonPushed,
                                   s2ButtonPushed);
            if (gameDone) {
                inFirstScreen = true; //prints game over screen when done
                inPlay = false;
                bowlingGameScreen = false;
                firstScreenPrinted = false;
            }
        }
        else if (inHighScore) {
            static bool highScorePrinted = false; //prints high score screen
            if (!highScorePrinted) {
                LCDClearDisplay(&g_sContext);
                highScoreGraphics(&g_sContext, state);
                highScorePrinted = true;
            }
            if (joystickPushed) {
                inFirstScreen = true;
                inHighScore = false;
                highScorePrinted = false;
                firstScreenPrinted = false;
            }

        }
        else if (inHowToPlay) //how to play screen
        {
            static bool howToPlayPrinted = false;
            if (!howToPlayPrinted) {
                LCDClearDisplay(&g_sContext);
                howToPlayGraphics(&g_sContext, state);
                howToPlayPrinted = true;
            }
            if (joystickPushed) {
                inFirstScreen = true;
                inHowToPlay = false;
                howToPlayPrinted = false;
                firstScreenPrinted = false;
            }
            if (onFirst) {
                s1Pushed = false;
                if (SwitchStatus_Booster_Button1 != PRESSED)
                {
                    onUp = false;
                }
            }
            else if (onSecond){
                s2Pushed = false;
                if (SwitchStatus_Booster_Button2 != PRESSED) {
                    onDown = false;
                }
            }
            else {
                if (s1ButtonPushed) {
                    s1Pushed = true;
                    onUp = true;
                    LCDClearDisplay(&g_sContext);
                    instructionScreen1(&g_sContext, s1Pushed);
                }
                else if (s2ButtonPushed) {
                    s2Pushed = true;
                    onDown = true;
                    LCDClearDisplay(&g_sContext);
                    instructionScreen2(&g_sContext, s2Pushed);
                }
            }
        }
    }
}

void InitTimer()
{
// This timer is used for the stop-watch. It will be in one-shot mode.
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
    TIMER32_PERIODIC_MODE);

// We start it the first time, to make sure the timer reading is 0 when we press the Start/Stop button for the first time
    Timer32_setCount(TIMER32_0_BASE, 0);
    Timer32_startTimer(TIMER32_0_BASE, true); // start the timer in one-shot mode. This means the timer stops when it reaches 0
}

void InitGPIO()
{
    P4DIR |= JOYSTICK_BUTTON; //4.1
    P4REN |= JOYSTICK_BUTTON;

    P5DIR |= BOOSTER_BUTTON_1; //5.1
    P5REN |= BOOSTER_BUTTON_1;

    P3DIR |= BOOSTER_BUTTON_2; //3.5
    P3REN |= BOOSTER_BUTTON_2;
}

void initialize(Graphics_Context *g_sContext)
{
    WDT_A_hold(WDT_A_BASE);
    InitGPIO();
    InitGraphics(g_sContext);
    Timer32_initModule(TIMER32_0_BASE, // There are two timers, we are using the one with the index 0
            TIMER32_PRESCALER_1, // The prescaler value is 1; The clock is not divided before feeding the counter
            TIMER32_32BIT, // The counter is used in 32-bit mode; the alternative is 16-bit mode
            TIMER32_PERIODIC_MODE); //This options is irrelevant for a one-shot timer

    Timer32_initModule(TIMER32_1_BASE, // There are two timers, we are using the one with the index 1
            TIMER32_PRESCALER_1, // The prescaler value is 1; The clock is not divided before feeding the counter
            TIMER32_32BIT, // The counter is used in 32-bit mode; the alternative is 16-bit mode
            TIMER32_PERIODIC_MODE); //This options is irrelevant for a one-shot timer

    initADC();
    initJoyStick();
    startADC();

    initHWTimer0();
    initHWTimer1();
}

