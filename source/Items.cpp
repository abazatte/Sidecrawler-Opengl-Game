//
// Created by TimCirksena on 25.08.2022.
//

#include "Items.h"
#include "utils/Shader/PhongShader.h"


Items::Items(const char *itemModel) {
    pItem = new Model(itemModel, false);
    pItem->shader(new PhongShader(), true);
    pItem->shadowCaster(false);
}
Items::~Items() {
    delete pItem;
}


bool Items::loadModel(const char *itemModel) {
    pItem = new Model(itemModel, false);
    pItem->shader(pShader, false);

    return true;
}

void Items::update(float dtime) {
    Matrix TM, CP;
    CP = this->getItem()->transform();
    if(this->getItem()->transform().translation().Y > -13 && this->getItem()->transform().translation().Y < 14){

    }
}

void Items::draw(const BaseCamera &Cam) {
    pItem->draw(Cam);
}

Model *Items::getItem() {
    return pItem;
}
