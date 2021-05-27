#include "pid.h"

uint16_t Kp=750, Ki=75, Kd=0;

float err_int;
float err_prev;

int pid_update(float setpoint, float current_value)
{
	float error = setpoint - current_value;

    if (error >= 10)
    {
    	int output = 100;
    	err_int = 0;
		err_prev = error;
		return output;
    }
    int P = error * Kp / 100;
	
	// add Ki contribuition
    err_int += error;
	
	//err_int_max upper limit
    err_int = err_int < err_int_max ? err_int : err_int_max;
	//err_int_max lower limit
	err_int = err_int > -err_int_max ? err_int : -err_int_max;

	int I = err_int * (Ki / 100) * PID_PERIOD/1000;
	
	// add Kd contribuition
	int D = (error - err_prev) * ((Kd / 100) / (PID_PERIOD/1000)); 
	err_prev = error;
	
	int output = P + I + D;

	return output;
}

void pid_set_kp(uint16_t new_kp)
{
    Kp = new_kp;
}

void pid_set_ki(uint16_t new_ki)
{
    Ki = new_ki;
}

void pid_set_kd(uint16_t new_kd)
{
    Kd = new_kd;
}

uint16_t pid_get_kp()
{
    return Kp;
}

uint16_t pid_get_ki()
{
    return Ki;
}

uint16_t pid_get_kd()
{
    return Kd;
}