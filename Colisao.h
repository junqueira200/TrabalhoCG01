//
// Created by igor on 12/11/19.
//
#include "Retangulo.h"
#include "Posicao.h"
#include "OpenGL_CallBack.h"
#include <vector>

using namespace std;

#ifndef TRABALHOCG01_COLISAO_H
#define TRABALHOCG01_COLISAO_H

const float BALL_RADIUS = 0.10;
triangle auxColisionObjects;
int timerDeathObject1 =200;
int timerDeathObject2 =200;

int detecColisionLadoDireito(triangle t);
int detecColisionLadoEsquerdo(triangle t);
bool chekColisionwithBall(float xP, float yP);
void handleColisionsWithObjects();
void killObject(int id);

#endif //TRABALHOCG01_COLISAO_H
