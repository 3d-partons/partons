#include "Alu.h"

#include "../../BaseObjectRegistry.h"
#include "../../utils/vector/Vector3D.h"
#include "../ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int Alu::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new Alu("Alu"));

Alu::Alu(const std::string &className) :
        Observable(className) {
}

Alu::Alu(const Alu& other) :
        Observable(other) {
}

Alu::~Alu() {
}

Alu* Alu::clone() const {
    return new Alu(*this);
}

//TODO vérifier
//TODO !!! division par zero !!!
//TODO optimisation remplacer les multiples appels similaires par A - B / A + B
double Alu::compute(ProcessModule* pDVCSModule, double phi) {
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
