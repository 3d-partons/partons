#ifndef SQUARE_MESH_H
#define SQUARE_MESH_H

/**
 * @file SquareMesh.h
 * @author Cedric MEZRAG
 * @date 22 March 2016
 * @version 1.0
 *
 * @class SquareMesh
 */

#include "../../BaseObject.h"

class SquareMesh: public BaseObject {
public:
    SquareMesh();
    virtual ~SquareMesh();

    void BuildMesh();

};

#endif /* SQUARE_MESH_H */
