//
// Created by igor on 12/11/19.
//
#include "Retangulo.h"
#include "Posicao.h"
#include "OpenGL_CallBack.h"
#include "objectsHandler.h"
#include <vector>

using namespace std;

#ifndef TRABALHOCG01_COLISAO_H
#define TRABALHOCG01_COLISAO_H

extern const float BALL_RADIUS;
extern triangle auxColisionObjects;
extern int timerDeathObject1;
extern int timerDeathObject2;

int detecColisionLadoDireito(triangle t);
int detecColisionLadoEsquerdo(triangle t);
bool chekColisionwithBall(float xP, float yP);
void handleColisionsWithObjects();
void killObject(int id);

#endif //TRABALHOCG01_COLISAO_H
