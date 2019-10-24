#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
//#include "extras.h"
#include "etc.h" //TODO: reorganize modules
#include <iostream>
#include <cstring>


#define MAXLINHA 10

using namespace std;

// Definitions
float fixRange(float value, float min, float max, bool circular = false);
bool colisaoRebatedor(float movement, const float maxRange[]);

/// Globals
float zdist = 4.0;
float rotationX = 0, rotationY = 0;
int last_x, last_y;
int width, height;
int angulo = 0;
const float BALL_RADIUS = 0.10;
const float TRIANGLE_RADIUS = 0.3;
const float FPS = 60;
const float BHF = 2; // Board Half Width
bool fullScreen = false;
bool perspective = true;
const int MaxRetanHorizontal = 10;//10
const int MaxRetanVertical = 1;//4
const int NUMRETAN = 10; //40
vertex v1,v2,v3,v4,v5,v6,v0;



const float RAIORETANGULO = 0.215058132;
const float RAIO_REBATEDOR = 0.71    ;
bool pause = false;
// prismas iniciais
float prismaPositions[4][3];
char texto[100];

float velocity = 1.5 * 0.5;
float initialDirection = 0;
float direction[2] = {0.5, 0.5};
float position[2] = {0, -1.01};
triangle prismas[4];
retangulo retangulos[44];
Rebatedor rebatedor = Rebatedor();

int xAntigo;

bool animate = false;
bool venceu = false;

/// Functions
void init(void) {
    //initLight(width, height); // Função extra para tratar iluminação.

    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_LIGHT0);                   // habilita luz 0
    glEnable(GL_DEPTH_TEST);


    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
    GLfloat posicao_luz[] = { (float) width, (float)height, 1000.0, 1.0};

    // Define parametros da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    // Quando a opção "two_side lighting" está ativada, o opengl inverte a orientação das normais
    // permitindo que tanto as faces externas quanto internas sejam iluminadas.
    // Essa opção é mais lenta se comparada a renderização de apenas um dos lados
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2);
    xAntigo = glutGet(GLUT_WINDOW_WIDTH)/2;

    glutSetCursor(GLUT_CURSOR_NONE);

}

// Calculate normal
vertex calcNormal(triangle t) {
    vertex vn;
    vertex v_0 = t.v[0],
            v_1 = t.v[1],
            v_2 = t.v[2];
    vertex v1, v2;
    double len;

    /* Encontra vetor v1 */
    v1.x = v_1.x - v_0.x;
    v1.y = v_1.y - v_0.y;
    v1.z = v_1.z - v_0.z;

    /* Encontra vetor v2 */
    v2.x = v_2.x - v_0.x;
    v2.y = v_2.y - v_0.y;
    v2.z = v_2.z - v_0.z;

    /* Calculo do produto vetorial de v1 e v2 */
    vn.x = (v1.y * v2.z) - (v1.z * v2.y);
    vn.y = (v1.z * v2.x) - (v1.x * v2.z);
    vn.z = (v1.x * v2.y) - (v1.y * v2.x);

    /* normalizacao de n */
    len = sqrt(pow(vn.x, 2) + pow(vn.y, 2) + pow(vn.z, 2));

    vn.x /= len;
    vn.y /= len;
    vn.z /= len;

    return vn;
}

// calculate and set GLUT normal
void setCalcNormal(triangle t) {
    vertex normal = calcNormal(t);
    glNormal3f(normal.x, normal.y, normal.z);

}

void calculaNormal(vertex *vetor)
{
    vertex vn;
    vertex v1, v2;
    double len;

    /* Encontra vetor v1 */
    v1.x = vetor[0].x - vetor[2].x;
    v1.y = vetor[0].y - vetor[2].y;
    v1.z = vetor[0].z - vetor[2].z;

    /* Encontra vetor v2 */
    v2.x = vetor[0].x - vetor[1].x;
    v2.y = vetor[0].y - vetor[1].y;
    v2.z = vetor[0].z - vetor[1].z;

    /* Calculo do produto vetorial de v1 e v2 */
    vn.x = (v1.y * v2.z) - (v1.z * v2.y);
    vn.y = (v1.z * v2.x) - (v1.x * v2.z);
    vn.z = (v1.x * v2.y) - (v1.y * v2.x);

    /* normalizacao de n */
    len = sqrt(pow(vn.x, 2) + pow(vn.y, 2) + pow(vn.z, 2));

    vn.x /= len;
    vn.y /= len;
    vn.z /= len;

    glNormal3b(vn.x, vn.y, vn.z);
}

