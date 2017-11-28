/* DriverLib Includes */
#include "driverlib.h"
#include "stdio.h"
#include "timer_a.h"
#include "string.h"
#include "math.h"
#include "time.h"

// helpful macros
#define PERIOD 468 //Gives period 4687-10Hz-100ms
//#define LEFT round(0.05*PERIOD) //for servo positioning
//#define MIDDLE round(0.075*PERIOD) //for servo positioning
//#define RIGHT round(0.1*PERIOD) //for servo positioning
#define LEFT round(0.13*PERIOD) //for servo positioning
#define MIDDLE round(0.15*PERIOD) //for servo positioning
#define RIGHT round(0.17*PERIOD) //for servo positioning
#define SERVO1 TA0CCR1
#define SERVO2 TA0CCR2
#define SERVO3 TA0CCR3
#define SERVO4 TA0CCR4
#define SERVO5 TA1CCR1
#define SERVO6 TA1CCR2
#define SERVO7 TA1CCR3
#define SERVO8 TA1CCR4

/*Timer_A PWM Configuration Parameter*/
Timer_A_PWMConfig pwmConfig1 ={
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_64,
    PERIOD,
    TIMER_A_CAPTURECOMPARE_REGISTER_1,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0 //Pulse width (in ticks. max val= period, initialize to zero)
};
Timer_A_PWMConfig pwmConfig2 ={
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_64,
    PERIOD,
    TIMER_A_CAPTURECOMPARE_REGISTER_2,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0 //Pulse width (in ticks. max val= period, initialize to zero)
};
Timer_A_PWMConfig pwmConfig3 ={
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_64,
    PERIOD,
    TIMER_A_CAPTURECOMPARE_REGISTER_3,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0 //Pulse width (in ticks. max val= period, initialize to zero)
};
Timer_A_PWMConfig pwmConfig4 ={
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_64,
    PERIOD,
    TIMER_A_CAPTURECOMPARE_REGISTER_4,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0 //Pulse width (in ticks. max val= period, initialize to zero)
};

const Timer_A_UpModeConfig upConfig = // Configure counter in Up mode
{   TIMER_A_CLOCKSOURCE_SMCLK,      // Tie Timer A to SMCLK
    TIMER_A_CLOCKSOURCE_DIVIDER_64,     // Increment counter every 1 clock cycle
    3E+3,              // Period of Timer A (this value placed in TAxCCR0)
    TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer A rollover interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable Capture Compare interrupt
    TIMER_A_DO_CLEAR            // Clear counter upon initialization
};

//Global variables and structs
float distance;
float oldDist;
float newDist;
float alpha;
float discount;
int action1;
int action2;
int action3;
int action4;
int action5;
int action6;
int action7;
int action8;
uint8_t state1;
uint8_t state2;
uint8_t state3;
uint8_t state4;
uint8_t state5;
uint8_t state6;
uint8_t state7;
uint8_t state8;
uint8_t state1old;
uint8_t state2old;
uint8_t state3old;
uint8_t state4old;
uint8_t state5old;
uint8_t state6old;
uint8_t state7old;
uint8_t state8old;
uint8_t oldState;
uint8_t newState;

const int state_space=3;
const int action_space=2;
struct servo{
    int state[state_space];
    int action[state_space][action_space];
    float Q[state_space][action_space];
};
struct servo joint[8];
int i;
uint8_t reward;
int episode;

//Helper function declarations
float maxQ(uint8_t state, struct servo joint);
uint8_t CheckState(int pos);
void InitializeSensor(void);
void InitializePWM(void);
void InitializeQ(void);
int argmax(int eps, uint8_t state, struct servo joint);
float GetDistance(void);
