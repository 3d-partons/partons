#include "../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFConstant.h"
#include "../../../../../include/partons/modules/scales/ScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/XiConverterModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/beans/Scales.h"
#include "../../../../../include/partons/services/ConvolCoeffFunctionService.h"
#include "../../../../../include/partons/services/DVCSConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

DVCSProcessModule::DVCSProcessModule(const std::string &className) :
        ProcessModule(className, ChannelType::DVCS), m_xB(0.), m_t(0.), m_Q2(
                0.), m_E(0.), m_phi(0.), m_tmin(0.), m_tmax(0.), m_xBmin(0), m_y(
                0.), m_epsilon(0.), m_pConvolCoeffFunctionModule(0) {
}

DVCSProcessModule::~DVCSProcessModule() {

    if (m_pConvolCoeffFunctionModule != 0) {
        setConvolCoeffFunctionModule(0);
        m_pConvolCoeffFunctionModule = 0;
    }
}

DVCSProcessModule::DVCSProcessModule(const DVCSProcessModule& other) :
        ProcessModule(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(
                other.m_Q2), m_E(other.m_E), m_phi(other.m_phi), m_tmin(
                other.m_tmin), m_tmax(other.m_tmax), m_xBmin(other.m_xBmin), m_y(
                other.m_y), m_epsilon(other.m_epsilon), m_pConvolCoeffFunctionModule(
                0) {

    m_lastCCFKinematics = other.m_lastCCFKinematics;
    m_dvcsConvolCoeffFunctionResult = other.m_dvcsConvolCoeffFunctionResult;

    if (other.m_pConvolCoeffFunctionModule != 0) {
        m_pConvolCoeffFunctionModule =
                (other.m_pConvolCoeffFunctionModule)->clone();
    }
}

std::string DVCSProcessModule::toString() const {
    ProcessModule<DVCSObservableKinematic>::toString();
}

void DVCSProcessModule::resolveObjectDependencies() {
    ProcessModule<DVCSObservableKinematic>::resolveObjectDependencies();
}

void DVCSProcessModule::run() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Users should evaluate cross-sections through respective ObservableService");
}

void DVCSProcessModule::configure(const ElemUtils::Parameters &parameters) {
    ProcessModule<DVCSObservableKinematic>::configure(parameters);
}

void DVCSProcessModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    ProcessModule<DVCSObservableKinematic>::prepareSubModules(subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //check if ccf module dependent
    if (isCCFModuleDependent()) {

        //search for ccf module
        it =
                subModulesData.find(
                        DVCSConvolCoeffFunctionModule::CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

        //check if there
        if (it != subModulesData.end()) {

            //check if already set
            if (m_pConvolCoeffFunctionModule) {

                setConvolCoeffFunctionModule(0);
                m_pConvolCoeffFunctionModule = 0;
            }

            //set
            if (!m_pConvolCoeffFunctionModule) {

                m_pConvolCoeffFunctionModule =
                        Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                                (it->second).getModuleClassName());

                info(__func__,
                        ElemUtils::Formatter()
                                << "Configured with ConvolCoeffFunctionModule = "
                                << m_pConvolCoeffFunctionModule->getClassName());

                m_pConvolCoeffFunctionModule->configure(
                        (it->second).getParameters());
                m_pConvolCoeffFunctionModule->prepareSubModules(
                        (it->second).getSubModules());
            }

        } else {

            //throw error
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << getClassName()
                            << " is ConvolCoeffFunctionModule dependent and you have not provided one");
        }
    }
}

double DVCSProcessModule::compute(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization,
        const DVCSObservableKinematic& kinematic) {
    return compute(beamHelicity, beamCharge, targetPolarization, kinematic,
            DVCSSubProcessType::ALL);
}

double DVCSProcessModule::compute(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization,
        const DVCSObservableKinematic& kinematic,
        DVCSSubProcessType::Type processType) {

    // reset kinematics (virtuality)
    setKinematics(kinematic);

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    double result;

    if (processType == DVCSSubProcessType::ALL
            || processType == DVCSSubProcessType::DVCS) {
        result += CrossSectionVCS(beamHelicity, beamCharge, targetPolarization);
    }

    if (processType == DVCSSubProcessType::ALL
            || processType == DVCSSubProcessType::BH) {
        result += CrossSectionBH(beamHelicity, beamCharge, targetPolarization);
    }

    if (processType == DVCSSubProcessType::ALL
            || processType == DVCSSubProcessType::INT) {
        result += CrossSectionInterf(beamHelicity, beamCharge,
                targetPolarization);
    }

    return result;
}

bool DVCSProcessModule::isPreviousCCFKinematicsDifferent(
        const DVCSConvolCoeffFunctionKinematic& kinematic) const {

    return ((kinematic.getXi() != m_lastCCFKinematics.getXi())
            || (kinematic.getT() != m_lastCCFKinematics.getT())
            || (kinematic.getQ2() != m_lastCCFKinematics.getQ2())
            || (kinematic.getMuF2() != m_lastCCFKinematics.getMuF2())
            || (kinematic.getMuR2() != m_lastCCFKinematics.getMuR2()));
}

