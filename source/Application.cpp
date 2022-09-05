//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "../include/Application.h"

#ifdef WIN32

#include <GL/glew.h>
#include <glfw/glfw3.h>

#define _USE_MATH_DEFINES

#include <cmath>

#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
//#include <glfw/glfw3.h>
#endif

#include "utils/Models/LinePlaneModel.h"
#include "utils/Models/TrianglePlaneModel.h"
#include "utils/Models/TriangleSphereModel.h"
#include "utils/Models/LineBoxModel.h"
#include "utils/Models/TriangleBoxModel.h"
#include "utils/Models/Model.h"
#include "utils/Lights/ShaderLightMapper.h"
#include "utils/Shader/PhongShader.h"



//freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../include/Enemy.h"
#include "utils/Shader/ParticleShader.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow *pWin) : pWindow(pWin), cam(pWin), pModel(NULL), shadowGenerator(2048, 2048) {

    createScene();


    //glfwSetKeyCallback(pWindow, key_callback);
    //createNormalTestScene();
    //createShadowTestScene();


}

void Application::start() {
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime) {
    /**     Camfahrt      **/
    /**                   **/
    /**                   **/
    /**                   **/
    /*if (!game) {
        if (cam.position().X <= -25) {
            Vector v1 = pSpaceship->getTop()->transform().translation();
            v1.Z += 30;
            cam.setTarget(v1);
            cam.setPosition(camUrsprung);
            game = true;
        } else {
            Vector trans(cam.position().X - (dtime),0,cam.position().Z);
            cam.setPosition(trans);
        }
    } else {
*/

    /** Init              **/
    /**                   **/
    /**                   **/
    /**                   **/

    int upState = glfwGetKey(pWindow, GLFW_KEY_W);
    int downState = glfwGetKey(pWindow, GLFW_KEY_S);
    int positionState = glfwGetKey(pWindow, GLFW_KEY_P);
    int shot = glfwGetKey(pWindow, GLFW_KEY_SPACE);
    float upDown = 0.0f;
    float f = pSpaceship->getBounding()->transform().translation().Y;


    /** User input        **/
    /**                   **/
    /**                   **/
    /**                   **/


    if (f < 13.5 && upState == GLFW_PRESS && downState == GLFW_RELEASE) {
        if (item1) {
            upDown = 25.0f;
        } else {
            upDown = 17.5f;
        }

    } else if (f > -12.4 && downState == GLFW_PRESS && upState == GLFW_RELEASE) {
        if (item1) {
            upDown = -25.0f;
        } else {
            upDown = -17.5f;
        }
    }
    if (shot == GLFW_PRESS && laserTimer <= 0) {
        Matrix CP;
        laserTimer = 1;
        CP = pSpaceship->getBounding()->transform();
        if (pCurrentLaser >= laserModels.size() - 1) {
            pCurrentLaser = 0;
            laserModels.at(pCurrentLaser)->transform(CP);
        } else {
            pCurrentLaser++;
            laserModels.at(pCurrentLaser)->transform(CP);
        }
    }

    /**  Particle    **/
    /**  Effects für **/
    /**  Raumschiff  **/
    /**              **/
    if (particleSchubTimer < 0) {
        Vector pos = pSpaceship->getBounding()->transform().translation();
        pos.Z = pSpaceship->getBounding()->transform().translation().Z - 3;
        particleProps = ParticleProps();
        particleProps.position = pos;
        particleProps.velocity = Vector(0, 0, -15);
        particleProps.sizeBegin = 1;
        particleProps.sizeVariation = 0.5f;
        particleProps.sizeEnd = 0.001f;
        particleProps.lifeTime = 1;

        particleSystem->emit(particleProps);

        particleSchubTimer = 0.05f;
    }



    /**  DTIME METHODEN   **/
    /**                   **/
    /**                   **/
    /**                   **/
    if (laserBossTimer < 0) {
        Matrix CP, S;
        laserBossTimer = 2;
        CP = pBoss->getEnemy()->transform();
        S.scale(0.5);
        if (pCurrentBossLaser >= laserBossModels.size() - 1) {
            pCurrentBossLaser = 0;
            laserBossModels.at(pCurrentBossLaser)->transform(CP * S);
        } else {
            pCurrentBossLaser++;
            laserBossModels.at(pCurrentBossLaser)->transform(CP * S);
        }
    }

    //Monster werden nach 10 Zeiteinheiten am rechten Rand platziert
    if (monsterTimer <= 0) {
        Matrix CP;
        monsterTimer = 10;
        float tmp = randomFloat(12, -12);
        CP.translation(0, tmp, 90);
        if (pCurrentMonster >= monsterModels.size() - 1) {
            pCurrentMonster = 0;
            monsterModels.at(pCurrentMonster)->getEnemy()->transform(CP);
        } else {
            pCurrentMonster++;
            monsterModels.at(pCurrentMonster)->getEnemy()->transform(CP);
        }

    }
    if (itemTimer <= 0) {
        Matrix CP;
        itemTimer = 20;
        float tmp = randomFloat(12, -12);
        CP.translation(0, tmp, 90);
        if (pCurrentItem >= itemsModels.size() - 1) {
            pCurrentItem = 0;
            itemsModels.at(pCurrentItem)->getItem()->transform(CP);
        } else {
            pCurrentItem++;
            itemsModels.at(pCurrentItem)->getItem()->transform(CP);
        }
    }

    pSpaceship->steer(upDown);
    pSpaceship->update(dtime);
    spaceship->transform(pSpaceship->getBounding()->transform());


    /** FPS Anzeigen und Score */


    crntTime = glfwGetTime();
    timeDiff = crntTime - prevTime;
    counter++;
    if (timeDiff >= 1.0 / 30.0) { // If last prinf() was more than 1 sec ago
        std::string FPS = std::to_string((1.0 / timeDiff) * counter);
        std::string ms = std::to_string((timeDiff / counter) * 1000);
        std::string deinScore = std::to_string(score);
        std::string newTitle = "HSOSNABRUECK - " + FPS + "FPS / " + ms + "ms /       \" Dein Score\"  " + deinScore;
        prevTime = crntTime;
        counter = 0;
        glfwSetWindowTitle(pWindow, newTitle.c_str());

    }

    if (score == 2) {
        pBoss->setBossStatus(true);
    }
    if (bossHit == 10) {
        pBoss->setBossStatus(false);
        Matrix TM;
        for (int i = 0; i < laserBossModels.size(); ++i) {
            TM.translation(0, -80, 0);
            laserBossModels.at(i)->transform(TM);
        }
    }


    /** Update alle Spielmodels **/


    if (pBoss->isBossStatus() && bossTimer < 0) {
        if (camTimer > 0) {
            cam.setTarget(pBoss->getEnemy()->transform().translation());
            camTimer -= dtime;
        } else {
            //Vector v1 = pSpaceship->getTop()->transform().translation();
            //v1.Z += 30;
            v1.X += 25;
            cam.setTarget(v1);
            v1.X -= 25;
            cam.setPosition(camUrsprung);
        }


        isBossHit();
        updateBoss(dtime);

        shake(dtime, 15, 15);
    }

    updateMonster(dtime);
    updateLaser(dtime);
    updateItem(dtime);
    //updatePlanet(dtime);
    collisionItem(dtime);
    collisionPlayer(dtime);
    updateParticle(dtime);
    loopCollision(dtime);
    cam.update();

    /** TIMER UPDATE  */
    if (laserTimer > -1000 && !item2) {
        laserTimer = laserTimer - (dtime * 4);
    } else if (laserTimer > -1000 && item2) {
        laserTimer = laserTimer - (dtime * 8);
    }
    if (monsterTimer > -1000) {
        monsterTimer = monsterTimer - (dtime * 4);
    }
    if (bossTimer > -5) {
        bossTimer -= dtime;
    }
    if (bossTimerMovement > -1) {
        bossTimerMovement -= dtime;
    }
    if (laserBossTimer > -1) {
        laserBossTimer -= dtime;
    }
    if (particleSchubTimer >= 0) {
        particleSchubTimer -= dtime;
    }
    if (particleEnemySchub >= 0) {
        particleEnemySchub -= dtime;
    }
    if (itemTimer > -1) {
        itemTimer -= dtime;
    }
    // }
}

