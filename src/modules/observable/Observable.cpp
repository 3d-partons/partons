#include "Observable.h"

#include "../../beans/observable/ObservableKinematic.h"
#include "../../beans/observable/ObservableResult.h"
#include "../../utils/math/MathUtils.h"
#include "../../utils/ParameterList.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../ProcessModule.h"

Observable::Observable(const std::string &className) :
        BaseObject(className), m_channel(ObservableChannel::UNDEFINED), m_beamHelicity(
                0.), m_beamCharge(0.), m_targetPolarization(
                NumA::Vector3D(0., 0., 0.)), m_observableType(
                ObservableType::PHI), m_pProcess(0) {
}

Observable::Observable(const Observable& other) :
        BaseObject(other) {
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
        observableResult.setKinematic(ObservableKinematic(xB, t, Q2, phi));

    } else
    // check if this observable is a phi observable
    if (m_observableType == ObservableType::PHI) {

        //TODO improve
        observableResult = ObservableResult(getClassName(),
                compute(m_pProcess, MathUtils::convertDegreeToRadian(phi)));
        observableResult.setComputationModuleName(m_pProcess->getClassName());
        observableResult.setObservableType(m_observableType);
        observableResult.setKinematic(ObservableKinematic(xB, t, Q2, phi));
    }

    else {
        throwException(__func__,
                Formatter() << "Unknow observable type : "
                        << ObservableType(m_observableType).toString());
    }

    return observableResult;
}

double Observable::compute(ProcessModule* pDVCSModule, double phi) {
    throwException(__func__,
            "Nothing to do ; Must be implemented in daugther class");
    return 0.;
}

double Observable::compute() {
    throwException(__func__,
            "Nothing to do ; Must be implemented in daugther class");
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
