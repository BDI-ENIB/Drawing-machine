#include "BasicStepperDriver.h"
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
#define RPM 150

#define MICROSTEPS 4

#define DIRx 8
#define STEPx 9

#define DIRy 10
#define STEPy 11

BasicStepperDriver stepperx(MOTOR_STEPS, DIRx, STEPx);
BasicStepperDriver steppery(MOTOR_STEPS, DIRy, STEPy);

int posX;
int pastX = 0;
int posY;
int pastY = 0;


int x =0;
int tmp_x =0;
boolean negX = false;

int y =0;
int tmp_y =0;
boolean negY = false;

int z =0;
int tmp_z =0;


int ser;
int attache = 6;

boolean bp_x = false;
boolean bp_y = false;

void reset(){
    
   while(bp_x==false){
      x=x-1;
      stepperx.move(x);
      digitalRead(bp_x);
   }
    x=0;
    
    while(bp_y==false){
      y=y-1;
      steppery.move(y);
      digitalRead(bp_y);
   }
    y=0;
  
}


void setup() {
    Serial.begin(9600);

   //setup motor
    stepperx.begin(RPM, MICROSTEPS);
    steppery.begin(RPM, MICROSTEPS);

   //servo motor
    servo.attach(attache);

    reset();

   //Start
    Serial.print("Demar");
    Serial.println("rage");

}
void loop() {

detection();
positions();
deplacement();

}
void positions(){
  x = posX-pastX;
  y = posY-pastY;


    pastX = posX;
    pastY = posY;

 posX = 0;
 posY = 0;
  
  
  }
void deplacement(){
  
  stepperx.move(x);
  steppery.move(y);
  servo.write(z);
  
  }
void detection(){
  ser = Serial.read();
 
  if ( ser > 0) {
      switch(ser){                          //Dection des entres du serial

        case 120:                   //pour detecter x 
          while (ser != 36){
            ser = Serial.read();
            if ( ser == 36){        //Si les signe $ (signe de fin de commande) est donner
              break;
            }
            if (ser == 45){         //si il y a un signe negatif
               negX = true;
            }
              
            if ( ser >= 0) {        //Memoire du nombre
              tmp_x = (tmp_x * 10) + (ser-48);                     
            }
          }


          if (negX == true){        //mise en place du signe negatif
            tmp_x = tmp_x *-1;
            negX = false;
          }
          
          posX = tmp_x;
          tmp_x = 0;
          break;


        
        case 121:                 //pour detecter y
          while (ser != 36){
            ser = Serial.read();
            
            if ( ser == 36){      //Si les signe $ (signe de fin de commande) est donner
              break;
            }
            
            if (ser == 45){       //si il y a un signe negatif
               negY = true;
            }
            
            if ( ser >= 0) {      //Memoire du nombre
              tmp_y = (tmp_y * 10) + (ser-48);                    
            }
          }
          
            if (negY == true){     //mise en place du signe negatif
              tmp_y = tmp_y *-1;
              negY = false;
          }
          posY = tmp_y;
          tmp_y = 0;
          
          break;


          
          case 122:  //pour detecter z 
          while (ser != 36){
            ser = Serial.read();
            if ( ser == 36){
              break;
            }
            
            if ( ser >= 0) {
            tmp_z = (tmp_z * 10) + (ser-48); 
            }
            break;
          }
      }
  }
}

