//
// Created by igor on 12/11/19.
//

#include "Retangulo.h"

void Rebatedor::atualizaPosicao(float x , float y )
{
    centro.z = 0;

    centro.x = x;
    centro.y = y;

    largura = 4*0.35;
    altura = 0.5*0.25;

    //Pontos superiores infEsq, infDir, supDir, supEsq;

    vetorPontos[2].x = centro.x + largura/2.0;
    vetorPontos[2].y = centro.y + altura/2.0;

    vetorPontos[3].x = centro.x - largura/2.0;
    vetorPontos[3].y = centro.y + altura/2.0;

    //Pontos inferiores

    vetorPontos[1].x = centro.x + largura/2.0;
    vetorPontos[1].y = centro.y - altura/2.0;

    vetorPontos[0].x = centro.x - largura/2.0;
    vetorPontos[0].y = centro.y - altura/2.0;

    pontosExtremos[0] = {centro.x, float(-altura/2.0 + centro.y), 0};
    pontosExtremos[1] ={float(largura/2.0 + centro.x), centro.y, 0};
    pontosExtremos[2] = {centro.x, float(altura/2.0 + centro.y), 0};
    pontosExtremos[3] = {float(-largura/2.0 + centro.x), centro.y, 0};

}

Rebatedor::Rebatedor()
{
    retangulo();
    atualizaPosicao();
}

retangulo::retangulo()
{
    centro.z = 0;
    colisao = false;
    reduzir = false;
    escala = 1.0;
}

void makeRetangulo(float x, float y, retangulo &r)
{

    r.centro.x = x;
    r.centro.y = y;

    r.largura = 0.35;
    r.altura = 0.25;

    //Pontos superiores infEsq, infDir, supDir, supEsq;

    r.vetorPontos[2].x = x + r.largura/2.0;
    r.vetorPontos[2].y = y + r.altura/2.0;

    r.vetorPontos[3].x = x - r.largura/2.0;
    r.vetorPontos[3].y = y + r.altura/2.0;

    //Pontos inferiores

    r.vetorPontos[1].x = x + r.largura/2.0;
    r.vetorPontos[1].y = y - r.altura/2.0;

    r.vetorPontos[0].x = x - r.largura/2.0;
    r.vetorPontos[0].y = y - r.altura/2.0;

/*    r.pontosExtremos[0] = {0, float(-0.125 + y), 0};
    r.pontosExtremos[1] ={float(0.175 + x), 0, 0};
    r.pontosExtremos[2] = {0, float(0.125 + y), 0};
    r.pontosExtremos[3] = {float(-0.175 + x), 0, 0};*/

    r.pontosExtremos[0] = {float(x - r.largura/2.0), float(y - r.altura/2.0), 0};
    r.pontosExtremos[1] ={float(x + r.largura/2.0), float(y - r.altura/2.0), 0};
    r.pontosExtremos[2] = {float(x + r.largura/2.0), float(y + r.altura/2.0), 0};
    r.pontosExtremos[3] = {float( x - r.largura/2.0), float(y + r.altura/2.0), 0};


}

triangle makeTriangle(float x , float y = 0, float rotation)
{
    int i;
    triangle t;

    //generate the base triangle
    for (i = 0; i < 3; ++i) {
        t.v[i].x = TRIANGLE_RADIUS * sin(rad(90 + ((2 - i) * 120)));
        t.v[i].y = TRIANGLE_RADIUS * cos(rad(90 + ((2 - i) * 120)));
    }

    // rotate the triangle
    float r = -rotation * M_PI / 180;
    for (i = 0; i < 3; ++i) {
        float x = t.v[i].x * cos(r) - t.v[i].y * sin(r);
        t.v[i].y = t.v[i].x * sin(r) + t.v[i].y * cos(r);
        t.v[i].x = x;
    }

    // translate
    for (i = 0; i < 3; ++i) {
        t.v[i].x += x;
        t.v[i].y += y;
    };

    return t;
}