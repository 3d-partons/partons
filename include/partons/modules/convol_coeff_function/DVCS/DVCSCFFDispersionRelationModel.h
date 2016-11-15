/*
 * DVCSCFFDispersionRelationModel.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef DVCSCFFDISPERSIONRELATIONMODEL_H_
#define DVCSCFFDISPERSIONRELATIONMODEL_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <string>
#include <vector>

#include "DVCSConvolCoeffFunctionModule.h"

class GPDSubtractionConstantModule;

class PartonDistribution;

namespace NumA {
class FunctionType1D;
}

/**
 * Dispersion relation DVCS/CFF model
 */
class DVCSCFFDispersionRelationModel: public DVCSConvolCoeffFunctionModule {

public:

    static const std::string SUBTRACTION_CONSTANT_MODULE_NAME; ///< Name of subtraction constant module to be set via configure

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /** Constructor
     * @param className Name of this class
     */
    DVCSCFFDispersionRelationModel(const std::string &className);

    /**
     * Destructor
     */
    virtual ~DVCSCFFDispersionRelationModel();

    virtual DVCSCFFDispersionRelationModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Get subtraction constant module
     */
    GPDSubtractionConstantModule* getSubtractionConstantModule() const;

    /**
     * Set subtraction constant module
     * @param subtractionConstantModule Subtraction constant module to be set
     */
    void setSubtractionConstantModule(
            GPDSubtractionConstantModule* subtractionConstantModule);

protected:

    /** Copy constructor
     * @param other Object to be copied
     */
    DVCSCFFDispersionRelationModel(const DVCSCFFDispersionRelationModel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    /**
     * Sum for quark singlet combinations weighted by squares of charges
     * @param xi GPD xi variable
     */
    double computeSquareChargeAveragedGPD(double xi);

    /**
     * Dispersion relation integral function for 1/(x - xi) part
     * @param xi GPD xi variable
     * @param par Additional parameters (not used here)
     */
    double dispersionRelationIntegralPartDiagonalA(double xi,
            std::vector<double> par);

    /**
     * Dispersion relation integral function for 1/(x + xi) part
     * @param xi GPD xi variable
     * @param par Additional parameters (not used here)
     */
    double dispersionRelationIntegralPartDiagonalB(double xi,
            std::vector<double> par);

    /**
     * Initialize functors
     */
    void initFunctorsForIntegrations();

    NumA::FunctionType1D* m_p_int_dispersionRelationIntegralPartDiagonalA; ///< functor related to dispersionRelationIntegralPartDiagonalA() function
    NumA::FunctionType1D* m_p_int_dispersionRelationIntegralPartDiagonalB; ///< functor related to dispersionRelationIntegralPartDiagonalB() function

    GPDSubtractionConstantModule* m_pSubtractionConstantModule;
};

#endif /* DVCSCFFDISPERSIONRELATIONMODEL_H_ */
