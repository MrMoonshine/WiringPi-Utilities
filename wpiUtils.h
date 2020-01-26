#ifndef HEADER_FILE
#define HEADER_FILE


//A simple PWM pin
struct pwmPin{
    unsigned int id;
    unsigned int f; //frequency in Hz
    unsigned int dc; //DutyCycle in %
    pthread_t t1;
};

//this will initialize a GPIO pin as a PWM. this pwm pin can be updated in a thread
struct pwmPin _initPwmPin(unsigned int id,unsigned int f);

void _writeDC(struct pwmPin *pin,unsigned int dc);
//WARNING every PWM pin starts a new thread!!!
void _PWM_Start(struct pwmPin *pin);
//terminate the PWM Thread
void _PWM_Stop(struct pwmPin *pin);

//read cpu temperature
float get_CPU_Temp();

//set all vector pins as output
void initVector(int *pt);

//create a vector from the bus 
void wpiVector(int *pt,int val);

//set a void function to be executed at ctrl-C
void treminationEvent(void actor());

#endif





