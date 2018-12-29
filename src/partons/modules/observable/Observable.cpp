#include "../../../../include/partons/modules/observable/Observable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/utils/MathUtils.h>
#include <iostream>
#include <utility>

#include "../../../../include/partons/modules/process/ProcessModule.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/ObservableService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {


Observable::Observable(const std::string &className) :
        ModuleObject(className), m_channel(ChannelType::UNDEFINED), m_beamHelicity(
                0.), m_beamCharge(0.), m_targetPolarization(
                NumA::Vector3D(0., 0., 0.)), m_observableType(
                ObservableType::PHI), m_pProcessModule(0) {
}

Observable::Observable(const Observable& other) :
        ModuleObject(other) {
    m_channel = other.m_channel;
    m_beamHelicity = other.m_beamHelicity;
    m_beamCharge = other.m_beamCharge;
    m_targetPolarization = other.m_targetPolarization;
    m_observableType = other.m_observableType;

    if (other.m_pProcessModule != 0) {
        m_pProcessModule = other.m_pProcessModule->clone();
    } else {
        m_pProcessModule = 0;
    }
}

Observable::~Observable() {
    if (m_pProcessModule != 0) {
        setProcessModule(0);
        m_pProcessModule = 0;
    }
}

void Observable::initModule() {
//TODO implement
}

void Observable::isModuleWellConfigured() {
    //TODO implement
}

void Observable::run() {

    try {

        // Retrieve Observable service
        ObservableService* pObservableService =
                Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

        while (!(pObservableService->isEmptyTaskQueue())) {
            ObservableKinematic kinematic;
            List<GPDType> gpdType;
            ElemUtils::Packet packet = pObservableService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdType;

            info(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            pObservableService->add(compute(kinematic, gpdType));

            // sf::sleep(sf::milliseconds(3));
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}

ObservableResult Observable::compute(const ObservableKinematic &kinematic,
        const List<GPDType> & gpdType) {

    ObservableResult result = compute(kinematic.getXB(), kinematic.getT(),
            kinematic.getQ2(), kinematic.getE(), kinematic.getPhi().getValue(),
            gpdType);
    result.setKinematic(kinematic);

    return result;
}

ObservableResult Observable::compute(double xB, double t, double Q2, double E,
        double phi, const List<GPDType> & gpdType) {

    debug(__func__,
            ElemUtils::Formatter() << "Processing ... xB = " << xB << " t = "
                    << t << " Q2 = " << Q2 << " E = " << E << " phi = " << phi);

    ObservableResult observableResult;

    m_pProcessModule->computeConvolCoeffFunction(xB, t, Q2, E, gpdType);

    // check if this observable is a fourier observable
    if (m_observableType == ObservableType::FOURIER) {

        //TODO improve
        observableResult = ObservableResult(getClassName(),
                computeFourierObservable());
        observableResult.setComputationModuleName(
                m_pProcessModule->getClassName());
        observableResult.setObservableType(m_observableType);
        observableResult.setKinematic(ObservableKinematic(xB, t, Q2, E));

    } else
    // check if this observable is a phi observable
    if (m_observableType == ObservableType::PHI) {

        //TODO improve
        observableResult = ObservableResult(getClassName(),
                computePhiObservable(
                        NumA::MathUtils::convertDegreeToRadian(phi)));
        observableResult.setComputationModuleName(
                m_pProcessModule->getClassName());
        observableResult.setObservableType(m_observableType);
        observableResult.setKinematic(ObservableKinematic(xB, t, Q2, E, phi));
    }

    else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Unknow observable type : "
                        << ObservableType(m_observableType).toString());
    }

    return observableResult;
}

double Observable::computePhiObservable(double phi) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Nothing to do ; Must be implemented in daugther class");
    return 0.;
}

double Observable::computeFourierObservable() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Nothing to do ; Must be implemented in daugther class");
    return 0.;

}

//double Observable::Num(ProcessModule* pDVCSModule, double phi) {
//    error(__func__, "Nothing to do ; Must be implemented in daugther class");
//    return 0.;
//}
//
//double Observable::Den(ProcessModule* pDVCSModule, double phi) {
//    error(__func__, "Nothing to do ; Must be implemented in daugther class");
//    return 0.;
//}

void Observable::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    ModuleObject::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(ProcessModule::PROCESS_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {
        if (m_pProcessModule) {
            setProcessModule(0);
            m_pProcessModule = 0;
        }
        if (!m_pProcessModule) {
            m_pProcessModule =
                    Partons::getInstance()->getModuleObjectFactory()->newProcessModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter() << "Configured with ProcessModule = "
                            << m_pProcessModule->getClassName());

            m_pProcessModule->configure((it->second).getParameters());

            m_pProcessModule->prepareSubModules((it->second).getSubModules());
        }
    }
}

// ##### GETTERS & SETTERS #####

double Observable::getBeamCharge() const {
    return m_beamCharge;
}

void Observable::setBeamCharge(double beamCharge) {
    m_beamCharge = beamCharge;
}

double Observable::getBeamHelicity() const {
    return m_beamHelicity;
}

void Observable::setBeamHelicity(double beamHelicity) {
    m_beamHelicity = beamHelicity;
}

const NumA::Vector3D& Observable::getTargetPolarization() const {
    return m_targetPolarization;
}

void Observable::setTargetPolarization(
        const NumA::Vector3D& targetPolarization) {
    m_targetPolarization = targetPolarization;
}

ProcessModule* Observable::getProcessModule() const {
    return m_pProcessModule;
}

void Observable::setProcessModule(ProcessModule* pProcessModule) {
    m_pModuleObjectFactory->updateModulePointerReference(m_pProcessModule,
            pProcessModule);
    m_pProcessModule = pProcessModule;

    if (m_pProcessModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "ProcessModule is set to : "
                        << m_pProcessModule->getClassName());
    } else {
        info(__func__, "ProcessModule is set to : 0");
    }
}

void Observable::configure(const ElemUtils::Parameters &parameters) {
    ModuleObject::configure(parameters);
}

ChannelType::Type Observable::getChannel() const {
    return m_channel;
}

void Observable::setChannel(ChannelType::Type channel) {
    m_channel = channel;
}

} /* namespace PARTONS */