/** Dance til your dead **/

void Application::shake(float dtime, int a, int b) {
    Vector v, CP;
    v.X = 1;
    //0.1 -0.1 sweet spot ohne DTime
    v.Y = randomFloat(a, -b);
    v.Z = randomFloat(a, -b);
    CP = camUrsprung;
    CP.X *= v.X;
    CP.Y += v.Y * dtime;
    CP.Z += v.Z * dtime;
    cam.setPosition(CP);

}


/**   Update Methoden **/
/**                   **/
/**                   **/
/**                   **/
void Application::updateParticle(float dtime) {
    particleSystem->update(dtime);
}

void Application::updateLaser(float dtime) {
    Matrix TM, CP;
    for (int (i) = 0; (i) < laserModels.size(); ++(i)) {
        CP = laserModels.at(i)->transform();
        if (laserModels.at(i)->transform().translation().Y > -13 &&
            laserModels.at(i)->transform().translation().Y < 14) {
            TM.translation(0, 0, 20.0f * dtime);
            laserModels.at(i)->transform(CP * TM);
            //hitboxListLaser.at(i)->transform(CP * TM);
            //std::cout << "Posi Laser:" << laserModels.at(i)->transform().translation().Z << std::endl;

        }
        if (laserModels.at(i)->transform().translation().Z > 80) {
            TM.translation(0, -40, 0);
            laserModels.at(i)->transform(TM);
            //hitboxListLaser.at(i)->transform(TM);

        }
    }
}

