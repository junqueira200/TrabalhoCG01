//
// Created by igor on 12/11/19.
//
#include "Posicao.h"

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
