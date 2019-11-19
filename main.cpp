#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include "glcTexture.h"
#include "etc.h" //TODO: reorganize modules
#include <iostream>
#include <cstring>
#include <vector>
#include "Retangulo.h"
#include "cor.h"
#include "Normal.h"
#include "Posicao.h"
#include "OpenGL_CallBack.h"
#include "Colisao.h"

using namespace std;


/// Globals

vertex normalFaces;
vertex centroRebatedor;
int angulo = 0;
vertex normalBorder;
const float FPS = 60;
const float BHF = 2; // Board Half Width
const int MaxRetanHorizontal = 10;//10
int timerInicialColision = 30;
const int MaxRetanVertical = 1;//4
vertex v1,v2,v3,v4,v5,v6,v0;
vector<vertex> normais;
int timerColision = 100;
///triangulo utilizado como modelo para organizar vertices para calcular normais das faces da parte oval
vector<triangle> vertices;
///mesmo caso, utilizados para calculo de normais
triangle tn1,tn2,tn3,tn4,tn5,tn6,tn7,tn8,tn9,tn10;
triangle tn11,tn12,tn13,tn14,tn15,tn16,tn17,tn18,tn19,tn20;
///flag paar controlar a inserçaõ de vertices no vetor de vertices
int verticeAdded =0;






const float RAIORETANGULO = 0.215058132;

// prismas iniciais
char texto[100];
float velocity = 1.5 * 0.5;
triangle prismas[4];
vertex auxCalcNormal;
int xAntigo;

vertex trianglesNormals[60];

int vidas = 5;

glcTexture *textureManager;

/// Functions
void init(void) {


    textureManager = new glcTexture;
    textureManager->SetNumberOfTextures(4);       // Estabelece o número de texturas que será utilizado
    textureManager->CreateTexture("/home/igor/Documentos/TrabalhoCG1/cmake-build-debug/texturas/texturaPlano.png", 0);

    // LOAD OBJECTS



    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_LIGHT0);                   // habilita luz 0
    glEnable(GL_DEPTH_TEST);


    GLfloat cor_luz[]  = { 1.0, 1.0, 1.0, 1.0};
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

/**
 * Fix an value range
 * @param circular if true, when the values overflow the min value, it's reset to the max value and vice-versa
 */


float calcDistance(float aX, float aY, float bX, float bY) {
    return sqrt(pow(aX - bX, 2) + pow(aY - bY, 2));
}