/**
 * Fix an value range
 * @param circular if true, when the values overflow the min value, it's reset to the max value and vice-versa
 */
float fixRange(float value, float min, float max, bool circular) {
    if (value > max)
        return circular ? min : max;
    else if (value < min)
        return circular ? max : min;
    return value;
}

float rad(float angle) {
    return angle * M_PI / 180;
}

float calcDistance(float aX, float aY, float bX, float bY) {
    return sqrt(pow(aX - bX, 2) + pow(aY - bY, 2));
}

void drawBoard() {
    glPushMatrix();


    GLfloat objeto_especular[] = { 0.626, 0.626, 0.626, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0.6, 0.6, 0.0, 0.1 };


    GLfloat objeto_difusa[]    = { 0.6, 0.6, 0.0, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);

/*    glMaterialfv(GL_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_BACK, GL_SHININESS, objeto_brilho);*/



    // base

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, 0, 1);
    glVertex3f(-BHF, -BHF, 0);

    glNormal3f(0, 0, 1);
    glVertex3f(BHF, -BHF, 0);

    glNormal3f(0, 0, 1);
    glVertex3f(BHF, BHF, 0);

    glNormal3f(0, 0, 1);
    glVertex3f(-BHF, BHF, 0);
    glEnd();

    // bottom

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, 1, 0);
    glVertex3f(-BHF, -BHF, 0.5);

    glNormal3f(0, 1, 0);
    glVertex3f(BHF, -BHF, 0.5);

    glNormal3f(0, 1, 0);
    glVertex3f(BHF, -BHF, 0);

    glNormal3f(0, 1, 0);
    glVertex3f(-BHF, -BHF, 0);
    glEnd();

    // right

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(-1, 0, 0);
    glVertex3f(BHF, -BHF, 0.5);

    glNormal3f(-1, 0, 0);
    glVertex3f(BHF, BHF, 0.5);

    glNormal3f(-1, 0, 0);
    glVertex3f(BHF, BHF, 0);

    glNormal3f(-1, 0, 0);
    glVertex3f(BHF, -BHF, 0);
    glEnd();

    // top

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, -1, 0);
    glVertex3f(BHF, BHF, 0.5);

    glNormal3f(0, -1, 0);
    glVertex3f(-BHF, BHF, 0.5);

    glNormal3f(0, -1, 0);
    glVertex3f(-BHF, BHF, 0);

    glNormal3f(0, -1, 0);
    glVertex3f(BHF, BHF, 0);
    glEnd();

    // left

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(1, 0, 0);
    glVertex3f(-BHF, BHF, 0.5);

    glNormal3f(1, 0, 0);
    glVertex3f(-BHF, -BHF, 0.5);

    glNormal3f(1, 0, 0);
    glVertex3f(-BHF, -BHF, 0);

    glNormal3f(1, 0, 0);
    glVertex3f(-BHF, BHF, 0);

    glEnd();
    glPopMatrix();
}

void drawSphere() {

    glPushMatrix();
    glNormal3f(0, 0, 1);

    GLfloat objeto_especular[] = { 0.626, 0.626, 0.626, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0.1, 0.1, 0.1, 1.0 };

    GLfloat objeto_difusa[]    = { 0, 1, 0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);

    glTranslatef(position[0], position[1], BALL_RADIUS);
    glutSolidSphere(BALL_RADIUS, 100, 100);
    glPopMatrix();

}


void drawArrow() {
    const float height = velocity + 0.01;
    glPushMatrix();

    glPushMatrix();

    GLfloat objeto_especular[] = { 0.626, 0.626, 0.626, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0.1, 0.1, 0.1, 1.0 };

    GLfloat objeto_difusa[]    = { 0, 0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);

    glTranslatef(position[0], position[1], BALL_RADIUS);
    glRotatef(initialDirection, 0, 0, 1);
    glTranslatef(0, BALL_RADIUS * 1.2, 0);
    glRotatef(90, -1, 0, 0);
    glutSolidCone(BALL_RADIUS / 2, height, 100, 100);
    glPopMatrix();
    glPopMatrix();

}