void Application::updateMonster(float dtime) {
    Matrix TM, CP;
    for (int i = 0; i < monsterModels.size(); ++i) {
        if (!pBoss->isBossStatus()) {
            if (monsterModels.at(i)->getEnemy()->transform().translation().Y > -13 &&
                monsterModels.at(i)->getEnemy()->transform().translation().Y < 14) {
                monsterModels.at(i)->update(dtime);
            }
            if (monsterModels.at(i)->getEnemy()->transform().translation().Z < -20){
                TM.translation(0, -50, 0);
                monsterModels.at(i)->getEnemy()->transform(TM);
            }
        }
        if (pBoss->isBossStatus()) {
            TM.translation(0, -50, 0);
            monsterModels.at(i)->getEnemy()->transform(TM);
        }
    }
}

void Application::updateItem(float dtime) {
    for (int i = 0; i < itemsModels.size(); ++i) {
        itemsModels.at(i)->update(dtime);
    }
}

void Application::updateBoss(float dtime) {
    Matrix TM, CP;
    CP = pBoss->getEnemy()->transform();
    //Movet Boss an Anfangposition im Spielfeld
    if (CP.translation().Y < -60) {
        TM.translation(0, 0, 100);
        pBoss->getEnemy()->transform(TM);
    }
    //Wenn Boss Ausgangsposition noch nicht erreicht hat, bewegt sich weiter vorwärts
    if (CP.translation().Z > 60) {
        TM.translation(0, 0, -5.0f * dtime);
        pBoss->getEnemy()->transform(CP * TM);
    }

    if (bossTimerMovement < 0) {
        //std::cout << "YOOO" << std::endl;
        randomMonsterMovement = randomFloat(-5, 5);
        bossTimerMovement = 1.5f;
    }
    if (pBoss->getEnemy()->transform().translation().Y > -13 &&
        pBoss->getEnemy()->transform().translation().Y < 14) {
        if (pBoss->getEnemy()->transform().translation().Y < -11) {
            if (randomMonsterMovement < 0) {
                randomMonsterMovement *= -1;
                //std::cout << randomMonsterMovement << std::endl;
            }
        }
        if (pBoss->getEnemy()->transform().translation().Y > 12) {
            if (randomMonsterMovement > 0) {
                randomMonsterMovement *= -1;
            }
        }
        CP = pBoss->getEnemy()->transform();
        TM.translation(0, randomMonsterMovement * dtime, 0);
        pBoss->getEnemy()->transform(CP * TM);
    }
    Matrix R, RI;
    if (pBoss->transform().translation().Z < 61) {
        for (int i = 0; i < laserBossModels.size(); ++i) {
            CP = laserBossModels.at(i)->transform();
            if (laserBossModels.at(i)->transform().translation().Y > -13 &&
                laserBossModels.at(i)->transform().translation().Y < 14) {
                //R.rotationY(AI_DEG_TO_RAD(25) * dtime);
                //RI = R.invert();
                TM.translation(0, 0, -20.0f * dtime);
                laserBossModels.at(i)->transform(CP * TM);
            }
            if (laserBossModels.at(i)->transform().translation().Z < -20) {
                TM.translation(0, -80, 0);
                laserBossModels.at(i)->transform(CP * TM);
            }

        }
    }
}
/*
void Application::updatePlanet(float dtime) {
    Matrix TM, CP, CP2, N, R;
    N.scale(2);
    R.rotationY(AI_DEG_TO_RAD(-1.0f) * dtime);
    CP = venus->transform();
    CP2 = earth->transform();
    TM.translation(0, 0, 10.0f * dtime);
    venus->transform(R * CP * TM * R);
    TM.translation(0, 0, 5.0f * dtime);
    earth->transform(R * CP2 * TM * R);
    if (venus->transform().translation().Z > 200) {
        TM.translation(50, 0, -100);
        venus->transform(R * TM * N * R);
    } else if (earth->transform().translation().Z > 150) {
        TM.translation(60, 0, -110);
        earth->transform(R * TM * N * R);
    }
}
*/

