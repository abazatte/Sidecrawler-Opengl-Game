//
// Created by aazat on 27.08.2022.
//

#ifndef CGPRAKT6_CLION_COLORA_H
#define CGPRAKT6_CLION_COLORA_H


class ColorA {
public:
    float R;
    float G;
    float B;
    float A;

    ColorA();
    ColorA( float r, float g, float b, float a);
    ColorA operator*(const ColorA& c) const;
    ColorA operator*(const float Factor) const;
    ColorA operator+(const ColorA& c) const;
    ColorA& operator+=(const ColorA& c);

};


#endif //CGPRAKT6_CLION_COLORA_H