void reiniciaJogo()
{
    for(int i = 0; i < NUMRETAN;++i)
    {
        retangulos[i].colisao = false;
    }

    pause = false;
    venceu = false;
    animate = false;

    rebatedor.atualizaPosicao();
    initialDirection = 0;
    initialDirection = fixRange(initialDirection, -180, 180, true);

    position[0] = 0;
    position[1] = -1.01;

    direction[0] = cos((initialDirection + 90) * M_PI / 180);
    direction[1] = sin((initialDirection + 90) * M_PI / 180);

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

triangle makeTriangle(float x = 0, float y = 0, float rotation = 0) {
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

void desenhaRetangulo(retangulo &r)
{
    int i;

    glPushMatrix();



    //GLfloat objeto_especular[] = { 0.626, 0.626, 0.626, 1.0 };
    GLfloat objeto_especular[] = { 1.0, 0.0, 0.0, 0.2 };
    GLfloat objeto_brilho[]    = { 20.0f };
    GLfloat objeto_ambient[]   = { 0, 0.0, 0.0, 0.001 };


    GLfloat objeto_difusa[]    = { 1, 0, 0, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);

    //glNormal3f(0, 0, 1);
    glBegin(GL_QUADS);
    for (i = 0; i < 4; ++i)
    {
        glNormal3f(0, 0, 1);
        glVertex3f(r.vetorPontos[i].x, r.vetorPontos[i].y, r.altura); //Face superior

    }

    glEnd();

    //glNormal3f(0,0,1);

    //Face que aponta para (0, -1, 0)
    glBegin(GL_QUADS);

    glNormal3f(0,-1,0);
    glVertex3f(r.vetorPontos[0].x, r.vetorPontos[0].y, 0);

    glNormal3f(0,-1,0);
    glVertex3f(r.vetorPontos[1].x, r.vetorPontos[1].y, 0);

    glNormal3f(0,-1,0);
    glVertex3f(r.vetorPontos[1].x, r.vetorPontos[1].y, r.altura);

    glNormal3f(0,-1,0);
    glVertex3f(r.vetorPontos[0].x, r.vetorPontos[0].y, r.altura);


    glEnd();

    //glNormal3f(0,-1,0);

    //Face que aponta para (1, 0, 0)
    glBegin(GL_QUADS);

    glNormal3f(1, 0,0);
    glVertex3f(r.vetorPontos[1].x, r.vetorPontos[1].y, 0);

    glNormal3f(1, 0,0);
    glVertex3f(r.vetorPontos[1].x, r.vetorPontos[1].y + r.altura, 0);

    glNormal3f(1, 0,0);
    glVertex3f(r.vetorPontos[1].x , r.vetorPontos[1].y + r.altura, r.altura);

    glNormal3f(1, 0,0);
    glVertex3f(r.vetorPontos[1].x, r.vetorPontos[1].y, r.altura);


    glEnd();

    //glNormal3f(-1, 0, 0);

    glBegin(GL_QUADS);

    glNormal3f(-1, 0, 0);
    glVertex3f(r.vetorPontos[0].x, r.vetorPontos[0].y, 0);

    glNormal3f(-1, 0, 0);
    glVertex3f(r.vetorPontos[0].x, r.vetorPontos[0].y, r.altura);

    glNormal3f(-1, 0, 0);
    glVertex3f(r.vetorPontos[3].x, r.vetorPontos[3].y, r.altura);

    glNormal3f(-1, 0, 0);
    glVertex3f(r.vetorPontos[3].x, r.vetorPontos[3].y, 0);


    glEnd();
    glPopMatrix();
}


void drawPrism(triangle t) {
    int i;

    glPushMatrix();

    //setColor(1, 0, 0);
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    for (i = 0; i < 3; ++i)
        glVertex3f(t.v[i].x, t.v[i].y, 0.5);
    glEnd();

    for (i = 0; i < 3; ++i) {

        int next = (i + 1) % 3;
        setCalcNormal({{
                               {t.v[i].x, t.v[i].y, 0.5},
                               {t.v[i].x, t.v[i].y, 0},
                               {t.v[next].x, t.v[next].y, 0},
                       }
                      });
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(t.v[i].x, t.v[i].y, 0.5);
        glVertex3f(t.v[i].x, t.v[i].y, 0);
        glVertex3f(t.v[next].x, t.v[next].y, 0);
        glVertex3f(t.v[next].x, t.v[next].y, 0.5);
        glEnd();
    }

    glPopMatrix();

}


void drawBorderss1(){


 v1.z = v2.z = v3.z = v4.z = v5.z =v6.z = v0.z= 0.5;
  v0.x = 2;
  v0.y = 0;

  v1.x  = 2;
  v1.y  = 1;

  v2.x  =  1.8;
  v2.y  =  0.9;

  v3.x  = 1.7;
  v3.y  = 0.8;

  v4.x = 1.6;
  v4.y = 0.6;

  v5.x=1.55;
  v5.y = 0.4;

  v6.x = 1.555;
  v6.y = 0;

   glPushMatrix();
    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v1.x,v1.y,v1.z);
    glVertex3f(v2.x,v2.y,v2.z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v2.x,v2.y,v2.z);
    glVertex3f(v3.x,v3.y,v3.z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v3.x,v3.y,v3.z);
    glVertex3f(v4.x,v4.y,v4.z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v4.x,v4.y,v4.z);
    glVertex3f(v5.x,v5.y,v5.z);
    glEnd();


    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v5.x,v5.y,v5.z);
    glVertex3f(v6.x,v6.y,v6.z);
    glEnd();

    // desenho a outra metade  direita da parede

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v2.x,-v2.y,v2.z);
    glVertex3f(v1.x,-v1.y,v1.z);

    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v3.x,-v3.y,v3.z);
    glVertex3f(v2.x,-v2.y,v2.z);

    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
     glVertex3f(v4.x,-v4.y,v4.z);
     glVertex3f(v3.x,-v3.y,v3.z);

    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v5.x,-v5.y,v5.z);
    glVertex3f(v4.x,-v4.y,v4.z);

    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v6.x,-v6.y,v6.z);
    glVertex3f(v5.x,-v5.y,v5.z);

    glEnd();
   glPopMatrix();

       glPushMatrix();
    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v2.x,v2.y,v2.z);
    glVertex3f(-v1.x,v1.y,v1.z);

    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v3.x,v3.y,v3.z);
    glVertex3f(-v2.x,v2.y,v2.z);

    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v4.x,v4.y,v4.z);
    glVertex3f(-v3.x,v3.y,v3.z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v5.x,v5.y,v5.z);
    glVertex3f(-v4.x,v4.y,v4.z);

    glEnd();


    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v6.x,v6.y,v6.z);
    glVertex3f(-v5.x,v5.y,v5.z);

    glEnd();

    // desenho a outra metade da parede

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,-v0.y,v0.z);
    glVertex3f(-v1.x,-v1.y,v1.z);
    glVertex3f(-v2.x,-v2.y,v2.z);

    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,-v0.y,v0.z);
    glVertex3f(-v2.x,-v2.y,v2.z);
    glVertex3f(-v3.x,-v3.y,v3.z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,-v0.y,v0.z);
     glVertex3f(-v3.x,-v3.y,v3.z);
     glVertex3f(-v4.x,-v4.y,v4.z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,-v0.y,v0.z);
    glVertex3f(-v4.x,-v4.y,v4.z);
    glVertex3f(-v5.x,-v5.y,v5.z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v5.x,-v5.y,v5.z);
    glVertex3f(-v6.x,-v6.y,v6.z);
    glEnd();

   glPopMatrix();

}