void drawBoard() {



    glPushMatrix();


/*    GLfloat objeto_especular[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat objeto_brilho[]    = { 40.0f };
    GLfloat objeto_ambient[]   = { 0.6, 0.6, 0.0, 0.1 };


    GLfloat objeto_difusa[]    = { 0.6, 0.6, 0.0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, corParede[fase].objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corParede[fase].objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);*/

    GLfloat objeto_ambient[]   = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat objeto_difusa[]    = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat objeto_especular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat objeto_brilho[]    = { 70.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);

    textureManager->Bind(0);
    float aspectRatio = textureManager->GetAspectRatio(0);

    // Calculo abaixo funciona apenas se textura estiver centralizada na origem
    float h = 1.0f;
    float w = 1.0f;

    // base

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-BHF, -BHF, 0);

    glNormal3f(0, 0, 1);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(BHF, -BHF, 0);


    glNormal3f(0, 0, 1);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(BHF, BHF, 0);

    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-BHF, BHF, 0);
    glEnd();

    // bottom

    float largura = 2.0*BHF;


    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 0.0); //ok
    glVertex3f(-BHF, -BHF, 0.5);

    glNormal3f(0, 1, 0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(BHF, -BHF, 0.5);

    glNormal3f(0.0,1.0,0.0 );
    glTexCoord2f(1.0, 0.005);
    glVertex3f(BHF, -BHF, 0);

    glNormal3f(0, 1, 0);
    glTexCoord2f(0.005, 1.0);
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

    textureManager->Disable();

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

    for(int i = 0; i < vidas; ++i)
    {

        glPushMatrix();
        glTranslatef(-5 + i*0.5, 2.5, BALL_RADIUS);
        glutSolidSphere(BALL_RADIUS, 20, 20);

        glPopMatrix();


    }

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


void drawBorderss1()
{
    GLfloat objeto_especular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0, 0.0, 0.0, 0.001 };


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corRebatedorLaterais[fase].objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);

    v1.z = v2.z = v3.z = v4.z = v5.z =v6.z = v0.z= 0.5;
    v0.x = 2;
    v0.y = 0;

    v1.x  = 2;
    v1.y  = 1;

    v2.x  =  1.98;
    v2.y  =  0.9;

    v3.x  = 1.89;
    v3.y  = 0.8;

    v4.x = 1.79;
    v4.y = 0.6;

    v5.x=1.75;
    v5.y = 0.4;

    v6.x = 1.71;
    v6.y = 0;

    trianglesNormals[0].x = v0.x;
    trianglesNormals[0].y = v0.y;
    trianglesNormals[0].z = 0;
    CalculaNormal2(v0,v1,v2,&normalBorder);
    glPushMatrix();
    //glNormal3f(0,0,1);
    glBegin(GL_TRIANGLES);
    glNormal3f(normalBorder.x,normalBorder.y,normalBorder.z);
    glVertex3f(v0.x,v0.y,v0.z);
    glNormal3f(normalBorder.x,normalBorder.y,normalBorder.z);
    glVertex3f(v1.x,v1.y,v1.z);
    glNormal3f(normalBorder.x,normalBorder.y,normalBorder.z);
    glVertex3f(v2.x,v2.y,v2.z);
    glEnd();

    // printf("NORMAL %f: %f: %f \n",n1.x,v2.y,v2.z);
    if(verticeAdded ==0);
    {

        tn1.v[0]=trianglesNormals[0];
        tn1.v[1]=v1;
        tn1.v[2]=v2;
        vertices.push_back(tn1);
    }

    glBegin(GL_TRIANGLES);
    glNormal3f(normalBorder.x,normalBorder.y,normalBorder.z);
    glVertex3f(v0.x,v0.y,v0.z);
    glNormal3f(normalBorder.x,normalBorder.y,normalBorder.z);
    glVertex3f(v2.x,v2.y,v2.z);
    glNormal3f(normalBorder.x,normalBorder.y,normalBorder.z);
    glVertex3f(v3.x,v3.y,v3.z);
    glEnd();

    if(verticeAdded ==0)
    {   trianglesNormals[1].x = v0.x;
        trianglesNormals[1].y = v0.y;
        trianglesNormals[1].z = 0;
        tn2.v[0]=trianglesNormals[1];
        tn2.v[1]=v2;
        tn2.v[2]=v3;
        vertices.push_back(tn2);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v3.x,v3.y,v3.z);
    glVertex3f(v4.x,v4.y,v4.z);
    glEnd();
    if(verticeAdded ==0)
    {   trianglesNormals[2].x = v0.x;
        trianglesNormals[2].x = v0.y;
        trianglesNormals[2].x = 0;

        tn3.v[0]=trianglesNormals[2];
        tn3.v[1]=v3;
        tn3.v[2]=v4;
        vertices.push_back(tn3);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v4.x,v4.y,v4.z);
    glVertex3f(v5.x,v5.y,v5.z);
    glEnd();
    if(verticeAdded ==0)
    {   trianglesNormals[3].x = v0.x;
        trianglesNormals[3].y= v0.y;
        trianglesNormals[3].z= 0;

        tn4.v[0] = trianglesNormals[3];
        tn4.v[1]=v4;
        tn4.v[2]=v5;
        vertices.push_back(tn4);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v5.x,v5.y,v5.z);
    glVertex3f(v6.x,v6.y,v6.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[4].x = v0.x;
        trianglesNormals[4].y = v0.y;
        trianglesNormals[4].z = 0;

        tn5.v[0]=trianglesNormals[4];
        tn5.v[1]=v5;
        tn5.v[2]=v6;
        vertices.push_back(tn5);
    }
    // desenho a outra metade  direita da parede

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v2.x,-v2.y,v2.z);
    glVertex3f(v1.x,-v1.y,v1.z);
    glEnd();
    if(verticeAdded == 0)
    {
        trianglesNormals[5].x = v0.x;
        trianglesNormals[5].x =v0.y;
        trianglesNormals[5].z =0;
        trianglesNormals[6].x= v2.x;
        trianglesNormals[6].y= -v2.y;
        trianglesNormals[6].z= v2.z;
        trianglesNormals[7].x= v1.x;
        trianglesNormals[7].y = -v1.y;
        trianglesNormals[7].z= v1.z;
        tn6.v[0]=trianglesNormals[5];
        tn6.v[1]=trianglesNormals[6];
        tn6.v[2]=trianglesNormals[7];
        vertices.push_back(tn6);
    }


    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v3.x,-v3.y,v3.z);
    glVertex3f(v2.x,-v2.y,v2.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[8].x = v0.x;
        trianglesNormals[8].x =v0.y;
        trianglesNormals[8].z =0;


        trianglesNormals[9].x= v3.x;
        trianglesNormals[9].y= -v3.y;
        trianglesNormals[9].z= v3.z;
        trianglesNormals[10].x= v2.x;
        trianglesNormals[10].y= -v2.y;
        trianglesNormals[10].z= v2.z;
        tn7.v[0]=trianglesNormals[8];
        tn7.v[1]=trianglesNormals[9];
        tn7.v[2]=trianglesNormals[10];
        vertices.push_back(tn7);
    }


    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v4.x,-v4.y,v4.z);
    glVertex3f(v3.x,-v3.y,v3.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[11].x = v0.x;
        trianglesNormals[11].x =v0.y;
        trianglesNormals[11].z = 0;

        trianglesNormals[12].x= v4.x;
        trianglesNormals[12].y= -v4.y;
        trianglesNormals[12].z= v4.z;
        trianglesNormals[13].x= v3.x;
        trianglesNormals[13].y= -v3.y;
        trianglesNormals[13].z= v3.z;

        tn8.v[0]=trianglesNormals[11];
        tn8.v[1]= trianglesNormals[12];
        tn8.v[2]=trianglesNormals[13];
        vertices.push_back(tn8);
    }


    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v5.x,-v5.y,v5.z);
    glVertex3f(v4.x,-v4.y,v4.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[11].x = v0.x;
        trianglesNormals[11].x =-v0.y;
        trianglesNormals[11].z = 0;
        trianglesNormals[12].x= v5.x;
        trianglesNormals[12].y= -v5.y;
        trianglesNormals[12].z= v5.z;
        trianglesNormals[5].x= v4.x;
        trianglesNormals[5].y= -v4.y;
        trianglesNormals[5].z= v4.z;
        tn9.v[0]=trianglesNormals[11];
        tn9.v[1]=trianglesNormals[12];
        tn9.v[2]=trianglesNormals[13];
        vertices.push_back(tn9);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x,v0.y,v0.z);
    glVertex3f(v6.x,-v6.y,v6.z);
    glVertex3f(v5.x,-v5.y,v5.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[14].x= v0.x;
        trianglesNormals[14].y =-v0.y;
        trianglesNormals[14].z = 0;
        trianglesNormals[15].x= v6.x;
        trianglesNormals[15].y= -v6.y;
        trianglesNormals[15].z= v6.z;
        trianglesNormals[16].x= v5.x;
        trianglesNormals[16].y= -v5.y;
        trianglesNormals[16].z= v5.z;
        tn10.v[0]=trianglesNormals[14];
        tn10.v[1]=trianglesNormals[15];
        tn10.v[2]= trianglesNormals[16];
        vertices.push_back(tn10);
    }


    ///aksnkansdkanjkandjknjsndjdsd

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v2.x,v2.y,v2.z);
    glVertex3f(-v1.x,v1.y,v1.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[17].x= -v0.x;
        trianglesNormals[17].y= v0.y;
        trianglesNormals[17].z= 0;
        trianglesNormals[18].x= -v2.x;
        trianglesNormals[18].y= v2.y;
        trianglesNormals[18].z= v2.z;
        trianglesNormals[19].x= -v1.x;
        trianglesNormals[19].y= v1.y;
        trianglesNormals[19].z= v1.z;
        tn11.v[0]=trianglesNormals[17];
        tn11.v[1]=trianglesNormals[18];
        tn11.v[2]=trianglesNormals[19];
        vertices.push_back(tn11);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v3.x,v3.y,v3.z);
    glVertex3f(-v2.x,v2.y,v2.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[20].x= -v0.x;
        trianglesNormals[20].y= v0.y;
        trianglesNormals[20].z= 0;

        trianglesNormals[21].x= -v3.x;
        trianglesNormals[21].y= v3.y;
        trianglesNormals[21].z= v3.z;

        trianglesNormals[22].x= -v2.x;
        trianglesNormals[22].y= v2.y;
        trianglesNormals[22].z= v2.z;

        tn12.v[0]=trianglesNormals[20];
        tn12.v[1]=trianglesNormals[21];
        tn12.v[2]=trianglesNormals[22];
        vertices.push_back(tn12);
    }


    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v4.x,v4.y,v4.z);
    glVertex3f(-v3.x,v3.y,v3.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[23].x= -v0.x;
        trianglesNormals[23].y= v0.y;
        trianglesNormals[23].z= 0;
        trianglesNormals[24].x= -v4.x;
        trianglesNormals[24].y= v4.y;
        trianglesNormals[24].z= v4.z;
        trianglesNormals[25].x= -v3.x;
        trianglesNormals[25].y= v3.y;
        trianglesNormals[25].z= v3.z;
        tn13.v[0]=trianglesNormals[23];
        tn13.v[1]=trianglesNormals[24];
        tn13.v[2]=trianglesNormals[25];
        vertices.push_back(tn13);
    }


    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v5.x,v5.y,v5.z);
    glVertex3f(-v4.x,v4.y,v4.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[26].x= -v0.x;
        trianglesNormals[26].y= v0.y;
        trianglesNormals[26].z= 0;
        trianglesNormals[27].x= -v5.x;
        trianglesNormals[27].y= v5.y;
        trianglesNormals[27].z= v5.z;
        trianglesNormals[28].x= -v4.x;
        trianglesNormals[28].y= v4.y;
        trianglesNormals[28].z= v4.z;
        tn14.v[0]=trianglesNormals[26];
        tn14.v[1]=trianglesNormals[27];
        tn14.v[2]=trianglesNormals[28];
        vertices.push_back(tn14);
    }


    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v6.x,v6.y,v6.z);
    glVertex3f(-v5.x,v5.y,v5.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[27].x= -v0.x;
        trianglesNormals[27].y= v0.y;
        trianglesNormals[27].z= 0;
        trianglesNormals[28].x= -v6.x;
        trianglesNormals[28].y= v6.y;
        trianglesNormals[28].z= v6.z;
        trianglesNormals[29].x= -v5.x;
        trianglesNormals[29].y= v5.y;
        trianglesNormals[29].z= v5.z;
        tn15.v[0]=trianglesNormals[27];
        tn15.v[1]=trianglesNormals[28];
        tn15.v[2]=trianglesNormals[29];
        vertices.push_back(tn15);
    }


    // desenho a outra metade da parede

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,-v0.y,v0.z);
    glVertex3f(-v1.x,-v1.y,v1.z);
    glVertex3f(-v2.x,-v2.y,v2.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[30].x= -v0.x;
        trianglesNormals[30].y= -v0.y;
        trianglesNormals[30].z= 0;
        trianglesNormals[31].x= -v1.x;
        trianglesNormals[31].y= -v1.y;
        trianglesNormals[31].z= v1.z;
        trianglesNormals[32].x= -v2.x;
        trianglesNormals[32].y= -v2.y;
        trianglesNormals[32].z= v2.z;
        tn16.v[0]=trianglesNormals[30];
        tn16.v[1]=trianglesNormals[31];
        tn16.v[2]=trianglesNormals[32];
        vertices.push_back(tn16);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,-v0.y,v0.z);
    glVertex3f(-v2.x,-v2.y,v2.z);
    glVertex3f(-v3.x,-v3.y,v3.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[33].x= -v0.x;
        trianglesNormals[33].y= -v0.y;
        trianglesNormals[33].z= 0;
        trianglesNormals[34].x= -v2.x;
        trianglesNormals[34].y= -v2.y;
        trianglesNormals[34].z= v2.z;
        trianglesNormals[35].x= -v3.x;
        trianglesNormals[35].y= -v3.y;
        trianglesNormals[35].z= v3.z;
        tn17.v[0]=trianglesNormals[33];
        tn17.v[1]=trianglesNormals[34];
        tn17.v[2]=trianglesNormals[35];
        vertices.push_back(tn17);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,-v0.y,v0.z);
    glVertex3f(-v3.x,-v3.y,v3.z);
    glVertex3f(-v4.x,-v4.y,v4.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[36].x= -v0.x;
        trianglesNormals[36].y= -v0.y;
        trianglesNormals[36].z= 0;
        trianglesNormals[37].x= -v3.x;
        trianglesNormals[37].y= -v3.y;
        trianglesNormals[37].z=  v3.z;
        trianglesNormals[38].x= -v4.x;
        trianglesNormals[38].y= -v4.y;
        trianglesNormals[38].z= v4.z;
        tn18.v[0]=trianglesNormals[36];
        tn18.v[1]=trianglesNormals[37];
        tn18.v[2]=trianglesNormals[38];
        vertices.push_back(tn18);
    }


    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,-v0.y,v0.z);
    glVertex3f(-v4.x,-v4.y,v4.z);
    glVertex3f(-v5.x,-v5.y,v5.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[39].x= -v0.x;
        trianglesNormals[39].y= -v0.y;
        trianglesNormals[39].z= 0;
        trianglesNormals[40].x= -v4.x;
        trianglesNormals[40].y= -v4.y;
        trianglesNormals[40].z= v4.z;
        trianglesNormals[41].x= -v5.x;
        trianglesNormals[41].y= -v5.y;
        trianglesNormals[41].z= v5.z;
        tn19.v[0]=trianglesNormals[39];
        tn19.v[1]=trianglesNormals[40];
        tn19.v[2]=trianglesNormals[41];
        vertices.push_back(tn19);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(-v0.x,v0.y,v0.z);
    glVertex3f(-v5.x,-v5.y,v5.z);
    glVertex3f(-v6.x,-v6.y,v6.z);
    glEnd();
    if(verticeAdded ==0)
    {
        trianglesNormals[42].x= -v0.x;
        trianglesNormals[42].y= v0.y;
        trianglesNormals[42].z= 0;
        trianglesNormals[43].x= -v5.x;
        trianglesNormals[43].y= -v5.y;
        trianglesNormals[43].z= v5.z;
        trianglesNormals[44].x= -v6.x;
        trianglesNormals[44].y= -v6.y;
        trianglesNormals[44].z= v6.z;
        tn20.v[0]=trianglesNormals[42];
        tn20.v[1]=trianglesNormals[43];
        tn20.v[2]=trianglesNormals[44];
        vertices.push_back(tn20);
    }

    glPopMatrix();
    verticeAdded =1;
}

