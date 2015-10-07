#include "AluObservable.h"

#include "../../BaseObjectRegistry.h"
#include "../../modules/ObservableModule.h"
#include "../../utils/vector/Vector3D.h"

// Initialise [class]::classId with a unique name.
const unsigned int AluObservable::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new AluObservable("AluObservable"));

AluObservable::AluObservable(const std::string &className) :
        Observable(className) {
}

AluObservable::AluObservable(const AluObservable& other) :
        Observable(other) {
}

AluObservable::~AluObservable() {
}

AluObservable* AluObservable::clone() const {
    return new AluObservable(*this);
}

//TODO vérifier
double AluObservable::compute(ObservableModule* pDVCSModule, double phi) {
    double result = (pDVCSModule->computeCrossSection(+1, -1,
            Vector3D(0., 0., 0.), phi)
            - pDVCSModule->computeCrossSection(-1, -1, Vector3D(0., 0., 0.),
                    phi))
            / (pDVCSModule->computeCrossSection(+1, -1, Vector3D(0., 0., 0.),
                    phi)
                    + pDVCSModule->computeCrossSection(-1, -1,
                            Vector3D(0., 0., 0.), phi));
    return result;
}
