/*
 * GPDSubtractionConstantMMS13.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef GPDSUBTRACTIONCONSTANTMMS13_H
#define GPDSUBTRACTIONCONSTANTMMS13_H

#include <map>
#include <string>
#include <vector>

#include "../../beans/automation/BaseObjectData.h"
#include "../GPDSubtractionConstantModule.h"
#include "../MathIntegratorModule.h"

class GPDMMS13;

/**
 * Subtraction constant model based on MM13Model D-term
 */
class GPDSubtractionConstantMMS13: public GPDSubtractionConstantModule,
        public MathIntegratorModule {

public:

    static const unsigned int classId;    ///< ID assigned by BaseObjectRegistry

    /**
     * Default constructor
     * @param className Class name
     */
    GPDSubtractionConstantMMS13(const std::string &className);

    /**
     * Destructor
     */
    virtual ~GPDSubtractionConstantMMS13();

    virtual GPDSubtractionConstantMMS13* clone() const;

    virtual void resolveObjectDependencies();

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:

    /** Copy constructor
     @param other Object to be copied
     */
    GPDSubtractionConstantMMS13(const GPDSubtractionConstantMMS13& other);

    virtual double computeH();
    virtual double computeE();

private:

    /**
     * Integral of D-term
     * @param zeta GPD x/xi variable
     * @param par Additional parameters (not used here)
     */
    double dTermIntegral(double zeta, std::vector<double> par);

    /**
     * Initialize functors
     */
    void initFunctorsForIntegrations();

    NumA::FunctionType1D* m_p_int_dTermIntegral; ///< functor related to dTermIntegral() function

    GPDMMS13* m_pMMS13Model;  ///< Pointer to MMS13Model model
};

#endif /* GPDSUBTRACTIONCONSTANTMMS13_H */
