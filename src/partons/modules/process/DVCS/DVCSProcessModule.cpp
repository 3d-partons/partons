#include "../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/beans/Scales.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/DVCSConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string DVCSProcessModule::DVCS_PROCESS_MODULE_CLASS_NAME =
        "DVCSProcessModule";

DVCSProcessModule::DVCSProcessModule(const std::string &className) :
        ProcessModule(className, ChannelType::DVCS), m_xB(0.), m_t(0.), m_Q2(
                0.), m_E(0.), m_phi(0.), m_tmin(0.), m_tmax(0.), m_xBmin(0), m_y(
                0.), m_epsilon(0.), m_pScaleModule(0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(
                0) {
}

DVCSProcessModule::~DVCSProcessModule() {

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

DVCSProcessModule::DVCSProcessModule(const DVCSProcessModule& other) :
        ProcessModule(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(
                other.m_Q2), m_E(other.m_E), m_phi(other.m_phi), m_tmin(
                other.m_tmin), m_tmax(other.m_tmax), m_xBmin(other.m_xBmin), m_y(
                other.m_y), m_epsilon(other.m_epsilon), m_pScaleModule(0), m_pXiConverterModule(
                0), m_pConvolCoeffFunctionModule(0) {

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

std::string DVCSProcessModule::toString() const {
    ProcessModule<DVCSObservableKinematic, DVCSObservableResult>::toString();
}

void DVCSProcessModule::resolveObjectDependencies() {
    ProcessModule<DVCSObservableKinematic, DVCSObservableResult>::resolveObjectDependencies();
}

void DVCSProcessModule::run() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Users should evaluate cross-sections through respective ObservableService");
}

void DVCSProcessModule::configure(const ElemUtils::Parameters &parameters) {
    ProcessModule<DVCSObservableKinematic, DVCSObservableResult>::configure(
            parameters);
}

void DVCSProcessModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    ProcessModule<DVCSObservableKinematic, DVCSObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for scales module
    it = subModulesData.find(DVCSScalesModule::DVCS_SCALES_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newDVCSScalesModule(
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
    it = subModulesData.find(XiConverterModule::XI_CONVERTER_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(
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
                        DVCSConvolCoeffFunctionModule::DVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

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

DVCSObservableResult DVCSProcessModule::compute(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization,
        const DVCSObservableKinematic& kinematic,
        const List<GPDType> & gpdType) {
    return compute(beamHelicity, beamCharge, targetPolarization, kinematic,
            gpdType, VCSSubProcessType::ALL);
}

List<GPDType> DVCSProcessModule::getListOfAvailableGPDTypeForComputation() const {

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

DVCSObservableResult DVCSProcessModule::compute(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization,
        const DVCSObservableKinematic& kinematic, const List<GPDType>& gpdType,
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
    DVCSObservableResult result(kinematic);

    PhysicalType<double> value(0., PhysicalUnit::GEVm2);

    if (processType == VCSSubProcessType::ALL
            || processType == VCSSubProcessType::DVCS) {
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

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

PhysicalType<double> DVCSProcessModule::CrossSectionBH(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

PhysicalType<double> DVCSProcessModule::CrossSectionVCS(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

PhysicalType<double> DVCSProcessModule::CrossSectionInterf(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

void DVCSProcessModule::resetPreviousKinematic() {

    m_dvcsConvolCoeffFunctionResult = DVCSConvolCoeffFunctionResult();
    m_lastCCFKinematics = DVCSConvolCoeffFunctionKinematic();
}

bool DVCSProcessModule::isPreviousCCFKinematicDifferent(
        const DVCSConvolCoeffFunctionKinematic& kinematic) const {

    return ((kinematic.getXi() != m_lastCCFKinematics.getXi())
            || (kinematic.getT() != m_lastCCFKinematics.getT())
            || (kinematic.getQ2() != m_lastCCFKinematics.getQ2())
            || (kinematic.getMuF2() != m_lastCCFKinematics.getMuF2())
            || (kinematic.getMuR2() != m_lastCCFKinematics.getMuR2()));
}

DVCSScalesModule* DVCSProcessModule::getScaleModule() const {
    return m_pScaleModule;
}

void DVCSProcessModule::setScaleModule(DVCSScalesModule* pScaleModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pScaleModule,
            pScaleModule);
    m_pScaleModule = pScaleModule;

    if (m_pScaleModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "DVCSScalesModule is set to: "
                        << pScaleModule->getClassName());
    } else {
        info(__func__, "DVCSScalesModule is set to: 0");
    }
}

XiConverterModule* DVCSProcessModule::getXiConverterModule() const {
    return m_pXiConverterModule;
}

void DVCSProcessModule::setXiConverterModule(
        XiConverterModule* pXiConverterModule) {

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

    resetPreviousKinematic();
}

void DVCSProcessModule::setKinematics(
        const DVCSObservableKinematic& kinematic) {

    m_xB = kinematic.getXB().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
}

void DVCSProcessModule::setExperimentalConditions(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {
}

void DVCSProcessModule::initModule() {

    //run for mother
    ProcessModule<DVCSObservableKinematic, DVCSObservableResult>::initModule();

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
    ProcessModule<DVCSObservableKinematic, DVCSObservableResult>::isModuleWellConfigured();

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

void DVCSProcessModule::computeConvolCoeffFunction(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //compute scales
    Scales scale = m_pScaleModule->compute(kinematic);

    //compute xi
    PhysicalType<double> xi;// = m_pXiConverterModule->compute(kinematic);

    //create ccf kinematics
    DVCSConvolCoeffFunctionKinematic ccfKinematics(xi, kinematic.getT(),
            kinematic.getQ2(), scale.getMuF2(), scale.getMuR2());

    //check if different
    if (isPreviousCCFKinematicDifferent(ccfKinematics)) {

        //evaluate
        m_dvcsConvolCoeffFunctionResult =
                Partons::getInstance()->getServiceObjectRegistry()->getDVCSConvolCoeffFunctionService()->computeSingleKinematic(
                        ccfKinematics, m_pConvolCoeffFunctionModule, gpdType);

        //set corresponding kinematics
        m_lastCCFKinematics = ccfKinematics;
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

} /* namespace PARTONS */

