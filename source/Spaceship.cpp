//
// Created by aazat on 18.08.2022.
//

#include <utils/Shader/PhongShader.h>
#include "../include/Spaceship.h"

Spaceship::Spaceship(const char* spaceshipFile, const char* boundingFile)
{
    pSpaceship = new Model(spaceshipFile, false);
    pSpaceship->shader(new PhongShader(), true);
    pSpaceship->shadowCaster(false);

    bounding = new Model(boundingFile, false);
    bounding->shader(new PhongShader(), true);
    bounding->shadowCaster(false);

    //die Bounding bisschen nach vorne bringen
    Matrix TM, CP, S;
    CP = bounding->transform();
    TM.translation(Vector(0,0.5f,2.5f));
    S.scale(Vector(1,1,1.5f));
    bounding->transform(CP * TM * S);
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
    Matrix TM, CP, CP1;
    CP = pSpaceship->transform(); //Current Position
    //std::cout << dtime <<std::endl;
    //std::cout << m_upDown << std::endl;
    TM.translation(0, m_upDown * dtime,0);

    pSpaceship->transform(CP * TM);
    //S.scale(Vector(1,1,3));
    CP1 = bounding->transform();
    bounding->transform(CP1 * TM);
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

Model* Spaceship::getBounding() {
    return bounding;
}

Model *Spaceship::getSpaceShip() {
    return pSpaceship;
}
