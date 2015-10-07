#include "AcObservable.h"

//#include <string>

#include "../../BaseObjectRegistry.h"
#include "../../modules/ObservableModule.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../../utils/vector/Vector3D.h"

// Initialise [class]::classId with a unique name.
const unsigned int AcObservable::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AcObservable("AcObservable"));

AcObservable::AcObservable(const std::string &className) :
        Observable(className) {
}

AcObservable::AcObservable(const AcObservable& other) :
        Observable(other) {
}

AcObservable::~AcObservable() {
}

AcObservable* AcObservable::clone() const {
    return new AcObservable(*this);
}

//TODO vérifier
double AcObservable::compute(ObservableModule* pDVCSModule, double phi) {

    debug(__func__, Formatter() << "phi = " << phi);

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
