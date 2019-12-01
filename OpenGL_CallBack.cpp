//
// Created by igor on 12/11/19.
//

#include "OpenGL_CallBack.h"

//Definição de variáveis
int width;
int height;
bool fullScreen = false;
bool animate = false;
bool pause = false;
float xCamera = 0;
float yCamera = -2;
bool perspective = true;
float zdist = 4.0;
float rotationX = 0;
float rotationY = 0;
int last_x;
int last_y;
int onStartScreen;
int gameStarted =0;
float initialDirection = 0;
bool venceu = false;
bool perdeu = false;

void reshape(int w, int h)
{
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


// Motion callback
void motion(int x, int y)
{

    if(pause ==1){/// Se o game está pausado, então eu posso girar a câmera livremente

        rotationX += (float) (y - last_y);
        last_y = y;

        rotationY += (float) (x - last_x);
        last_x = x;
    }

}

// Mouse callback
void mouse(int button, int state, int x, int y)
{

    if(!animate && !pause)
    {
        if ((button == GLUT_LEFT_BUTTON ) && (state == GLUT_DOWN))
        {
            if(onStartScreen){
                onStartScreen =0;
            } else {
                animate = !animate;
                gameStarted = 1;
            }
        }
        else if(button == 3)
        {
            initialDirection += 3;
        }
        else
        {
            initialDirection -= 3;
        }
        initialDirection = fixRange(initialDirection, -180, 180, true);
        direction[0] = cos((initialDirection + 90) * M_PI / 180);
        direction[1] = sin((initialDirection + 90) * M_PI / 180);
    }

}

void reiniciaJogo()
{
    for(int i = 0; i < NUMRETAN;++i)
    {
        retangulos[i].colisao = false;
        retangulos[i].reduzir = false;
        retangulos[i].escala = 1.0;
    }

    pause = false;
    venceu = false;
    animate = false;
    perdeu = false;

    rebatedor.atualizaPosicao();
    initialDirection = 0;
    initialDirection = fixRange(initialDirection, -180, 180, true);

    position[0] = 0;
    position[1] = -1.01;

    direction[0] = cos((initialDirection + 90) * M_PI / 180);
    direction[1] = sin((initialDirection + 90) * M_PI / 180);

}