/**COLLISION METHODEN **/
/**                   **/
/**                   **/
/**                   **/
void Application::collisionPlayer(float dtime) {
    Matrix TM, CP, CP2, CP3;
    for (int i = 0; i < laserBossModels.size(); ++i) {
        CP = laserBossModels.at(i)->transform();
        CP2 = pSpaceship->getBounding()->transform();
        if (AABB::collision(pSpaceship->boundingBox().transform(CP2),
                            laserBossModels.at(i)->boundingBox().transform(CP))) {
            particleProps = ParticleProps();
            particleProps.position = laserBossModels.at(i)->transform().translation();
            particleProps.sizeBegin = 2;
            if (score > 0) {
                score--;
            }
            for (int j = 0; j < 3; ++j) {
                float speed = Application::randomFloat(-4, 4);
                particleProps.velocity = Vector(0, speed, 0);
                particleSystem->emit(particleProps);
            }
            for (int j = 0; j < 3; ++j) {
                float speed = Application::randomFloat(-4, 4);
                particleProps.velocity = Vector(speed, speed, 0);
                particleSystem->emit(particleProps);
            }
            for (int k = 0; k < 3; ++k) {
                float speed = Application::randomFloat(-4, 4);
                particleProps.velocity = Vector(speed, 0, 0);
                particleSystem->emit(particleProps);
            }
            for (int k = 0; k < 3; ++k) {
                float speed = Application::randomFloat(-4, 4);
                particleProps.velocity = Vector(speed, 0, speed);
                particleSystem->emit(particleProps);
            }
            for (int j = 0; j < 3; ++j) {
                float speed = Application::randomFloat(-4, 4);
                particleProps.velocity = Vector(0, 0, speed);
                particleSystem->emit(particleProps);
            }
            for (int j = 0; j < 3; ++j) {
                float speed = Application::randomFloat(-4, 4);
                particleProps.velocity = Vector(0, speed, speed);
                particleSystem->emit(particleProps);
            }

            for (int j = 0; j < 3; ++j) {
                float speed = Application::randomFloat(-4, 4);
                particleProps.velocity = Vector(speed, speed, speed);
                particleSystem->emit(particleProps);
            }
            TM.translation(0, -80, 0);
            laserBossModels.at(i)->transform(TM);
        }
    }
}


