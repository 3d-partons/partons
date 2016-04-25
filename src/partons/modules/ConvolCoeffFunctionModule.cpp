#include "../../../include/partons/modules/ConvolCoeffFunctionModule.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <exception>
#include <string>

#include "../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../include/partons/beans/gpd/GPDType.h"
#include "../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

//TODO change string value
const std::string ConvolCoeffFunctionModule::GPD_MODULE_ID =
        "DVCS_CONVOL_COEFF_FUNCTION_GPD_MODULE_ID";

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const std::string &className) :
        ModuleObject(className), MathIntegratorModule(), m_pGPDModule(0), m_channel(
                ObservableChannel::UNDEFINED) {
}

ConvolCoeffFunctionModule::~ConvolCoeffFunctionModule() {
}

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const ConvolCoeffFunctionModule& other) :
        ModuleObject(other), MathIntegratorModule(other) {

    m_channel = other.m_channel;

    if (other.m_pGPDModule != 0) {
        // GPDModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pGPDModule = (other.m_pGPDModule)->clone();
    } else {
        m_pGPDModule = 0;
    }
}

void ConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {
    configureIntegrator(parameters);

    if (parameters.isAvailable(ConvolCoeffFunctionModule::GPD_MODULE_ID)) {
        //TODO why create new GPDModule here ?
        // TODO passer par le setter de m_pGPDModule pour affecter le nouveau module de GPD. Car il faut détruire le précédent pointer s'il existe. Pour libérer l'allocation mémoire avant d'affecter le nouveau.

        //TODO tester l'imbrication des try/catch
        //TODO Dangereux de construire le module GPD comme ça car on ne peut pas le configurer
        try {
            m_pGPDModule =
                    Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                            parameters.getLastAvailable().toUInt());
        } catch (const std::exception &e) {
            try {
                m_pGPDModule =
                        Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                                parameters.getLastAvailable().toString());
            } catch (std::exception e) {
                error(__func__,
                        ElemUtils::Formatter()
                                << "Cannot create GPDModule from data provided with parameter = "
                                << DVCSConvolCoeffFunctionModule::GPD_MODULE_ID
                                << e.what());
            }
        }

        info(__func__,
                ElemUtils::Formatter()
                        << DVCSConvolCoeffFunctionModule::GPD_MODULE_ID
                        << " configured with value = "
                        << m_pGPDModule->getClassName());
    }

    ModuleObject::configure(parameters);
}

DVCSConvolCoeffFunctionResult ConvolCoeffFunctionModule::compute(
        const DVCSConvolCoeffFunctionKinematic &kinematic,
        GPDType::Type gpdType) {
    return compute(kinematic.getXi(), kinematic.getT(), kinematic.getQ2(),
            kinematic.getMuF2(), kinematic.getMuR2(), gpdType);
}

const GPDModule* ConvolCoeffFunctionModule::getGPDModule() const {
    return m_pGPDModule;
}

void ConvolCoeffFunctionModule::setGPDModule(GPDModule* gpdModule) {
    m_pGPDModule = gpdModule;
}

ObservableChannel::Type ConvolCoeffFunctionModule::getChannel() const {
    return m_channel;
}

void ConvolCoeffFunctionModule::setChannel(ObservableChannel::Type channel) {
    m_channel = channel;
}
