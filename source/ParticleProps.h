//
// Created by aazat on 30.08.2022.
//

#ifndef CGPRAKT6_CLION_PARTICLEPROPS_H
#define CGPRAKT6_CLION_PARTICLEPROPS_H


#include <utils/YourClasses/vector.h>
#include <utils/YourClasses/ColorA.h>

class ParticleProps {
public:
    Vector position;
    Vector velocity, velocityVariation;
    Vector rotation;
    Vector rotationSpeed;
    ColorA colorBegin, colorEnd;

    float sizeBegin, sizeEnd, sizeVariation;
    float lifeTime = 1.0f;

    ParticleProps();
};


#endif //CGPRAKT6_CLION_PARTICLEPROPS_H
