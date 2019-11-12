//
// Created by igor on 12/11/19.
//

#ifndef TRABALHOCG01_COR_H
#define TRABALHOCG01_COR_H

typedef struct
{
    GLfloat objeto_ambient[4];
    GLfloat objeto_difusa[4];
}Cor;

Cor corBlocos[3] = {{{ 0.0, 0.0, 0.0, 0.1 }, { 1, 0, 0, 1.0 }},
                    {{ 0.0, 0.0, 1.0, 0.1 }, { 0.0, 1.0, 0.0, 1.0 }},
                    {{ 0.0, 1.0, 0.0, 0.1 }, { 0.0, 0.0, 1.0, 1.0 }}};


Cor corParede[3] = {{{ 0.6, 0.6, 0.0, 0.1 }, { 0.6, 0.6, 0.0, 1.0 }},
                    {{ 0,1.0,1.0, 0.1 }, { 0,1.0,1.0, 1.0 }},
                    {{ 139/255.0,69/255.0,19/255.0, 0.1 }, { 139/255.0,69/255.0,19/255.0, 1.0 }}};

Cor corRebatedorLaterais[3] = {{{ 0.0, 0.0, 0.0, 0.1 },{ 0.0, 1.0, 0.0, 1.0 }},
                               {{ 0.0, 0.0, 0.0, 0.1 },{ 0.0, 0.0, 1.0, 1.0 }},
                               {{ 0.0, 0.0, 0.0, 0.1 },{ 1.0, 0.0, 0.0, 1.0 }}};

#endif //TRABALHOCG01_COR_H