void Application::loopCollision(float dtime) {
    Matrix TM, CP, CP2, CP3;
    for (int i = 0; i < laserModels.size(); ++i) {
        for (int j = 0; j < monsterModels.size(); ++j) {
            CP = laserModels.at(i)->transform();
            CP2 = monsterModels.at(j)->getEnemy()->transform();
            CP3 = pSpaceship->getBounding()->transform();
            if (AABB::collision(pSpaceship->boundingBox().transform(CP3),
                                monsterModels.at(j)->getEnemy()->boundingBox().transform(CP2))) {
                TM.translation(0, -50, 0);
                monsterModels.at(j)->getEnemy()->transform(TM);
                //std::cout<< "du wurdest getroffen von Monster Nummer: " << j << std::endl;
                if (score > 1) {
                    score -= 1;
                }

            }
            if (AABB::collision(laserModels.at(i)->boundingBox().transform(CP),
                                monsterModels.at(j)->getEnemy()->boundingBox().transform(CP2))) {
                if (monsterModels.at(j)->getPLeben() == 0) {
                    particleProps = ParticleProps();
                    particleProps.position = monsterModels.at(j)->getEnemy()->transform().translation();
                    particleProps.sizeBegin = 1;

                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(0, speed, 0);
                        particleSystem->emit(particleProps);
                    }
                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(speed, speed, 0);
                        particleSystem->emit(particleProps);
                    }
                    for (int k = 0; k < 3; ++k) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(speed, 0, 0);
                        particleSystem->emit(particleProps);
                    }
                    for (int k = 0; k < 3; ++k) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(speed, 0, speed);
                        particleSystem->emit(particleProps);
                    }
                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(0, 0, speed);
                        particleSystem->emit(particleProps);
                    }
                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(0, speed, speed);
                        particleSystem->emit(particleProps);
                    }

                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(speed, speed, speed);
                        particleSystem->emit(particleProps);
                    }

                    TM.translation(0, -40, 0);
                    laserModels.at(i)->transform(TM);
                    //hitboxListLaser.at(i)->transform(TM);
                    TM.translation(0, -50, 0);
                    monsterModels.at(j)->getEnemy()->transform(TM);
                    if (item3) {
                        score += 2;
                    } else {
                        score++;
                    }


                } else {
                    TM.translation(0, -40, 0);
                    laserModels.at(i)->transform(TM);
                    //hitboxListLaser.at(i)->transform(TM);
                    monsterModels.at(j)->setPLeben(monsterModels.at(j)->getPLeben() - 1);
                }

                std::cout << score << std::endl;
            }
        }
    }
}

