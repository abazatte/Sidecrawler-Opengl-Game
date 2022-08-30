//
// Created by aazat on 30.08.2022.
//

#include "ParticleProps.h"

ParticleProps::ParticleProps() {
    position = Vector(0, 0, 0);
    velocity = Vector(-1, 0.3f, 0);
    velocityVariation = Vector(0.2f, 0.3f, 0.2f);
    rotation = Vector(0, 0, 0);
    rotationSpeed = Vector(2.0f*3.14f, 0, 0);
    colorBegin = ColorA(1, 0, 0, 1);
    colorEnd = ColorA(0, 0, 1, 0.2f);
    sizeBegin = 0.5f;
    sizeEnd = 0.001f;
    sizeVariation = 0.1f;
    lifeTime = 1.0f;
}
