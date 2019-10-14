#include "../../../include/partons/beans/MesonType.h"

namespace PARTONS {

MesonType::MesonType() :
        m_type(MesonType::UNDEFINED) {
}

MesonType::MesonType(const MesonType& other) {
    m_type = other.m_type;
}

MesonType::MesonType(Type type) :
        m_type(type) {
}
MesonType::operator MesonType::Type() const {
    return m_type;
}

std::string MesonType::toString() const {

    switch (m_type) {

    case RHOMINUS:
        return "RHOMINUS";
        break;
    case RHO0:
        return "RHO0";
        break;
    case RHOPLUS:
        return "RHOPLUS";
        break;
    case OMEGA:
        return "OMEGA";
        break;
    case PHI:
        return "PHI";
        break;
    case JPSI:
        return "JPSI";
        break;
    case UPSILON:
        return "UPSILON";
        break;

    case PIMINUS:
        return "PIMINUS";
        break;
    case PI0:
        return "PI0";
        break;
    case PIPLUS:
        return "PIPLUS";
        break;
    default:
        return "UNDEFINED";
    }
}

std::string MesonType::getShortName() {

    switch (m_type) {

    case RHOMINUS:
           return "rho-";
           break;
       case RHO0:
           return "rho0";
           break;
       case RHOPLUS:
           return "rho+";
           break;
       case OMEGA:
           return "omega";
           break;
       case PHI:
           return "phi";
           break;
       case JPSI:
           return "J/Psi";
           break;
       case UPSILON:
           return "upsilon";
           break;

       case PIMINUS:
           return "pi-";
           break;
       case PI0:
           return "pi0";
           break;
       case PIPLUS:
           return "pi+";
           break;
       default:
           return "UNDEFINED";
    }
}

MesonType::Type MesonType::getType() const {
    return m_type;
}

void MesonType::setType(Type type) {
    m_type = type;
}

MesonType::~MesonType() {
}

} /* namespace PARTONS */