void Application::collisionItem(float dtime) {
    Matrix TM, CP, CP2, o;
    for (int i = 0; i < itemsModels.size(); ++i) {
        CP = itemsModels.at(i)->getItem()->transform();
        CP2 = pSpaceship->getBounding()->transform();
        if (AABB::collision(pSpaceship->boundingBox().transform(CP2),
                            itemsModels.at(i)->getItem()->boundingBox().transform(CP))) {
            //std::cout << "Item hittet" << std::endl;
            //Schneller Bewegen Buff
            if (itemsModels.at(i)->getType() == 0) {
                //Falls Item doppelt eingesammelt wird
                if (item1) {
                    itemTime = 3.0f;
                }
                TM.translation(0, -60, 0);
                itemsModels.at(i)->getItem()->transform(TM);
                item1 = true;
                //Schneller schießen buff
            } else if (itemsModels.at(i)->getType() == 1) {
                //Falls Item doppelt eingesammelt wird
                if (item2) {
                    item2Time = 3.0f;
                }
                TM.translation(0, -60, 0);
                itemsModels.at(i)->getItem()->transform(TM);
                item2 = true;
                //Doppelt Punkte buff
            } else {
                TM.translation(0, -60, 0);

                itemsModels.at(i)->getItem()->transform(TM);
                item3 = true;
            }
        }
    }
    //Timer für hoch und runter boost
    if (item1) {
        itemTime -= dtime;
        if (itemTime <= 0) {
            //std::cout << itemTime << "HOCHRUNTER" << std::endl;
            item1 = false;
            itemTime = 3;
        }
    }
    //Timer für schneller schießen
    if (item2) {
        item2Time -= dtime;
        if (item2Time <= 0) {
            //std::cout << item2Time << "FAST" << std::endl;
            item2 = false;
            item2Time = 3;
        }
    }
    if (item3) {
        item3Time -= dtime;
        if (item3Time <= 0) {
            //std::cout << item3Time << "FAST" << std::endl;
            item3 = false;
            item3Time = 3;
        }
    }

}


void Application::isBossHit() {
    Matrix TM, CP, CP2, CP3;
    for (int i = 0; i < laserModels.size(); ++i) {
        CP = laserModels.at(i)->transform();
        CP2 = pBoss->getEnemy()->transform();
        if (AABB::collision(laserModels.at(i)->boundingBox().transform(CP),
                            pBoss->getEnemy()->boundingBox().transform(CP2))) {

            bossHit++;
            TM.translation(0, -40, 0);
            laserModels.at(i)->transform(TM);
            if (bossHit == 10) {
                /** Wenn boss gestorben dann particle */
                particleProps = ParticleProps();
                particleProps.position = CP2.translation();
                particleProps.sizeBegin = 3;
                particleProps.lifeTime = 1.0f;

                for (int j = 0; j < 3; ++j) {
                    float speed = Application::randomFloat(-8, 8);
                    particleProps.velocity = Vector(0, speed, 0);
                    particleSystem->emit(particleProps);
                }
                for (int j = 0; j < 3; ++j) {
                    float speed = Application::randomFloat(-8, 8);
                    particleProps.velocity = Vector(speed, speed, 0);
                    particleSystem->emit(particleProps);
                }
                for (int k = 0; k < 3; ++k) {
                    float speed = Application::randomFloat(-8, 8);
                    particleProps.velocity = Vector(speed, 0, 0);
                    particleSystem->emit(particleProps);
                }
                for (int k = 0; k < 3; ++k) {
                    float speed = Application::randomFloat(-8, 8);
                    particleProps.velocity = Vector(speed, 0, speed);
                    particleSystem->emit(particleProps);
                }
                for (int j = 0; j < 3; ++j) {
                    float speed = Application::randomFloat(-8, 8);
                    particleProps.velocity = Vector(0, 0, speed);
                    particleSystem->emit(particleProps);
                }
                for (int j = 0; j < 3; ++j) {
                    float speed = Application::randomFloat(-8, 8);
                    particleProps.velocity = Vector(0, speed, speed);
                    particleSystem->emit(particleProps);
                }

                for (int j = 0; j < 3; ++j) {
                    float speed = Application::randomFloat(-8, 8);
                    particleProps.velocity = Vector(speed, speed, speed);
                    particleSystem->emit(particleProps);
                }

                TM.translation(0, -60, 0);
                pBoss->getEnemy()->transform(TM);
                this->score += 10;
            }
        }
        for (int j = 0; j < laserBossModels.size(); ++j) {
            CP3 = laserBossModels.at(j)->transform();
            if (AABB::collision(laserModels.at(i)->boundingBox().transform(CP),
                                laserBossModels.at(j)->boundingBox().transform(CP3))) {

                if (laserBossModels.at(j)->isHitBefore()) {
                    particleProps = ParticleProps();
                    particleProps.position = CP3.translation();
                    particleProps.sizeBegin = 2;
                    particleProps.lifeTime = 1.0f;
                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(0, speed, 0);
                        particleSystem->emit(particleProps);
                    }
                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(speed, speed, 0);
                        particleSystem->emit(particleProps);
                    }
                    for (int k = 0; k < 3; ++k) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(speed, 0, 0);
                        particleSystem->emit(particleProps);
                    }
                    for (int k = 0; k < 3; ++k) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(speed, 0, speed);
                        particleSystem->emit(particleProps);
                    }
                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(0, 0, speed);
                        particleSystem->emit(particleProps);
                    }
                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(0, speed, speed);
                        particleSystem->emit(particleProps);
                    }

                    for (int j = 0; j < 3; ++j) {
                        float speed = Application::randomFloat(-4, 4);
                        particleProps.velocity = Vector(speed, speed, speed);
                        particleSystem->emit(particleProps);
                    }
                    TM.translation(0, -80, 0);
                    laserBossModels.at(j)->transform(TM);
                }
                TM.translation(0, -40, 0);
                laserModels.at(i)->transform(TM);
                laserBossModels.at(j)->setHitBefore(true);
            }
        }
    }
}


