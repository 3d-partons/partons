#ifndef CONVOL_COEFF_FUNCTION_MODULE_H
#define CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file ConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../beans/automation/BaseObjectData.h"
#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../beans/observable/ObservableChannel.h"
#include "../ModuleObject.h"
#include "MathIntegratorModule.h"

class GPDModule;

/**
 * @class ConvolCoeffFunctionModule
 *
 * @brief
 */
class ConvolCoeffFunctionModule: public ModuleObject,
        public MathIntegratorModule {
public:
    static const std::string CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    ConvolCoeffFunctionModule(const std::string &className);
    /**
     * Default destructor.
     */
    virtual ~ConvolCoeffFunctionModule();

    virtual ConvolCoeffFunctionModule* clone() const = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void run();

    /**
     * Computes the coefficient functions at given kinematics.
     * @param kinematic
     * @param gpdType Type of CCF to compute.
     * @return Complex result.
     */
    virtual std::complex<double> compute(
            const DVCSConvolCoeffFunctionKinematic &kinematic,
            GPDType::Type gpdType);

    /**
     * Computes the coefficient functions at given kinematics.
     * @param xi Skewness.
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
     * @param Q2 Virtuality of the photon in Born approximation (in GeV^2).
     * @param MuF2 Factorization scale (in GeV^2).
     * @param MuR2 Renormalization scale (in GeV^2).
     * @param gpdType Type of CCF to compute.
     * @return Complex result.
     */
    virtual std::complex<double> compute(double xi, double t, double Q2,
            double MuF2, double MuR2, GPDType::Type gpdType) = 0;

    /**
     * @brief
     *
     * Must be implemented in child class.
     * @return List of GPD/CCF types the child class can compute.
     */
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const = 0;

    // ##### GETTERS & SETTERS #####

    GPDModule* getGPDModule() const;

    void setGPDModule(GPDModule* gpdModule);

    ObservableChannel::Type getChannel() const;
    void setChannel(ObservableChannel::Type channel);
    bool isGPDModuleDependent() const;
    void setIsGPDModuleDependent(bool isGPDModuleDependent);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:
    /**
     * Copy constructor
     */
    ConvolCoeffFunctionModule(const ConvolCoeffFunctionModule &other);

    //TODO doc
    ObservableChannel::Type m_channel;

    GPDModule* m_pGPDModule;

private:
    bool m_isGPDModuleDependent;
};

#endif /* CONVOL_COEFF_FUNCTION_MODULE_H */
