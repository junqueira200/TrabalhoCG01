//
// Created by igor on 12/11/19.
//
#include "Retangulo.h"
#include "Posicao.h"

#ifndef TRABALHOCG01_OPENGL_CALLBACK_H
#define TRABALHOCG01_OPENGL_CALLBACK_H

//Declaração de variáveis
extern int width;
extern int height;
extern bool fullScreen;
extern bool animate;
extern bool pause ;
extern float xCamera;
extern float yCamera ;
extern bool perspective;
extern float zdist;
extern float rotationX;
extern float rotationY;
extern int last_x;
extern int last_y;
extern int gameStarted;
extern int onStartScreen;
extern float initialDirection;
extern bool venceu;
extern bool perdeu;

void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void Spekeyboard(int key, int x, int y);
void reshape(int w, int h);
void reiniciaJogo();

#endif //TRABALHOCG01_OPENGL_CALLBACK_H
