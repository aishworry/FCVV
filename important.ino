#define HIGH 1
#define LOW 0
/******************************************************************************
 *                         RC REMOTE and array elements
 * 
 * 
 *       |                                               |
 *       |   1.Throttle                                  | 3.Pitch
 *       |                                               |
 *       |                                               |
 *       |                                               |
 *   4.Yaw                                              2.Roll
 *   ----------------                               ----------------
 * 
 * *****************************************************************************/


//// motor type   
#define BDC
//not selected
#undef BLDC_ESC
//motor pin
#define pin_motor_1 3
#define pin_motor_2 5
#define pin_motor_3 6
#define pin_motor_4 9
#define motor_1_port PORTD
#define motor_2_port PORTD
#define motor_3_port PORTD
#define motor_4_port PORTB

char motor_1_register = B00001000;
char motor_2_register = B00100000;
char motor_3_register = B01000000;
char motor_4_register = B00000001;

//mpu
#define mpu_6050
//not selected for now

//mpu pin
#ifdef mpu_6050
#define mpu_scl A5
#define mpu_sda A4
#endif
//receiver
#define rx_rf
#undef rx_flysky
#undef rx_wifi
#undef rx_nrf24

//receiver pin for rx_flysky
#ifdef rx_flysky
#define flysky pin_int_c1 PCINT0 //pin 8
#define flysky pin_int_c2 PCINT1 //pin 9
#define flysky pin_int_c3 PCINT2 //pin 10
#define flysky pin_int_c4 PCINT4 //pin 12
const short rc_channel1 =0;
const short rc_channel2 =1;
const short rc_channel3 =2;
const short rc_channel4 =3;
#endif
#ifdef rx_rf
int rx_pin{11}
#define rf_pin_rx PCINT3
#endif
//indicators
#define error_led 13
#define ready_led 2

void setTimerIndicator();
//indicators
bool error_led_status =0;
bool ready_led_status =0;
class Indicator_Led
{
    public:
    Indicator_Led():status(false){}
    void flip(){status=status^true;digitalWrite(status);}
    void set(){status=true;digitalWrite(status);}
    void reset(){status=false;digitalWrite(status);}
    private:
    bool status;
}
Indicator_Led error_led,ready_led;
int sequence=0;

const short max_ele{4};
//indicators
void indicate(int seq)
{
    sequence=seq;
        /***************
         * patterns
         * -10:fatal error frequnt simultaneous blink error led
         * 1 :not ready :both continously on
         * 2 :getting ready :green on, red blink
         * 3 :ready :green blink
         * 4 :working : green on
         * 5 :eror(might be curable):red blink 
         * 6 :stop flight ,error :red and green alternate
        ****************/
}
void updateIndicator()
{
     switch(sequence)
       {
           case -10:
           error_led.flip();
           ready_led.reset();
           break;
           case 1:
           error_led.set();
           ready_led.set();
           break;
           case 2:
           error_led.flip();
           ready_led.set();
           break;
           case 3:
           error_led.reset();
           ready_led.flip();
           break;
           case 4:
           error_led.reset();
           ready_led.set();
           break;
           case 5:
           error_led.flip();
           ready_led.reset();
           break;
           case 6:
           error_led.flip();
           ready_led.flip();
           break;
           default:
           break;
       }
}
void setTimerIndicator()
{
    cli();                      //stop interrupts for till we make the settings
    /*1. First we reset the control register to amke sure we start with everything disabled.*/
    TCCR0A = 0;                 // Reset entire TCCR1A to 0 
    TCCR0B = 0;                 // Reset entire TCCR1B to 0
    /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
    TCCR0B |= B00000011;        //Set CS12 to 1 so we get prescalar 256  
    /*3. We enable compare match mode on register A*/
    TIMSK0 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
    /*4. Set the value of register A to 31250*/
    OCR0A = 31250;             //Finally we set compare register A to this value  
    sei();                     //Enable back the interrupts
}