void DVCSProcessModule::resetPrevious() {

    m_dvcsConvolCoeffFunctionResult = DVCSConvolCoeffFunctionResult();
    m_lastCCFKinematics = DVCSConvolCoeffFunctionKinematic();
}

void DVCSProcessModule::computeConvolCoeffFunction(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //compute scales
    Scales scale = m_pScaleModule->compute(kinematic.getQ2().getValue());

    //compute xi
    double xi = m_pXiConverterModule->compute(kinematic.getXB().getValue(),
            kinematic.getT().getValue(), kinematic.getQ2().getValue());

    //create ccf kinematics
    DVCSConvolCoeffFunctionKinematic ccfKinematics(xi, kinematic.getT().getValue(),
            kinematic.getQ2().getValue(), scale.getMuF2(), scale.getMuR2());

    //check if different
    if (isPreviousCCFKinematicsDifferent(ccfKinematics)
            || (BaseObjectRegistry::getInstance()->getObjectClassIdByClassName(
                    m_pConvolCoeffFunctionModule->getClassName())
                    == DVCSCFFConstant::classId)) {

        //evaluate
        m_dvcsConvolCoeffFunctionResult =
                Partons::getInstance()->getServiceObjectRegistry()->getDVCSConvolCoeffFunctionService()->computeSingleKinematic(
                        ccfKinematics, m_pConvolCoeffFunctionModule, gpdType);

        //set corresponding kinematics
        m_lastCCFKinematics = ccfKinematics;
    }
}

void DVCSProcessModule::setKinematics(
        const DVCSObservableKinematic& kinematic) {

    // set variables
    m_xB = kinematic.getXB().getValue();
    m_t = kinematic.getT().getValue();
    m_Q2 = kinematic.getQ2().getValue();
    m_E = kinematic.getE().getValue();
    m_phi = kinematic.getPhi().getValue();
}

void DVCSProcessModule::initModule() {

    //run for mother
    ProcessModule<DVCSObservableKinematic>::initModule();

    //evaluate internal variables
    m_epsilon = 2 * m_xB * Constant::PROTON_MASS / sqrt(m_Q2);
    m_y = m_Q2 / (2 * m_xB * Constant::PROTON_MASS * m_E);
    double eps2 = m_epsilon * m_epsilon;
    double epsroot = sqrt(1 + eps2);
    double tfactor = -m_Q2 / (4 * m_xB * (1 - m_xB) + eps2);
    m_tmin = tfactor * (2 * (1 - m_xB) * (1 - epsroot) + eps2);
    m_tmax = tfactor * (2 * (1 - m_xB) * (1 + epsroot) + eps2);
    m_xBmin = 2 * m_Q2 * m_E / Constant::PROTON_MASS / (4 * m_E * m_E - m_Q2);
}

void DVCSProcessModule::isModuleWellConfigured() {

    //run for mother
    ProcessModule<DVCSObservableKinematic>::isModuleWellConfigured();

    //test kinematic domain of xB
    if (m_xB < m_xBmin || m_xB > 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of xB = " << m_xB
                << " does not lay between xBmin = " << m_xBmin << " and 1";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of t
    if (m_t > m_tmin || m_t < m_tmax) {
        ElemUtils::Formatter formatter;
        formatter << " Input value of t = " << m_t
                << " does not lay between t_max = " << m_tmax << " and t_min = "
                << m_tmin << " (DVCS kinematic limits)";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of Q2
    if (m_Q2 < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of Q2 = " << m_Q2 << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of E
    if (m_E < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of beam energy
    if (m_y < 0. || m_y > 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of y = " << m_y
                << " (lepton energy fraction) does not lay between 0 and 1";
        warn(__func__, formatter.str());
    }
}

std::complex<double> DVCSProcessModule::getConvolCoeffFunctionValue(
        GPDType::Type gpdType) {
    std::complex<double> result = std::complex<double>(0., 0.);

    try {
        if (m_dvcsConvolCoeffFunctionResult.isAvailable(gpdType)) {
            result = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
        }
    } catch (std::exception &e) {
        // Nothing to do
        // If gpdType is not available for this CCFResult then continue and return 0.
    }

    return result;
}

DVCSConvolCoeffFunctionModule* DVCSProcessModule::getConvolCoeffFunctionModule() const {
    return m_pConvolCoeffFunctionModule;
}

void DVCSProcessModule::setConvolCoeffFunctionModule(
        DVCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {

    m_pModuleObjectFactory->updateModulePointerReference(
            m_pConvolCoeffFunctionModule, pConvolCoeffFunctionModule);
    m_pConvolCoeffFunctionModule = pConvolCoeffFunctionModule;

    if (m_pConvolCoeffFunctionModule != 0) {
        info(__func__,
                ElemUtils::Formatter()
                        << "ConvolCoeffFunctionModule is set to: "
                        << pConvolCoeffFunctionModule->getClassName());
    } else {
        info(__func__, "ConvolCoeffFunctionModule is set to: 0");
    }

    resetPrevious();
}

} /* namespace PARTONS */

