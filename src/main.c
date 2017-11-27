/* DriverLib Includes */
#include "driverlib.h"
#include "stdio.h"
#include "timer_a.h"
#include "string.h"
#include "math.h"

// helpful macros
int i;
#define WAIT i=0;while(i<100000)i++;
#define PERIOD 468 //Gives period 4687-10Hz-100ms
//#define LEFT round(0.05*PERIOD) //for servo positioning
//#define MIDDLE round(0.075*PERIOD) //for servo positioning
//#define RIGHT round(0.1*PERIOD) //for servo positioning
#define LEFT round(0.13*PERIOD) //for servo positioning
#define MIDDLE round(0.15*PERIOD) //for servo positioning
#define RIGHT round(0.17*PERIOD) //for servo positioning
//#define RIGHT round(0.15*PERIOD)

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

//Global variables:
float distance;


void main(void)
{
    // Halt watchdog timer
    MAP_WDT_A_holdTimer();
    //Enable floating point module
    MAP_FPU_enableModule();
    // Set LEDs at output pins and set their initial state to off
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /************************ Configuring PWM outputs for driving servos ***********************/
    unsigned int dcoFrequency= 3E+6;
    MAP_CS_setDCOFrequency(dcoFrequency); // Set DCO clock source frequency
    MAP_CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // Tie SMCLK to DCO
    //Configuring GPIO2.4, 2.5, 2.6, 2.7 as peripheral outputs for PWM (shoulder servos)
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig1);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig3);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig4);
    //Configuring GPIO7.4, 7.5, 7.6, 7.7 as peripheral outputs for PWM (knee servos)
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN6,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig1);
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig2);
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig3);
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig4);

    /************************ Configuring I/O and timer for ultrasonic sensor  ***********************/
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN0);        // make pin P4.0 output (TRIG)
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN0);
    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5,GPIO_PIN0);         // make pin P5.0 input (ECHO)
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN0, GPIO_LOW_TO_HIGH_TRANSITION ); //Interrupt on the rising edge of HCSR04 echo
    MAP_Timer_A_configureUpMode(TIMER_A2_BASE,&upConfig);  // Configure Timer A using above struct

    MAP_GPIO_clearInterruptFlag (GPIO_PORT_P5, GPIO_PIN0);
    MAP_GPIO_enableInterrupt (GPIO_PORT_P5, GPIO_PIN0);
    MAP_Interrupt_enableInterrupt(INT_PORT5);       // Enable NVIC PORT5 interrupts
    MAP_Interrupt_enableMaster();

    while(1){
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN0);          // generate pulse
        __delay_cycles(3*10);             // for 10us
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN0);      // stop pulse
        __delay_cycles(3*30000);          // delay for 30ms (after this time echo times out if there is no object detected)
        if(distance < 20 && distance != 0) MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);  //turning LED on if distance is less than 20cm and if distance isn't 0.
        else MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        printf("Distance = %f\n", distance);

        TA0CCR1=RIGHT;
        TA0CCR3=RIGHT;
        TA1CCR1=RIGHT;
        TA1CCR3=RIGHT;
        __delay_cycles(30*3*30000);
        TA0CCR1=LEFT;
        TA0CCR3=LEFT;
        TA1CCR1=LEFT;
        TA1CCR3=LEFT;
        __delay_cycles(30*3*30000);
        TA0CCR2=RIGHT;
        TA0CCR4=RIGHT;
        TA1CCR2=RIGHT;
        TA1CCR4=RIGHT;
        __delay_cycles(30*3*30000);
        TA1CCR2=LEFT;
        TA1CCR4=LEFT;
        TA0CCR2=LEFT;
        TA0CCR4=LEFT;
        __delay_cycles(30*3*30000);

    }

}


void PORT5_IRQHandler(void){
    //MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if(GPIO_PIN0 == GPIO_getInterruptStatus(GPIO_PORT_P5, GPIO_PIN0)) //rising edge
    {
        MAP_Timer_A_clearTimer(TIMER_A2_BASE);
        MAP_Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);  // Start Timer A
        while(MAP_GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN0)){} //let timer run as long as echo is high
        distance =  2.0*343.0*(float)(MAP_Timer_A_getCounterValue(TIMER_A2_BASE))/3000.0;    //calculating ECHO lenght

        //MAP_Interrupt_disableInterrupt(INT_TA2_0);
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5,GPIO_PIN0);             //clear flag
    }
}

void TA2_0_IRQHandler(void){
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_Timer_A_clearCaptureCompareInterrupt (TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
