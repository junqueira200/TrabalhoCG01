//
// Created by igor on 12/11/19.
//
#include "Posicao.h"

//Definição de variáveis

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

void updateObjects1Position(){

    if(positionYObject1+0.2 > 2 || positionYObject1- 0.2< -2)
        deltaYobject1 = -deltaYobject1;
    if(positionXObject1-0.2 < -2 || positionXObject1+ 0.2 >  2)
        deltaXobject1 = -deltaXobject1;

    if(positionYObject2+0.2 > 2 || positionYObject2- 0.2< -2)
        deltaYobject2 = -deltaYobject2;

    if(positionXObject2 -0.2 < -2 || positionXObject2+ 0.2 >  2)
        deltaXobject2 = -deltaXobject2;

    positionXObject2 +=deltaXobject2;
    positionYObject2 += deltaYobject2;

    positionXObject1 +=deltaXobject1;
    positionYObject1 +=deltaYobject1;
}
