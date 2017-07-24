#ifndef SQUARE_MESH_H
#define SQUARE_MESH_H

/**
 * @file SquareMesh.h
 * @author Cedric MEZRAG
 * @date 22 March 2016
 * @version 1.0
 */

#include "../../BaseObject.h"

namespace PARTONS {

/**
 * @class SquareMesh
 */
class SquareMesh: public BaseObject {
public:
    SquareMesh();
    virtual ~SquareMesh();

    void BuildMesh();

};

} /* namespace PARTONS */

#endif /* SQUARE_MESH_H */
