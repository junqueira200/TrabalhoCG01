//
// Created by igor on 12/11/19.
//
#include "Retangulo.h"
#include "Posicao.h"
#include "OpenGL_CallBack.h"

#ifndef TRABALHOCG01_COLISAO_H
#define TRABALHOCG01_COLISAO_H

const float BALL_RADIUS = 0.10;
triangle auxColisionObjects;

int detecColisionLadoDireito(triangle t);
int detecColisionLadoEsquerdo(triangle t);
bool chekColisionwithBall(float xP, float yP);

#endif //TRABALHOCG01_COLISAO_H