/** https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats **/
float Application::randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void Application::draw() {
    shadowGenerator.generate(models);

    // 1. clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderLightMapper::instance().activate();
    // 2. setup shaders and draw models
    for (ModelList::iterator it = models.begin(); it != models.end(); ++it) {
        (*it)->draw(cam);
    }
    ShaderLightMapper::instance().deactivate();

    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error == 0);

    particleSystem->draw(cam);
}

void Application::end() {
    for (ModelList::iterator it = models.begin(); it != models.end(); ++it)
        delete *it;

    models.clear();
}


/**  CREATE METHODEN  **/
/**                   **/
/**                   **/
/**                   **/



void Application::createMonster(Matrix m, Matrix o, ConstantShader *pConstShader) {
    for (int i = 0; i < 10; i++) {
        //std::cout << i << ". Monster wird erstellt." << std::endl;
        pEnemy = new Enemy(ASSET_DIRECTORY "enemy.obj");
        m.translation(0, -50, 0);
        o.rotationY(AI_DEG_TO_RAD(180.0f));
        pEnemy->getEnemy()->transform(m * o);

        models.push_back(pEnemy->getEnemy());
        monsterModels.push_back(pEnemy);

        hitboxModel = new LineBoxModel(pEnemy->getBlockModel()->boundingBox().Max,
                                       pEnemy->getBlockModel()->boundingBox().Min);
        pConstShader = new ConstantShader();
        pConstShader->color(Color(0, 1, 0));
        hitboxModel->shader(pConstShader, true);
        hitboxModel->transform(m * o);
        models.push_back(hitboxModel);
        hitboxListMonster.push_back(hitboxModel);
    }
}

void Application::createBoss(Matrix m, Matrix o, ConstantShader *pConstShader) {
    pBoss = new Enemy();
    m.translation(0, -70, 0);
    o.rotationY(AI_DEG_TO_RAD(180.0f));
    pBoss->getEnemy()->transform(m * o);
    models.push_back(pBoss->getEnemy());
}

void Application::createBossLaser(Matrix m, Matrix o, ConstantShader *pConstShader) {
    for (int i = 0; i < 10; i++) {
        pModel = new Model(ASSET_DIRECTORY "Earth_2K.obj", false);
        pModel->shader(new PhongShader(), true);
        m.translation(0, -80, 0);
        pModel->transform(m);
        models.push_back(pModel);
        laserBossModels.push_back(pModel);
    }
}