void drawFaces()
{

    GLfloat objeto_especular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat objeto_brilho[]    = { 40.0f };
    GLfloat objeto_ambient[]   = { 0, 0.0, 0.0, 0.001 };
    GLfloat objeto_difusa[]    = { 1.0, 0.0, 0.0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corRebatedorLaterais[fase].objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);

    bool flagColisao = false;
   timerColision--;
   glPushMatrix();
    for(int i=0; i<vertices.size(); i++)
    {
        auxCalcNormal.x = vertices[i].v[2].x;
        auxCalcNormal.y = vertices[i].v[2].y;
        auxCalcNormal.z = 0;
        CalculaNormal2(auxCalcNormal,vertices[i].v[2],vertices[i].v[1],&normalFaces);
        normalFaces.z =0;
        //normalFaces.z= 0.5;
        if( timerColision <20 && (detecColisionLadoDireito(vertices[i])||detecColisionLadoEsquerdo(vertices[i])))
        {
            ///Fazer a soma vetorial aqui
             printf("x directionAntes %f ",direction[0]);
            printf("y directionAntes %f \n",direction[1]);
            direction[0] = direction[0]+normalFaces.x;
            direction[1] = direction[1]+normalFaces.y;
            printf("x direction %f ",direction[0]);
            printf("y direction %f \n",direction[1]);
            timerColision =100;

        }
        glNormal3f(normalFaces.x,normalFaces.y,normalFaces.z);
        glBegin(GL_QUADS);
        glVertex3f(vertices[i].v[2].x,vertices[i].v[2].y,vertices[i].v[2].z);
        glVertex3f(vertices[i].v[1].x,vertices[i].v[1].y,vertices[i].v[1].z);
        glVertex3f(vertices[i].v[1].x,vertices[i].v[1].y,0);
        glVertex3f(vertices[i].v[2].x,vertices[i].v[2].y,0);
        glEnd();
  }



glPopMatrix();

}