void drawBorderss2(){


}


void drawHitter(vertex center,float sizeHitter){
  float deltaX = 0.2;
  float deltaY = 0.13;

  vertex v1,v2,v3,v4;

  v1.z = v2.z =v3.z =v4.z =  0.2;
  center.z= 0.2;

  v1.x = center.x+sizeHitter;
  v1.y =0;

  v2.x = v1.x -deltaX;
  v2.y = v1.y +deltaY;


   int cont =0;
   //desenho a primeira metade do rebatedor
   glPushMatrix();
   while(cont < 3){

    glBegin(GL_QUADS);
    glVertex3f(v2.x,v2.y,v2.z);
    glVertex3f(v1.x,v1.y,v1.z);
    glVertex3f(v1.x,v1.y,0.0);
    glVertex3f(v2.x,v2.y,0.0);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(v1.x-2*(v1.x-center.x),v1.y,v1.z);
    glVertex3f(v2.x-2*(v2.x-center.x),v2.y,v2.z);
    glVertex3f(v2.x-2*(v2.x-center.x),v2.y,0.0);
    glVertex3f(v1.x-2*(v1.x-center.x),v1.y,0.0);
    glEnd();

    //parte de baixo do rebatedor
    glBegin(GL_QUADS);
    glVertex3f(center.x+sizeHitter,center.y,center.z);
    glVertex3f(center.x-sizeHitter,center.y,center.z);
    glVertex3f(center.x-sizeHitter,center.y,0.0);
    glVertex3f(center.x+sizeHitter,center.y,0.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(center.x,center.y,center.z);
    glVertex3f(v1.x,v1.y,v1.z);
    glVertex3f(v2.x,v2.y,v2.z);
    glEnd();

    // desenho a outra metade do rebatedor
    glBegin(GL_TRIANGLES);
    glVertex3f(center.x,center.y,center.z);
    glVertex3f(v2.x-2*(v2.x-center.x),v2.y,v2.z);
    glVertex3f(v1.x-2*(v1.x-center.x),v1.y,v1.z);
    glEnd();

     v1.x = v2.x;
     v1.y = v2.y;
     v2.x = v2.x - deltaX;
     v2.y = v2.y + deltaY/3;
     cont++;
   }
   glPopMatrix();
}






void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    int ortho = 2;


    if(perspective)
    {

        //gluPerspective(0, (GLfloat) width / (GLfloat) height, 0.01, 200.0);
        gluPerspective(10.0, (GLfloat) 1000.0 / (GLfloat) 600.0, 4, -1);



    }
    else
    {


            glOrtho (-ortho, ortho, -ortho*1000.0/600.0, ortho*1000.0/600.0, -1, 1);




        //glOrtho(-2, 2, -2, 2, -1.0f, 1.0f);
        //glOrtho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);




    }


    glMatrixMode(GL_MODELVIEW);




    gluLookAt(0.0, -1.0, 1, 0.0, 0.0, 0.0, 0.0, 0.2, 0);
    glPushMatrix();

    glLoadIdentity();





    glRotatef(rotationY, 0, 1, 1);
    glRotatef(rotationX, 1, 0, 1);
    glRotatef(angulo, 1, 0, 0);
    glPushMatrix();

    if (!animate)
        drawArrow();
    drawSphere();

    for (int i = 0; i < NUMRETAN; ++i)
    {
        if(!retangulos[i].colisao)
            desenhaRetangulo(retangulos[i]);
    }

    desenhaRetangulo(rebatedor);


    drawBoard();

    glPopMatrix();


    //**************************************************************

    //  glDisable(GL_LIGHTING);


    if(pause)
    {

        glDisable(GL_LIGHTING);

        glRasterPos2f(0, 0);

        glColor3f(0, 0, 1);




        if(pause)
        {
            strcpy(texto, "Jogo Pausado");
            glTranslatef(-0.7, -0.01, rebatedor.centro.z + 0.5);

        }

        if(venceu)
        {
            strcpy(texto, "Voce Venceu");
            glTranslatef(0.05, -0.01, rebatedor.centro.z + 0.5);
        }

        glScalef(0.001, 0.001, 0.001);


        for (int i = 0; texto[i] != '\0'; i++)
        {

            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, texto[i]);
        }


        glEnable(GL_LIGHTING);

    }

    glPopMatrix();
    glutSwapBuffers();
    glutPostRedisplay();

}

