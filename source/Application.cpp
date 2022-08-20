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


#ifdef WIN32
#define ASSET_DIRECTORY "../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow* pWin) : pWindow(pWin), cam(pWin), pModel(NULL), shadowGenerator(2048, 2048)
{

    createScene();
    //glfwSetKeyCallback(pWindow, key_callback);
    //createNormalTestScene();
    //createShadowTestScene();


}
void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime)
{

    int upState = glfwGetKey(pWindow, GLFW_KEY_W);
    int downState = glfwGetKey(pWindow, GLFW_KEY_S);
    int positionState = glfwGetKey(pWindow, GLFW_KEY_P);
    int fireState = glfwGetKey(pWindow, GLFW_KEY_SPACE);
    int shot = glfwGetKey(pWindow, GLFW_KEY_SPACE);
    //int forwardStat = glfwGetKey(pWindow, GLFW_KEY_D);
    //int backward = glfwGetKey(pWindow, GLFW_KEY_A);
    float upDown = 0.0f;
    //float forwardBackward = 0.0f;


    float f = pSpaceship->getTop()->transform().translation().Y;
    //std::cout << f << std:: endl;
    if (f < 13.5 && upState == GLFW_PRESS && downState == GLFW_RELEASE) {
        upDown = 17.5f;
    }
    else if (f > -12.4 && downState == GLFW_PRESS && upState == GLFW_RELEASE) {
        upDown = -17.5f;
    }
    if (positionState == GLFW_PRESS) {
        std::cout << "x: " << pSpaceship->getTop()->transform().translation().X << "y: " << pSpaceship->getTop()->transform().translation().Y << "z: " << pSpaceship->getTop()->transform().translation().Z << std::endl;
    }
    if (shot == GLFW_PRESS && laserTimer<0) {
        Matrix CP;
        laserTimer = 50;
        CP = pSpaceship->getTop()->transform();
        std::cout << pCurrentLaser << std::endl;
        if (pCurrentLaser >= 39) {
            pCurrentLaser = 0;
            laserModels.at(pCurrentLaser)->transform(CP);
            hitboxListLaser.at(pCurrentLaser)->transform(CP);


        } else {
            pCurrentLaser++;
            laserModels.at(pCurrentLaser)->transform(CP);
            hitboxListLaser.at(pCurrentLaser)->transform(CP);
        }
    }
    pSpaceship->steer(upDown);
    //pSpaceship->fire(dtime, shooting);
    pSpaceship->update(dtime);

    updateLaser(dtime);
    updateMonster(dtime);
    cam.update();
    if(laserTimer > -1000){
        laserTimer--;
    }

    std::cout << laserTimer << std::endl;
}
void Application::updateLaser(float dtime) {
    Matrix TM, CP;
    for (int (i) = 0; (i) < laserModels.size(); ++(i)) {
        CP = laserModels.at(i)->transform();
        if(laserModels.at(i)->transform().translation().Y > -13 && laserModels.at(i)->transform().translation().Y < 14 ){
            TM.translation(0, 0, 20.0f * dtime);
            laserModels.at(i)->transform(CP * TM);
            hitboxListLaser.at(i)->transform(CP * TM);

            std::cout << "Posi Laser:" << laserModels.at(i)->transform().translation().Z << std::endl;

        }
        if(laserModels.at(i)->transform().translation().Z > 90){
            TM.translation(0,-40,0);
            laserModels.at(i)->transform(TM);
            hitboxListLaser.at(i)->transform(TM);

        }
    }
}
void Application::updateMonster(float dtime){
    Matrix TM, CP;
    for (int i = 0; i < monsterModels.size(); ++i) {

        CP = monsterModels.at(i)->transform();
        TM.translation(0, 0, 1.0f * dtime);
        monsterModels.at(i)->transform(CP * TM);
        std::cout << "Posi Monster:" << monsterModels.at(i)->transform().translation().Z << std::endl;
        hitboxListMonster.at(i)->transform(CP * TM);
    }
}
void Application::loopCollision(){
    for (int i = 0; i < hitboxListLaser.size(); ++i) {

        for (int j = 0; j < hitboxListMonster.size(); ++j) {
            //hitboxListLaser.at(i)->boundingBox().
        }
    }
}


