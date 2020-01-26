#include<wiringPi.h>
#include<signal.h>
#include<stdio.h>
#include<pthread.h>

#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"

    //pthread_t t1;
    //pthread_create(&t1,NULL,blink,&pth);

struct pwmPin{
    unsigned int id;
    unsigned int f; //frequency in Hz
    unsigned int dc; //DutyCycle in %
    pthread_t _pwm_thr; // the thread
};

//execute the pwm
void _do_PWM_(struct pwmPin *pin){
    unsigned int __T;
    unsigned int __t_on;
    for(;;){
        if(pin->dc == 100)
        digitalWrite(pin->id,HIGH);
        else if(pin->dc == 0)
        digitalWrite(pin->id,LOW);
        else{
        __T = 1000.0/(float)(pin->f);//T in ms
        __t_on = ((float)pin->dc/100)*__T;
        //printf("ton %d; T %d",__t_on,__T);
        digitalWrite(pin->id,HIGH);
        delay(__t_on);
        digitalWrite(pin->id,LOW);
        delay(__T - __t_on);
        }
    }
}

struct pwmPin _initPwmPin(unsigned int id,unsigned int f){
    struct pwmPin p;
    p.id = id;
    p.f = f;
    p.dc = 0;
    pinMode(id,OUTPUT);
    return p;
}

void _writeDC(struct pwmPin *pin,unsigned int dc){
    if(dc >= 0 && dc <= 100)
    pin->dc = dc;
}

//WARNING every PWM pin starts a new thread!!!
void _PWM_Start(struct pwmPin *pin){
    //printf("My PWM Start::: f = %d, DC = %d \n",pin->f,pin->dc);
   pthread_create(&pin->_pwm_thr,NULL,_do_PWM_,pin);
}
//terminate the PWM Thread
void _PWM_Stop(struct pwmPin *pin){
    pthread_cancel(&pin->_pwm_thr);
}

float get_CPU_Temp(){
    
    FILE *fp;
    unsigned int th = 0;
    float tf = 0;

        fp = fopen(TEMP_PATH,"r");

            if(fp == NULL)
            return 0;

        fscanf(fp,"%d",&th);
        fclose(fp);
        return (float)th/(float)1000;

}

void initVector(int *pt){
	for(int x = 0;x<sizeof(pt);x++){
	pinMode(*(pt+x),OUTPUT);
}
}

void wpiVector(int *pt,int val){
    int b = 1;
    for(int x = 0;x<sizeof(pt);x++){
        if((val & b)!= 0)
        digitalWrite(*(pt+x),HIGH);
        else
        digitalWrite(*(pt+x),LOW);
    b =  b*2;
    }
}

void treminationEvent(void actor()){
    signal(SIGINT,actor);
}

