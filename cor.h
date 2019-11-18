//
// Created by igor on 12/11/19.
//

#ifndef TRABALHOCG01_COR_H
#define TRABALHOCG01_COR_H
#include <GL/gl.h>

typedef struct
{
    GLfloat objeto_ambient[4];
    GLfloat objeto_difusa[4];
}Cor;

extern Cor corBlocos[3];


extern Cor corParede[3];

extern Cor corRebatedorLaterais[3];

#endif //TRABALHOCG01_COR_H
