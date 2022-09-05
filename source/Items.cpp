//
// Created by TimCirksena on 25.08.2022.
//

#include "Items.h"
#include "utils/Shader/PhongShader.h"
#include "../include/Application.h"


Items::Items() {
    this->setItemType((int) Application::randomFloat(0.0f,4.0f));
    std::cout << getType() << std::endl;
    Matrix RY, TM;
    if(getType() == 0){
        pItem = new Model("../assets/item1.obj", false);
        pItem->shader(new PhongShader(), true);
        pItem->shadowCaster(false);
        TM.translation(0, -60, 0);
        RY.rotationY(AI_DEG_TO_RAD(180.0f));
        pItem->transform(TM * RY);
    }else if(getType() == 1){
        pItem = new Model("../assets/item2.obj", false);
        pItem->shader(new PhongShader(), true);
        pItem->shadowCaster(false);
        TM.translation(0, -60, 0);
        RY.rotationY(AI_DEG_TO_RAD(180.0f));
        pItem->transform(TM * RY);
    }else{
        pItem = new Model("../assets/item3.obj", false);
        pItem->shader(new PhongShader(), true);
        pItem->shadowCaster(false);
        TM.translation(0, -60, 0);
        RY.rotationY(AI_DEG_TO_RAD(180.0f));
        pItem->transform(TM * RY);
    }


}
Items::~Items() {
    delete pItem;
}

void Items::setItemType(int itemType) {
    Items::itemType = itemType;
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
        TM.translation(0,0,-10.0f  * dtime);

    }
    this->getItem()->transform(CP * TM);
    this->getItem()->boundingBox().transform(CP * TM);
}

void Items::draw(const BaseCamera &Cam) {
    pItem->draw(Cam);
}

Model *Items::getItem() {
    return pItem;
}
int Items::getType() {
    return this->itemType;
}
