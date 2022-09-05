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
#include <ParticleProps.h>
#include "camera.h"

#include "utils/Shader/ConstantShader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "utils/Models/BaseModel.h"
#include "utils/Lights/ShadowMapGenerator.h"
#include "Spaceship.h"
#include "utils/Models/LineBoxModel.h"
#include "Enemy.h"
#include "Items.h"
#include "ParticleSystem.h"

class Application
{
public:
    typedef std::vector<LineBoxModel*> HitboxVectorLaser;
    typedef std::vector<LineBoxModel*> HitboxVectorMonster;
    typedef std::vector<Enemy*> MonsterVector;
    typedef std::vector<Items*> ItemsVector;
    typedef std::list<BaseModel*> ModelList;
    typedef std::vector<BaseModel*> LaserVector;
    typedef std::vector<BaseModel*> LaserBossVector;
    explicit Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    static float randomFloat(float a, float b);
    void draw();
    void end();
protected:

    int pCurrentMonster;
    int pCurrentItem;
    int score = 0;
    int pCurrentLaser;
    int pCurrentBossLaser;
    int bossHit = 0;
    bool item1 = false;
    bool item2 = false;
    bool item3 = false;
    bool game = false;
    float randomMonsterMovement = 1.0f;
    float adjustAmplitudePerSecond = -10.0f;
    float timeElapsed = 0.0f;
    float itemTimer;
    float laserTimer;
    float monsterTimer;
    float bossTimer = 5.0f;
    float laserBossTimer = 1.5f;
    float bossTimerMovement = 1.5f;
    float itemTime = 3.0f;
    float item2Time = 3.0f;
    float item3Time = 3.0f;
    float camTimer = 4.0f;
    float particleSchubTimer = 0.05f;
    float particleEnemySchub = 0.05f;
    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;
    Vector camUrsprung;
    Vector v1;
    Camera cam;
    ModelList models;
    MonsterVector monsterModels;
    ItemsVector itemsModels;
    LineBoxModel* hitboxModel;
    LineBoxModel* spaceship;
    HitboxVectorMonster hitboxListMonster;
    HitboxVectorLaser hitboxListLaser;
    LaserVector laserModels;
    LaserBossVector laserBossModels;
    GLFWwindow* pWindow;
    BaseModel* pModel;
    BaseModel* earth;
    BaseModel* venus;
    Spaceship* pSpaceship;
    Enemy* pEnemy;
    Enemy* pBoss;

    ParticleProps particleProps;
    ParticleSystem* particleSystem;
public:
    Enemy *getPBoss() const;

protected:
    Items* pItem;
    ShadowMapGenerator shadowGenerator;
    void loopCollision(float dtime);
    void updatePlanet(float dtime);
    void createMonster(Matrix m,Matrix o,ConstantShader *pConstShader);
    void createLaser(int modelsNumber, Matrix m,ConstantShader *pConstShader);
    void collisionItem(float dtime);
    void createItems(int modelsNumber, Matrix m,Matrix o, ConstantShader *pConstShader);
    void updateItem(float dtime);
    void createBoss(Matrix m, Matrix o, ConstantShader *pConstShader);
    void updateMonster(float dtime);
    void createScene();
    void updateLaser(float dtime);
    void updateBoss(float dtime);
    void updateParticle(float dtime);
    void isBossHit();

    void shake(float dtime, int a, int b);

    void createBossLaser(Matrix m, Matrix o, ConstantShader *pConstShader);
    void collisionPlayer(float dtime);
};

#endif /* Application_hpp */
