//
// Created by igor on 25/11/19.

#include <iostream>
#include "SkyBox.h"
#define A 2
void renderSkyBox()
{

    std::cout<<"Sky box\n";

    glPushMatrix();
/*    GLfloat objeto_ambient[]   = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat objeto_difusa[]    = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat objeto_especular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat objeto_brilho[]    = { 70.0f };*/

    GLfloat objeto_ambient[]   = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat objeto_difusa[]    = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat objeto_especular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat objeto_brilho[]    = { 70.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);



    textureManager->Bind(0);

    // Calculo abaixo funciona apenas se textura estiver centralizada na origem
    float h = 1.0f;
    float w = 1.0f;

    // base

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0);
    glVertex3f(-A, A, -2);


    glNormal3f(0, -1, 0.0);
    glTexCoord2f(1, 0);
    glVertex3f(A, A, -2.0);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(1, 1);
    glVertex3f(A, A, 2.0);

    glNormal3f(0, -1, 0.0);
    glTexCoord2f(0, 1);
    glVertex3f(-A, A, 2.0);
    glEnd();

    textureManager->Disable();
}
