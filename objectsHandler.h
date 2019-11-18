//
// Created by ark on 15/11/19.
//

#ifndef TRABALHOCG01_OBJECTSHANDLER_H
#define TRABALHOCG01_OBJECTSHANDLER_H

#include <vector>
#include "Retangulo.h"
#define NUM_OBJECTS 2
using  namespace std;

extern float deltaScaleObject1;
extern float deltaScaleObject2;
extern vector<int> objectsALives;

class objectsHandler {
public:
    void initObjects();
    void drawObjects();


};


#endif //TRABALHOCG01_OBJECTSHANDLER_H
