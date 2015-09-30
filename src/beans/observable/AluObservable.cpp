#include "AluObservable.h"

#include "../../modules/ObservableModule.h"
#include "../../utils/vector/Vector3D.h"

AluObservable::AluObservable() :
        Observable("AluObservable") {
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
