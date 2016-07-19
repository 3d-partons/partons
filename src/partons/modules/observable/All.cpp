#include "../../../../include/partons/modules/observable/All.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int All::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new All("All"));

All::All(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

All::All(const All& other) :
        Observable(other) {
}

All::~All() {
}

All* All::clone() const {
    return new All(*this);
}


double All::compute(double phi) {

    double result = 0.;




    double SigmaPP =  m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., +1.0), phi);

    double SigmaMM =  m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., -1.0), phi);

    double SigmaMP =  m_pProcessModule->computeCrossSection(-1, -1,
            NumA::Vector3D(0., 0., +1.0), phi);

    double SigmaPM =  m_pProcessModule->computeCrossSection(+1, -1,
            NumA::Vector3D(0., 0., -1.0), phi);

    //TODO !!! division par zero !!!
    result = (SigmaPP + SigmaMM - SigmaMP - SigmaPM) / (SigmaPP + SigmaMM + SigmaMP + SigmaPM);


    return result;
}





