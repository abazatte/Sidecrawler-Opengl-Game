//
// Created by aazat on 28.08.2022.
//

#ifndef CGPRAKT6_CLION_PARTICLESHADER_H
#define CGPRAKT6_CLION_PARTICLESHADER_H

#include "utils/YourClasses/color.h"
#include "GL/glew.h"
#include "BaseShader.h"
#include "utils/YourClasses/ColorA.h"

#define PARTICLE_ANZAHL 200

class ParticleShader : public BaseShader
{
public:
    ParticleShader();
    void addMatrix(Matrix& mat, int index);
    void setColorA(const ColorA& color) { this->colorA = ColorA(color); }
    const ColorA& getColorA() { return colorA; }
    virtual void activate(const BaseCamera& Cam) const;


private:
    ColorA colorA;
    //locations der einzelnen uniforms
    GLint ColorLoc;
    GLint ViewProjLoc;
    GLint ModelMatLoc;
    //es muss ein inline float array verwendet werden, da ein array von matrizen erst umgewandelt werden müsste
    //bevor es an den glsl shader übergeben werden kann
    float modelTransforms[PARTICLE_ANZAHL * 16];
    //man könnte auch ein color array machen für jedes partikel
};

#endif //CGPRAKT6_CLION_PARTICLESHADER_H
