//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"

#include "utils/Shader/ConstantShader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "utils/Models/BaseModel.h"
#include "utils/Lights/ShadowMapGenerator.h"
#include "Spaceship.h"
#include "utils/Models/LineBoxModel.h"

class Application
{
public:
    typedef std::vector<LineBoxModel*> HitboxVector;
    typedef std::vector<BaseModel*> MonsterVector;
    typedef std::list<BaseModel*> ModelList;
    typedef std::vector<BaseModel*> LaserVector;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();

protected:
    void updateMonster(float dtime);
    void createScene();
    void createNormalTestScene();
    void createShadowTestScene();
    void updateLaser(float dtime);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    Camera Cam;
    ModelList Models;
    MonsterVector MonsterModels;
    LineBoxModel* hitboxModel;
    HitboxVector hitboxList;
    LaserVector LaserModels;
    GLFWwindow* pWindow;
    BaseModel* pModel;
    Spaceship* pSpaceship;
    ShadowMapGenerator ShadowGenerator;
    int pCurrentLaser;
    int laserTimer;
};

#endif /* Application_hpp */
