//
// Created by aazat on 27.08.2022.
//

#include <ostream>
#include "ColorA.h"

ColorA::ColorA() {
    this->R = 0.0f;
    this->G = 0.0f;
    this->B = 0.0f;
    this->A = 0.0f;
}

ColorA::ColorA(float r, float g, float b, float a): R(r), G(g), B(b), A(a) {
    if (R < 0.0f) {
        R = 0.0f;
    } else if ( R > 1.0f) {
        R = 1.0f;
    }
    if (G < 0.0f) {
        G = 0.0f;
    } else if ( G > 1.0f) {
        G = 1.0f;
    }
    if (B < 0.0f) {
        B = 0.0f;
    } else if ( B > 1.0f) {
        B = 1.0f;
    }
    if (A < 0.0f) {
        A = 0.0f;
    } else if ( A > 1.0f) {
        A = 1.0f;
    }

}

ColorA ColorA::operator*(const ColorA &c) const {
    ColorA erg;
    erg.R = this->R * c.R;
    erg.G = this->G * c.G;
    erg.B = this->B * c.B;
    erg.A = this->A * c.A;
    return erg;
}

ColorA ColorA::operator*(const float Factor) const {
    ColorA erg;
    erg.R = this->R * Factor;
    erg.G = this->G * Factor;
    erg.B = this->B * Factor;
    erg.A = this->A * Factor;
    return erg;
}

ColorA ColorA::operator+(const ColorA &c) const {
    ColorA erg;
    erg.R = this->R + c.R;
    erg.G = this->G + c.G;
    erg.B = this->B + c.B;
    erg.A = this->A + c.A;
    return erg;
}

ColorA &ColorA::operator+=(const ColorA &c) {
    this->R += c.R;
    this->G += c.G;
    this->B += c.B;
    this->A += c.A;

    return *this;
}

std::ostream &operator<<(std::ostream &s, const ColorA &out) {
    s << "R: " << out.R << std::endl
      << "G: " << out.G << std::endl
      << "B: " << out.B << std::endl
      << "A: " << out.A << std::endl;

    return s;
}