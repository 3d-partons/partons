#include "CrossSectionObservable.h"

#include "../../BaseObjectRegistry.h"
#include "../../utils/vector/Vector3D.h"
#include "../ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int CrossSectionObservable::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new CrossSectionObservable("CrossSectionObservable"));

CrossSectionObservable::CrossSectionObservable(const std::string &className) :
        Observable(className) {
}

CrossSectionObservable::CrossSectionObservable(
        const CrossSectionObservable& other) :
        Observable(other) {
}

CrossSectionObservable::~CrossSectionObservable() {
}

CrossSectionObservable* CrossSectionObservable::clone() const {
    return new CrossSectionObservable(*this);
}

double CrossSectionObservable::compute(ProcessModule* pDVCSModule, double phi) {
    double result = pDVCSModule->computeCrossSection(+1, -1,
            Vector3D(0., 1., 0.), phi);
    return result;
}
