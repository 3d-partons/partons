/*
 * SquareMesh.cpp
 *
 *  Created on: 22 Mar 2016
 *      Author: guest
 */

#include "../../../../include/partons/modules/radon_inverse/SquareMesh.h"

SquareMesh::SquareMesh() {
    // TODO Auto-generated constructor stub

}

SquareMesh::~SquareMesh() {
    // TODO Auto-generated destructor stub
}

void SquareMesh::BuildMesh() {
    std::vector<size_t> dimensionsAndSize;
    dimensionsAndSize.push_back(10);
    dimensionsAndSize.push_back(10);

    RadonMeshCell defaultCell;

    MDArray<RadonMeshCell> matrix(dimensionsAndSize, defaultCell);
    matrix(0,2) = defaultCell;
    RadonMeshCell otherCell = matrix(1,3);


}
