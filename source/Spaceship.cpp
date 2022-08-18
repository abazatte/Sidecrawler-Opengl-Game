//
// Created by aazat on 18.08.2022.
//

#include <utils/Shader/PhongShader.h>
#include "../include/Spaceship.h"

Spaceship::Spaceship(const char* spaceshipFile)
{
    Model* gModel = new Model(spaceshipFile, false);
    gModel->shader(new PhongShader(), true);
    gModel->shadowCaster(false);

    pSpaceship = gModel;
}

Spaceship::~Spaceship()
{
    delete pSpaceship;
    // TODO: Add your code
}

bool Spaceship::loadModel(const char* spaceshipmodel)
{
    pSpaceship = new Model(spaceshipmodel, false);
    pSpaceship->shader(pShader, false);

    return true;
}

void Spaceship::steer(float upDown)
{
    this->m_upDown = upDown;
    //this->m_LeftRight = farward;
}

void Spaceship::aim(const Vector& Target)
{
    this->t = Target;
}

void Spaceship::update(float dtime)
{
    Matrix TM, CP;
    CP = pSpaceship->transform(); //Current Position
    //std::cout << dtime <<std::endl;
    //std::cout << m_upDown << std::endl;
    TM.translation(0, m_upDown * dtime,0);

    pSpaceship->transform(CP * TM);

    /*
    // Zur ausrichung an MausCoords
    top.lookAt(this->t, bottom.up(), bottom.translation());
    // Brauchen wir sonst ist Kanone und Mauspos verdreht
    top.invert();
    //Weil er sonst immer 90Grad nach links gucken würde
    rotM.rotationY(AI_DEG_TO_RAD(90));
    //Bewegung in vorwärts und rückwärts
    pSpaceship->transform(top * TM * rotM);
    */
}

void Spaceship::draw(const BaseCamera& Cam)
{
    pSpaceship->draw(Cam);
}

Model* Spaceship::getTop() {
    return pSpaceship;
}