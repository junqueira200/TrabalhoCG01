//
// Created by igor on 12/11/19.
//

#include "Colisao.h"


int detecColisionLadoDireito(triangle t)
{
    if(gameStarted == 1){

        if(position[1]+BALL_RADIUS < t.v[2].y)
            return false;
        if(position[1] - BALL_RADIUS > t.v[1].y)
            return false;
        if(position[0] + BALL_RADIUS < t.v[2].x)
            return false;
        if(position[0] - BALL_RADIUS > t.v[1].x)
            return false;

        return true;
    }
    else{
        return false;
    }
}



int detecColisionLadoEsquerdo(triangle t)
{
    if(gameStarted ==1){

        if(position[1]+BALL_RADIUS < t.v[1].y)
            return false;
        if(position[1] - BALL_RADIUS > t.v[2].y)
            return false;
        if(position[0] + BALL_RADIUS < t.v[1].x)
            return false;
        if(position[0] - BALL_RADIUS > t.v[2].x)
            return false;

        return true;
    }else{
        return false;
    }
}


bool chekColisionwithBall(float xP, float yP)
{

    auxColisionObjects.v[0].x = xP-0.5;
    auxColisionObjects.v[0].y = yP+0.5;
    auxColisionObjects.v[0].z = 0.2;

    auxColisionObjects.v[1].x = xP+0.5;
    auxColisionObjects.v[1].y = yP-0.5;
    auxColisionObjects.v[1].z = 0.2;

    auxColisionObjects.v[2].x = xP+0.5;
    auxColisionObjects.v[2].y = yP+0.5;
    auxColisionObjects.v[2].z = 0.2;

    if(detecColisionLadoEsquerdo(auxColisionObjects))
        return true;

    auxColisionObjects.v[0].x = xP-0.5;
    auxColisionObjects.v[0].y = yP+0.5;
    auxColisionObjects.v[0].z = 0.2;

    auxColisionObjects.v[1].x = xP+0.5;
    auxColisionObjects.v[1].y = yP-0.5;
    auxColisionObjects.v[1].z = 0.2;

    auxColisionObjects.v[2].x = xP-0.5;
    auxColisionObjects.v[2].y = yP-0.5;
    auxColisionObjects.v[2].z = 0.2;
    if(detecColisionLadoDireito(auxColisionObjects))
        return true;

    return false;

}
