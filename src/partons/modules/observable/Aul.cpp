#include "../../../../include/partons/modules/observable/Aul.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int Aul::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new Aul("Aul"));

Aul::Aul(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

Aul::Aul(const Aul& other) :
        Observable(other) {
}

Aul::~Aul() {
}

Aul* Aul::clone() const {
    return new Aul(*this);
}

double Aul::compute(double phi) {

    double result = 0.;

    double A = m_pProcessModule->computeCrossSection(0, -1,
            NumA::Vector3D(0., 0., +1.), phi);

    double B = m_pProcessModule->computeCrossSection(0, -1,
            NumA::Vector3D(0., 0., -1.), phi);

    //TODO !!! division par zero !!!
    result = (A - B) / (A + B);

    return result;
}
