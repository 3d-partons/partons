#include "../../../../include/partons/modules/observable/Alu.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int Alu::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new Alu("Alu"));

Alu::Alu(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

Alu::Alu(const Alu& other) :
        Observable(other) {
}

Alu::~Alu() {
}

Alu* Alu::clone() const {
    return new Alu(*this);
}

// optimisation remplacement des multiples appels similaires par A - B / A + B
double Alu::compute(double phi) {

    double result = 0.;

    double A = m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., 0.), phi);

    //TODO !!! division par zero !!!
    result = (A - B) / (A + B);

    return result;
}
