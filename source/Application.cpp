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


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048)
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
    if (shot == GLFW_PRESS) {
        Matrix CP;
        CP = pSpaceship->getTop()->transform();
        std::cout << pCurrentLaser << std::endl;
        if (pCurrentLaser >= 14) {
            pCurrentLaser = 0;
            LaserModels.at(pCurrentLaser)->transform(CP);
        }
        else {
            pCurrentLaser++;
            LaserModels.at(pCurrentLaser)->transform(CP);
        }
    }

    /*if (forwardStat == GLFW_PRESS && backward == GLFW_RELEASE) {
        std::cout << "vorn" << std::endl;
        forwardBackward = 10.0f;
    }
    else if (backward == GLFW_PRESS && forwardStat == GLFW_RELEASE) {
        std::cout << "hinten" << std::endl;
        forwardBackward = -10.0f;
    }*/


    pSpaceship->steer(upDown);
    //pSpaceship->fire(dtime, shooting);
    pSpaceship->update(dtime);
    updateLaser(dtime);
    GLFW
    Cam.update();
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        Matrix CP;
        CP = pSpaceship->getTop()->transform();
        std::cout << pCurrentLaser << std::endl;
        if (pCurrentLaser >= 14) {
            pCurrentLaser = 0;
            LaserModels.at(pCurrentLaser)->transform(CP);
        }
        else {
            pCurrentLaser++;
            LaserModels.at(pCurrentLaser)->transform(CP);
        }
    }
}

void Application::updateLaser(float dtime) {
    Matrix TM, CP;
    for (BaseModel * laser : LaserModels)
    {
        CP = laser->transform();
        TM.translation(0, 0, 10.0f * dtime);
        laser->transform(CP * TM);
    }
}

void Application::draw()
{
    ShadowGenerator.generate(Models);

    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderLightMapper::instance().activate();
    // 2. setup shaders and draw models
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
    ShaderLightMapper::instance().deactivate();

    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;

    Models.clear();
}

void Application::createScene()
{
    Matrix m, n, o;
    float rad;
    PhongShader* pPhongShader;

    pCurrentLaser = 0;

    pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
    pModel->shader(new PhongShader(), true);
    pModel->shadowCaster(false);
    Models.push_back(pModel);


    pPhongShader = new PhongShader();
    pSpaceship = new Spaceship(ASSET_DIRECTORY "spaceship.dae");
    Models.push_back(pSpaceship->getTop());

    Vector v1 = pSpaceship->getTop()->transform().translation();
    Vector v2 = pSpaceship->getTop()->transform().translation();
    v1.Z += 30;
    Cam.setTarget(v1);
    //v1.Z -= 30;
    v1.X -= 25;
    Cam.setPosition(v1);

    pModel = new Model(ASSET_DIRECTORY "spaceship.dae", false);
    pModel->shader(new PhongShader(), true);
    m.translation(0,0,40);
    pModel->shadowCaster(false);
    pModel->transform(m);
    Models.push_back(pModel);
    //pSpaceship->loadModel(ASSET_DIRECTORY "spaceship.dae");

    /*
    pSpaceship->shader(pPhongShader, true);

    m.translation(-20, 0, -10);
    rad = AI_DEG_TO_RAD(90);
    n.rotationX(rad);
    o.rotationY(rad);
    pSpaceship->transform(m * n * o);

    */


    // directional lights
    DirectionalLight* dl = new DirectionalLight();
    dl->direction(Vector(0.2f, -1, 1));
    dl->color(Color(1, 1, 1));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    for (int i = 0;i<15;i++)
    {
        std::cout << i << " wird erstellt." << std::endl;
        pModel = new Model(ASSET_DIRECTORY "laser.dae", false);
        pModel->shader(new PhongShader(), false);
        //pModel->shadowCaster(false);
        m.translation(0, -40, 0);
        pModel->transform(m);
        Models.push_back(pModel);
        LaserModels.push_back(pModel);
    }
}

void Application::createNormalTestScene()
{
    pModel = new LinePlaneModel(10, 10, 10, 10);
    ConstantShader* pConstShader = new ConstantShader();
    pConstShader->color(Color(0, 0, 0));
    pModel->shader(pConstShader, true);
    // add to render list
    Models.push_back(pModel);


    pModel = new Model(ASSET_DIRECTORY "cube.obj", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);


}

void Application::createShadowTestScene()
{
    pModel = new Model(ASSET_DIRECTORY "shadowcube.obj", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);

    pModel = new Model(ASSET_DIRECTORY "bunny.dae", false);
    pModel->shader(new PhongShader(), true);
    Models.push_back(pModel);

    // directional lights
    DirectionalLight* dl = new DirectionalLight();
    dl->direction(Vector(0, -1, -1));
    dl->color(Color(0.5, 0.5, 0.5));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    SpotLight* sl = new SpotLight();
    sl->position(Vector(2, 2, 0));
    sl->color(Color(0.5, 0.5, 0.5));
    sl->direction(Vector(-1, -1, 0));
    sl->innerRadius(10);
    sl->outerRadius(13);
    sl->castShadows(true);
    ShaderLightMapper::instance().addLight(sl);
}