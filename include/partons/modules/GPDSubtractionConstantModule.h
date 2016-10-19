/*
 * GPDSubtractionConstantModule.h
 *
 *  Created on: Oct 18, 2016
 *      Author: debian
 */

#ifndef GPDSUBTRACTIONCONSTANTMODULE_H_
#define GPDSUBTRACTIONCONSTANTMODULE_H_

#include <map>
#include <string>

#include "../beans/gpd/GPDBorderFunctionKinematic.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../ModuleObject.h"

/**
 * Mother class to create models of GPD subtraction constant
 */
class GPDSubtractionConstantModule: public ModuleObject {

public:

    /**
     * Default constructor
     * @param className
     */
    GPDSubtractionConstantModule(const std::string &className);

    /**
     * Destructor
     */
    virtual ~GPDSubtractionConstantModule();

    /**
     * Clone
     */
    virtual GPDSubtractionConstantModule* clone() const = 0;

    /**
     * Compute for given kinematics and GPD type
     * @param xi Average longitudinal momentum of parton
     * @param t Four-momentum transfer to target nucleon (Mandelstam variable)
     * @param MuF2 Square of factorization scale
     * @param MuR2 Square of renormalization scale
     * @param gpdType GPD type
     */
    virtual double compute(double xi, double t, double MuF2, double MuR2,
            GPDType::Type gpdType);

    /**
     * Compute for given kinematics and GPD type
     * @param kinematic Kinematics to be used
     * @param gpdType GPD type
     */
    virtual double compute(const GPDBorderFunctionKinematic &kinematic,
            GPDType gpdType);

    /**
     * Calculate for GPD H
     */
    virtual double computeH();

    /**
     * Calculate for GPD E
     */
    virtual double computeE();

    /**
     * Calculate for GPD Ht
     */
    virtual double computeHt();

    /**
     * Calculate for GPD Et
     */
    virtual double computeEt();

    /**
     * Get list of available GPD types to be computed
     */
    List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString() const;

protected:

    /**
     * Copy constructor
     * @param other Object to be copied
     */
    GPDSubtractionConstantModule(const GPDSubtractionConstantModule &other);

    /**
     * Initialize module
     */
    virtual void initModule();

    /**
     * Check if module if well configured
     */
    virtual void isModuleWellConfigured();

    /**
     * Pre-compute
     * @param xi Average longitudinal momentum of parton
     * @param t Four-momentum transfer to target nucleon (Mandelstam variable)
     * @param MuF2 Square of factorization scale
     * @param MuR2 Square of renormalization scale
     * @param gpdType GPD type
     */
    void preCompute(double xi, double t, double MuF2, double MuR2,
            GPDType::Type gpdType);

    double m_xi;    ///< Average longitudinal momentum of parton
    double m_t; ///< Four-momentum transfer to target nucleon (Mandelstam variable)
    double m_MuF2;  ///< Square of factorization scale
    double m_MuR2;  ///< Square of renormalization scale
    GPDType::Type m_gpdType; ///< GPD type

    std::map<GPDType::Type, double (GPDSubtractionConstantModule::*)()> m_listGPDComputeTypeAvailable; ///< list of available GPDs to be computed
    std::map<GPDType::Type, double (GPDSubtractionConstantModule::*)()>::iterator m_it; ///< iterator to list of available GPDs to be computed
};

#endif /* GPDSUBTRACTIONCONSTANTMODULE_H_ */
