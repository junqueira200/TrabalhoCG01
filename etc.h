//
// Created by igor on 31/08/19.
//

#ifndef TRABALHOCG01_ETC_H
#define TRABALHOCG01_ETC_H
/**
 * Structs and utils here
 */

// Vertex class
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

    retangulo()
    {
        centro.z = 0;
        colisao = false;
        reduzir = false;
        escala = 1.0;
    }

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
    Rebatedor()
    {
        retangulo();
        atualizaPosicao();
    }

    void atualizaPosicao(float x = 0, float y = -1.2)
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

};

#endif //TRABALHOCG01_ETC_H
