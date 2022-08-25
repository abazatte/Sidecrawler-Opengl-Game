//
// Created by TimCirksena on 24.08.2022.
//

#ifndef CGPRAKT6_CLION_FREETYPE_H
#define CGPRAKT6_CLION_FREETYPE_H


#include <map>
#include "utils/YourClasses/vector.h"

class FreeType {
public:
    FreeType();
    bool look();
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        Vector   Size;       // Size of glyph
        Vector   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };
    std::map<char, Character> Characters;
};


#endif //CGPRAKT6_CLION_FREETYPE_H
