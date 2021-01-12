#ifndef ALGEBRAICTOYMODEL_H
#define ALGEBRAICTOYMODEL_H

/**
 * @file algebraicToyModel.h
 * @author José Manuel Morgado Chavez (University Of Huelva)
 * @author Cédric Mezrag (CEA Saclay)
 * @date 12th January 2021 
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>


#include "../GPDModule.h"

#include <include/NumA/linear_algebra/vector/VectorD.h>
#include <include/NumA/linear_algebra/matrix/MatrixD.h>
#include <include/NumA/triangulation/mesh.h>
#include <include/NumA/RadonTransform/RadonTransform.h>

namespace PARTONS {

class algebraicToyModel: public PARTONS::GPDModule 
{

public:

    /**
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;

    /**
     * Default constructor.
     * @param className Name of class.
     */
    algebraicToyModel(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~algebraicToyModel();
    virtual algebraicToyModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    // virtual std::string toString() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     * */

    algebraicToyModel(const algebraicToyModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PARTONS::PartonDistribution computeH();

    /**
     * Computation of the double distribution.
     * 
     * Sets the DD attribute of the class.
     */

    void computeDD();

    /**
     * Mesh building.
     * 
     * Sets the mesh attribute of the class.
     */
    NumA::Mesh setMesh();

private:

    double c;                                   // t-dependece of the GPD model.
    double cM;                                  // Auxilary t-dependece for the computation of the singlet distribution.
    double m2;                                  // Squared pion mass scale.

    NumA::Mesh mesh;                            // Mesh over the double distribution domain.

    vector<double> DD;                          // Declare double distribution.
    NumA::MatrixD RTmatrix;                     // Radon transform matrix for uVal.

};


} //end of namespace

# endif