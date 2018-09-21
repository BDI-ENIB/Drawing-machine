#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"

#include <Servo.h>


Servo servo;


/*
  x ou y ou z represente l'axe de deplacement
  le chiffre represente l'angle de rotation
  $ est le code pour fermer la boucle


  Example de commande de demande:
  x90$ y70$ z90$  x-80$ y-70$



*/


//Moteur pas a pas config

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 100

#define MICROSTEPS 4

#define DIRx 8
#define STEPx 9

#define DIRy 10
#define STEPy 11

BasicStepperDriver stepperx(MOTOR_STEPS, DIRx, STEPx);
BasicStepperDriver steppery(MOTOR_STEPS, DIRy, STEPy);
SyncDriver controller(stepperx, steppery);

//calcul de positionnement et deplacement
int posX;
int pastX = 0;
int posY;
int pastY = 0;
boolean posZ = false;

// comunication
int x = 0;
int tmp_x = 0;

int y = 0;
int tmp_y = 0;

int z = 0;
int tmp_z = 0;

// Servo motor
int ser;
int attache = 6;

// capteur fin de course
int bp_x = 1;
int bp_y = 1;
int entbp_x = 12;
int entbp_y = 13;

void reset() {

  servo.write(90);

  while (bp_x == 1) {
    stepperx.move(-4);
    bp_x = digitalRead(entbp_x);
  }
  pastX = 0;

  while (bp_y == 1) {
    steppery.move(-4);
    bp_y = digitalRead(entbp_y);
  }
  pastY = 0;

}


void setup() {
  Serial.begin(9600);

  //setup motor
  stepperx.begin(RPM, MICROSTEPS);
  steppery.begin(RPM, MICROSTEPS);

  //servo motor
  servo.attach(attache);

  pinMode(entbp_x, INPUT_PULLUP);
  pinMode(entbp_y, INPUT_PULLUP);

  reset();

  //Start
  Serial.print("Demar");
  Serial.println("rage");

}
void loop() {

  detection();
  if (posX != 0 or posY != 0 or posZ == true) {
    positions();
    deplacement();
  }
}
void positions() {

  x = pastX - posX;
  y = pastY - posY;


  pastX = posX;
  pastY = posY;

  posX = 0;
  posY = 0;
  posZ = false;


}
void deplacement() {

  servo.write(z);

  controller.move(x, y);


}
void detection() {
  ser = Serial.read();

  if ( ser > 0) {
    switch (ser) {                        //Dection des entres du serial

      case 120:                   //pour detecter x
        while (ser != 32) {
          ser = Serial.read();
          if ( ser == 32) {      //Si le signe " "(espace) (signe de fin de commande) est donner
            break;
          }

          if ( ser >= 0) {        //Memoire du nombre
            tmp_x = (tmp_x * 10) + (ser - 48);
          }
        }

        posX = tmp_x;
        tmp_x = 0;
        break;



      case 121:                 //pour detecter y
        while (ser != 32) {
          ser = Serial.read();

          if ( ser == 32) {     //Si les signe " "(espace) (signe de fin de commande) est donner
            break;
          }


          if ( ser >= 0) {      //Memoire du nombre
            tmp_y = (tmp_y * 10) + (ser - 48);
          }
        }


        posY = tmp_y;
        tmp_y = 0;

        break;



      case 122:  //pour detecter z
        while (ser != 32) {
          ser = Serial.read();
          posZ = true;
          if ( ser == 32) {     //Si le signe " "(espace) (signe de fin de commande) est donner
            break;
          }

          if ( ser >= 0) {
            tmp_z = (tmp_z * 10) + (ser - 48);
          }
          z = tmp_z;
          tmp_z = 0;

          break;
        }
    }
  }
}