float vectorAngle(float ax, float ay) {
    return acos(ax * 1 + ay * 0) * (180.0 / M_PI);

}

// update all states
void updateState() {
    if (!animate) return;

    // maximum board position
    const float maxRange[] = {-BHF + BALL_RADIUS, BHF - BALL_RADIUS};
    float movement = velocity * 0.1;


    // Move the ball and handle overflow
    position[0] = fixRange(position[0] + movement * direction[0], maxRange[0], maxRange[1]);
    position[1] = fixRange(position[1] + movement * direction[1], maxRange[0], maxRange[1]);

    if((position[1] - BALL_RADIUS) <= - BHF )
    {
        animate = false;
        position[0] = rebatedor.centro.x;
        position[1] = -1.01;

        return;
    }

    if(true)
    {

        // TODO: handle prisma colision
        for (int j = 0; j < NUMRETAN; ++j)
        {
            if(retangulos[j].colisao)
                continue;

            int i;
            int pontoDistante = 0;
            // se a distancia e maior do que o raio da bola + raio do retangulo
            float distance = calcDistance(retangulos[j].centro.x, retangulos[j].centro.y, position[0], position[1]);//
            if (distance > (BALL_RADIUS + RAIORETANGULO)) continue;

            //v1 = inferior esquerdo, altura = altura;
            //v2 = superior direito, altura = 0

            printf("colision happened?\n");


            if(position[0] > 0 )
            {
                if(abs(position[0] + BALL_RADIUS) < abs(retangulos[j].pontosExtremos[0].x))
                {
                    cout<<"+0.0\n";
                    //pause = !pause;

                    continue;
                }

                if(abs(position[0] - BALL_RADIUS) > abs(retangulos[j].pontosExtremos[2].x))
                {
                    cout<<"+0.1\n";
                   // pause = !pause;
                    continue;

                }
            }
            else
            {
                if((position[0] - BALL_RADIUS) < (retangulos[j].pontosExtremos[0].x))
                {
                    cout<<"-0.0\n";
                    cout<<(retangulos[j].pontosExtremos[0].x)<<endl;
                    cout<<(position[0] - BALL_RADIUS)<<endl<<endl;
                    //pause = !pause;
                    continue;
                }

                if((position[0] - BALL_RADIUS) > (retangulos[j].pontosExtremos[2].x))
                {
                    cout<<"-0.1\n";
                    cout<<"1° : "<<position[0] - BALL_RADIUS<<"\n 2°: "<<(retangulos[j].pontosExtremos[2].x)<<endl;

                    //pause = !pause;
                    continue;

                }
            }

            if(position[1] > 0)
            {


                if (abs(position[1] + BALL_RADIUS) < abs(retangulos[j].pontosExtremos[0].y))
                {
                    cout<<"+1.0\n";
                    //pause = !pause;
                    continue;
                }
                if (abs(position[1] - BALL_RADIUS) > abs(retangulos[j].pontosExtremos[2].y))
                {

                    cout<<"+1.1\n";
                    //pause = !pause;
                    continue;

                }

            }
            else
            {
                if ((position[1] - BALL_RADIUS) < (retangulos[j].pontosExtremos[0].y))
                {
                    cout<<"-1.0\n";
                    //pause = !pause;
                    continue;
                }

                if (abs(position[1] + BALL_RADIUS) > abs(retangulos[j].pontosExtremos[2].y))
                {
                    cout<<"-1.1\n";
                    //pause = !pause;
                    continue;

                }
            }



            printf("!!!!!Colision happened!!!!!!\n");

            //pause = !pause;

            int side;

            //Verifica colisão com as quinas

            bool colisaoQuina = 0;

            for(int i = 0; i < 4; ++i)
            {
                float dist = calcDistance(position[0], position[1], retangulos[j].pontosExtremos[i].x, retangulos[j].pontosExtremos[i].y );
                if(fabs(dist - BALL_RADIUS)  <= 0.001)
                {
                    colisaoQuina = 1;
                    break;
                }

            }

            if(!colisaoQuina)
            {

                ///direita
                if (((position[0]) - BALL_RADIUS <= (retangulos[j].pontosExtremos[2].x)) &&
                    (position[1] < retangulos[j].pontosExtremos[2].y) &&
                    (position[1] > retangulos[j].pontosExtremos[0].y))
                {
                    printf("Lado direito\n");


                    side = 3;
                }
                    ///esquerda
                else if (((position[0]) + BALL_RADIUS >= (retangulos[j].pontosExtremos[0].x)) &&
                         (position[1] < retangulos[j].pontosExtremos[2].y) &&
                         (position[1] > retangulos[j].pontosExtremos[0].y))
                {
                    printf("Lado esquerdo\n");
                    side = 4;


                } else if ((position[1]) - BALL_RADIUS <= (retangulos[j].pontosExtremos[2].y))
                {
                    side = 1;


                    printf("Lado cima \n");
                }
///baixo
                else if ((position[1]) + BALL_RADIUS >= (retangulos[j].pontosExtremos[0].y))
                {
                    printf("Lado baixo\n");


                    side = 2;
                }


                cout << "Posicao[o] + raio = " << position[0] + BALL_RADIUS << "\npontoExtrmo[2].x = "
                     << retangulos[j].pontosExtremos[2].x << endl << endl;
                position[0] = fixRange(position[0] + movement * direction[0], maxRange[0], maxRange[1]);
                position[1] = fixRange(position[1] + movement * direction[1], maxRange[0], maxRange[1]);

                if (side == 1)
                {
                    direction[1] = -direction[1];
                } else if (side == 2)
                {
                    direction[1] = -direction[1];
                } else if (side == 3)
                {
                    direction[0] = -direction[0];
                } else if (side == 4)
                {
                    direction[0] = -direction[0];
                }
            }
            else
            {

                position[0] = fixRange(position[0] + movement * direction[0], maxRange[0], maxRange[1]);
                position[1] = fixRange(position[1] + movement * direction[1], maxRange[0], maxRange[1]);

                direction[1] = -direction[1];
                direction[0] = -direction[0];

                cout<<"Quina.\n";
            }

            retangulos[j].colisao = true;
            venceu = true;

            for(int k = 0; k < NUMRETAN; ++k)
            {
                if(!retangulos[k].colisao)
                {
                    venceu = false;
                    break;
                }
            }

            if(venceu)
                pause = true;

            break;

        }

    }

    // borders colision
    for (
            int i = 0;
            i < 2; ++i) {
        if (position[i] == maxRange[0] || position[i] == maxRange[1])
            direction[i] *= -1;
    }
}

