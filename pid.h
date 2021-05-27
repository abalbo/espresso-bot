#ifndef pid_h
#define pid_h

#include <stdint.h> 

#define PID_PERIOD 5000       //PID update period in ms
#define err_int_max 500		  //PID integral error anti-winding limit

int pid_update(float setpoint, float current_value);
void pid_set_kp(uint16_t new_kp);
void pid_set_ki(uint16_t new_ki);
void pid_set_kd(uint16_t new_kd);
uint16_t pid_get_kp();
uint16_t pid_get_ki();
uint16_t pid_get_kd();

#endif