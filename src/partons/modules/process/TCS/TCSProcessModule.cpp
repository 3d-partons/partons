#include "../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/beans/Scales.h"
#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/scales/ScalesModule.h"
#include "../../../../../include/partons/modules/scales/TCS/TCSScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/TCS/TCSXiConverterModule.h"
#include "../../../../../include/partons/modules/xi_converter/XiConverterModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/TCSConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const std::string TCSProcessModule::TCS_PROCESS_MODULE_CLASS_NAME =
        "TCSProcessModule";

TCSProcessModule::TCSProcessModule(const std::string &className) :
        ProcessModule(className, ChannelType::TCS), m_t(0.), m_Q2Prim(0.), m_E(
                0.), m_phi(0.), m_theta(0.), m_beamPolarization(0.), m_tmin(0.), m_tmax(
                0.), m_pScaleModule(0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(
                0) {
}

TCSProcessModule::~TCSProcessModule() {

    if (m_pScaleModule != 0) {
        setScaleModule(0);
        m_pScaleModule = 0;
    }

    if (m_pXiConverterModule != 0) {
        setXiConverterModule(0);
        m_pXiConverterModule = 0;
    }

    if (m_pConvolCoeffFunctionModule != 0) {
        setConvolCoeffFunctionModule(0);
        m_pConvolCoeffFunctionModule = 0;
    }
}

TCSProcessModule::TCSProcessModule(const TCSProcessModule& other) :
        ProcessModule(other), m_t(other.m_t), m_Q2Prim(other.m_Q2Prim), m_E(
                other.m_E), m_phi(other.m_phi), m_theta(other.m_theta), m_beamPolarization(
                other.m_beamPolarization), m_targetPolarization(
                other.m_targetPolarization), m_tmin(other.m_tmin), m_tmax(
                other.m_tmax), m_pScaleModule(0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(
                0) {

    m_lastCCFKinematics = other.m_lastCCFKinematics;
    m_dvcsConvolCoeffFunctionResult = other.m_dvcsConvolCoeffFunctionResult;

    if (other.m_pScaleModule != 0) {
        m_pScaleModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pScaleModule);
    }

    if (other.m_pXiConverterModule != 0) {
        m_pXiConverterModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pXiConverterModule);
    }

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

    //search for scales module
    it = subModulesData.find(TCSScalesModule::TCS_SCALES_MODULE_CLASS_NAME);

    //check if there
    if (it != subModulesData.end()) {

        //check if already set
        if (m_pScaleModule) {

            setScaleModule(0);
            m_pScaleModule = 0;
        }

        //set
        if (!m_pScaleModule) {

            m_pScaleModule =
                    Partons::getInstance()->getModuleObjectFactory()->newTCSScalesModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter() << "Configured with ScaleModule = "
                            << m_pScaleModule->getClassName());

            m_pScaleModule->configure((it->second).getParameters());
            m_pScaleModule->prepareSubModules((it->second).getSubModules());
        }

    } else {

        //throw error
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << getClassName()
                        << " is ScaleModule dependent and you have not provided one");
    }

    //search for xi module
    it = subModulesData.find(
            TCSXiConverterModule::TCS_XI_CONVERTER_MODULE_CLASS_NAME);

    //check if there
    if (it != subModulesData.end()) {

        //check if already set
        if (m_pXiConverterModule) {

            setXiConverterModule(0);
            m_pXiConverterModule = 0;
        }

        //set
        if (!m_pXiConverterModule) {

            m_pXiConverterModule =
                    Partons::getInstance()->getModuleObjectFactory()->newTCSXiConverterModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter()
                            << "Configured with XiConverterModule = "
                            << m_pXiConverterModule->getClassName());

            m_pXiConverterModule->configure((it->second).getParameters());
            m_pXiConverterModule->prepareSubModules(
                    (it->second).getSubModules());
        }

    } else {

        //throw error
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << getClassName()
                        << " is XiConverterModule dependent and you have not provided one");
    }

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