void idle()
{


    if(!pause)
    {

        float t, frameTime;
        static float tLast = 0.0;
        // Get elapsed time
        t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
        // Calculate frame time
        frameTime = t - tLast;

        // Check if the desired frame time was achieved. If not, skip animation.
        if (frameTime <= 1.0 / FPS)
            return;


        updateState();



        glutPostRedisplay();
    }

}

void reshape(int w, int h) {
    width = w;
    height = h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 0.01, 200.0);
}

void Spekeyboard(int key, int x, int y)
{
    switch (key)
    {

        case GLUT_KEY_F12:
            fullScreen = !fullScreen;
            if (fullScreen)
            {
                glutFullScreen();
            }
            else
            {
                glutReshapeWindow(1000, 600);
            }

            break;
    }


}

void keyboard(unsigned char key, int x, int y) {


    if (!animate && !pause) {
        switch (tolower(key)) {

            case 'p':

                perspective = !perspective;

/*                if(angulo == -30)
                    angulo = 0;
                else
                    angulo = -30;*/


                break;

            case 'r':
                reiniciaJogo();
                break;

            case 27:
                exit(0);
                break;



        }
        velocity = fixRange(velocity, -1, 1);
        initialDirection = fixRange(initialDirection, -180, 180, true);
        direction[0] = cos((initialDirection + 90) * M_PI / 180);
        direction[1] = sin((initialDirection + 90) * M_PI / 180);
    } else
    {

        switch(tolower(key))
        {

            case 'p':

                perspective = !perspective;

                break;

            case ' ':

                if(pause)
                    pause=0;
                else
                    pause = 1;

                break;

            case 'r':
                reiniciaJogo();
                break;

            case 27:
                exit(0);
                break;

        }
    }


}

