/**
 * @file pid.ino
 * @master flight_control_v101.ino
 * @version 0.1
 * @date 2022-28-05
 * selector_object_val
 **/
#include"important.ino"
int i_counter=0;
//pid constants
//order throttle ,roll,pitch,yaw;
float pid_p_gain[4]={1,1.5,1.5,4.0};                //Gain setting for the roll P-controller
float pid_i_gain[4]={0,0.05,0.05,0.02};               //Gain setting for the roll I-controller
float pid_d_gain[4]={0,17.0,17.0,0.0};               //Gain setting for the roll D-controller
int pid_max_roll = 400;                     //Maximum output of the PID-controller (+/-)

//global variables for pid
float pid_i_mem[4] =      {0,0,0,0};
float pid_d_prev[4]=      {0,0,0,0};

float pid_error_temp;
short calcPID(const float *feedback[],const float *refrence[],float *output);
short calcPID(const float *input[],const float *setpoint[],float *output)
{
    for(i_counter=0;i_counter<max_ele;i_counter++)
    {
        pid_error_temp = input[i_counter] - setpoint[i_counter];
        pid_i_mem[i_counter] += pid_i_gain[i_counter] * pid_error_temp;

        output[i_counter] = (pid_p_gain[i_counter] * pid_error_temp) + pid_i_mem[i_counter] + (pid_d_gain[i_counter] * (pid_error_temp - pid_d_prev[i_counter]));
        
        if(output[i_counter] > pid_max_roll)output[i_counter] = pid_max_roll;
        else if(output[i_counter] < pid_max_roll * -1)output[i_counter] = (pid_max_roll * -1);
        pid_d_prev[i_counter] = pid_error_temp;

    }
    return 0;
}
