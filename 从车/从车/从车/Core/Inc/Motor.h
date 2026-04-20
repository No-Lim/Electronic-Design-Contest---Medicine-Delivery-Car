#ifndef __MOTOR_H
#define __MOTOR_H

extern void Moto_forward_run();
extern void Moto_back_run();
extern void Moto_turn_left();
extern void Moto_turn_right();
extern void Moto_stop();

void Motor_Init();
void SetSpeed_L(int16_t Speed);
void SetSpeed_R(int16_t Speed);

#endif
