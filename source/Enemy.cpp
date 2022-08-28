//
// Created by aazat on 24.08.2022.
//

#include <utils/Shader/PhongShader.h>
#include "../include/Enemy.h"
#include "../include/Application.h"

Enemy::Enemy(const char *enemyModel) {
    pEnemy = new Model(enemyModel, false);
    pEnemy->shader(new PhongShader(), true);
    pEnemy->shadowCaster(false);

    //Geschwindigkeit
    this->setPSkill((int) Application::randomFloat(0.0f, 3.0f));
    //Hoch runter
    this->setPObenUnten((int) Application::randomFloat(0.0f, 2.0f));
    //Wenn man hoch runter geht
    this->setPVorher(5.25f);
    //Wie viele Hits ein Enemy braucht zum sterben
    this->setPLeben((int) Application::randomFloat(1.0f, 4.0f));
}
Enemy::Enemy() {
    pEnemy = new Model("../assets/untitled.obj", false);
    pEnemy->shader(new PhongShader(), true);
    pEnemy->shadowCaster(false);
    this->setPLeben(30);
}

Enemy::~Enemy() {
    delete pEnemy;
}

bool Enemy::loadModel(const char *enemyModel) {
    pEnemy = new Model(enemyModel, false);
    pEnemy->shader(pShader, false);

    return true;
}

void Enemy::update(float dtime) {
    Matrix TM, CP;
    CP = this->getEnemy()->transform();
    if (this->getEnemy()->transform().translation().Y > -13 &&
        this->getEnemy()->transform().translation().Y < 14) {
        if (this->getPSkill() == 0) {
            if (this->getPObenUnten() == 0) {
                TM.translation(0, 0, -10.0f * dtime); //1.0f * dtime
            } else if (this->getPObenUnten() == 1) {
                if (this->getEnemy()->transform().translation().Y < -11) {
                    TM.translation(0, 5.25f * dtime, -10.0f * dtime); //1.0f * dtime
                    this->setPVorher(5.25f);
                } else if (this->getEnemy()->transform().translation().Y > 12) {
                    TM.translation(0, -5.25f * dtime, -10.0f * dtime); //1.0f * dtime
                    this->setPVorher(-5.25f);
                } else {
                    TM.translation(0, this->getPVorher() * dtime, -10.0f * dtime); //1.0f * dtime
                }
            }
        } else if (this->getPSkill() == 1) {
            if (this->getPObenUnten() == 0) {
                TM.translation(0, 0, -10.0f * dtime * 2); //1.0f * dtime
            } else if (this->getPObenUnten() == 1) {
                if (this->getEnemy()->transform().translation().Y < -11) {
                    TM.translation(0, 5.0f * dtime, -10.0f * dtime * 2); //1.0f * dtime
                    this->setPVorher(5.25f);
                } else if (this->getEnemy()->transform().translation().Y > 12) {
                    TM.translation(0, -5.0f * dtime, -10.0f * dtime * 2); //1.0f * dtime
                    this->setPVorher(-5.25f);
                } else {
                    TM.translation(0, this->getPVorher() * dtime, -10.0f * dtime * 2); //1.0f * dtime
                }
            }
        } else if (this->getPSkill() == 2) {
            if (this->getPObenUnten() == 0) {
                TM.translation(0, 0, -10.0f * dtime * 3); //1.0f * dtime
            } else if (this->getPObenUnten() == 1) {
                if (this->getEnemy()->transform().translation().Y < -11) {
                    TM.translation(0, 5.0f * dtime, -10.0f * dtime * 3); //1.0f * dtime
                    this->setPVorher(5.25f);
                } else if (this->getEnemy()->transform().translation().Y > 12) {
                    TM.translation(0, -5.0f * dtime, -10.0f * dtime * 3); //1.0f * dtime
                    this->setPVorher(-5.25f);
                } else {
                    TM.translation(0, this->getPVorher() * dtime, -10.0f * dtime * 3); //1.0f * dtime
                }
            }
        }
        this->getEnemy()->transform(CP * TM);
    }
}

void Enemy::draw(const BaseCamera &Cam) {
    pEnemy->draw(Cam);
}

Model *Enemy::getEnemy() {
    return pEnemy;
}

int Enemy::getPSkill() {
    return pSkill;
}

int Enemy::getPObenUnten() {
    return pObenUnten;
}

float Enemy::getPVorher() {
    return pVorher;
}

int Enemy::getPLeben() {
    return pLeben;
}

void Enemy::setPSkill(int pSkill) {
    this->pSkill = pSkill;
}

void Enemy::setPObenUnten(int pObenUnten) {
    this->pObenUnten = pObenUnten;
}

void Enemy::setPVorher(float pVorher) {
    this->pVorher = pVorher;
}

void Enemy::setPLeben(int pLeben) {
    this->pLeben = pLeben;
}

bool Enemy::isBossStatus() const {
    return bossStatus;
}

void Enemy::setBossStatus(bool bossStatus) {
    Enemy::bossStatus = bossStatus;
}
