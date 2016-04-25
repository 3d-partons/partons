#include "../../../../include/partons/modules/observable/Ac.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int Ac::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new Ac("Ac"));

Ac::Ac(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

Ac::Ac(const Ac& other) :
        Observable(other) {
}

Ac::~Ac() {
}

Ac* Ac::clone() const {
    return new Ac(*this);
}

//TODO vérifier
//TODO !!! division par zero !!!
double Ac::compute(ProcessModule* pDVCSModule, double phi) {

    debug(__func__, ElemUtils::Formatter() << "phi = " << phi);

    double result = 0.;

    result = ((pDVCSModule->computeCrossSection(+1, +1,
            NumA::Vector3D(0., 0., 0.), phi)
            + pDVCSModule->computeCrossSection(-1, +1,
                    NumA::Vector3D(0., 0., 0.), phi))
            - ((pDVCSModule->computeCrossSection(+1, -1,
                    NumA::Vector3D(0., 0., 0.), phi)
                    + pDVCSModule->computeCrossSection(-1, -1,
                            NumA::Vector3D(0., 0., 0.), phi))))
            / (4.
                    * (pDVCSModule->computeCrossSection(0., 0.,
                            NumA::Vector3D(0., 0., 0.), phi)));
    return result;
}
