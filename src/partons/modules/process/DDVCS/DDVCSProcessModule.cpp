#include "../../../../../include/partons/modules/process/DDVCS/DDVCSProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/beans/Scales.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/scales/DDVCS/DDVCSScalesModule.h"
#include "../../../../../include/partons/modules/scales/ScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/DDVCS/DDVCSXiConverterModule.h"
#include "../../../../../include/partons/modules/xi_converter/XiConverterModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/DDVCSConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string DDVCSProcessModule::DDVCS_PROCESS_MODULE_CLASS_NAME =
        "DDVCSProcessModule";

DDVCSProcessModule::DDVCSProcessModule(const std::string &className) :
        ProcessModule(className, ChannelType::DDVCS), m_xB(0.), m_t(0.), m_Q2(
                0.), m_Q2Prim(0.), m_E(0.), m_phi(0.), m_phiL(0.), m_thetaL(0.), m_beamHelicity(
                0.), m_beamCharge(0.), m_pScaleModule(0), m_pXiConverterModule(
                0), m_pConvolCoeffFunctionModule(0) {
}

DDVCSProcessModule::~DDVCSProcessModule() {

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

DDVCSProcessModule::DDVCSProcessModule(const DDVCSProcessModule& other) :
        ProcessModule(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(
                other.m_Q2), m_Q2Prim(other.m_Q2Prim), m_E(other.m_E), m_phi(
                other.m_phi), m_phiL(other.m_phiL), m_thetaL(other.m_thetaL), m_beamHelicity(
                other.m_beamHelicity), m_beamCharge(other.m_beamCharge), m_targetPolarization(
                other.m_targetPolarization), m_pScaleModule(0), m_pXiConverterModule(
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

std::string DDVCSProcessModule::toString() const {
    ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>::toString();
    return ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>::toString();
}

void DDVCSProcessModule::resolveObjectDependencies() {
    ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>::resolveObjectDependencies();
}

void DDVCSProcessModule::run() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Users should evaluate cross-sections through respective ObservableService");
}

void DDVCSProcessModule::configure(const ElemUtils::Parameters &parameters) {
    ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>::configure(
            parameters);
}

void DDVCSProcessModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for scales module
    it = subModulesData.find(DDVCSScalesModule::DDVCS_SCALES_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newDDVCSScalesModule(
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
            DDVCSXiConverterModule::DDVCS_XI_CONVERTER_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newDDVCSXiConverterModule(
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
                        DDVCSConvolCoeffFunctionModule::DDVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

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
                        Partons::getInstance()->getModuleObjectFactory()->newDDVCSConvolCoeffFunctionModule(
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

DDVCSObservableResult DDVCSProcessModule::compute(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization,
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType> & gpdType) {
    return compute(beamHelicity, beamCharge, targetPolarization, kinematic,
            gpdType, VCSSubProcessType::ALL);
}

List<GPDType> DDVCSProcessModule::getListOfAvailableGPDTypeForComputation() const {

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

DDVCSObservableResult DDVCSProcessModule::compute(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization,
        const DDVCSObservableKinematic& kinematic, const List<GPDType>& gpdType,
        VCSSubProcessType::Type processType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //set experimental conditions
    setExperimentalConditions(beamHelicity, beamCharge, targetPolarization);

    //compute CCF
    if (m_isCCFModuleDependent)
        computeConvolCoeffFunction(kinematic, gpdType);

    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    DDVCSObservableResult result(kinematic);

    PhysicalType<double> value(0., PhysicalUnit::GEVm2);

    if (processType == VCSSubProcessType::ALL
            || processType == VCSSubProcessType::DDVCS) {
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

PhysicalType<double> DDVCSProcessModule::CrossSectionBH() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

PhysicalType<double> DDVCSProcessModule::CrossSectionVCS() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

PhysicalType<double> DDVCSProcessModule::CrossSectionInterf() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

void DDVCSProcessModule::resetPreviousKinematic() {

    m_dvcsConvolCoeffFunctionResult = DDVCSConvolCoeffFunctionResult();
    m_lastCCFKinematics = DDVCSConvolCoeffFunctionKinematic();
}

bool DDVCSProcessModule::isPreviousCCFKinematicDifferent(
        const DDVCSConvolCoeffFunctionKinematic& kinematic) const {

    return ((kinematic.getXi() != m_lastCCFKinematics.getXi())
            || (kinematic.getT() != m_lastCCFKinematics.getT())
            || (kinematic.getQ2() != m_lastCCFKinematics.getQ2())
            || (kinematic.getQ2Prim() != m_lastCCFKinematics.getQ2Prim())
            || (kinematic.getMuF2() != m_lastCCFKinematics.getMuF2())
            || (kinematic.getMuR2() != m_lastCCFKinematics.getMuR2()));
}

DDVCSScalesModule* DDVCSProcessModule::getScaleModule() const {
    return m_pScaleModule;
}

void DDVCSProcessModule::setScaleModule(DDVCSScalesModule* pScaleModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pScaleModule,
            pScaleModule);
    m_pScaleModule = pScaleModule;

    if (m_pScaleModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "DDVCSScalesModule is set to: "
                        << pScaleModule->getClassName());
    } else {
        info(__func__, "DDVCSScalesModule is set to: 0");
    }
}

DDVCSXiConverterModule* DDVCSProcessModule::getXiConverterModule() const {
    return m_pXiConverterModule;
}

void DDVCSProcessModule::setXiConverterModule(
        DDVCSXiConverterModule* pXiConverterModule) {

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

DDVCSConvolCoeffFunctionModule* DDVCSProcessModule::getConvolCoeffFunctionModule() const {
    return m_pConvolCoeffFunctionModule;
}

void DDVCSProcessModule::setConvolCoeffFunctionModule(
        DDVCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {

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

void DDVCSProcessModule::setKinematics(
        const DDVCSObservableKinematic& kinematic) {

    m_xB = kinematic.getXB().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
    m_phiL = kinematic.getPhiL().makeSameUnitAs(PhysicalUnit::RAD).getValue();
    m_thetaL =
            kinematic.getThetaL().makeSameUnitAs(PhysicalUnit::RAD).getValue();
}

void DDVCSProcessModule::setExperimentalConditions(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {

    m_beamHelicity = beamHelicity;
    m_beamCharge = beamCharge;
    m_targetPolarization = targetPolarization;
}

void DDVCSProcessModule::initModule() {

    //run for mother
    ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>::initModule();
}

void DDVCSProcessModule::isModuleWellConfigured() {

    //run for mother
    ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>::isModuleWellConfigured();

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
    if (m_xB < 0. || m_xB > 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of xB = " << m_xB
                << " does not lay between xBmin = " << 0. << " and 1";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of t
    if (m_t > 0.) {
        ElemUtils::Formatter formatter;
        formatter << " Input value of t = " << m_t << " grater than zero"
                << " (DDVCS kinematic limits)";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of Q2
    if (m_Q2 < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of Q2 = " << m_Q2 << " is not > 0";
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

    //test beam helicity
    if (fabs(m_beamHelicity) != 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Beam helicity = " << m_beamHelicity << "is not +/- 1";
        warn(__func__, formatter.str());
    }

    //test beam charge
    if (fabs(m_beamCharge) != 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Beam charge = " << m_beamCharge << "is not +/- 1";
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

void DDVCSProcessModule::computeConvolCoeffFunction(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //compute scales
    Scales scale = m_pScaleModule->compute(kinematic);

    //compute xi
    PhysicalType<double> xi = m_pXiConverterModule->compute(kinematic);

    //create ccf kinematics
    DDVCSConvolCoeffFunctionKinematic ccfKinematics(xi, kinematic.getT(),
            kinematic.getQ2(), kinematic.getQ2Prim(), scale.getMuF2(),
            scale.getMuR2());

    //check if different
    if (isPreviousCCFKinematicDifferent(ccfKinematics)) {

        //evaluate
        m_dvcsConvolCoeffFunctionResult =
                Partons::getInstance()->getServiceObjectRegistry()->getDDVCSConvolCoeffFunctionService()->computeSingleKinematic(
                        ccfKinematics, m_pConvolCoeffFunctionModule, gpdType);

        //set corresponding kinematics
        m_lastCCFKinematics = ccfKinematics;
    }
}

std::complex<double> DDVCSProcessModule::getConvolCoeffFunctionValue(
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

