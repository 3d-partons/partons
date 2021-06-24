#ifndef ALGEBRAICTOYMODEL_H
#define ALGEBRAICTOYMODEL_H

/**
 * @file    algebraicToyModel.h
 * @author  José Manuel Morgado Chavez (University Of Huelva)
 * @author  Cédric Mezrag (CEA Saclay)
 * @date    Friday 10th June 2021 
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

#include <map>

namespace PARTONS {

class algebraicToyModel: public PARTONS::GPDModule
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
      * Initialize Radon transform 
      */
    void initRT();

    NumA::RadonTransform RT;                    // Create Radon transform.

private:

    bool num = 0;                               // Boolean to choose whether to evaluate the model from its analytic expressions (num = 0) or using the covariant extension (num = 1)

    double c;                                   // t-dependece of the GPD model.
    double c1;                                  // t-depence x=1 limit ERBL region (Analytical expressions)
    double cM;                                  // Auxilary t-dependece for the computation of the singlet distribution.
    double m2;                                  // Squared pion mass scale.
    double ca;                                  // Auxiliary t-dependence parameter for computing the inverse Radon transform.

    double dt;                                  // D-term t-dependence parametrized as a monopole: 1//(1-t/4m2)
    double alpha;                               // Kinematic variable for D-terms: \alpha = m_x/m_xi
    double dplus;                               // D-term analytical computation
    double dminus;                              // D-term analytical computation

    double m2D;                                 // D-term t-dependence (mass-scale). Fitting of Phys. Rev. D 97, 014020 (2018) gravitational FFs.

    std::map<double, Eigen::VectorXd> DD;
    std::map<double, Eigen::VectorXd>::iterator search;

    Eigen::VectorXd DDt = Eigen::VectorXd::Zero(1);                         // Double distribution.
    Eigen::VectorXd DDt0 = Eigen::VectorXd::Zero(1);                       // Double distribution at zero momentum transfer (it is necessary for the computation of D-terms.)
};


} //end of namespace

# endif