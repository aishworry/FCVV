/**
 * @file angles.ino
 * @master flight_control_v101.ino
 * @version 0.1
 * @date 2022-28-05
 * selector_object_val
 **/

#include"important.ino"
//global variables for useing data 
const short calibration_count=2000;
double gyro_calibiration_data[3]={0.0};
double accel_calibiration_data[3]={0.0};
double gyro_instant_data[3]={0.0};
double accel_instant_data[3]={0.0};


bool setupMpu();
bool calib_Mpu();
bool updateMpu();
void calcAngles();

bool setupMpu()
{
    //SETUP MPU AND FILL THE FIRST 
    return 0;
}

bool calib_Mpu()
{
    int counter;
    for(counter=0;counter<calibration_count;counter++)
    {
        if(!updateMpu()){counter--;sequence(2);}
        else
        {
        gyro_calibiration_data[0]+=gyro_instant_data[0];
        gyro_calibiration_data[1]+=gyro_instant_data[1];
        gyro_calibiration_data[2]+=gyro_instant_data[2];

        accel_calibiration_data[0]+=accel_instant_data[0];
        accel_calibiration_data[1]+=accel_instant_data[1];
        accel_calibiration_data[2]+=accel_instant_data[2];
        }
        delay(1);
    }
        gyro_calibiration_data[0]/=calibration_count;
        gyro_calibiration_data[1]/=calibration_count;
        gyro_calibiration_data[2]/=calibration_count;
        accel_calibiration_data[0]/=calibration_count;
        accel_calibiration_data[1]/=calibration_count;
        accel_calibiration_data[2]/=calibration_count;
            
}