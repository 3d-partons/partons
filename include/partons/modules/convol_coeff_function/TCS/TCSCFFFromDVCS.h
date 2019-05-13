#ifndef TCS_CFF_FROM_DVCS_H
#define TCS_CFF_FROM_DVCS_H

/**
 * @file TCSCFFFromDVCS.h
 * @author Oskar Grocholski (UW/NCBJ)
 * @date September 08, 2014
 * @version 1.0
 */

#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "TCSConvolCoeffFunctionModule.h"
#include "../DVCS/DVCSConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class TCSCFFFromDVCS
 *
 * This module calculates TCS Compton Form Factors using DVCS CFFs
 *
 * Available CFF types: H, E, Ht, Et.
 */
class TCSCFFFromDVCS: public TCSConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    TCSCFFFromDVCS(const std::string &className);

    virtual TCSCFFFromDVCS* clone() const;

    /**
     * Default destructor.
     */
    virtual ~TCSCFFFromDVCS();

    virtual void resolveObjectDependencies();

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Set DVCSConvolCoeffFunctionModule.
     */
    void setDVCSConvolCoeffFunctionModule(
            DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule);

    /**
     * Get DVCSConvolCoeffFunctionModule.
     */
    DVCSConvolCoeffFunctionModule* getDVCSConvolCoeffFunctionModule() const;

protected:

    /**
     * Copy constructor.
     * @param other
     */
    TCSCFFFromDVCS(const TCSCFFFromDVCS &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

private:

    /**
     * Pointer to DVCSConvolCoeffFunctionModule to be used.
     */
    DVCSConvolCoeffFunctionModule* m_pDVCSConvolCoeffFunctionModule;
};

} /* namespace PARTONS */

#endif /* TCS_CFF_FROM_DVCS_H */
