//
// Created by aazat on 27.08.2022.
//

#include "Utils.h"

//Quelle: https://stackoverflow.com/questions/4353525/floating-point-linear-interpolation
float Utils::lerp(float a, float b, float f) {
    return (a * (1.0f - f)) + (b * f);
}
