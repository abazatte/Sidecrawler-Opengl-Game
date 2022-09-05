//
// Created by aazat on 26.08.2022.
//

#ifndef CGPRAKT6_CLION_PARTICLESYSTEM_H
#define CGPRAKT6_CLION_PARTICLESYSTEM_H

#include <utils/YourClasses/vector.h>
#include <utils/YourClasses/color.h>
#include <vector>
#include <utils/YourClasses/ColorA.h>
#include <ParticleProps.h>
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class ParticleSystem : public BaseModel {
public:
    ParticleSystem();

    void update(float dtime);

    virtual void draw(const BaseCamera &Cam);

    void emit(const ParticleProps &particleProps);

    static ColorA& randomColor();

    ParticleProps createExampleProps();

protected:
    void createParticleModel();
    struct Particle {
        Vector position;
        Vector velocity;
        ColorA colorBegin, colorEnd;
        Vector rotation;
        Vector rotationSpeed;

        float sizeBegin, sizeEnd;
        float lifeTime = 1.0f;
        float lifeRemaining = 0.0f;
    };
    std::vector<Particle> pParticlePool;
    uint32_t pPoolIndex = 199;
    GLuint pQuadVA = 0;
    //Shader

    GLint pParticleShaderViewProj, pParticleShaderTransform, pParticleShaderColor;
    VertexBuffer VB;
    IndexBuffer IB;
};


#endif //CGPRAKT6_CLION_PARTICLESYSTEM_H
