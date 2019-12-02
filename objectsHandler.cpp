//
// Created by ark on 15/11/19.
//

#include "objectsHandler.h"
#include "glcWavefrontObject.h"
#include "Posicao.h"
#include "Colisao.h"
#include "glcTexture.h"

float deltaScaleObject1 = 0.08;
float deltaScaleObject2 = 0.08;
vector<int> objectsALives;
float lastXObject,lastYObject;
glcTexture *textureManagerObjects = NULL;

char objectFiles[NUM_OBJECTS][50] =
        {
                "../objects/ball/13517_Beach_Ball_v2_L3.obj",
                "../objects/helmo/HelmetOBJ.obj"
        };
char textureFiles[NUM_OBJECTS][50] =
        {
                "../objects/ball/Beach_Ball_diffuse.png",
                "../objects/helmo/BaseColor.png"
        };
typedef struct
{
    //GLMmodel* pmodel = NULL
    glcWavefrontObject *pmodel = NULL;
} object;

int selectedShading = SMOOTH_SHADING;
int selectedRender = USE_MATERIAL;
float randomStart[9] ={0.01,-0.02,0.03,-0.04,-0.05,0.06,0.-07,0.08,-0.09};

glcWavefrontObject* objectsHandler::initObjects() {

    objectsALives.push_back(0);
    objectsALives.push_back(0);
// LOAD OBJECTS
    // LOAD TEXTURES
    textureManagerObjects = new glcTexture();
    textureManagerObjects->SetNumberOfTextures(NUM_OBJECTS);
    textureManagerObjects->SetWrappingMode(GL_REPEAT);
    for(int i = 0; i < NUM_OBJECTS; i++)
        textureManagerObjects->CreateTexture( textureFiles[i], i);

    // LOAD OBJECTS
    glcWavefrontObject* objecstManager = new glcWavefrontObject();
    objecstManager->SetNumberOfObjects(NUM_OBJECTS);
    for(int i = 0; i < NUM_OBJECTS; i++)
    {
        objecstManager->SelectObject(i);
        objecstManager->ReadObject(objectFiles[i]);
        objecstManager->Unitize();
        objecstManager->FacetNormal();
        objecstManager->VertexNormals(90.0);
        objecstManager->Scale(5);
    }
    return  objecstManager;
}

void objectsHandler::drawObjects(glcWavefrontObject*objectManagerParam,glcTexture *textureManagerParam, int timerPosition){
    textureManagerParam->Bind(0);
    glPushMatrix();
    if(!pause)
        updateObjects1Position(timerPosition);
    if(objectsALives[0]==0 && timerDeathObject1 ==0){
        positionXObject1 = -1.5;
        positionYObject1 = 1.8;
        int i = rand() % 8;
        deltaXobject1 = randomStart[i];
        objectsALives[0] =1;
        i = NULL;
    }

    objectManagerParam->SelectObject(0);
    objectManagerParam->SetShadingMode(SMOOTH_SHADING);
    objectManagerParam->SetRenderMode(USE_TEXTURE_AND_MATERIAL);
    objectManagerParam->Unitize();
    glPushMatrix();
    glTranslatef(positionXObject1,positionYObject1,0.2);
    if(timerDeathObject1>0){
        objectManagerParam->Scale(0.5-deltaScaleObject1);
        deltaScaleObject1 = deltaScaleObject1+0.005;
        timerDeathObject1--;
        if(0.5-deltaScaleObject1<0){
            timerDeathObject1 = 0;
            objectsALives[0]=0;
            deltaScaleObject1 = 0.08;
        }
    }else{
        objectManagerParam->Scale(0.5);
    }
    objectManagerParam->Draw();
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

    objectManagerParam->SelectObject(1);
    objectManagerParam->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
    objectManagerParam->SetRenderMode(USE_TEXTURE_AND_MATERIAL);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
    objectManagerParam->Unitize();
    glPushMatrix();
    glTranslatef(positionXObject2,positionYObject2,0.2);
    if(timerDeathObject2>0){
        objectManagerParam->Scale(0.5-deltaScaleObject2);
        deltaScaleObject2 = deltaScaleObject2+0.005;
        timerDeathObject2--;
        if(0.5-deltaScaleObject2<0){
            timerDeathObject2 = 0;
            objectsALives[1]=0;
            deltaScaleObject2 = 0.08;
        }
    }else{
        objectManagerParam->Scale(0.5);
    }
    objectManagerParam->Draw();
    glPopMatrix();
    glPopMatrix();
    textureManagerParam->Disable();
}