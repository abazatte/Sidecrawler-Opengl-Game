//
// Created by aazat on 18.08.2022.
//

#ifndef CGPRAKT6_CLION_SPACESHIP_H
#define CGPRAKT6_CLION_SPACESHIP_H


#include <utils/Models/BaseModel.h>
#include <utils/Models/Model.h>

class Spaceship : public BaseModel
{
public:
    Spaceship();
    Spaceship(const char* spaceshipModel);
    virtual ~Spaceship();
    bool loadModel(const char* spaceshipmodel);
    void steer(float ForwardBackward);
    void aim(const Vector& Target);
    void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
    Model* getTop();
protected:
    Model* pSpaceship;
    float m_LeftRight;
    float m_upDown;
    float m_Boost;
    Vector t;
};


#endif //CGPRAKT6_CLION_SPACESHIP_H
