#include "../../../include/partons/modules/ConvolCoeffFunctionModule.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>
#include <exception>
#include <iostream>

#include "../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/services/ConvolCoeffFunctionService.h"
#include "../../../include/partons/ServiceObjectRegistry.h"
#include "../../../include/partons/ServiceObjectTyped.h"

ConvolCoeffFunctionModule::ConvolCoeffFunctionModule(
        const std::string &className) :
        ModuleObject(className), m_isGPDModuleDependent(true), MathIntegratorModule(), m_pGPDModule(
                0), m_channel(ObservableChannel::UNDEFINED) {
}

ConvolCoeffFunctionModule::~ConvolCoeffFunctionModule() {
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
            GPDType gpdType;

            ElemUtils::Packet packet = pService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdType;

            info(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            pService->add(compute(kinematic, gpdType));

            //TODO useful to do a sleep ?
            // sf::sleep(sf::milliseconds(3));
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

DVCSConvolCoeffFunctionResult ConvolCoeffFunctionModule::compute(
        const DVCSConvolCoeffFunctionKinematic &kinematic,
        GPDType::Type gpdType) {
    DVCSConvolCoeffFunctionResult result = compute(kinematic.getXi(),
            kinematic.getT(), kinematic.getQ2(), kinematic.getMuF2(),
            kinematic.getMuR2(), gpdType);
    result.setKinematic(kinematic);

    return result;
}

GPDModule* ConvolCoeffFunctionModule::getGPDModule() const {
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

bool ConvolCoeffFunctionModule::isGPDModuleDependent() const {
    return m_isGPDModuleDependent;
}

void ConvolCoeffFunctionModule::setIsGPDModuleDependent(
        bool isGPDModuleDependent) {
    m_isGPDModuleDependent = isGPDModuleDependent;
}
