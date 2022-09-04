//
//  BaseModel.hpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef BaseModel_hpp
#define BaseModel_hpp

#include <stdio.h>
#include "../../../include/Camera.h"
#include "../../../include/Matrix.h"
#include "utils/Shader/BaseShader.h"
#include "Aabb.h"

class BaseModel
{
public:
    BaseModel();
    virtual ~BaseModel();
    virtual void draw(const BaseCamera& Cam);
    const Matrix& transform() const { return Transform; }
    void transform( const Matrix& m) { Transform = m; }
    virtual void shader( BaseShader* shader, bool deleteOnDestruction=false );
    virtual BaseShader* shader() const { return pShader; }
    virtual const AABB& boundingBox() const { return AABB::unitBox(); }
	bool shadowCaster() const { return ShadowCaster; }
    bool isHitBefore();
	void shadowCaster(bool sc) { ShadowCaster = sc; }
    BaseModel* getBlockModel();
protected:
    Matrix Transform;
    BaseShader* pShader;
    bool DeleteShader;
	bool ShadowCaster;
    bool hitBefore = false;
public:
    void setHitBefore(bool hitBefore);
};


#endif /* BaseModel_hpp */