void drawHitter(vertex center,float sizeHitter){

   glPushMatrix();
    GLfloat objeto_especular[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0.1, 0.1, 0.1, 1.0};

    GLfloat objeto_difusa[]    = { 0.0, 1.0, 0.0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);


  float deltaX = 0.2;
  float deltaY = 0.13;

  vertex v1,v2,v3,v4;

  v1.z = v2.z =v3.z =v4.z =  0.3;
  center.z= 0.3;

  v1.x = center.x+sizeHitter;
  v1.y =center.y;

  v2.x = v1.x -deltaX;
  v2.y = v1.y +deltaY;

   vertex aux1,normalVector,aux2,aux3;

   int cont =0;
   //desenho a primeira metade do rebatedor

   while(cont < 3){

    aux1.x = v1.x;
    aux1.y = v1.y;
    aux1.z = 0;

    triangle r;

    r.v[0] = v2;
    r.v[1] = v1;
    r.v[2] = aux1;

    CalculaNormal2(v2,v1,aux1,&normalVector);
     if(detecColisionLadoDireito(r) && timerInicialColision ==0){
        direction[0] = direction[0]+normalVector.x;
        direction[1] = direction[1]+normalVector.y;
     }
    glNormal3f(normalVector.x,normalVector.y,normalVector.z);
    glBegin(GL_QUADS);
    glVertex3f(v2.x,v2.y,v2.z);
    glVertex3f(v1.x,v1.y,v1.z);
    glVertex3f(v1.x,v1.y,0.0);
    glVertex3f(v2.x,v2.y,0.0);
    glEnd();


    aux1.x = v1.x-2*(v1.x-center.x);
    aux1.y = v1.y;
    aux1.z = v1.z;

    aux2.x = v2.x-2*(v2.x-center.x);
    aux2.y = v2.y;
    aux2.z = v2.z;

    aux3.x = v2.x-2*(v2.x-center.x);
    aux3.y = v2.y;
    aux2.z = 0;


    triangle r1;

    r1.v[0] = center;
    r1.v[1] = aux1;
    r1.v[2] = aux2;
    normalVector = calcNormal(r1);

        CalculaNormal2(center,aux2,aux1,&normalVector);
           if(detecColisionLadoEsquerdo(r1) && timerInicialColision ==0){
        direction[0] = -direction[0] + normalVector.x*0.7;
        direction[1] = -direction[1] + normalVector.y*0.7;     }

    glNormal3f(normalVector.x,normalVector.y,normalVector.z);
    glBegin(GL_QUADS);
    glVertex3f(v1.x-2*(v1.x-center.x),v1.y,v1.z);
    glVertex3f(v2.x-2*(v2.x-center.x),v2.y,v2.z);
    glVertex3f(v2.x-2*(v2.x-center.x),v2.y,0.0);
    glVertex3f(v1.x-2*(v1.x-center.x),v1.y,0.0);
    glEnd();

    //parte de baixo do rebatedor
     aux1.x = v1.x;
    aux1.y = v1.y;
    aux1.z = 0;
    triangle r3;

    vertex vaux, vaux2,vaux3;

    vaux.x= center.x+sizeHitter;
    vaux.y = center.y;
    vaux.z =  center.z;

    vaux2.x= center.x-sizeHitter;
    vaux2.y = center.y;
    vaux2.z =  center.z;

    vaux3.x= center.x+sizeHitter;
    vaux3.y = center.y;
    vaux3.z =  0;

    r3.v[0] = v2;
    r3.v[1] = aux1;
    r3.v[2] = v1;
    vertex normalVector3;
    normalVector3 = calcNormal(r3);
    glNormal3f(normalVector3.x,normalVector3.y,normalVector3.z);

    /// parte de baixo
    glBegin(GL_QUADS);

    glNormal3f(0,-1,0);
    glVertex3f(center.x+sizeHitter,center.y,center.z);
    glVertex3f(center.x-sizeHitter,center.y,center.z);
    glVertex3f(center.x-sizeHitter,center.y,0.0);
    glVertex3f(center.x+sizeHitter,center.y,0.0);
    glEnd();

    triangle t4;
    vertex normalVector4;
    t4.v[0] = center;
    t4.v[1] = v1;
    t4.v[2] = v2;
    normalVector4 =calcNormal(t4);
    glNormal3f(normalVector4.x,normalVector4.y,normalVector4.z);
    glBegin(GL_TRIANGLES);
    glVertex3f(center.x,center.y,center.z);
    glVertex3f(v1.x,v1.y,v1.z);
    glVertex3f(v2.x,v2.y,v2.z);
    glEnd();

    /// desenho a outra metade do rebatedor


     triangle t2;

    vertex normalVector2, aux11,aux12,aux13;
    aux12.x = v2.x-2*(v2.x-center.x);
    aux12.y= v2.y;
    aux2.z = v2.z;

    aux11.x = v1.x-2*(v1.x-center.x);
    aux11.y= v1.y;
    aux11.z = v1.z;

    aux13.x= v2.x-2*(v2.x-center.x);
    aux13.y = v2.y;
    aux13.z =v2.z;

    t2.v[0] = center;
    t2.v[1] = aux11;
    t2.v[2] = aux12;

    CalculaNormal2(aux11,aux13,aux12,&normalVector2);
  if(detecColisionLadoEsquerdo(t2) && timerInicialColision==0){

        direction[0] = -direction[0] + normalVector2.x;
        direction[1] = -direction[1] + normalVector2.y;
        }
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

void drawExits(){

    GLfloat objeto_especular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0.3, 0.3, 0.3, 1.0};
    GLfloat objeto_difusa[]    = { 0.0, 1.0, 0.0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);

    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(-1.8,2.0,0.5);
    glVertex3f(-1.8,1.8,0.5);
    glVertex3f(-1.2,1.8,0.5);
    glVertex3f(-1.2,2.0,0.5);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(1.8,2.0,0.5);
    glVertex3f(1.2,2.0,0.5);
    glVertex3f(1.2,1.8,0.5);
    glVertex3f(1.8,1.8,0.5);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(1,0,0);
    glVertex3f(1.8,2.0,0.5);
    glVertex3f(1.8,1.8,0.5);
    glVertex3f(1.8,1.8,0);
    glVertex3f(1.8,2.0,0);
    glEnd();

     glBegin(GL_QUADS);
    glNormal3f(-1,0,0);
    glVertex3f(-1.8,2.0,0.5);
    glVertex3f(-1.8,1.8,0.5);
    glVertex3f(-1.8,1.8,0);
    glVertex3f(-1.8,2.0,0);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0,-1,0);
    glVertex3f(-1.8,1.8,0.5);
    glVertex3f(-1.8,1.8,0);
    glVertex3f(-1.2,1.8,0.0);
    glVertex3f(-1.2,1.8,0.5);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0,-1,0);
    glVertex3f(1.8,1.8,0);
    glVertex3f(1.8,1.8,0.5);
    glVertex3f(1.2,1.8,0.5);
    glVertex3f(1.2,1.8,0.0);
    glEnd();

     glBegin(GL_QUADS);
    glNormal3f(0,-1,0);
     glVertex3f(1.2,2.0,0.5);
    glVertex3f(1.2,2.0,0);
    glVertex3f(1.2,1.8,0.0);
    glVertex3f(1.2,1.8,0.5);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0,-1,0);
    glVertex3f(-1.2,2.0,0);
    glVertex3f(-1.2,2.0,0.5);
    glVertex3f(-1.2,1.8,0.5);
    glVertex3f(-1.2,1.8,0.0);
    glEnd();


 glPopMatrix();

}




