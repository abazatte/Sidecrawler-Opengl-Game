//
// Created by aazat on 24.08.2022.
//

#ifndef CGPRAKT6_CLION_ENEMY_H
#define CGPRAKT6_CLION_ENEMY_H


#include <utils/Models/BaseModel.h>
#include <utils/Models/Model.h>

class Enemy : public BaseModel {
public:
    Enemy();

    Enemy(const char* enemyModel);
    virtual ~Enemy();
    bool loadModel(const char* enemyModel);
    void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
    Model* getEnemy();
    int getPSkill();
    int getPObenUnten();
    float getPVorher();
    int getPLeben();

    void setPSkill(int pSkill);
    void setPObenUnten(int pObenUnten);
    void setPVorher(float pVorher);
    void setPLeben(int pLeben);
protected:
    bool bossStatus = false;
public:
    bool isBossStatus() const;

    void setBossStatus(bool bossStatus);

protected:
    Model* pEnemy;
    int pSkill;
    int pObenUnten;
    float pVorher;
    int pLeben;
};


#endif //CGPRAKT6_CLION_ENEMY_H
