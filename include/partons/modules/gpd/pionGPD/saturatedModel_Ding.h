#ifndef SATURATEDMODEL_DING_H
#define SATURATEDMODEL_DING_H

/**
 * @file    saturatedModel_Ding.h
 * @author  José Manuel Morgado Chavez (University Of Huelva)
 * @author  Cédric Mezrag (CEA Saclay)
 * @date    Friday 23rd April 2021 
 * @version 1.0
 */


#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>
#include <iostream>

#include "../GPDModule.h"

#include <eigen3/Eigen/Dense>
#include <NumA/triangulation/mesh.h>
#include <NumA/RadonTransform/RadonTransform.h>

namespace PARTONS {

class saturatedModel_Ding: public PARTONS::GPDModule
{

public:

    static const std::string COVARIANT_EXTENSION;

    /**
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;

    /**
     * Default constructor.
     * @param className Name of class.
     */
    saturatedModel_Ding(const std::string& className);

    /**
     * Destructor.
     */
    virtual ~saturatedModel_Ding();
    virtual saturatedModel_Ding* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    // virtual std::string toString() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     * */

    saturatedModel_Ding(const saturatedModel_Ding& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PARTONS::PartonDistribution computeH();

    /**
      * Initialize Radon transform 
      */
    void initRT();

    NumA::RadonTransform RT;                                // Create Radon transform.

private:

    double c;                                   // t-dependece of the GPD model.
    double cM;                                  // Auxilary t-dependece for the computation of the singlet distribution.
    double m2;                                  // Squared pion mass scale.

    double dt;                                  // D-term t-dependence parametrized as a monopole: 1//(1-t/4m2)
    double alpha;                               // Kinematic variable for D-terms: \alpha = m_x/m_xi
    double dplus;                               // D-term analytical computation
    double dminus;                              // D-term analytical computation
    double m2D;                                 // D-term t-dependence (mass-scale). Fitting of Phys. Rev. D 97, 014020 (2018) gravitational FFs.

    // TODO: Explore the possibility of storing DDs into a map: ( double t-value, std::vector<double> DDt-value ). So at the level of evaluation on just needs to check that map.
    Eigen::VectorXd DD = Eigen::VectorXd::Zero(1);                         // Double distribution.
    Eigen::VectorXd DDt0 = Eigen::VectorXd::Zero(1);                       // Double distribution at zero momentum transfer (it is necessary for the computation of D-terms.)
};


} //end of namespace

# endif