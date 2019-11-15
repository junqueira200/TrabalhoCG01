//
// Created by ark on 15/11/19.
//

#include "objectsHandler.h"
#include "glcWavefrontObject.h"
#include "Posicao.h"
#include "Colisao.h"
float deltaScaleObject1 = 0.08;
float deltaScaleObject2 = 0.08;
vector<int> objectsALives;

float lastXObject,lastYObject;

char objectFiles[NUM_OBJECTS][50] =
        {
                "data/obj/soccerball.obj",
                "data/obj/dolphins.obj"
        };

typedef struct
{
    //GLMmodel* pmodel = NULL
    glcWavefrontObject *pmodel = NULL;
} object;

object *objectList;
glcWavefrontObject *objectManager = NULL;
bool drawboundingbox = false;
int selected = 0;
int selectedShading = SMOOTH_SHADING;
int selectedRender = USE_MATERIAL;
float randomStart[9] ={0.01,-0.02,0.03,-0.04,-0.05,0.06,0.-07,0.08,-0.09};




void objectsHandler::initObjects() {
    objectsALives.push_back(0);
    objectsALives.push_back(0);
// LOAD OBJECTS
    objectManager = new glcWavefrontObject();
    objectManager->SetNumberOfObjects(NUM_OBJECTS);
    for (int i = 0; i < NUM_OBJECTS; i++) {
        objectManager->SelectObject(i);
        objectManager->ReadObject(objectFiles[i]);
        objectManager->Unitize();
        objectManager->FacetNormal();
        objectManager->VertexNormals(90.0);
    }
}



void objectsHandler::drawObjects(){

    glPushMatrix();
    updateObjects1Position();
    if(objectsALives[0]==0 && timerDeathObject1 ==0){
        positionXObject1 = -1.5;
        positionYObject1 = 1.8;
        int i = rand() % 8;
        deltaXobject1 = randomStart[i];
        objectsALives[0] =1;
        i = NULL;
    }
    objectManager->SelectObject(0);
    objectManager->SetShadingMode(selectedShading); // Possible values: FLAT_SHADING e SMOOTH_SHADING
    objectManager->SetRenderMode(selectedRender);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
    objectManager->Unitize();
    glPushMatrix();
    glTranslatef(positionXObject1,positionYObject1,0.2);
    if(timerDeathObject1>0){
        objectManager->Scale(0.5-deltaScaleObject1);
        deltaScaleObject1 = deltaScaleObject1+0.005;
        timerDeathObject1--;
        if(0.5-deltaScaleObject1<0){
            timerDeathObject1 = 0;
            objectsALives[0]=0;
            deltaScaleObject1 = 0.08;
        }
    }else{
        objectManager->Scale(0.5);
    }
    objectManager->Draw();
    glPopMatrix();


    ///desenho o segundo objeto
    if(objectsALives[1]==0 && timerDeathObject2 ==0){
        positionXObject2 = 1.5;
        positionYObject2 = 1.8;
        int i = rand() % 7;
        deltaXobject1 = randomStart[i];
        objectsALives[1] =1;
        i = NULL;
    }
    objectManager->SelectObject(1);
    objectManager->SetShadingMode(selectedShading); // Possible values: FLAT_SHADING e SMOOTH_SHADING
    objectManager->SetRenderMode(selectedRender);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
    objectManager->Unitize();
    glPushMatrix();
    glTranslatef(positionXObject2,positionYObject2,0.2);
    if(timerDeathObject2>0){
        objectManager->Scale(0.5-deltaScaleObject2);
        deltaScaleObject2 = deltaScaleObject2+0.005;
        timerDeathObject2--;
        if(0.5-deltaScaleObject2<0){
            timerDeathObject2 = 0;
            objectsALives[1]=0;
            deltaScaleObject2 = 0.08;
        }
    }else{
        objectManager->Scale(0.5);
    }
    objectManager->Draw();
    glPopMatrix();
    glPopMatrix();
}