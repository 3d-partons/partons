#include "Observable.h"

#include "../../modules/ObservableModule.h"
#include "../../utils/math/MathUtils.h"
#include "../../utils/stringUtils/Formatter.h"
#include "ObservableResult.h"
#include "ObservableResultList.h"

Observable::Observable(const std::string &className) :
        BaseObject(className), m_channel(ObservableChannel::UNDEFINED), m_beamHelicity(
                0.), m_beamCharge(0.), m_targetPolarization(
                Vector3D(0., 0., 0.)), m_observableType(ObservableType::PHI), m_pDVCSModule(
                0) {
}

Observable::Observable(const Observable& other) :
        BaseObject(other) {
    m_channel = other.m_channel;
    m_beamHelicity = other.m_beamHelicity;
    m_beamCharge = other.m_beamCharge;
    m_targetPolarization = other.m_targetPolarization;
    m_observableType = other.m_observableType;

    if (m_pDVCSModule != 0) {
        m_pDVCSModule = other.m_pDVCSModule->clone();
    } else {
        m_pDVCSModule = 0;
    }
}

Observable::~Observable() {
}

ObservableResultList Observable::compute(double xB, double t, double Q2,
        std::vector<double> listOfPhi) {

    ObservableResultList observableResultList;

    //TODO replace hard coded value
    double MuF = 4., MuR = 4.;

    m_pDVCSModule->computeConvolCoeffFunction(xB, t, Q2, MuF, MuR);

    // if listOfPhi empty then run computation of fourrier observable
    if (listOfPhi.empty()) {
        // check if this observable is a fourrier observable
        if (m_observableType == ObservableType::FOURRIER) {
            observableResultList.add(ObservableResult(compute()));
        }
        // else throw exception
        else {
            throwException(__func__,
                    Formatter()
                            << "Cannot perform fourrier computation with a phi dependencies observable");
        }
    }
    // else run computation of observable with phi dependencies
    else {
        // check if this observable is a fourrier observable
        if (m_observableType == ObservableType::PHI) {
            for (unsigned int i = 0; i != listOfPhi.size(); i++) {
                observableResultList.add(
                        ObservableResult(listOfPhi[i],
                                compute(m_pDVCSModule,
                                        MathUtils::convertDegreeToRadian(
                                                listOfPhi[i]))));
            }
        }
        // else throw exception
        else {
            throwException(__func__,
                    Formatter()
                            << "Cannot perform phi dependencies observable computation with fourrier observable");
        }
    }

    return observableResultList;
}

double Observable::compute(ObservableModule* pDVCSModule, double phi) {
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

const Vector3D& Observable::getTargetPolarization() const {
    return m_targetPolarization;
}

void Observable::setTargetPolarization(const Vector3D& targetPolarization) {
    m_targetPolarization = targetPolarization;
}

const ObservableModule* Observable::getDVCSModule() const {
    return m_pDVCSModule;
}

void Observable::setDVCSModule(ObservableModule* pDVCSModule) {
    m_pDVCSModule = pDVCSModule;
}
