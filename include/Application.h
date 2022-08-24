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
#include "Enemy.h"

class Application
{
public:
    typedef std::vector<LineBoxModel*> HitboxVectorLaser;
    typedef std::vector<LineBoxModel*> HitboxVectorMonster;
    typedef std::vector<Enemy*> MonsterVector;
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
    Camera cam;
    ModelList models;
    MonsterVector monsterModels;
    LineBoxModel* hitboxModel;
    LineBoxModel* spaceship;
    HitboxVectorMonster hitboxListMonster;
    HitboxVectorLaser hitboxListLaser;
    LaserVector laserModels;
    GLFWwindow* pWindow;
    BaseModel* pModel;
    BaseModel* earth;
    BaseModel* venus;
    Spaceship* pSpaceship;
    Enemy* pEnemy;
    ShadowMapGenerator shadowGenerator;
    int pCurrentLaser;
    float laserTimer;
    float monsterTimer;
    void loopCollision();
    int pCurrentMonster;
    int score;
    float randomFloat(float a, float b);
    void updatePlanet(float dtime);
};

#endif /* Application_hpp */
