//
// Created by igor on 25/11/19.

#include <iostream>
#include "SkyBox.h"
#define A 8
void renderSkyBox()
{

    std::cout<<"Sky box\n";

    glPushMatrix();
/*    GLfloat objeto_ambient[]   = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat objeto_difusa[]    = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat objeto_especular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat objeto_brilho[]    = { 70.0f };*/

    GLfloat objeto_ambient[]   = { 1, 1, 1, 1.0 };
    GLfloat objeto_difusa[]    = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat objeto_especular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat objeto_brilho[]    = { 70.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);



    textureManager->Bind(6);

    // Calculo abaixo funciona apenas se textura estiver centralizada na origem
    float h = 1.0f;
    float w = 1.0f;

    //  parte de tras

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.25, 0.33);
    glVertex3f(-A, A, -A);


    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.33);
    glVertex3f(A, A, -A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.66);
    glVertex3f(A, A, A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.25, 0.66);
    glVertex3f(-A, A, A);
    glEnd();


   // esquerda
    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.25, 0.33);
    glVertex3f(-A, A, -A);


    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.33);
    glVertex3f(-A, -A, -A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.66);
    glVertex3f(-A, -A, A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.25, 0.66);
    glVertex3f(-A, A, A);
    glEnd();



    //direita

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.25, 0.33);
    glVertex3f(A, A, -A);


    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.33);
    glVertex3f(A, -A, -A
    );

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.66);
    glVertex3f(A, -A, A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.25, 0.66);
    glVertex3f(A, A, A
    );
    glEnd();


    // baixo
    textureManager->Bind(7);
    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.25, 0.33);
    glVertex3f(-A, -A, -A
    );


    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.33);
    glVertex3f(-A, A, -A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.66);
    glVertex3f(A, A, -A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.25, 0.66);
    glVertex3f(A, -A, -A);
    glEnd();

   textureManager->Disable();

    //frente
    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.25, 0.33);
    glVertex3f(A, -A, A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.33);
    glVertex3f(A, -A, -A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.66);
    glVertex3f(-A, -A, -A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.25, 0.66);
    glVertex3f(-A, -A, A);
    glEnd();


    //cima
    textureManager->Bind(7);
    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.25, 0.33);
    glVertex3f(-A, -A, A);


    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.33);
    glVertex3f(-A, A, A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.5, 0.66);
    glVertex3f(A, A, A);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0.25, 0.66);
    glVertex3f(A, -A, A);
    glEnd();
      textureManager->Disable();
    textureManager->Disable();
}
