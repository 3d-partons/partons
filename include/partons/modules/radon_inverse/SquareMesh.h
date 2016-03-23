/*
 * SquareMesh.h
 *
 *  Created on: 22 Mar 2016
 *      Author: guest
 */

#ifndef SQUAREMESH_H_
#define SQUAREMESH_H_

#include "RadonInverseModule.h"



class SquareMesh: public RadonInverseModule {
public:
    SquareMesh();
    virtual ~SquareMesh();

    void BuildMesh();



};

#endif /* SQUAREMESH_H_ */
