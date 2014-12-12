#include "AluObservable.h"

#include <typeinfo>

#include "../../modules/observable/DVCSModule.h"
#include "../../utils/vector/Vector3D.h"
#include "ObservableChannel.h"

AluObservable::AluObservable()
        : Observable(typeid(*this).name(), ObservableChannel::DVCS, 0., 0.,
                Vector3D(0., 0., 0.)) {
}

AluObservable::~AluObservable() {
}

AluObservable* AluObservable::clone() const {
    return new AluObservable(*this);
}

double AluObservable::compute(DVCSModule* pDVCSModule) {
    double result = (pDVCSModule->computeCrossSection(+1, -1, Vector3D(0., 0., 0.))
            - pDVCSModule->computeCrossSection(-1, -1, Vector3D(0., 0., 0.)))
            / (pDVCSModule->computeCrossSection(+1, -1, Vector3D(0., 0., 0.))
                    + pDVCSModule->computeCrossSection(-1, -1,
                            Vector3D(0., 0., 0.)));
    return result;
}
