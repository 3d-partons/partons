#include "../../../../include/partons/modules/observable/Observable.h"

#include <NumA/utils/MathUtils.h>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <exception>
#include <iostream>

#include "../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../include/partons/modules/ProcessModule.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/ObservableService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../include/partons/utils/ParameterList.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../../include/partons/utils/thread/Packet.h"
#include "../../../../include/partons/utils/type/PhysicalType.h"

Observable::Observable(const std::string &className) :
        ModuleObject(className), m_channel(ObservableChannel::UNDEFINED), m_beamHelicity(
                0.), m_beamCharge(0.), m_targetPolarization(
                NumA::Vector3D(0., 0., 0.)), m_observableType(
                ObservableType::PHI), m_pProcess(0) {
}

Observable::Observable(const Observable& other) :
        ModuleObject(other) {
    m_channel = other.m_channel;
    m_beamHelicity = other.m_beamHelicity;
    m_beamCharge = other.m_beamCharge;
    m_targetPolarization = other.m_targetPolarization;
    m_observableType = other.m_observableType;

    if (other.m_pProcess != 0) {
        m_pProcess = other.m_pProcess->clone();
    } else {
        m_pProcess = 0;
    }
}

Observable::~Observable() {
}

void Observable::initModule() {

}

void Observable::isModuleWellConfigured() {

}

void Observable::run() {

    try {

        // Retrieve Observable service
        ObservableService* pObservableService =
                Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

        while (!(pObservableService->isEmptyTaskQueue())) {
            ObservableKinematic observableKinematic;
            Packet packet = pObservableService->popTaskFormQueue();
            packet >> observableKinematic;

            pObservableService->add(compute(observableKinematic));

//        info(__func__,
//                Formatter() << "[Thread] id = " << getThreadId() << " "
//                        << result.toString());

//TODO replace by standard sleep ; multiplatform
//usleep(30000);

            sf::sleep(sf::milliseconds(3));
        }

//    info(__func__,
//            Formatter() << "[Thread] id = " << getThreadId()
//                    << " empty task list, terminated.");

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}

ObservableResult Observable::compute(const ObservableKinematic &kinematic) {

    info(__func__, Formatter() << getObjectId() << " " << kinematic.toString());

    return compute(kinematic.getXB(), kinematic.getT(), kinematic.getQ2(),
            kinematic.getPhi().getValue());
}

ObservableResult Observable::compute(double xB, double t, double Q2,
        double phi) {

    ObservableResult observableResult;

    m_pProcess->computeConvolCoeffFunction(xB, t, Q2);

    // check if this observable is a fourier observable
    if (m_observableType == ObservableType::FOURIER) {

        //TODO improve
        observableResult = ObservableResult(getClassName(), compute());
        observableResult.setComputationModuleName(m_pProcess->getClassName());
        observableResult.setObservableType(m_observableType);
        observableResult.setKinematic(ObservableKinematic(xB, t, Q2));

    } else
    // check if this observable is a phi observable
    if (m_observableType == ObservableType::PHI) {

        //TODO improve
        observableResult = ObservableResult(getClassName(),
                compute(m_pProcess,
                        NumA::MathUtils::convertDegreeToRadian(phi)));
        observableResult.setComputationModuleName(m_pProcess->getClassName());
        observableResult.setObservableType(m_observableType);
        observableResult.setKinematic(ObservableKinematic(xB, t, Q2, phi));
    }

    else {
        error(__func__,
                Formatter() << "Unknow observable type : "
                        << ObservableType(m_observableType).toString());
    }

    return observableResult;
}

double Observable::compute(ProcessModule* pDVCSModule, double phi) {
    error(__func__, "Nothing to do ; Must be implemented in daugther class");
    return 0.;
}

double Observable::compute() {
    error(__func__, "Nothing to do ; Must be implemented in daugther class");
    return 0.;

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

const ProcessModule* Observable::getDVCSModule() const {
    return m_pProcess;
}

void Observable::setDVCSModule(ProcessModule* pDVCSModule) {
    m_pProcess = pDVCSModule;
}

void Observable::configure(ParameterList parameters) {
}