// Motion callback
void motion(int x, int y) {

}

// Mouse callback
void mouse(int button, int state, int x, int y)
{

    if(!animate && !pause)
    {
        if ((button == GLUT_LEFT_BUTTON ) && (state == GLUT_DOWN))
        {
            animate = !animate;
        }

        else if(button == 3)
        {
            initialDirection += 5;

        }
        else
        {
            initialDirection -= 5;
        }

        initialDirection = fixRange(initialDirection, -180, 180, true);
        direction[0] = cos((initialDirection + 90) * M_PI / 180);
        direction[1] = sin((initialDirection + 90) * M_PI / 180);
    }

}

void generatePrisms()
{

    float y = 0.5;
    float x = -2 + 0.35/2.0;
    //float x = 0.5;

    //Desenha linha de retangulos
    for(int j = 0; j < MaxRetanVertical;++j)
    {


        for (int i = 0; i < MaxRetanHorizontal; ++i)
        {  //4

            makeRetangulo(x, y, retangulos[(j*MaxRetanHorizontal)+i]);
            x += 0.39;

            //cout<<"x = "<<x<<" ";



        }

        //  cout<<endl;

        x = -2 + 0.35/2.0;
        y += 0.4;
    }

    for(int i = 0; i < NUMRETAN; ++i)
    {
        //cout<<"Centro: "<<retangulos[i].centro.x<<" "<<retangulos[i].centro.y<<endl;
    }
}

