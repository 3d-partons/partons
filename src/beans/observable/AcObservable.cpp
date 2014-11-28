#include "AcObservable.h"

#include "../../utils/vector/Vector3D.h"
#include "ObservableChannel.h"

AcObservable::AcObservable()
        : Observable("AcObservable", ObservableChannel::DVCS, 0., 0.,
                Vector3D(0., 0., 0.)) {
}

AcObservable::~AcObservable() {
}

AcObservable* AcObservable::clone() const {
    return new AcObservable(*this);
}

//TODO implement
double AcObservable::compute(DVCSModule* pDVCSModule)
{
    return 0.;
}
