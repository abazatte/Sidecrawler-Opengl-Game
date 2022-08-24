//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "../include/Application.h"

#ifdef WIN32

#include <GL/glew.h>
#include <glfw/glfw3.h>

#define _USE_MATH_DEFINES

#include <math.h>

#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif

#include "utils/Models/LinePlaneModel.h"
#include "utils/Models/TrianglePlaneModel.h"
#include "utils/Models/TriangleSphereModel.h"
#include "utils/Models/LineBoxModel.h"
#include "utils/Models/TriangleBoxModel.h"
#include "utils/Models/Model.h"
#include "utils/Lights/ShaderLightMapper.h"
#include "utils/Shader/PhongShader.h"



//freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../include/Enemy.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow *pWin) : pWindow(pWin), cam(pWin), pModel(NULL), shadowGenerator(2048, 2048) {

    createScene();

    //glfwSetKeyCallback(pWindow, key_callback);
    //createNormalTestScene();
    //createShadowTestScene();


}

void Application::start() {
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime) {

    int upState = glfwGetKey(pWindow, GLFW_KEY_W);
    int downState = glfwGetKey(pWindow, GLFW_KEY_S);
    int positionState = glfwGetKey(pWindow, GLFW_KEY_P);
    int fireState = glfwGetKey(pWindow, GLFW_KEY_SPACE);
    int shot = glfwGetKey(pWindow, GLFW_KEY_SPACE);
    //int forwardStat = glfwGetKey(pWindow, GLFW_KEY_D);
    //int backward = glfwGetKey(pWindow, GLFW_KEY_A);
    float upDown = 0.0f;
    //float forwardBackward = 0.0f;
    // std::cout << dtime << std::endl;

    float f = pSpaceship->getTop()->transform().translation().Y;
    //std::cout << f << std:: endl;
    if (f < 13.5 && upState == GLFW_PRESS && downState == GLFW_RELEASE) {
        upDown = 17.5f;
    } else if (f > -12.4 && downState == GLFW_PRESS && upState == GLFW_RELEASE) {
        upDown = -17.5f;
    }
    if (positionState == GLFW_PRESS) {
        //std::cout << "x: " << pSpaceship->getTop()->transform().translation().X << "y: " << pSpaceship->getTop()->transform().translation().Y << "z: " << pSpaceship->getTop()->transform().translation().Z << std::endl;
    }
    if (shot == GLFW_PRESS && laserTimer <= 0) {
        Matrix CP;
        laserTimer = 1;
        CP = pSpaceship->getTop()->transform();
        //std::cout << pCurrentLaser << std::endl;
        if (pCurrentLaser >= laserModels.size() - 1) {
            pCurrentLaser = 0;
            laserModels.at(pCurrentLaser)->transform(CP);
            //hitboxListLaser.at(pCurrentLaser)->transform(CP);


        } else {
            pCurrentLaser++;
            laserModels.at(pCurrentLaser)->transform(CP);
            //hitboxListLaser.at(pCurrentLaser)->transform(CP);
        }
    }
    if (monsterTimer <= 0) {
        Matrix CP;
        monsterTimer = 10;
        float tmp = randomFloat(12, -12);
        CP.translation(0, tmp, 90);
        if (pCurrentMonster >= monsterModels.size() - 1) {
            pCurrentMonster = 0;
            monsterModels.at(pCurrentMonster)->getEnemy()->transform(CP);
            hitboxListMonster.at(pCurrentMonster)->transform(CP);
        } else {
            pCurrentMonster++;
            monsterModels.at(pCurrentMonster)->getEnemy()->transform(CP);
            hitboxListMonster.at(pCurrentMonster)->transform(CP);
        }
    }
    pSpaceship->steer(upDown);
    pSpaceship->update(dtime);
    spaceship->transform(pSpaceship->getTop()->transform());

    updateLaser(dtime);
    updateMonster(dtime);
    updatePlanet(dtime);
    loopCollision();
    cam.update();
    if (laserTimer > -1000) {
        laserTimer = laserTimer - (dtime * 4);
    }

    if (monsterTimer > -1000) {
        monsterTimer = monsterTimer - (dtime * 4);
    }
    //std::cout << laserTimer << std::endl;
}

