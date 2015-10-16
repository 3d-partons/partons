#include "Ac.h"

#include "../../BaseObjectRegistry.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../../utils/vector/Vector3D.h"
#include "../ObservableModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int Ac::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new Ac("Ac"));

Ac::Ac(const std::string &className) :
        Observable(className) {
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
double Ac::compute(ObservableModule* pDVCSModule, double phi) {

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
