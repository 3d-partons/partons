#include "../../../include/partons/modules/ConvolCoeffFunctionModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>
#include <iostream>
#include <utility>

#include "../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/services/ConvolCoeffFunctionService.h"
#include "../../../include/partons/ServiceObjectRegistry.h"
#include "../../../include/partons/ServiceObjectTyped.h"

const std::string ConvolCoeffFunctionModule::CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME =
        "ConvolCoeffFunctionModule";

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const std::string &className) :
        ModuleObject(className), m_isGPDModuleDependent(true), MathIntegratorModule(), m_pGPDModule(
                0), m_channel(ObservableChannel::UNDEFINED) {
}

ConvolCoeffFunctionModule::~ConvolCoeffFunctionModule() {
    // Remove reference to our current GPDModule pointer.
    setGPDModule(0);
}

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const ConvolCoeffFunctionModule& other) :
        ModuleObject(other), MathIntegratorModule(other) {

    m_isGPDModuleDependent = other.m_isGPDModuleDependent;
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
    // Propagate parameters to the IntegrationModule object to configure integration routine.
    configureIntegrator(parameters);

    ModuleObject::configure(parameters);
}

void ConvolCoeffFunctionModule::run() {
    try {
        ConvolCoeffFunctionService* pService =
                Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

        while (!(pService->isEmptyTaskQueue())) {
            DVCSConvolCoeffFunctionKinematic kinematic;
            List<GPDType> gpdTypeList;

            ElemUtils::Packet packet = pService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdTypeList;

            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            DVCSConvolCoeffFunctionResult result;
            result.setKinematic(kinematic);
            result.setComputationModuleName(getClassName());

            //Helpful to sort later if kinematic is coming from database
            result.setIndexId(kinematic.getIndexId());

            for (unsigned int i = 0; i != gpdTypeList.size(); i++) {
                result.add(gpdTypeList[i].getType(),
                        compute(kinematic, gpdTypeList[i].getType()));
            }

            pService->add(result);

            //TODO useful to do a sleep ?
            // sf::sleep(sf::milliseconds(3));
        }
    } catch (std::exception &e) {
        //TODO remove and improve
        std::cerr << e.what() << std::endl;
    }
}

std::complex<double> ConvolCoeffFunctionModule::compute(
        const DVCSConvolCoeffFunctionKinematic &kinematic,
        GPDType::Type gpdType) {

    return compute(kinematic.getXi(), kinematic.getT(), kinematic.getQ2(),
            kinematic.getMuF2(), kinematic.getMuR2(), gpdType);
}

GPDModule* ConvolCoeffFunctionModule::getGPDModule() const {
    return m_pGPDModule;
}

void ConvolCoeffFunctionModule::setGPDModule(GPDModule* gpdModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pGPDModule,
            gpdModule);
    m_pGPDModule = gpdModule;
}

ObservableChannel::Type ConvolCoeffFunctionModule::getChannel() const {
    return m_channel;
}

void ConvolCoeffFunctionModule::setChannel(ObservableChannel::Type channel) {
    m_channel = channel;
}

bool ConvolCoeffFunctionModule::isGPDModuleDependent() const {
    return m_isGPDModuleDependent;
}

void ConvolCoeffFunctionModule::setIsGPDModuleDependent(
        bool isGPDModuleDependent) {
    m_isGPDModuleDependent = isGPDModuleDependent;
}

void ConvolCoeffFunctionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    ModuleObject::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    if (isGPDModuleDependent()) {
        it = subModulesData.find(GPDModule::GPD_MODULE_CLASS_NAME);

        if (it != subModulesData.end()) {
            if (m_pGPDModule != 0) {
                setGPDModule(0);
            }
            if (m_pGPDModule == 0) {
                m_pGPDModule =
                        Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                                (it->second).getModuleClassName());

                info(__func__,
                        ElemUtils::Formatter() << "Configured with GPDModule = "
                                << m_pGPDModule->getClassName());

                m_pGPDModule->configure((it->second).getParameters());

                m_pGPDModule->prepareSubModules((it->second).getSubModules());
            }
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << getClassName()
                            << " is GPDModule dependent and you have not provided one");
        }
    }
}
