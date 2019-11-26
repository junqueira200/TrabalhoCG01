//
// Created by igor on 12/11/19.
//
#include <cstdlib>
#include <ctime>
#include "Posicao.h"
//Definição de variáveis

#include <cstdlib>

float position[2] = {0,-1.01};
float positionXObject1 = -1.5;
float positionYObject1 = 1.7;
float positionXObject2 = 1.5;
float positionYObject2 = 1.7;
float deltaXobject1 = 0.01;
float deltaYobject1 = 0.01;
float deltaXobject2 = 0.01;
float deltaYobject2 = 0.01;
float direction[2] = {0.5, 0.5};
int randIndexaux1 =0;
int randIndexaux2 =0;
int randIndexaux3 =0;
int randIndexaux4 =0;
int randIndexaux5 =0;

int numbers[30] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
double  randIncrement[30] = {0.01,-0.01,0.02,-0.02,0.03,-0.03,0.018,-0.018,0.012,-0.012,0.03,-0.03,0.07,-0.07,
                             -0.08,0.08,0.09,-0.09,0.010,-0.010,0.011,-0.011,
                             0.012,-0.012,-0.013,0.013,-0.014,0.014,0.015,-0.015};


void changeDirections(){
    randIndexaux1  = rand() % 30;
    randIndexaux2  = rand() % 30;
    randIndexaux3  = rand() % 30;
    randIndexaux4  = rand() % 30;
    randIndexaux5  = rand() % 30;

    deltaXobject2 = randIncrement[randIndexaux2];
    deltaYobject2 = randIncrement[randIndexaux3];

    deltaXobject1 = randIncrement[randIndexaux4];
    deltaYobject1 = randIncrement[randIndexaux5];

    if(numbers[randIndexaux1]%2 ==0) {
        deltaXobject2 =- deltaXobject2;
        deltaYobject2 =- deltaYobject2;

        deltaXobject1 =- deltaXobject1;
        deltaYobject1 =- deltaYobject1;
    }

}


void updateObjects1Position(int timer){
    if (positionYObject1 + 0.2 > 2 || positionYObject1 - 0.2 < -2)
        deltaYobject1 = -deltaYobject1;
    if (positionXObject1 - 0.2 < -2 || positionXObject1 + 0.2 > 2)
        deltaXobject1 = -deltaXobject1;

    if (positionYObject2 + 0.2 > 2 || positionYObject2 - 0.2 < -2)
        deltaYobject2 = -deltaYobject2;

    if (positionXObject2 - 0.2 < -2 || positionXObject2 + 0.2 > 2)
        deltaXobject2 = -deltaXobject2;

   if(timer <= 0 )
       changeDirections();

    positionXObject2 += deltaXobject2;
    positionYObject2 += deltaYobject2;

    positionXObject1 += deltaXobject1;
    positionYObject1 += deltaYobject1;

}
