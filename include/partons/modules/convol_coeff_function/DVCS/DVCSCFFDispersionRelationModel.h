#ifndef DVCSCFFDISPERSIONRELATIONMODEL_H_
#define DVCSCFFDISPERSIONRELATIONMODEL_H_

/**
 * @file DVCSCFFDispersionRelationModel.h
 * @author Pawel Sznajder (IPNO)
 * @date Oct 17, 2016
 * @version 1.0
 */

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
 * @class DVCSCFFDispersionRelationModel
 *
 * Dispersion relation DVCS/CFF model. TODO: Add description.
 */
class DVCSCFFDispersionRelationModel: public DVCSConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     *
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVCSCFFDispersionRelationModel(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCFFDispersionRelationModel();

    virtual DVCSCFFDispersionRelationModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Get subtraction constant module.
     */
    GPDSubtractionConstantModule* getSubtractionConstantModule() const;

    /**
     * Set subtraction constant module.
     * @param subtractionConstantModule Subtraction constant module to be set.
     */
    void setSubtractionConstantModule(
            GPDSubtractionConstantModule* subtractionConstantModule);

protected:

    /** Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCFFDispersionRelationModel(const DVCSCFFDispersionRelationModel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    /**
     * Sum for quark singlet combinations weighted by squares of charges.
     * @param xi GPD xi variable.
     */
    double computeSquareChargeAveragedGPD(double xi);

    /**
     * Dispersion relation integral function for 1/(x - xi) part.
     * @param xi GPD xi variable.
     * @param par Additional parameters (not used here).
     */
    double dispersionRelationIntegralPartDiagonalA(double xi,
            std::vector<double> par);

    /**
     * Dispersion relation integral function for 1/(x + xi) part.
     * @param xi GPD xi variable.
     * @param par Additional parameters (not used here).
     */
    double dispersionRelationIntegralPartDiagonalB(double xi,
            std::vector<double> par);

    /**
     * Initialize functors.
     */
    void initFunctorsForIntegrations();

    NumA::FunctionType1D* m_p_int_dispersionRelationIntegralPartDiagonalA; ///< functor related to dispersionRelationIntegralPartDiagonalA() function
    NumA::FunctionType1D* m_p_int_dispersionRelationIntegralPartDiagonalB; ///< functor related to dispersionRelationIntegralPartDiagonalB() function

    GPDSubtractionConstantModule* m_pSubtractionConstantModule; ///< Subtraction constant module.
};

#endif /* DVCSCFFDISPERSIONRELATIONMODEL_H_ */