void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);

    float yVision= 0;
    glLoadIdentity();

    int ortho = 2;

    if(perspective){
        gluPerspective(60, (GLfloat) width / (GLfloat) height, 0.01, 200.0);
        }
    else
    {
        if (width <= height)
            glOrtho (-ortho, ortho, -ortho*height/width, ortho*height/width, 0, 200);
        else
            glOrtho (-ortho*width/height, ortho*width/height, -ortho, ortho, 0, 200);

    }

   glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   gluLookAt (xCamera, yCamera, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    if(pause ==0){
             rotationX = 0.0;
             rotationY=  0.0;
            }

   cout<<xCamera<<" "<<yCamera<<" "<<zdist<<endl;

    glRotatef(rotationY, 0, 1, 1);
    glRotatef(rotationX, 1, 0, 1);
    glRotatef(angulo, 1, 0, 0);
    handleColisionsWithObjects();
    if(gameStarted ==0){
        centroRebatedor.x=0;
        centroRebatedor.y =-1.3;
        centroRebatedor.z =0;
        gameStarted =1;
    }




     if(animate == true && timerInicialColision > 0)
            timerInicialColision= timerInicialColision-1;
    glPushMatrix();
    if (!animate)
        drawArrow();

        drawSphere();
    drawBorderss1();
    drawFaces();
    drawHitter(centroRebatedor,0.6);


    for (int i = 0; i < NUMRETAN; ++i)
    {
        if(retangulos[i].reduzir && !retangulos[i].colisao)
        {
            desenhaRetangulo(retangulos[i]);
        }

        else if(!retangulos[i].colisao)
            desenhaRetangulo(retangulos[i]);
    }

    //desenhaRetangulo(rebatedor);


    drawBoard();

    glPopMatrix();


    //**************************************************************

    //  glDisable(GL_LIGHTING);

    if(perdeu)
    {
        glDisable(GL_LIGHTING);

        glRasterPos2f(0, 0);

        glColor3f(0, 0, 1);

        glTranslatef(-0.5, -0.1, rebatedor.centro.z + 0.5);

        strcpy(texto, "Voce Perdeu");
        glScalef(0.001, 0.001, 0.001);



        for (int i = 0; texto[i] != '\0'; i++)
        {
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, texto[i]);
        }

        glEnable(GL_LIGHTING);
    }

    else
    if(pause)
    {

        glDisable(GL_LIGHTING);

        glRasterPos2f(0, 0);

        glColor3f(0, 0, 1);




        if(pause)
        {
            strcpy(texto, "Jogo Pausado");
            glTranslatef(-0.5, -0.1, rebatedor.centro.z + 0.5);

        }

        if(venceu)
        {
            strcpy(texto, "Voce Venceu");
            glTranslatef(0.05, -0.5, rebatedor.centro.z + 0.5);
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


// update all states
void updateState() {
    if (!animate) return;
    // maximum board position
    const float maxRange[] = {-BHF + BALL_RADIUS, BHF - BALL_RADIUS};

    float movement = velocity * 0.12;

    float tam = sqrt(pow(direction[0],2) + pow(direction[1],2));

    direction[0] /=tam;
    direction[1] /=tam;

    // Move the ball and handle overflow
    position[0] = fixRange(position[0] + movement * direction[0], maxRange[0], maxRange[1]);
    position[1] = fixRange(position[1] + movement * direction[1], maxRange[0], maxRange[1]);

    if(((position[1] - BALL_RADIUS) <= - BHF) && vidas > 0)
    {
        vidas -= 1;
        animate = false;
        position[0] = rebatedor.centro.x;
        position[1] = -1.01;
        timerInicialColision =30;

        if(vidas <= 0)
        {
            animate = false;
            perdeu = true;
            pause = true;
        }

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
                    (position[1] > retangulos[j].pontosExtremos[0].y) && (position[0]) > (retangulos[j].pontosExtremos[0].x))

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


} else if (((position[1]) - BALL_RADIUS <= (retangulos[j].pontosExtremos[2].y))&& (position[1] > retangulos[j].pontosExtremos[0].y))                {
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
                    if(direction[1] < 0)
                        direction[1] = -direction[1];

                } else if (side == 2)
                {
                   if(direction[1] > 0)
                        direction[1] = -direction[1];
                } else if (side == 3)
                {
                     if(direction[0] < 0)
                        direction[0] = -direction[0];
                } else if (side == 4)
                {
                        if(direction[0] > 0)
                        direction[0] = -direction[0];                }
            }
            else
            {

                position[0] = fixRange(position[0] + movement * direction[0], maxRange[0], maxRange[1]);
                position[1] = fixRange(position[1] + movement * direction[1], maxRange[0], maxRange[1]);

                direction[1] = -direction[1];
                direction[0] = -direction[0];

                cout<<"Quina.\n";
            }

            retangulos[j].reduzir = true;
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
            {

                if(fase == 2)
                {
                    pause = true;

                }
                else
                {
                    fase +=1;
                    reiniciaJogo();
                    venceu = false;
                    break;
                }
            }
            retangulos[j].colisao = false;
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



void generatePrisms()
{

    float y = 0.5;
    float x = -1.7 + 0.35/2.0;
    //float x = 0.5;

    //Desenha linha de retangulos


        for (int i = 0; i < 9; ++i)
        {  //4

            makeRetangulo(x, y, retangulos[i]);
            x += 0.39;

            //cout<<"x = "<<x<<" ";
        }

        //  cout<<endl;

        y += 0.4;
        x = -1.9 + 0.35/2.0;


        for (int i = 0; i < 10; ++i)
        {  //4

            makeRetangulo(x, y, retangulos[9+i]);
            x += 0.39;

            //cout<<"x = "<<x<<" ";
        }

        //  cout<<endl;

    x = -2 + 0.35/2.0;
    y += 0.4;

        for (int i = 0; i < 10; ++i)
        {  //4

        makeRetangulo(x, y, retangulos[19+i]);
        x += 0.39;

        //cout<<"x = "<<x<<" ";
         }

/*    x = -2 + 0.35/2.0;
    y += 0.3;

    for (int i = 0; i < 10; ++i)
    {  //4

        makeRetangulo(x, y, retangulos[i]);
        x += 0.39;

        //cout<<"x = "<<x<<" ";
    }*/





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
            centroRebatedor.x = rebatedor.centro.x +0.1;
        }

    } else if ((x - xAntigo) < 0)
    {
        if (fabs(rebatedor.centro.x - rebatedor.largura / 2 - 0.1) < 2)
        {

            rebatedor.atualizaPosicao(rebatedor.centro.x - 0.1);
            centroRebatedor.x = centroRebatedor.x -0.1;
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

void keyboard(unsigned char key, int x, int y) {


    if (!animate && !pause) {
        switch (tolower(key)) {



            case 'r':
                reiniciaJogo();
                break;

            case 27:
                exit(0);
                break;

            case ' ':

                if(pause){
                    pause=0;

                }
                else{
                    pause = 1;
                }
                break;




        }
        velocity = fixRange(velocity, -1, 1);
        initialDirection = fixRange(initialDirection, -180, 180, true);
        direction[0] = cos((initialDirection + 90) * M_PI / 180);
        direction[1] = sin((initialDirection + 90) * M_PI / 180);
    } else
    {
        double cosAngulo, senAngulo;
        float xNew, yNew, zNew;

        GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
        GLfloat posicao_luz[] = { xCamera , yCamera, zdist, 1.0};


        switch(tolower(key))
        {

            case 'p':

                perspective = !perspective;
                glDisable(GL_LIGHTING);                 // Habilita luz
                glDisable(GL_LIGHT0);                   // habilita luz 0
                glDisable(GL_DEPTH_TEST);
                xCamera = yCamera = 0;
                posicao_luz[0] =  xCamera;
                posicao_luz[1] = yCamera;
                posicao_luz[2] = 1000;

                // Define parametros da luz
                glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
                glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
                glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

                glEnable(GL_LIGHTING);                 // Habilita luz
                glEnable(GL_LIGHT0);                   // habilita luz 0
                glEnable(GL_DEPTH_TEST);

                glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

                updateState();


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

            case 'd':

                if(!perspective)
                    break;
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                cosAngulo = cos((5*3.14)/180);
                senAngulo = sin((5*3.14)/180);

                xNew = xCamera*cosAngulo - yCamera*senAngulo;
                yNew = xCamera*senAngulo + yCamera*cosAngulo;


                xCamera = xNew;
                yCamera = yNew;


                // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)



                glDisable(GL_LIGHTING);                 // Habilita luz
                glDisable(GL_LIGHT0);                   // habilita luz 0
                glDisable(GL_DEPTH_TEST);

                posicao_luz[0] =  xCamera;
                posicao_luz[1] = yCamera;
                posicao_luz[2] = 1000;

                // Define parametros da luz
                glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
                glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
                glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

                glEnable(GL_LIGHTING);                 // Habilita luz
                glEnable(GL_LIGHT0);                   // habilita luz 0
                glEnable(GL_DEPTH_TEST);

                glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

                break;

            case 'a':

                if(!perspective)
                    break;

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                cosAngulo = cos((-5*3.14)/180);
                senAngulo = sin((-5*3.14)/180);

                xNew = xCamera*cosAngulo - yCamera*senAngulo;
                yNew = xCamera*senAngulo + yCamera*cosAngulo;

                xCamera = xNew;
                yCamera = yNew;


                // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
                posicao_luz[0] =  xCamera;
                posicao_luz[1] = yCamera;
                posicao_luz[2] = 1000;

                glDisable(GL_LIGHTING);                 // Habilita luz
                glDisable(GL_LIGHT0);                   // habilita luz 0
                glDisable(GL_DEPTH_TEST);

                // Define parametros da luz
                glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
                glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
                glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

                glEnable(GL_LIGHTING);                 // Habilita luz
                glEnable(GL_LIGHT0);                   // habilita luz 0
                glEnable(GL_DEPTH_TEST);

                glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


                break;

            case 'w':

                if(!perspective)
                    break;

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                cosAngulo = cos((5*3.14)/180);
                senAngulo = sin((5*3.14)/180);

                yNew = yCamera*cosAngulo - zdist*senAngulo;
                zNew = yCamera*senAngulo + zdist*cosAngulo;

                yCamera = yNew;
                zdist = zNew;


                // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
                posicao_luz[0] =  xCamera;
                posicao_luz[1] = yCamera;
                posicao_luz[2] = 1000;

                glDisable(GL_LIGHTING);                 // Habilita luz
                glDisable(GL_LIGHT0);                   // habilita luz 0
                glDisable(GL_DEPTH_TEST);

                // Define parametros da luz
                glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
                glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
                glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

                glEnable(GL_LIGHTING);                 // Habilita luz
                glEnable(GL_LIGHT0);                   // habilita luz 0
                glEnable(GL_DEPTH_TEST);

                glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

                break;

            case 's':

                if(!perspective)
                    break;

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                cosAngulo = cos((-5*3.14)/180);
                senAngulo = sin((-5*3.14)/180);

                yNew = yCamera*cosAngulo - zdist*senAngulo;
                zNew = yCamera*senAngulo + zdist*cosAngulo;

                yCamera = yNew;
                zdist = zNew;


                // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
                posicao_luz[0] =  xCamera;
                posicao_luz[1] = yCamera;
                posicao_luz[2] = 1000;

                glDisable(GL_LIGHTING);                 // Habilita luz
                glDisable(GL_LIGHT0);                   // habilita luz 0
                glDisable(GL_DEPTH_TEST);

                // Define parametros da luz
                glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
                glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
                glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

                glEnable(GL_LIGHTING);                 // Habilita luz
                glEnable(GL_LIGHT0);                   // habilita luz 0
                glEnable(GL_DEPTH_TEST);

                glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

                break;


            case '32':
                if(pause)
                    pause=0;
                else
                    pause = 1;

                break;

        }
    }


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
    glEnable(GL_DEPTH_TEST);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}

