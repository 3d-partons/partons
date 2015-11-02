#include "ObservableKinematic.h"

#include <stddef.h>

#include "../../utils/GenericType.h"
#include "../../utils/ParameterList.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../../utils/stringUtils/StringUtils.h"

const std::string ObservableKinematic::PARAMETER_NAME_XB = "xB";
const std::string ObservableKinematic::PARAMETER_NAME_T = "t";
const std::string ObservableKinematic::PARAMETER_NAME_Q2 = "Q2";
const std::string ObservableKinematic::PARAMETER_NAME_PHI = "phi";

ObservableKinematic::ObservableKinematic() :
        BaseObject("ObservableKinematic"), m_xB(0.), m_t(0.), m_Q2(0.) {
}

ObservableKinematic::ObservableKinematic(ParameterList &parameterList) :
        BaseObject("ObservableKinematic"), m_xB(0.), m_t(0.), m_Q2(0.) {
    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_XB)) {
        m_xB = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_XB << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_T << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_Q2)) {
        m_Q2 = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_Q2 << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_PHI)) {

        std::string temp_str = parameterList.getLastAvailable().toString();
        if (!temp_str.empty()) {
            std::vector<std::string> listOfPhi_str = StringUtils::split(
                    parameterList.getLastAvailable().toString(), '|');

            for (unsigned int i = 0; i != listOfPhi_str.size(); i++) {
                m_listOfPhi.push_back(
                        StringUtils::fromStringToDouble(listOfPhi_str[i]));
            }
        }
    }
//    else {
//        throwException(__func__,
//                Formatter() << "Missing parameter <"
//                        << ObservableKinematic::PARAMETER_NAME_PHI << ">");
//    }

}

ObservableKinematic::ObservableKinematic(double xB, double t, double Q2,
        std::vector<double> listOfPhi) :
        BaseObject("ObservableKinematic"), m_xB(xB), m_t(t), m_Q2(Q2), m_listOfPhi(
                listOfPhi) {
}

ObservableKinematic::ObservableKinematic(const std::string &xB,
        const std::string &t, const std::string &Q2,
        const std::string &listOfPhi) :
        BaseObject("ObservableKinematic"), m_xB(
                StringUtils::fromStringToDouble(xB)), m_t(
                StringUtils::fromStringToDouble(t)), m_Q2(
                StringUtils::fromStringToDouble(Q2)) {
    std::vector<std::string> listOfPhiValue = StringUtils::split(listOfPhi,
            ';');
    if (!listOfPhiValue.empty()) {
        for (size_t i = 0; i != listOfPhiValue.size(); i++) {
            m_listOfPhi.push_back(
                    StringUtils::fromStringToDouble(listOfPhiValue[i]));
        }
    }
}

ObservableKinematic::~ObservableKinematic() {
}

std::string ObservableKinematic::toString() {
    Formatter formatter;

    formatter << toStringWithoutPhi() << '\n';

    for (unsigned int i = 0; i < m_listOfPhi.size(); i++) {
        formatter << "m_phi = " << m_listOfPhi[i] << "\n";
    }

    return formatter;
}

std::string ObservableKinematic::toStringWithoutPhi() {
    Formatter formatter;

    formatter << "m_xB = " << m_xB << " m_t = " << m_t << " (GeV2) m_Q2 = "
            << m_Q2 << " (GeV2)";

    return formatter;
}

// ##### GETTERS & SETTERS #####

const std::vector<double>& ObservableKinematic::getListOfPhi() const {
    return m_listOfPhi;
}

void ObservableKinematic::setListOfPhi(const std::vector<double>& listOfPhi) {
    m_listOfPhi = listOfPhi;
}

double ObservableKinematic::getQ2() const {
    return m_Q2;
}

void ObservableKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
}

double ObservableKinematic::getT() const {
    return m_t;
}

void ObservableKinematic::setT(double t) {
    m_t = t;
}

double ObservableKinematic::getXB() const {
    return m_xB;
}

void ObservableKinematic::setXB(double xB) {
    m_xB = xB;
}
