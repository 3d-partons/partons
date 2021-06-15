#ifndef INTERPOLATEDGPD_H
#define INTERPOLATEDGPD_H

/**
 * @file    interpolatedGPD.h
 * @author  José Manuel Morgado Chavez (University Of Huelva)
 * @date    Tuesday 15th June 2021 
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>

#include <NumA/interpolation/CubicSpline.h>

#include "../GPDModule.h"

namespace PARTONS {

class interpolatedGPD: public PARTONS::GPDModule
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
    interpolatedGPD(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~interpolatedGPD();
    virtual interpolatedGPD* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    // virtual std::string toString() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     * */

    interpolatedGPD(const interpolatedGPD& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PARTONS::PartonDistribution computeH();

private:

    NumA::CubicSpline* uValInt;                                                 // Interpolator for u-GPD.

};


} //end of namespace

# endif