void Application::draw()
{
    shadowGenerator.generate(models);

    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderLightMapper::instance().activate();
    // 2. setup shaders and draw models
    for(ModelList::iterator it = models.begin(); it != models.end(); ++it )
    {
        (*it)->draw(cam);
    }
    ShaderLightMapper::instance().deactivate();

    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
    for(ModelList::iterator it = models.begin(); it != models.end(); ++it )
        delete *it;
    models.clear();
}

void Application::createScene()
{
    Matrix m, n, o;
    float rad;
    PhongShader* pPhongShader;
    int modelsNumber = 40;
    hitboxListLaser.reserve(sizeof(BaseModel) * modelsNumber);
    laserModels.reserve(sizeof(BaseModel) * modelsNumber);
    hitboxListMonster.reserve(sizeof(BaseModel) * modelsNumber);
    pCurrentLaser = 0;
    laserTimer = 50;
    pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
    pModel->shader(new PhongShader(), true);
    pModel->shadowCaster(false);
    models.push_back(pModel);


    pPhongShader = new PhongShader();
    pSpaceship = new Spaceship(ASSET_DIRECTORY "spaceship.dae");
    models.push_back(pSpaceship->getTop());

    Vector v1 = pSpaceship->getTop()->transform().translation();
    Vector v2 = pSpaceship->getTop()->transform().translation();
    v1.Z += 30;
    cam.setTarget(v1);
    //v1.Z -= 30;
    v1.X -= 25;
    cam.setPosition(v1);

    pModel = new Model(ASSET_DIRECTORY "spaceship.dae", false);
    pModel->shader(new PhongShader(), true);
    m.translation(0,0,40);
    pModel->shadowCaster(false);
    pModel->transform(m);
    models.push_back(pModel);
    //pSpaceship->loadModel(ASSET_DIRECTORY "spaceship.dae");

    // directional lights
    DirectionalLight* dl = new DirectionalLight();
    dl->direction(Vector(0.2f, -1, 1));
    dl->color(Color(1, 1, 1));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    for (int i = 0;i<modelsNumber;i++)
    {
        std::cout << i << " wird erstellt." << std::endl;
        pModel = new Model(ASSET_DIRECTORY "LaserTry.obj", false);
        pModel->shader(new PhongShader(), false);
        //pModel->shadowCaster(false);
        m.translation(0, -40, 0);
        pModel->transform(m);
        models.push_back(pModel);
        laserModels.push_back(pModel);

        ConstantShader* pConstShader;

        hitboxModel = new LineBoxModel(pModel->getBlockModel()->boundingBox().Max, pModel->getBlockModel()->boundingBox().Min);
        pConstShader = new ConstantShader();
        pConstShader->color(Color(0, 1, 0));
        hitboxModel->shader(pConstShader, true);
        hitboxModel->transform(m);
        models.push_back(hitboxModel);
        hitboxListLaser.push_back(hitboxModel);
    }
    for (int i = 0;i<15;i++){
        std::cout << i << ". Monster wird erstellt." << std::endl;
        pModel = new Model(ASSET_DIRECTORY "enemy.obj", false);
        pModel->shader(new PhongShader(), false);
        m.translation(0,0,15+15*i);
        n.scale(0.01f);
        o.rotationY(AI_DEG_TO_RAD(180.0f));
        pModel->transform(m*o);
        models.push_back(pModel);
        monsterModels.push_back(pModel);

        ConstantShader* pConstShader;

        hitboxModel = new LineBoxModel(pModel->getBlockModel()->boundingBox().Max, pModel->getBlockModel()->boundingBox().Min);
        pConstShader = new ConstantShader();
        pConstShader->color(Color(0, 1, 0));
        hitboxModel->shader(pConstShader, true);
        hitboxModel->transform(m*o);
        models.push_back(hitboxModel);
        hitboxListMonster.push_back(hitboxModel);
    }
}
