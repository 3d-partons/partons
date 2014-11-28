#include "Observable.h"

//Observable::Observable(double he, double Qe, double polarity)
//        : BaseObject("Observable"), m_he(he), m_Qe(Qe), m_polarity(polarity) {
//}

Observable::Observable(std::string className, ObservableChannel::Type channel,
        double beamHelicity, double beamCharge, Vector3D targetPolarization)
        : BaseObject(className), m_channel(channel), m_beamHelicity(
                beamHelicity), m_beamCharge(beamCharge), m_targetPolarization(
                targetPolarization) {
}

Observable::Observable(const Observable& other)
        : BaseObject(other) {
    m_channel = other.m_channel;
    m_beamHelicity = other.m_beamHelicity;
    m_beamCharge = other.m_beamCharge;
    m_targetPolarization = other.m_targetPolarization;
}

//Observable* Observable::clone() const {
//    return new Observable(*this);
//}

Observable::~Observable() {
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
