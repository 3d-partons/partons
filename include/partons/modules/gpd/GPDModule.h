#ifndef GPD_MODULE_H
#define GPD_MODULE_H

/**
 * @file GPDModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 06, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/gpd/GPDKinematic.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/List.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../ModuleObject.h"

namespace PARTONS {

class GPDEvolutionModule;

/**
 * @class GPDModule
 * @brief Abstract class that provides a skeleton to implement a Generalized Parton Distributions (GPD) module.
 *
 * It is best to use this module with the corresponding service: GPDService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class GPDModule: public ModuleObject {
public:

    static const std::string GPD_MODULE_CLASS_NAME; ///< Type of the module in XML automation.
    static const std::string GPD_TYPE; ///< Parameter used in configure() or XML automation to set the type of the GPD to compute.

    //TODO delete
    enum Keys {
        e_MUF_REF_STRING_KEY
    };

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDModule(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~GPDModule();

    virtual GPDModule* clone() const = 0;

    virtual void run();

    void virtual configure(const ElemUtils::Parameters &parameters);

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param kinematic GPD kinematics object.
     * @param gpdType H, Ht, E, Et, ... or ALL. See GPDType for more details.
     * @param evolution Boolean to use evolution. Default: false.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution compute(const GPDKinematic &kinematic,
            GPDType gpdType, bool evolution = true);

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param x Longitudinal momentum fraction of the active parton.
     * @param xi Skewness.
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
     * @param MuF2 Factorization scale.
     * @param MuR2 Renormalization scale.
     * @param gpdType H, Ht, E, Et, ... or ALL. See GPDType for more details.
     * @param evolution Boolean to use evolution. Default: false.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution compute(double x, double xi, double t,
            double MuF2, double MuR2, GPDType::Type gpdType, bool evolution =
                    true);

    /**
     * This method can be implemented in the child class if the GPD H is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeH();
    /**
     * This method can be implemented in the child class if the GPD E is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeE();
    /**
     * This method can be implemented in the child class if the GPD Ht is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeHt();
    /**
     * This method can be implemented in the child class if the GPD Et is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeEt();

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    /**
     *
     * @return Pointer to the underlying GPD Evolution module.
     */
    const GPDEvolutionModule* getEvolQcdModule() const;
    /**
     *
     * @param pEvolQcdModule Pointer to the underlying GPD Evolution module.
     */
    void setEvolQcdModule(GPDEvolutionModule* pEvolQcdModule);
    /**
     *
     * @return Reference factorization scale used by the GPD model before evolution.
     */
    double getMuF2Ref() const;
    /**
     *
     * @return Factorization scale.
     */
    double getMuF2() const;
    /**
     *
     * @param muF2 Factorization scale.
     */
    void setMuF2(double muF2);
    /**
     *
     * @return Renormalization scale.
     */
    double getMuR2() const;
    /**
     *
     * @param muR2 Renormalization scale.
     */
    void setMuR2(double muR2);
    /**
     *
     * @return Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
     */
    double getT() const;
    /**
     *
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
     */
    void setT(double t);
    /**
     *
     * @return Longitudinal momentum fraction of the active parton.
     */
    double getX() const;
    /**
     *
     * @param x Longitudinal momentum fraction of the active parton.
     */
    void setX(double x);
    /**
     *
     * @return Skewness.
     */
    double getXi() const;
    /**
     *
     * @param xi Skewness.
     */
    void setXi(double xi);

    /**
     *
     * @return List of available GPDTypes in the model considered. This list is set in the child class.
     */
    List<GPDType> getListOfAvailableGPDTypeForComputation() const;

//    void prepareComputationConfiguration(
//            const List<ElemUtils::Parameter> &moduleNameList,
//            const unsigned int level = 0);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:
    double m_x; ///< Longitudinal momentum fraction of the active parton.
    double m_xi; ///< Skewness.
    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
    double m_MuF2; ///< Factorization scale.
    double m_MuR2; ///< Renormalization scale.
    GPDType::Type m_gpdType; ///< H, Ht, E, Et, ... or ALL. See GPDType for more details.

    double m_MuF2_ref; ///< Reference factorization scale (in GeV^2) used by the GPD model before evolution.

    GPDEvolutionModule* m_pGPDEvolutionModule; ///< Pointer to the underlying GPD Evolution module.

    /**
     * Copy constructor.
     *
     * @param other
     */
    GPDModule(const GPDModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * Method called at the start of the compute method to test the input.
     * Calls initModule and isModuleWellConfigured.
     * @param x Longitudinal momentum fraction of the active parton.
     * @param xi Skewness.
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
     * @param MuF2 Factorization scale.
     * @param MuR2 Renormalization scale.
     * @param gpdType
     */
    virtual void preCompute(double x, double xi, double t, double MuF2,
            double MuR2, GPDType::Type gpdType);

    /**
     * List of GPD types that can be computed by the child class.
     * Needs to be set in the constructor of the child class, with the corresponding methods to be used.
     */
    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()> m_listGPDComputeTypeAvailable;
    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()>::iterator m_it;

    // std::vector<GPDType::Type> m_listOfAvailableGPDTypeForComputation;

private:
//    void prepareGPDEvolutionModule(
//            const List<ElemUtils::Parameter> &moduleNameList,
//            unsigned int level = 0);
};

} /* namespace PARTONS */

#endif /* GPD_MODULE_H */
