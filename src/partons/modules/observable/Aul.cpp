#include "../../../../include/partons/modules/observable/Aul.h"
#include <NumA/linear_algebra/vector/Vector3D.h>
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int Aul::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new Aul("Aul"));

Aul::Aul(const std::string &className) :
        Observable(className) {
}

Aul::Aul(const Aul& other) :
        Observable(other) {
}

Aul::~Aul() {
}

Aul* Aul::clone() const {
    return new Aul(*this);
}

//numerator

double Aul::Num(ProcessModule* pDVCSModule, double phi){

    double A = pDVCSModule->computeCrossSection(0, -1,
              NumA::Vector3D(0., 0., +1.), phi);

      double B = pDVCSModule->computeCrossSection(0, -1,
              NumA::Vector3D(0., 0., -1.), phi);
return A - B;
}

double Aul::Den(ProcessModule* pDVCSModule, double phi){

    double A = pDVCSModule->computeCrossSection(0, -1,
              NumA::Vector3D(0., 0., +1.), phi);

      double B = pDVCSModule->computeCrossSection(0, -1,
              NumA::Vector3D(0., 0., -1.), phi);
return A + B;
}


// optimisation remplacement des multiples appels similaires par A - B / A + B
double Aul::compute(ProcessModule* pDVCSModule, double phi) {

    double result = 0.;

/*
    double A = pDVCSModule->computeCrossSection(0, -1,
            NumA::Vector3D(0., 0., +1.), phi);

    double B = pDVCSModule->computeCrossSection(0, -1,
            NumA::Vector3D(0., 0., -1.), phi);
*/

    //TODO !!! division par zero !!!
    result = this->Num(pDVCSModule,phi)/this->Den(pDVCSModule,phi);

    return result;
}
