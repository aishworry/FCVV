/**
 * @file flight_control_v101.ino
 * @version 0.1
 * @date 2022-01-01
 * selector_object_val
 */
//

#include"important.ino"
#include"motor.ino"
#include"input.ino"
//debug
//#define debug_mode
//debug using serial communication
#ifdef debug_mode
const int serial_baud_rate=9600;
const bool if_debug{true};
const bool debug_mpu=false;
const bool debug_loop=false;
const bool debug_receiver=false;
const bool debug_motors=false;
const bool debug_flight=false;
#endif

#define auto_pilot
//fligh controls
#ifdef auto_pilot
#include "pid.ino"
#include "angles.ino"
bool auto_level{true};
#endif
//system controls
bool stop=true;
short ready_status=0;

// input Variables
unsigned long loop_timer;

//function declaration
ISR(TIMER1_COMPA_vect)
{
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
  updateIndicator();            
}
void setup()
{
    setTimerIndicator();
    set_readyLed(HIGH);
    set_errorLed(HIGH);
    delay(2000);
    ready_status=0;
    if(if_debug)
    {
        Serial.begin(serial_baud_rate);
        Serial.println("Serial communication started\nMini QUAD`PTER fcv3 \nsoftware:v101\n................\n");
    }
    if(!setupMpu)
    {
        if(debug_mpu==true)
        Serial.println("MPU setup failed\n");
    }
    if(debug_mpu==true)
    Serial.println("MPU setup completed\n");
    if(debug_mpu==true){
    Serial.println("MPU Calibrated recorded values:");
    Serial.print("gy_x : \t");
    Serial.print(gyro_calibiration_data[0]);
    Serial.print("gy_y : \t");
    Serial.print(gyro_calibiration_data[1]);
    Serial.print("gy_z : \t");
    Serial.println(gyro_calibiration_data[2]);
    Serial.print("ac_x : \t");
    Serial.print(accel_calibiration_data[0]);
    Serial.print("ac_y : \t");
    Serial.print(accel_calibiration_data[1]);
    Serial.print("ac_z : \t");
    Serial.println(accel_calibiration_data[2]);}
    //enable interrupts to catch the incoming radio pwm signal
    
    pinMode(pin_motor_1,OUTPUT);
    pinMode(pin_motor_2,OUTPUT);
    pinMode(pin_motor_3,OUTPUT);
    pinMode(pin_motor_4,OUTPUT);
    sequence(3);
    loop_timer=micros();
}
 void loop()
 {
     getInput();
     calcAngles();
     if(ready_status==0)
      {sequence(3);
      if(rx_input[0]<<1050&&rc_channel[4]<1050)
      {
          ready_status=1;
      }
     }

     if(ready_status==1)
     {set_readyLed(HIGH);set_errorLed(LOW);
      if(rc_channel[rc_channel3]<<1050&&rc_channel[4]>1450)
      {
          ready_status=2;
      }
     }
     if(ready_status==2)
     {

     }


     float gyro_input[4]={0};
     float setpoint[4]={0};
     //
     float outCalc[4]={0};
     calcPID(gyro_input,setpoint,outCalc);
     calcMotors(outCalc);
     setMotors(&loop_timer,updateMpu());
 }
