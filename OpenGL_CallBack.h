//
// Created by igor on 12/11/19.
//
#include "Retangulo.h"
#include "Posicao.h"

#ifndef TRABALHOCG01_OPENGL_CALLBACK_H
#define TRABALHOCG01_OPENGL_CALLBACK_H

int width, height;
bool fullScreen = false;
bool animate = false;
bool pause = false;
float xCamera = 0;
float yCamera = -2;
bool perspective = true;
float zdist = 4.0;
float rotationX = 0, rotationY = 0;
int last_x, last_y;
int gameStarted =0;
float initialDirection = 0;
bool venceu = false;
bool perdeu = false;

void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void Spekeyboard(int key, int x, int y);
void reshape(int w, int h);
void reiniciaJogo();

#endif //TRABALHOCG01_OPENGL_CALLBACK_H