bool colisaoRebatedor(float movement, const float maxRange[])
{

/*    float raio = calcDistance(rebatedor.pontosExtremos[0].x, rebatedor.pontosExtremos[0].y, rebatedor.pontosExtremos[0].x, rebatedor.pontosExtremos[0].y-0.06);
    cout<<"Distancia: "<<raio<<endl;
    exit(-1);*/

    int i;
    int pontoDistante = 0;
    // se a distancia e maior do que o raio da bola + raio do retangulo
    float distance = calcDistance(rebatedor.centro.x, rebatedor.centro.y, position[0], position[1]);//
    if ( (distance - (BALL_RADIUS + RAIO_REBATEDOR) > 0.001)) return false;

    for (i = 0; i < 4; ++i)
    {
        // verifica ponto mais perto
        if (calcDistance(position[0], position[1], rebatedor.pontosExtremos[i].x, rebatedor.pontosExtremos[i].y) <
            calcDistance(position[0], position[1], rebatedor.pontosExtremos[pontoDistante].x, rebatedor.pontosExtremos[pontoDistante].y))
        {
            pontoDistante = i;
            //cout<<"Dist: "<<calcDistance(position[0], position[1], rebatedor.pontosExtremos[pontoDistante].x, rebatedor.pontosExtremos[pontoDistante].y)<<endl<<endl;
        }
    }


    // Obtem equação ax + by + c = 0
    // coeficiente angular.
    float a;
    float b;
    float c;

    if((pontoDistante == 0) || (pontoDistante == 2))// Eq y = constante
    {

        a = 0;
        b = 1;
        c = -rebatedor.pontosExtremos[(pontoDistante)].y;
    }
    else if((pontoDistante == 1)|| (pontoDistante == 3))//Eq x = constante
    {


        a = 1;
        b = 0;
        c = -rebatedor.pontosExtremos[(pontoDistante)].x;
    }

    // distancia entre centro da esfera e reta
    float d = fabs(a * position[0] + b*position[1] + c) /
              sqrt(pow(a, 2) + pow(b,2));

    //cout<<"d = "<<fabs(d - BALL_RADIUS)<<", lado da colisao: "<< ((pontoDistante))<<endl;

    if (fabs(d - BALL_RADIUS) <= 0.05   )
    {
        //cout<<"colision happened\n";

        cout<<"Lado "<<pontoDistante<<endl;

        // rollback animation (or else, it will get stuck)
        position[0] = fixRange(position[0] - movement * direction[0], maxRange[0], maxRange[1]);
        position[1] = fixRange(position[1] - movement * direction[1], maxRange[0], maxRange[1]);

        float directionAngle = ((position[1] + direction[1]) - position[1]) /
                               ((position[0] + direction[0]) - position[0]);

        //cout<<"directionAngle "<<directionAngle<<endl;

        if (fabs(directionAngle) == INFINITY)
        {
            cout<<"************INF***********\n";
            directionAngle = directionAngle >= 0 ? FLT_MIN : FLT_MAX;
        }

        // get the colision angle
        float angle = atan((
                                   (a - directionAngle) /
                                   (1 + a * directionAngle)
                           )) * (180.0 / M_PI);



        // rotate the direction
        float r = fabs(angle * 2) * (angle / fabs(angle));
        r = r * M_PI / 180;

        // rotação
        float x = direction[0] * cos(r) - direction[1] * sin(r);
        direction[1] = direction[0] * sin(r) + direction[1] * cos(r);
        direction[0] = x;

        // Re-run the movement of the ball
        position[0] = fixRange(position[0] + movement * direction[0], maxRange[0], maxRange[1]);
        position[1] = fixRange(position[1] + movement * direction[1], maxRange[0], maxRange[1]);

        return true;

    }

    return false;
}

void passiveMotion(int x, int y)
{
    if(pause)
    {
        //x = glutGet(GLUT_WINDOW_WIDTH) / 2;
        y = glutGet(GLUT_WINDOW_HEIGHT)/2;
        glutWarpPointer(xAntigo, y);

        return;
    }

    bool mudanca = 0;

    //cout<<"PassiveMotion\n";


    if ((x - xAntigo) > 0) //movimento para direita
    {
        if ((rebatedor.centro.x + rebatedor.largura / 2 + 0.1) < 2)
        {

            rebatedor.atualizaPosicao(rebatedor.centro.x + 0.1);
        }

    } else if ((x - xAntigo) < 0)
    {
        if (fabs(rebatedor.centro.x - rebatedor.largura / 2 - 0.1) < 2)
        {

            rebatedor.atualizaPosicao(rebatedor.centro.x - 0.1);
        }
    }

    if(!fullScreen)
    {

        //cout << "X " << x << endl;
        if (x >= 700 || x <= 200)
        {


            x = glutGet(GLUT_WINDOW_WIDTH) / 2;
            y = glutGet(GLUT_WINDOW_HEIGHT) / 2;
            glutWarpPointer(x, y);

        }
        //cout << "Y " << y;
        if (y >= 400 || y <= 200 || y <= 20)
        {


            x = glutGet(GLUT_WINDOW_WIDTH) / 2;
            y = glutGet(GLUT_WINDOW_HEIGHT) / 2;
            glutWarpPointer(x, y);

        }
    }
    else
    {
        //cout << "X " << x << endl;
        if (x < 300 || x > 1000)
        {
            x = glutGet(GLUT_WINDOW_WIDTH) / 2;
            y = glutGet(GLUT_WINDOW_HEIGHT) / 2;
            glutWarpPointer(x, y);

        }
    }



    if(!animate && !pause)
    {
        position[0] = rebatedor.centro.x;
    }
    xAntigo = x;


    return;



}

/// Main
int main(int argc, char **argv) {

    velocity = fixRange(velocity, -1, 1);
    initialDirection = fixRange(initialDirection, -180, 180, true);
    direction[0] = cos((initialDirection + 90) * M_PI / 180);
    direction[1] = sin((initialDirection + 90) * M_PI / 180);

    velocity = fixRange(velocity, -1, 1);
    generatePrisms();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(Spekeyboard);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glutIdleFunc(idle);


    glutMainLoop();

    return 0;
}
