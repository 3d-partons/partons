#ifndef CONVOL_COEFF_FUNCTION_MODULE_H
#define CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file ConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <string>

#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/observable/ObservableChannel.h"
#include "../ModuleObject.h"
#include "MathIntegratorModule.h"

class DVCSConvolCoeffFunctionResult;
class GPDModule;

/**
 * @class ConvolCoeffFunctionModule
 *
 * @brief
 */
class ConvolCoeffFunctionModule: public ModuleObject,
        public MathIntegratorModule {
public:
    static const std::string GPD_MODULE_ID;

    ConvolCoeffFunctionModule(const std::string &className);
    virtual ~ConvolCoeffFunctionModule();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual ConvolCoeffFunctionModule* clone() const = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void run();

    virtual DVCSConvolCoeffFunctionResult compute(
            const DVCSConvolCoeffFunctionKinematic &kinematic,
            GPDType::Type gpdType);

    virtual DVCSConvolCoeffFunctionResult compute(double xi, double t,
            double Q2, double MuF2, double MuR2, GPDType::Type gpdType) = 0;

    // ##### GETTERS & SETTERS #####

    GPDModule* getGPDModule() const;
    void setGPDModule(GPDModule* gpdModule);
    ObservableChannel::Type getChannel() const;
    void setChannel(ObservableChannel::Type channel);
    bool isGPDModuleDependent() const;
    void setIsGPDModuleDependent(bool isGPDModuleDependent);

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
