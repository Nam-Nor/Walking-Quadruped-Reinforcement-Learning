#include "header.h"

/*******************MAIN FUNCTION**********************************************/

void main(void)
{
    // Halt watchdog timer
    MAP_WDT_A_holdTimer();
    //Enable floating point module
    MAP_FPU_enableModule();
    // Set LEDs at output pins and set their initial state to off
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    //Configure PWM outputs for driving servos
    InitializePWM();

    //Configure I/O and timer for ultrasonic sensor
    InitializeSensor();

    //Q-learner initialization
    InitializeQ();

    //Enable all interrupts
    MAP_Interrupt_enableMaster();


    while(1){
        reward=0;
        state1old=CheckState(SERVO1);
        state2old=CheckState(SERVO2);
        state3old=CheckState(SERVO3);
        state4old=CheckState(SERVO4);
        state5old=CheckState(SERVO5);
        state6old=CheckState(SERVO6);
        state7old=CheckState(SERVO7);
        state8old=CheckState(SERVO8);

        oldDist=GetDistance();

        //Perform pseudo-random action for all joints
        action1=argmax(episode, state1, joint[0]);
        SERVO1=joint[0].action[state1][action1];

        action2=argmax(episode, state2, joint[1]);
        SERVO2=joint[1].action[state2][action2];

        action3=argmax(episode, state3, joint[2]);
        SERVO3=joint[2].action[state3][action3];

        action4=argmax(episode, state4, joint[3]);
        SERVO4=joint[3].action[state4][action4];

        action5=argmax(episode, state5, joint[4]);
        SERVO5=joint[4].action[state5][action5];

        action6=argmax(episode, state6, joint[5]);
        SERVO6=joint[5].action[state6][action6];

        action7=argmax(episode, state7, joint[6]);
        SERVO7=joint[6].action[state7][action7];

        action8=argmax(episode, state8, joint[7]);
        SERVO8=joint[7].action[state8][action8];

        newDist=GetDistance();

        //Calculate reward based on distance covered
        if(newDist<oldDist){
            //positive reward for forward movement
            reward=reward+1;
        }
        else{
            reward=reward-10;
        }

        //Update joint states
        state1=CheckState(SERVO1);
        state2=CheckState(SERVO2);
        state3=CheckState(SERVO3);
        state4=CheckState(SERVO4);
        state5=CheckState(SERVO5);
        state6=CheckState(SERVO6);
        state7=CheckState(SERVO7);
        state8=CheckState(SERVO8);

        printf('Episode:%i, Reward:%i, Distance Change:%f3.2\n',episode,reward,(newDist-oldDist));

        //Update Q values

        joint[0].Q[state1old][action1] = (1-alpha)*joint[0].Q[state1old][action1] + alpha*(reward+discount*(maxQ(state1,joint[0])));
        joint[1].Q[state2old][action2] = (1-alpha)*joint[1].Q[state1old][action2] + alpha*(reward+discount*(maxQ(state2,joint[1])));
        joint[2].Q[state3old][action3] = (1-alpha)*joint[2].Q[state1old][action3] + alpha*(reward+discount*(maxQ(state3,joint[2])));
        joint[3].Q[state4old][action4] = (1-alpha)*joint[3].Q[state1old][action4] + alpha*(reward+discount*(maxQ(state4,joint[3])));
        joint[4].Q[state5old][action5] = (1-alpha)*joint[4].Q[state1old][action5] + alpha*(reward+discount*(maxQ(state5,joint[4])));
        joint[5].Q[state6old][action6] = (1-alpha)*joint[5].Q[state1old][action6] + alpha*(reward+discount*(maxQ(state6,joint[5])));
        joint[6].Q[state7old][action7] = (1-alpha)*joint[6].Q[state1old][action7] + alpha*(reward+discount*(maxQ(state7,joint[6])));
        joint[7].Q[state8old][action8] = (1-alpha)*joint[7].Q[state1old][action8] + alpha*(reward+discount*(maxQ(state8,joint[7])));


        episode++;//Keep track of episodes run
    }

}



/*******************HELPER FUNCTIONS**********************************************/

float maxQ(uint8_t state, struct servo joint){
    float max=0;
    float temp;
    for(i=0;i<action_space;i++){
        temp=joint.Q[state][i];
        if(temp>max)
            max=temp;
    }
    return max;
}

uint8_t CheckState(int pos){
    if(pos==LEFT)
        return 0;
    else if(pos==MIDDLE)
        return 1;
    else if(pos==RIGHT)
        return 2;
}

void InitializeSensor(void){
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN0);        // make pin P4.0 output (TRIG)
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN0);
    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5,GPIO_PIN0);         // make pin P5.0 input (ECHO)
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN0, GPIO_LOW_TO_HIGH_TRANSITION ); //Interrupt on the rising edge of HCSR04 echo
    MAP_Timer_A_configureUpMode(TIMER_A2_BASE,&upConfig);  // Configure Timer A using above struct

    MAP_GPIO_clearInterruptFlag (GPIO_PORT_P5, GPIO_PIN0);
    MAP_GPIO_enableInterrupt (GPIO_PORT_P5, GPIO_PIN0);
    MAP_Interrupt_enableInterrupt(INT_PORT5);       // Enable NVIC PORT5 interrupts
}

void InitializePWM(void){
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
}

void InitializeQ(void){
    alpha=0.75;
    discount=0.95;
    episode=1;

    for(i=0;i<8;i++){
            joint[i].action[0][0] = MIDDLE;
            joint[i].action[1][0] = LEFT;
            joint[i].action[2][0] = LEFT;
            joint[i].action[0][1] = RIGHT;
            joint[i].action[1][1] = RIGHT;
            joint[i].action[2][1] = MIDDLE;
            joint[i].state[0]=LEFT;
            joint[i].state[1]=MIDDLE;
            joint[i].state[2]=RIGHT;
            joint[i].Q[0][0] = 0;
            joint[i].Q[1][0] = 0;
            joint[i].Q[2][0] = 0;
            joint[i].Q[0][1] = 0;
            joint[i].Q[1][1] = 0;
            joint[i].Q[2][1] = 0;
        }
        //Initial state 0 for each servo
        state1=0;
        SERVO1=LEFT;
        state2=0;
        SERVO2=LEFT;
        state3=0;
        SERVO3=LEFT;
        state4=0;
        SERVO4=LEFT;
        state5=0;
        SERVO5=LEFT;
        state6=0;
        SERVO6=LEFT;
        state7=0;
        SERVO7=LEFT;
        state8=0;
        SERVO8=LEFT;
        //Seed the random generator
        srand(time(NULL));
}

int argmax(int eps, uint8_t state, struct servo joint){
    int a;
    float max=0;
    float temp;
    for(i=0;i<action_space;i++){
        temp=joint.Q[state][i]+(rand()%(action_space+1))*(1/(float)(eps+1));
        if(temp>max){
            max=temp;
            a=i;
            //a=joint.action[i];
        }
    }
    return a;
}

float GetDistance(void){
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN0);          // generate pulse
    __delay_cycles(3*10);             // for 10us
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN0);      // stop pulse
    __delay_cycles(3*30000);          // delay for 30ms (after this time echo times out if there is no object detected)
    if(distance < 20 && distance != 0) MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);  //turning LED on if distance is less than 20cm and if distance isn't 0.
    else MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    return distance;
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
