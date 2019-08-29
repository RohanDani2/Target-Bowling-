#define JOYSTICK_BUTTON BIT1 //P4.1
#define BOOSTER_BUTTON_1 BIT1 //5.1 //S1
#define BOOSTER_BUTTON_2 BIT5 //3.5 //S2
#define PRESSED 0 // When a button is pressed, it is grounded (logic 0)
#define RELEASED 1
#define DEBOUNCE_TIME_IN_US 100000

typedef struct
{
    int32_t x, y;
} pin;

typedef enum
{
    throwing, position, direction
} GameModes;
void InitGraphics(Graphics_Context *g_sContext_p); //initialize graphics
void LCDClearDisplay(Graphics_Context *g_sContext_p); //clear display screen
void startingScreen(Graphics_Context *g_sContext_p); //show starting image
void secondScreen(Graphics_Context *g_sContext_p); // show menu screen
void MoveArrow(Graphics_Context *g_sContext_p, bool movetoUp, bool movetoDown,
               int state, int pastState); //move arrow between the 3 choices
bool joyStickUp(unsigned int vy); //moving joystick up
bool joyStickDown(unsigned int vy);
bool joyStickLeft(unsigned int vx); //moving joystick up
bool joyStickRight(unsigned int vx);
void instructionScreen1(Graphics_Context *g_sContext_p, bool s1Pushed);
void instructionScreen2(Graphics_Context *g_sContext_p, bool s2Pushed);
void bowlGameGraphics(Graphics_Context *g_sContext_p, int state);
void highScoreGraphics(Graphics_Context *g_sContext_p, int state);
void howToPlayGraphics(Graphics_Context *g_sContext_p, int state);
char SwitchStatus_JoyStick_Button(); //HAL function to trigger joystick button
char SwitchStatus_Booster_Button1();
char SwitchStatus_Booster_Button2();
bool bowlingGame(Graphics_Context *g_sContext_p, int angle,
                 bool s1ButtonPushed, bool s2ButtonPushed);
void GameOverScreen(Graphics_Context *g_sContext_p, int scoreCount);