void Application::createLaser(int modelsNumber, Matrix m, ConstantShader *pConstShader) {
    for (int i = 0; i < modelsNumber; i++) {
        //std::cout << i << " wird erstellt." << std::endl;
        pModel = new Model(ASSET_DIRECTORY "LaserTry.obj", false);
        pModel->shader(new PhongShader(), true);
        //pModel->shadowCaster(false);
        m.translation(0, -40, 0);
        pModel->transform(m);
        models.push_back(pModel);
        laserModels.push_back(pModel);
        /*
        hitboxModel = new LineBoxModel(pModel->getBlockModel()->boundingBox().Max,
                                       pModel->getBlockModel()->boundingBox().Min);
        pConstShader = new ConstantShader();
        pConstShader->color(Color(0, 1, 0));
        hitboxModel->shader(pConstShader, true);
        hitboxModel->transform(m);
        models.push_back(hitboxModel);
        hitboxListLaser.push_back(hitboxModel);
         */
    }
}

void Application::createItems(int modelsNumber, Matrix m, Matrix o, ConstantShader *pConstShader) {
    for (int i = 0; i < 5; i++) {
        //std::cout << i << " Item wird erstellt." << std::endl;
        pItem = new Items();
        /*m.translation(0, -60, 0);
        o.rotationY(AI_DEG_TO_RAD(180.0f));
        pItem->getItem()->transform(m * o);
*/
        models.push_back(pItem->getItem());
        itemsModels.push_back(pItem);
    }
}


void Application::createScene() {


    /** init **/


    Matrix m, n, o;
    float rad;
    PhongShader *pPhongShader;
    int modelsNumber = 40;
    hitboxListLaser.reserve(sizeof(BaseModel) * modelsNumber);
    laserModels.reserve(sizeof(BaseModel) * modelsNumber);
    hitboxListMonster.reserve(sizeof(BaseModel) * modelsNumber);
    pCurrentLaser = 0;
    pCurrentItem = 0;
    laserTimer = 0;
    monsterTimer = 3;
    itemTimer = 6;
    score = 0;


    /** Skybox **/


    pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
    pModel->shader(new PhongShader(), true);
    pModel->shadowCaster(false);
    n.scale(2.0f);
    pModel->transform(n);
    models.push_back(pModel);
    pPhongShader = new PhongShader();


    /** Player **/


    pSpaceship = new Spaceship(ASSET_DIRECTORY "SpaceShip.obj", ASSET_DIRECTORY "woodenObje.obj");
    models.push_back(pSpaceship->getSpaceShip());
    //models.push_back(pSpaceship->getBounding());
    v1 = pSpaceship->getBounding()->transform().translation();
    v1.Z += 30;
    cam.setTarget(v1);
    //v1.Z -= 30;
    v1.X -= 25;
    cam.setPosition(v1);
    camUrsprung = cam.position();
    ConstantShader *pConstShader;
    spaceship = new LineBoxModel(pSpaceship->getBounding()->getBlockModel()->boundingBox().Min,
                                 pSpaceship->getBounding()->getBlockModel()->boundingBox().Max);
    pConstShader = new ConstantShader();
    pConstShader->color(Color(0, 1, 0));
    spaceship->shader(pConstShader, true);
    //models.push_back(spaceship);


    /** Planeten **/
    venus = new Model(ASSET_DIRECTORY "Venus_1K.obj", false);
    venus->shader(new PhongShader(), true);
    n.scale(30);
    m.translation(50, 20, 70);
    venus->shadowCaster(false);
    venus->transform(m * n);
    models.push_back(venus);


    /**Erstell einige Laser, Item und Monster Objekte **/


    createLaser(modelsNumber, m, pConstShader);
    createMonster(m, o, pConstShader);
    createItems(modelsNumber, m, o, pConstShader);
    createBoss(m, o, pConstShader);
    createBossLaser(m, o, pConstShader);

    // directional lights
    DirectionalLight *dl = new DirectionalLight();
    dl->direction(Vector(0.2f, -1, 1));
    dl->color(Color(1, 1, 1));
    dl->castShadows(true);
    ShaderLightMapper::instance().addLight(dl);

    particleProps = ParticleProps();
    particleSystem = new ParticleSystem();
    particleSystem->shader(new ParticleShader(), true);
}


