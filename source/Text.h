//
// Created by TimCirksena on 22.08.2022.
//

#ifndef CGPRAKT6_CLION_TEXT_H
#define CGPRAKT6_CLION_TEXT_H

#include <string>

namespace tart {

    struct Text {
        std::string text{""};
        BaseModel x {0.0};
        Tart_real y {0.0};
        Tart_real z {0.0};
        Tart_real scale {0.0};
    };

}
#endif //CGPRAKT6_CLION_TEXT_H