TCSObservableResult TCSProcessModule::compute(double beamPolarization,
        NumA::Vector3D targetPolarization,
        const TCSObservableKinematic& kinematic,
        const List<GPDType> & gpdType) {
    return compute(beamPolarization, targetPolarization, kinematic, gpdType,
            VCSSubProcessType::ALL);
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

TCSObservableResult TCSProcessModule::compute(double beamPolarization,
        NumA::Vector3D targetPolarization,
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType,
        VCSSubProcessType::Type processType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //set experimental conditions
    setExperimentalConditions(beamPolarization, targetPolarization);

    //compute CCF
    if (m_isCCFModuleDependent)
        computeConvolCoeffFunction(kinematic, gpdType);

    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    TCSObservableResult result(kinematic);

    PhysicalType<double> value(0., PhysicalUnit::GEVm2);

    if (processType == VCSSubProcessType::ALL
            || processType == VCSSubProcessType::TCS) {
        value += CrossSectionVCS();
    }

    if (processType == VCSSubProcessType::ALL
            || processType == VCSSubProcessType::BH) {
        value += CrossSectionBH();
    }

    if (processType == VCSSubProcessType::ALL
            || processType == VCSSubProcessType::INT) {
        value += CrossSectionInterf();
    }

    //set value
    result.setValue(value);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

PhysicalType<double> TCSProcessModule::CrossSectionBH() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

PhysicalType<double> TCSProcessModule::CrossSectionVCS() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

PhysicalType<double> TCSProcessModule::CrossSectionInterf() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

void TCSProcessModule::resetPreviousKinematic() {

    m_dvcsConvolCoeffFunctionResult = TCSConvolCoeffFunctionResult();
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

TCSScalesModule* TCSProcessModule::getScaleModule() const {
    return m_pScaleModule;
}

void TCSProcessModule::setScaleModule(TCSScalesModule* pScaleModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pScaleModule,
            pScaleModule);
    m_pScaleModule = pScaleModule;

    if (m_pScaleModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "TCSScalesModule is set to: "
                        << pScaleModule->getClassName());
    } else {
        info(__func__, "TCSScalesModule is set to: 0");
    }
}

TCSXiConverterModule* TCSProcessModule::getXiConverterModule() const {
    return m_pXiConverterModule;
}

void TCSProcessModule::setXiConverterModule(
        TCSXiConverterModule* pXiConverterModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pXiConverterModule,
            pXiConverterModule);
    m_pXiConverterModule = pXiConverterModule;

    if (m_pXiConverterModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "XiConverterModule is set to: "
                        << pXiConverterModule->getClassName());
    } else {
        info(__func__, "XiConverterModule is set to: 0");
    }
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

    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
    m_theta = kinematic.getTheta().makeSameUnitAs(PhysicalUnit::RAD).getValue();
}

void TCSProcessModule::setExperimentalConditions(double beamPolarization,
        NumA::Vector3D targetPolarization) {

    m_beamPolarization = beamPolarization;
    m_targetPolarization = targetPolarization;
}

void TCSProcessModule::initModule() {

    //run for mother
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::initModule();

    //evaluate internal variables
    double s = pow(Constant::PROTON_MASS, 2) + 2. * Constant::PROTON_MASS * m_E;
    double tau = m_Q2Prim / (s - pow(Constant::PROTON_MASS, 2));
    double xi = tau / (2. - tau);

    double p1cm = m_E * Constant::PROTON_MASS / sqrt(s);
    double E3cm = (s + m_Q2Prim - pow(Constant::PROTON_MASS, 2))
            / (2 * sqrt(s));
    double p3cm = sqrt(pow(E3cm, 2) - m_Q2Prim);

    m_tmin = -4 * pow(Constant::PROTON_MASS, 2) * pow(xi, 2)
            / (1. - pow(xi, 2));
    m_tmax = m_tmin - 4 * p1cm * p3cm;
}

void TCSProcessModule::isModuleWellConfigured() {

    //run for mother
    ProcessModule<TCSObservableKinematic, TCSObservableResult>::isModuleWellConfigured();

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
    if (m_t > m_tmin || m_t < m_tmax) {
        ElemUtils::Formatter formatter;
        formatter << " Input value of t = " << m_t
                << " does not lay between t_max = " << m_tmax << " and t_min = "
                << m_tmin << " (TCS kinematic limits)";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of Q2
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

    //test if s >= (M + Q')^2
    if (2 * m_E * Constant::PROTON_MASS
            < 2 * Constant::PROTON_MASS * sqrt(m_Q2Prim) + m_Q2Prim) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " and Q2' = " << m_Q2Prim
                << " does not satisfy s >= (M + Q')^2";
        warn(__func__, formatter.str());
    }

    //test beam polarization
    if (fabs(m_beamPolarization) != 0. && fabs(m_beamPolarization) != 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Beam polarization = " << m_beamPolarization
                << "is not +/- 1 or 0";
        warn(__func__, formatter.str());
    }

    //test target polarization
    double targetMag = sqrt(
            pow(m_targetPolarization.getX(), 2)
                    + pow(m_targetPolarization.getY(), 2)
                    + pow(m_targetPolarization.getZ(), 2));

    if (targetMag != 0. && targetMag != 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Magnitude of target polarization ("
                << m_targetPolarization.toString() << ") neither 0 nor 1";
        warn(__func__, formatter.str());
    }
}

void TCSProcessModule::computeConvolCoeffFunction(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //compute scales
    Scales scale = m_pScaleModule->compute(kinematic);

    //compute xi
    PhysicalType<double> xi = m_pXiConverterModule->compute(kinematic);

    //create ccf kinematics
    TCSConvolCoeffFunctionKinematic ccfKinematics(xi, kinematic.getT(),
            kinematic.getQ2Prim(), scale.getMuF2(), scale.getMuR2());

    //check if different
    if (isPreviousCCFKinematicDifferent(ccfKinematics)) {

        //evaluate
        m_dvcsConvolCoeffFunctionResult =
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
        if (m_dvcsConvolCoeffFunctionResult.isAvailable(gpdType)) {
            result = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
        }
    } catch (std::exception &e) {
        // Nothing to do
        // If gpdType is not available for this CCFResult then continue and return 0.
    }

    return result;
}

} /* namespace PARTONS */