void Application::updateLaser(float dtime) {
    Matrix TM, CP;
    for (int (i) = 0; (i) < laserModels.size(); ++(i)) {
        CP = laserModels.at(i)->transform();
        if (laserModels.at(i)->transform().translation().Y > -13 &&
            laserModels.at(i)->transform().translation().Y < 14) {
            TM.translation(0, 0, 20.0f * dtime);
            laserModels.at(i)->transform(CP * TM);
            //hitboxListLaser.at(i)->transform(CP * TM);

            //std::cout << "Posi Laser:" << laserModels.at(i)->transform().translation().Z << std::endl;

        }
        if (laserModels.at(i)->transform().translation().Z > 90) {
            TM.translation(0, -40, 0);
            laserModels.at(i)->transform(TM);
            //hitboxListLaser.at(i)->transform(TM);

        }
    }
}

void Application::updateMonster(float dtime) {
    Matrix TM, CP;

    for (int i = 0; i < monsterModels.size(); ++i) {
            //TM.translation(0, 0, -10.0f * dtime * pGeschwindigkeit.at(i)); //1.0f * dtime
            monsterModels.at(i)->update(dtime);
            //std::cout << "Posi Monster:" << monsterModels.at(i)->transform().translation().Z << std::endl;
            hitboxListMonster.at(i)->transform(monsterModels.at(i)->getBlockModel()->transform());
            //std::cout << "Posi Laser:" << laserModels.at(i)->transform().translation().Z << std::endl;
    }
}

void Application::loopCollision() {
    Matrix TM, CP, CP2, CP3;
    for (int i = 0; i < laserModels.size(); ++i) {
        for (int j = 0; j < monsterModels.size(); ++j) {
            CP = laserModels.at(i)->transform();
            CP2 = monsterModels.at(j)->getEnemy()->transform();
            CP3 = pSpaceship->getTop()->transform();
            if (AABB::collision(pSpaceship->boundingBox().transform(CP3),
                                monsterModels.at(j)->getEnemy()->boundingBox().transform(CP2))) {
                TM.translation(0, -50, 0);
                monsterModels.at(j)->getEnemy()->transform(TM);
                hitboxListMonster.at(j)->transform(TM);
                //std::cout<< "du wurdest getroffen von Monster Nummer: " << j << std::endl;
                score = 0;

            }
            if (AABB::collision(laserModels.at(i)->boundingBox().transform(CP),
                                monsterModels.at(j)->getEnemy()->boundingBox().transform(CP2))) {
                if (monsterModels.at(j)->getPLeben() == 0) {
                    TM.translation(0, -40, 0);
                    laserModels.at(i)->transform(TM);
                    //hitboxListLaser.at(i)->transform(TM);
                    TM.translation(0, -50, 0);
                    monsterModels.at(j)->getEnemy()->transform(TM);
                    hitboxListMonster.at(j)->transform(TM);
                    score++;
                } else {
                    TM.translation(0, -40, 0);
                    laserModels.at(i)->transform(TM);
                    //hitboxListLaser.at(i)->transform(TM);
                    monsterModels.at(j)->setPLeben(monsterModels.at(j)->getPLeben()-1);
                }

                std::cout << score << std::endl;
            }
        }
    }
}

void Application::updatePlanet(float dtime) {
    Matrix TM, CP, CP2, N, R;
    N.scale(2);
    R.rotationY(AI_DEG_TO_RAD(-1.0f) * dtime);
    CP = venus->transform();
    CP2 = earth->transform();
    TM.translation(0, 0, 10.0f * dtime);
    venus->transform(R * CP * TM * R);
    TM.translation(0, 0, 5.0f * dtime);
    earth->transform(R * CP2 * TM * R);
    if (venus->transform().translation().Z > 200) {
        TM.translation(50, 0, -100);
        venus->transform(R * TM * N * R);
    } else if (earth->transform().translation().Z > 150) {
        TM.translation(60, 10, -110);
        earth->transform(R * TM * N * R);
    }
}

/** https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats **/
float Application::randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


void Application::draw() {
    shadowGenerator.generate(models);

    // 1. clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderLightMapper::instance().activate();
    // 2. setup shaders and draw models
    for (ModelList::iterator it = models.begin(); it != models.end(); ++it) {
        (*it)->draw(cam);
    }
    ShaderLightMapper::instance().deactivate();

    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error == 0);
}

void Application::end() {
    for (ModelList::iterator it = models.begin(); it != models.end(); ++it)
        delete *it;

    models.clear();
}

