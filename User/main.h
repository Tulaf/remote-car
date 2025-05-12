#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "robot.h"
#include "Incontrol.h"
#include "Irtracking.h"
#include "Irobstacle.h"
#include "Key.h"
#include "Serial.h"

#include "debug.h"

void Irscan(void);
void ControlCar_Ircontrol(void);
void Robot_Traction(void);
void Robot_Avoidance(void);

