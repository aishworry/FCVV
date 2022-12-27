/**
 * @file motor.ino
 * @master flight_control_v101.ino
 * @version 0.1
 * @date 2022-28-05
 * selector_object_val
 **/
#include"important.ino"
const int numMotors{4};
unsigned long timer_channel[4],esc_loop_timer;
float esc[numMotors];
//bool counter_motors[4];
void  setMotors(unsigned long *timer);
void calcMotors(const float *input);
void calcMotors(const float *input)
{
    esc[1] = input[0] - input[2] + input[1] - input[3]; //Calculate the pulse for esc 1 (front-right - CCW)
    esc[2] = input[0] + input[2] + input[1] + input[3]; //Calculate the pulse for esc 2 (rear-right - CW)
    esc[3] = input[0] + input[2] - input[1] - input[3]; //Calculate the pulse for esc 3 (rear-left - CCW)
    esc[4] = input[0] - input[2] - input[1] + input[3]; //Calculate the pulse for esc 4 (front-left - CW)

    if (esc[1] < 1100) esc[1] = 1100;                                         //Keep the motors running.
    if (esc[2] < 1100) esc[2] = 1100;                                         //Keep the motors running.
    if (esc[3] < 1100) esc[3] = 1100;                                         //Keep the motors running.
    if (esc[4] < 1100) esc[4] = 1100;                                         //Keep the motors running.

    if (esc[1] > 2000) esc[1] = 2000;                                           //Limit the esc-1 pulse to 2000us.
    if (esc[2] > 2000) esc[2] = 2000;                                           //Limit the esc-2 pulse to 2000us.
    if (esc[3] > 2000) esc[3] = 2000;                                           //Limit the esc-3 pulse to 2000us.
    if (esc[4] > 2000) esc[4] = 2000;                                           //Limit the esc-4 pulse to 2000us.  
  
}

//gives motors the signals , first element timer since loop begun,a function with void return type with no arguments to run pipelining (if any to save time).
void setMotors(unsigned long &loop_timer,void (*customFunction)())
{
  //The refresh rate is 250Hz. That means the esc's need there pulse every 4ms.
  while(micros() - loop_timer < 4000);                                      //We wait until 4000us are passed.
  loop_timer = micros();                                                    //Set the timer for the next loop.

  motor_1_port |= motor_1_register;
  motor_2_port |= motor_2_register;
  motor_3_port |= motor_3_register;
  motor_4_port |= motor_4_register;
  
  timer_channel[1] = esc[1] + loop_timer;                                     //Calculate the time of the faling edge of the esc-1 pulse.
  timer_channel[2] = esc[2] + loop_timer;                                     //Calculate the time of the faling edge of the esc-2 pulse.
  timer_channel[2] = esc[3] + loop_timer;                                     //Calculate the time of the faling edge of the esc-3 pulse.
  timer_channel[3] = esc[4] + loop_timer;                                     //Calculate the time of the faling edge of the esc-4 pulse.
  
  //There is always 1000us of spare time. So let's do something usefull that is very time consuming.
  //Get the current gyro and receiver data and scale it to degrees per second for the pid calculations.
  customFunction();
  bool counter_motors[4]={true};
  while((counter_motors[0]&&counter_motors[1]&&counter_motors[2]&&counter_motors[3]))
  {                                                       //Stay in this loop until output 4,5,6 and 7 are low.
    esc_loop_timer = micros();                                             
    if(timer_channel[4] <= esc_loop_timer){motor_1_port &= motor_1_register;counter_motors[0]=false;}
    if(timer_channel[4] <= esc_loop_timer){motor_2_port &= motor_2_register;counter_motors[1]=false;}
    if(timer_channel[4] <= esc_loop_timer){motor_3_port &= motor_3_register;counter_motors[2]=false;}
    if(timer_channel[4] <= esc_loop_timer){motor_4_port &= motor_4_register;counter_motors[3]=false;}
  }
}