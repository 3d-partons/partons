#ifndef PIONPDFTEST_H
#define PIONPDFTEST_H

/**
 * @file pionPDFtest.h
 * @author José Manuel Morgado Chavez (University Of Huelva)
 * @date 6th March 2021 
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>


#include "../GPDModule.h"

#include <NumA/linear_algebra/vector/VectorD.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <NumA/triangulation/mesh.h>
#include <NumA/RadonTransform/RadonTransform.h>

#include <NumA/interpolation/CubicSpline.h>

namespace PARTONS {

class pionPDFtest: public PARTONS::GPDModule 
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
    pionPDFtest(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~pionPDFtest();
    virtual pionPDFtest* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    // virtual std::string toString() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     * */

    pionPDFtest(const pionPDFtest& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PARTONS::PartonDistribution computeH();

private:

};


} //end of namespace

# endif