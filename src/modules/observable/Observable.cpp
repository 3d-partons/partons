#include "Observable.h"

#include "../../beans/observable/ObservableResult.h"
#include "../../beans/observable/ObservableResultList.h"
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

ObservableResultList Observable::compute(double xB, double t, double Q2,
        std::vector<double> listOfPhi) {

    ObservableResultList observableResultList;

    m_pProcess->computeConvolCoeffFunction(xB, t, Q2);

    // check if this observable is a fourier observable
    if (m_observableType == ObservableType::FOURIER) {

        //TODO improve
        ObservableResult observableResult(compute());
        observableResult.setObservableType(m_observableType);
        observableResultList.add(observableResult);
    } else
    // check if this observable is a phi observable
    if (m_observableType == ObservableType::PHI) {
        // if listOfPhi not empty then run computation of phi observable
        if (!listOfPhi.empty()) {
            for (unsigned int i = 0; i != listOfPhi.size(); i++) {

                //TODO improve
                ObservableResult observableResult(listOfPhi[i],
                        compute(m_pProcess,
                                MathUtils::convertDegreeToRadian(
                                        listOfPhi[i])));
                observableResult.setObservableType(m_observableType);
                observableResultList.add(observableResult);
            }
        } else {
            throwException(__func__,
                    Formatter()
                            << "The list of phi is empty ; You must provide at least one phi value to compute a phi dependencies observable");
        }
    } else {
        throwException(__func__,
                Formatter() << "Unknow observable type : "
                        << ObservableType(m_observableType).toString());
    }

    return observableResultList;
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
