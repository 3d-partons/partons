#include "AcObservable.h"

#include "../../modules/ObservableModule.h"
#include "../../utils/logger/LoggerManager.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../../utils/vector/Vector3D.h"

AcObservable::AcObservable() :
        Observable("AcObservable") {
}

AcObservable::~AcObservable() {
}

AcObservable* AcObservable::clone() const {
    return new AcObservable(*this);
}

//TODO vérifier
double AcObservable::compute(ObservableModule* pDVCSModule, double phi) {

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "phi = " << phi);

    double result = 0.;

    result = ((pDVCSModule->computeCrossSection(+1, +1, Vector3D(0., 0., 0.),
            phi)
            + pDVCSModule->computeCrossSection(-1, +1, Vector3D(0., 0., 0.),
                    phi))
            - ((pDVCSModule->computeCrossSection(+1, -1, Vector3D(0., 0., 0.),
                    phi)
                    + pDVCSModule->computeCrossSection(-1, -1,
                            Vector3D(0., 0., 0.), phi))))
            / (4.
                    * (pDVCSModule->computeCrossSection(0., 0.,
                            Vector3D(0., 0., 0.), phi)));
    return result;
}
