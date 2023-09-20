#include "../../../../../include/partons/modules/process/DVMP/DVMPProcessISSK15.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <complex>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../../../../../include/partons/modules/scales/DVMP/DVMPScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/beans/Scales.h"

namespace PARTONS {

const unsigned int DVMPProcessISSK15::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPProcessISSK15("DVMPProcessISSK15"));

DVMPProcessISSK15::DVMPProcessISSK15(const std::string &className) :
        DVMPProcessModule(className), m_pRunningAlphaStrongModule(0), m_W2(0.), m_xi(
                0.) {
}

DVMPProcessISSK15::DVMPProcessISSK15(const DVMPProcessISSK15& other) :
        DVMPProcessModule(other), m_W2(other.m_W2), m_xi(other.m_xi) {

    //clone alpaS module
    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }
}

DVMPProcessISSK15* DVMPProcessISSK15::clone() const {
    return new DVMPProcessISSK15(*this);
}

DVMPProcessISSK15::~DVMPProcessISSK15() {

    // destroy alphaS module
    if (m_pRunningAlphaStrongModule != 0) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrongModule = 0;
    }
}

void DVMPProcessISSK15::initModule() {

    //run for mother
    DVMPProcessModule::initModule();

    //evaluate W, gamma and epsilon
    m_W2 = pow(m_E + Constant::PROTON_MASS, 2) - pow(m_E, 2);

    //evaluate xi
    m_xi =
            getXiConverterModule()->compute(
                    DVMPObservableKinematic(m_xB, m_t, m_Q2, m_E, m_phi,
                            m_mesonType)).getValue();
}

void DVMPProcessISSK15::isModuleWellConfigured() {

    //run for mother
    ProcessModule<DVMPObservableKinematic, DVMPObservableResult>::isModuleWellConfigured();

    //check if pointer to scale module set
    if (m_pScaleModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pScaleModule is NULL pointer ; Use configure method to configure it");
    }

    //check if pointer to xi module set
    if (m_pXiConverterModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pXiConverterModule is NULL pointer ; Use configure method to configure it");
    }

    //check if pointer to cff module set
    if (isCCFModuleDependent() && m_pConvolCoeffFunctionModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pConvolCoeffFunctionModule is NULL pointer ; Use configure method to configure it");
    }

    //test kinematic domain of t
    double tmin = -4 * pow(m_xi, 2)
            / (1. - pow(m_xi, 2) * pow(Constant::PROTON_MASS, 2));
    double tmax = -1E12;

    if (m_t > tmin) {
        ElemUtils::Formatter formatter;
        formatter << " Input value of t = " << m_t
                << " does not lay between t_max = " << tmax << " and min = "
                << tmin << " (DVMP kinematic limits)";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of E
    if (m_E < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test meson type
    if (m_mesonType == MesonType::UNDEFINED) {
        ElemUtils::Formatter formatter;
        formatter << "Meson type in undefined";
        warn(__func__, formatter.str());
    }

}

PhysicalType<double> DVMPProcessISSK15::CrossSection() {

    //check meson type
    if (m_mesonType != MesonType::JPSI && m_mesonType != MesonType::UPSILON) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson "
                        << MesonType(m_mesonType).toString());
    }

    double ReH = getConvolCoeffFunctionValue(GPDType::H).real();
    double ImH = getConvolCoeffFunctionValue(GPDType::H).imag();
    double ReE = getConvolCoeffFunctionValue(GPDType::E).real();
    double ImE = getConvolCoeffFunctionValue(GPDType::E).imag();

    //prefactor
    double eq = 0.; //quark charge
    double mq = 0.; //quark mass
    double GammaEE = 0.; //meson partial width

    switch (m_mesonType) {

    case MesonType::JPSI: {
        eq = 2 / 3.;
        mq = Constant::MESON_JPSI_MASS / 2.;
        GammaEE = 5.55E-6;
        break;
    }

    case MesonType::UPSILON: {
        eq = -1 / 3.;
        mq = Constant::MESON_UPSILON_MASS / 2.;
        GammaEE = 1.34E-6; //confirm
        break;
    }

    default: {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "No implementation for meson "
                        << MesonType(m_mesonType).toString());
        break;
    }
    }

    double Nc = 3.;

    double alphaS = m_pRunningAlphaStrongModule->compute(
            m_pScaleModule->compute(
                    DVMPObservableKinematic(m_xB, m_t, m_Q2, m_E, m_phi,
                            m_mesonType)).getMuF2().getValue());
    double O1 =
            GammaEE * 3
                    / (2 * pow(eq, 2) * M_PI
                            * pow(Constant::FINE_STRUCTURE_CONSTANT, 2))
                    * pow(mq, 2) * pow(1 - 8 * alphaS / (3 * M_PI), -2);
    double prefactor = (1 / m_xi) * 4 * M_PI
            * sqrt(4 * M_PI * Constant::FINE_STRUCTURE_CONSTANT) * eq / Nc
            * sqrt(O1 / pow(mq, 3));

    //result
    double result = (1 + m_xi) / (1 - m_xi) * pow(prefactor, 2)
            / (16 * M_PI * pow(m_W2 - pow(Constant::PROTON_MASS, 2), 2))
            * ((1 - pow(m_xi, 2)) * (ReH * ReH + ImH * ImH)
                    + pow(m_xi, 4) / (1 - pow(m_xi, 2))
                            * (ReE * ReE + ImE * ImE)
                    - 2 * pow(m_xi, 2) * (ReH * ReE + ImH * ImE));

    return PhysicalType<double>(result, PhysicalUnit::GEVm2);
}

double DVMPProcessISSK15::lambdaFunction(double a, double b, double c) const {
    return pow(a, 2.0) + pow(b, 2.0) + pow(c, 2.0)
            - 2.0 * (a * b + a * c + b * c);
}

void DVMPProcessISSK15::resolveObjectDependencies() {

    //run for mother
    DVMPProcessModule::resolveObjectDependencies();

    //set alpha_s module
    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrongStandard::classId);
}

void DVMPProcessISSK15::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    DVMPProcessModule::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(
            RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        if (m_pRunningAlphaStrongModule != 0) {
            setRunningAlphaStrongModule(0);
            m_pRunningAlphaStrongModule = 0;
        }

        if (!m_pRunningAlphaStrongModule) {
            m_pRunningAlphaStrongModule =
                    Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                            (it->second).getModuleClassName());
            info(__func__,
                    ElemUtils::Formatter()
                            << "Configure with RunningAlphaStrongModule = "
                            << m_pRunningAlphaStrongModule->getClassName());
            m_pRunningAlphaStrongModule->configure(
                    (it->second).getParameters());
        }
    }
}

RunningAlphaStrongModule* DVMPProcessISSK15::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void DVMPProcessISSK15::setRunningAlphaStrongModule(
        RunningAlphaStrongModule* pRunningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(
            m_pRunningAlphaStrongModule, pRunningAlphaStrongModule);
    m_pRunningAlphaStrongModule = pRunningAlphaStrongModule;
}

} /* namespace PARTONS */
