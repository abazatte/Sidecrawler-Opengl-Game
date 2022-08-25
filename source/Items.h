//
// Created by TimCirksena on 25.08.2022.
//

#ifndef CGPRAKT6_CLION_ITEMS_H
#define CGPRAKT6_CLION_ITEMS_H


#include "utils/Models/BaseModel.h"
#include <utils/Models/Model.h>

class Items : public BaseModel{
public:
    Items(const char* itemModel);
    virtual ~Items();
    bool loadModel(const char* itemModel);
    void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
    Model* getItem();
protected:
    Model* pItem;

};


#endif //CGPRAKT6_CLION_ITEMS_H
