#ifndef GEGENMOD_H
#define GEGENMOD_H

/**
 * @file Gegenbauer_Model.h
 * @author Cédric Mezrag (CEA Saclay)
 * @date 02nd June 2021
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>

#include "../GPDModule.h"
#include "../../MathIntegratorModule.h"

#include "/local/home/cmezrag/Téléchargements/boost_1_76_0/boost/math/special_functions/gegenbauer.hpp"


namespace PARTONS {

class gegenbauerModel: public PARTONS::GPDModule    //, public MathIntegratorModule
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
    gegenbauerModel(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~gegenbauerModel();
    virtual gegenbauerModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    // virtual std::string toString() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     * */

    gegenbauerModel(const gegenbauerModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PARTONS::PartonDistribution computeH();


private:

    double m_Mx;                                                // -x variable
    double m_n ;												// order of the Gegenbauer polynomials

};


} //end of namespace

# endif
