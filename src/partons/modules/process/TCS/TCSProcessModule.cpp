#include "../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/scales/ScalesModule.h"
#include "../../../../../include/partons/beans/Scales.h"
#include "../../../../../include/partons/modules/xi_converter/XiConverterModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/ConvolCoeffFunctionService.h"
#include "../../../../../include/partons/services/TCSConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string TCSProcessModule::TCS_PROCESS_MODULE_CLASS_NAME =
        "TCSProcessModule";

TCSProcessModule::TCSProcessModule(const std::string &className) :
        ProcessModule(className, ChannelType::TCS), m_xB(0.), m_t(0.), m_Q2Prim(
                0.), m_E(0.), m_phi(0.), m_theta(0.), m_tmin(0.), m_tmax(0.), m_xBmin(
                0), m_y(0.), m_epsilon(0.), m_pConvolCoeffFunctionModule(0) {
}

TCSProcessModule::~TCSProcessModule() {

    if (m_pConvolCoeffFunctionModule != 0) {
        setConvolCoeffFunctionModule(0);
        m_pConvolCoeffFunctionModule = 0;
    }
}

TCSProcessModule::TCSProcessModule(const TCSProcessModule& other) :
        ProcessModule(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2Prim(
                other.m_Q2Prim), m_E(other.m_E), m_phi(other.m_phi), m_theta(
                other.m_theta), m_tmin(other.m_tmin), m_tmax(other.m_tmax), m_xBmin(
                other.m_xBmin), m_y(other.m_y), m_epsilon(other.m_epsilon), m_pConvolCoeffFunctionModule(
                0) {

    m_lastCCFKinematics = other.m_lastCCFKinematics;
    m_tcsConvolCoeffFunctionResult = other.m_tcsConvolCoeffFunctionResult;

    if (other.m_pConvolCoeffFunctionModule != 0) {
        m_pConvolCoeffFunctionModule =
                m_pModuleObjectFactory->cloneModuleObject(
                        other.m_pConvolCoeffFunctionModule);
    }
}

std::string TCSProcessModule::toString() const {
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::toString();
}

void TCSProcessModule::resolveObjectDependencies() {
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::resolveObjectDependencies();
}

void TCSProcessModule::run() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Users should evaluate cross-sections through respective ObservableService");
}

void TCSProcessModule::configure(const ElemUtils::Parameters &parameters) {
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::configure(
            parameters);
}

void TCSProcessModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //check if ccf module dependent
    if (isCCFModuleDependent()) {

        //search for ccf module
        it =
                subModulesData.find(
                        TCSConvolCoeffFunctionModule::TCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

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
                        Partons::getInstance()->getModuleObjectFactory()->newTCSConvolCoeffFunctionModule(
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

TCSObservableResult TCSProcessModule::compute(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization,
        const TCSObservableKinematic& kinematic,
        const List<GPDType> & gpdType) {
    return compute(beamHelicity, beamCharge, targetPolarization, kinematic,
            gpdType, VCSSubProcessType::ALL);
}

List<GPDType> TCSProcessModule::getListOfAvailableGPDTypeForComputation() const {

    //object to be returned
    List<GPDType> listOfAvailableGPDTypeForComputation;

    //check if CCF module dependent
    if (m_isCCFModuleDependent) {

        if (m_pConvolCoeffFunctionModule == 0) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "CCF module not set");
        }

        listOfAvailableGPDTypeForComputation =
                m_pConvolCoeffFunctionModule->getListOfAvailableGPDTypeForComputation();
    }

    //return
    return listOfAvailableGPDTypeForComputation;
}

TCSObservableResult TCSProcessModule::compute(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization,
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType,
        VCSSubProcessType::Type processType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //set experimental conditions
    setExperimentalConditions(beamHelicity, beamCharge, targetPolarization);

    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //compute CCF
    if (m_isCCFModuleDependent)
        computeConvolCoeffFunction(kinematic, gpdType);

    //object to be returned
    TCSObservableResult result(kinematic);

    PhysicalType<double> value(0., PhysicalUnit::GEVm2);

    if (processType == VCSSubProcessType::ALL
            || processType == VCSSubProcessType::TCS) {
        value += CrossSectionVCS(beamHelicity, beamCharge, targetPolarization);
    }

    if (processType == VCSSubProcessType::ALL
            || processType == VCSSubProcessType::BH) {
        value += CrossSectionBH(beamHelicity, beamCharge, targetPolarization);
    }

    if (processType == VCSSubProcessType::ALL
            || processType == VCSSubProcessType::INT) {
        value += CrossSectionInterf(beamHelicity, beamCharge,
                targetPolarization);
    }

    //set value
    result.setValue(value);

    //set type
    result.setObservableType(ObservableType::PHI);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

PhysicalType<double> TCSProcessModule::CrossSectionBH(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

PhysicalType<double> TCSProcessModule::CrossSectionVCS(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

PhysicalType<double> TCSProcessModule::CrossSectionInterf(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

void TCSProcessModule::resetPreviousKinematic() {

    m_tcsConvolCoeffFunctionResult = TCSConvolCoeffFunctionResult();
    m_lastCCFKinematics = TCSConvolCoeffFunctionKinematic();
}

bool TCSProcessModule::isPreviousCCFKinematicDifferent(
        const TCSConvolCoeffFunctionKinematic& kinematic) const {

    return ((kinematic.getXi() != m_lastCCFKinematics.getXi())
            || (kinematic.getT() != m_lastCCFKinematics.getT())
            || (kinematic.getQ2Prim() != m_lastCCFKinematics.getQ2Prim())
            || (kinematic.getMuF2() != m_lastCCFKinematics.getMuF2())
            || (kinematic.getMuR2() != m_lastCCFKinematics.getMuR2()));
}

TCSConvolCoeffFunctionModule* TCSProcessModule::getConvolCoeffFunctionModule() const {
    return m_pConvolCoeffFunctionModule;
}

void TCSProcessModule::setConvolCoeffFunctionModule(
        TCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {

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

    resetPreviousKinematic();
}

void TCSProcessModule::setKinematics(const TCSObservableKinematic& kinematic) {

    m_xB = kinematic.getXB().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
    m_theta = kinematic.getTheta().makeSameUnitAs(PhysicalUnit::RAD).getValue();
}

void TCSProcessModule::setExperimentalConditions(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {
}

void TCSProcessModule::initModule() {

    //run for mother
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::initModule();

    //evaluate internal variables
    m_epsilon = 2 * m_xB * Constant::PROTON_MASS / sqrt(m_Q2Prim); //TODO TB checked by JAKUB
    m_y = m_Q2Prim / (2 * m_xB * Constant::PROTON_MASS * m_E);
    double eps2 = m_epsilon * m_epsilon;
    double epsroot = sqrt(1 + eps2);
    double tfactor = -m_Q2Prim / (4 * m_xB * (1 - m_xB) + eps2);
    m_tmin = tfactor * (2 * (1 - m_xB) * (1 - epsroot) + eps2);
    m_tmax = tfactor * (2 * (1 - m_xB) * (1 + epsroot) + eps2);
    m_xBmin = 2 * m_Q2Prim * m_E / Constant::PROTON_MASS
            / (4 * m_E * m_E - m_Q2Prim);
}

void TCSProcessModule::isModuleWellConfigured() {

    //run for mother
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::isModuleWellConfigured();

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
                << m_tmin << " (TCS kinematic limits)";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of Q2'
    if (m_Q2Prim < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of Q2' = " << m_Q2Prim << " is not > 0";
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

void TCSProcessModule::computeConvolCoeffFunction(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //compute scales
    Scales scale = m_pScaleModule->compute(kinematic.getQ2Prim()); //TODO TB checked by JAKUB

    //compute xi
    PhysicalType<double> xi = m_pXiConverterModule->compute(kinematic.getXB(), //TODO TB checked by JAKUB
            kinematic.getT(), kinematic.getQ2Prim());

    //create ccf kinematics
    TCSConvolCoeffFunctionKinematic ccfKinematics(xi, kinematic.getT(),
            kinematic.getQ2Prim(), scale.getMuF2(), scale.getMuR2());

    //check if different
    if (isPreviousCCFKinematicDifferent(ccfKinematics)) {

        //evaluate
        m_tcsConvolCoeffFunctionResult =
                Partons::getInstance()->getServiceObjectRegistry()->getTCSConvolCoeffFunctionService()->computeSingleKinematic(
                        ccfKinematics, m_pConvolCoeffFunctionModule, gpdType);

        //set corresponding kinematics
        m_lastCCFKinematics = ccfKinematics;
    }
}

std::complex<double> TCSProcessModule::getConvolCoeffFunctionValue(
        GPDType::Type gpdType) {

    std::complex<double> result = std::complex<double>(0., 0.);

    try {
        if (m_tcsConvolCoeffFunctionResult.isAvailable(gpdType)) {
            result = m_tcsConvolCoeffFunctionResult.getLastAvailable();
        }
    } catch (std::exception &e) {
        // Nothing to do
        // If gpdType is not available for this CCFResult then continue and return 0.
    }

    return result;
}

} /* namespace PARTONS */