void Application::createScene() {
    Matrix m, n, o;
    float rad;
    PhongShader *pPhongShader;
    int modelsNumber = 40;
    hitboxListLaser.reserve(sizeof(BaseModel) * modelsNumber);
    laserModels.reserve(sizeof(BaseModel) * modelsNumber);
    hitboxListMonster.reserve(sizeof(BaseModel) * modelsNumber);
    pCurrentLaser = 0;
    laserTimer = 0;
    monsterTimer = 3;
    score = 0;

    pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
    pModel->shader(new PhongShader(), true);
    pModel->shadowCaster(false);
    n.scale(2.0f);
    pModel->transform(n);
    models.push_back(pModel);
    pPhongShader = new PhongShader();

    pSpaceship = new Spaceship(ASSET_DIRECTORY "woodenObje.obj");
    models.push_back(pSpaceship->getTop());
    Vector v1 = pSpaceship->getTop()->transform().translation();
    v1.Z += 30;
    cam.setTarget(v1);
    //v1.Z -= 30;
    v1.X -= 25;
    cam.setPosition(v1);
    ConstantShader *pConstShader;
    spaceship = new LineBoxModel(pSpaceship->getTop()->getBlockModel()->boundingBox().Min,
                                 pSpaceship->getTop()->getBlockModel()->boundingBox().Max);
    pConstShader = new ConstantShader();
    pConstShader->color(Color(0, 1, 0));
    spaceship->shader(pConstShader, true);
    models.push_back(spaceship);

    venus = new Model(ASSET_DIRECTORY "Venus_1K.obj", false);
    venus->shader(new PhongShader(), true);
    m.translation(50, 0, 0);
    n.scale(10);
    venus->shadowCaster(false);
    venus->transform(m * n);
    models.push_back(venus);

    earth = new Model(ASSET_DIRECTORY "Earth_2K.obj", false);
    earth->shader(new PhongShader(), true);
    m.translation(60, 10, 40);
    n.scale(10);
    earth->shadowCaster(false);
    earth->transform(m * n);
    models.push_back(earth);
    //pSpaceship->loadModel(ASSET_DIRECTORY "spaceship.dae");
/*
    pModel = new Model(ASSET_DIRECTORY "Mars_2k.obj", false);
    pModel->shader(new PhongShader(),true);
    pModel->shadowCaster(false);
    m.translation(50.0f, 0.0f, 0.0f);
    pModel->transform(m);
    models.push_back(pModel);
*/
    // directional lights
    DirectionalLight *dl = new DirectionalLight();
    dl->direction(Vector(0.2f, -1, 1));
    dl->color(Color(1, 1, 1));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    for (int i = 0; i < modelsNumber; i++) {
        std::cout << i << " wird erstellt." << std::endl;
        pModel = new Model(ASSET_DIRECTORY "LaserTry.obj", false);
        pModel->shader(new PhongShader(), true);
        //pModel->shadowCaster(false);
        m.translation(0, -40, 0);
        pModel->transform(m);
        models.push_back(pModel);
        laserModels.push_back(pModel);

        hitboxModel = new LineBoxModel(pModel->getBlockModel()->boundingBox().Max,
                                       pModel->getBlockModel()->boundingBox().Min);
        pConstShader = new ConstantShader();
        pConstShader->color(Color(0, 1, 0));
        hitboxModel->shader(pConstShader, true);
        hitboxModel->transform(m);
        models.push_back(hitboxModel);
        hitboxListLaser.push_back(hitboxModel);
    }
    for (int i = 0; i < 10; i++) {
        std::cout << i << ". Monster wird erstellt." << std::endl;
        pEnemy = new Enemy(ASSET_DIRECTORY "enemy.obj");
        m.translation(0, -50, 0);
        o.rotationY(AI_DEG_TO_RAD(180.0f));
        pEnemy->getEnemy()->transform(m * o);

        pEnemy->setPSkill((int) randomFloat(0.0f, 3.0f));
        pEnemy->setPObenUnten((int) randomFloat(0.0f, 2.0f));
        pEnemy->setPVorher(5.25f);
        pEnemy->setPLeben((int) randomFloat(1.0f, 4.0f));

        models.push_back(pEnemy->getEnemy());
        monsterModels.push_back(pEnemy);


        ConstantShader *pConstShader;
        hitboxModel = new LineBoxModel(pEnemy->getBlockModel()->boundingBox().Max,
                                       pEnemy->getBlockModel()->boundingBox().Min);
        pConstShader = new ConstantShader();
        pConstShader->color(Color(0, 1, 0));
        hitboxModel->shader(pConstShader, true);
        hitboxModel->transform(m * o);
        models.push_back(hitboxModel);
        hitboxListMonster.push_back(hitboxModel);
    }
}
