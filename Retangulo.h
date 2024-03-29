//
// Created by igor on 12/11/19.
//
#include "math.h"
#include <GL/glut.h>
#include "cor.h"
//const float TRIANGLE_RADIUS = 0.3;
#ifndef TRABALHOCG01_RETANGULO_H
#define TRABALHOCG01_RETANGULO_H


class vertex {
public:
    float x = 0, y = 0, z = 0;
};

// tirangle class
class triangle {
public:
    vertex v[3];
};

class retangulo
{
public:

    retangulo();

    vertex vetorPontos[4]; //infEsq, infDir, supDir, supEsq;
    vertex centro;

    vertex pontosExtremos[4];

    float largura, altura;
    bool colisao;
    bool reduzir;
    float escala;

};

class Rebatedor : public retangulo
{

public:
    Rebatedor();

    void atualizaPosicao(float x = 0, float y = -1.2);

};

extern retangulo retangulos[29];
extern const int NUMRETAN; //40
extern Rebatedor rebatedor;
extern const float TRIANGLE_RADIUS;
extern int fase;

triangle makeTriangle(float x = 0, float y = 0, float rotation = 0);
void makeRetangulo(float x, float y, retangulo &r);
float rad(float angle);
void desenhaRetangulo(retangulo &r);
float fixRange(float value, float min, float max, bool circular = false);

#endif //TRABALHOCG01_RETANGULO_H
