#include "../../../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"

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
#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/scales/DVMP/DVMPScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/DVMPConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/utils/VectorUtils.h"

#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06.h"
#include "../../../../../include/partons/modules/gpd/GPDGK19.h"
#include "../../../../../include/partons/modules/scales/DVMP/DVMPScalesQ2Multiplier.h"
#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterXBToXi.h"

namespace PARTONS {

const std::string DVMPProcessModule::DVMP_PROCESS_MODULE_CLASS_NAME =
        "DVMPProcessModule";

DVMPProcessModule::DVMPProcessModule(const std::string &className) :
        ProcessModule(className, ChannelType::DVMP), m_xB(0.), m_t(0.), m_Q2(
                0.), m_E(0.), m_phi(0.), m_mesonType(MesonType::UNDEFINED), m_beamHelicity(
                0.), m_beamCharge(0.), m_mesonPolarization(
                PolarizationType::UNDEFINED), m_tmin(0.), m_tmax(0.), m_xBmin(
                0), m_y(0.), m_pScaleModule(0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(
                0) {
}

DVMPProcessModule::~DVMPProcessModule() {

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

DVMPProcessModule::DVMPProcessModule(const DVMPProcessModule& other) :
        ProcessModule(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(
                other.m_Q2), m_E(other.m_E), m_phi(other.m_phi), m_mesonType(
                other.m_mesonType), m_beamHelicity(other.m_beamHelicity), m_beamCharge(
                other.m_beamCharge), m_targetPolarization(
                other.m_targetPolarization), m_mesonPolarization(
                other.m_mesonPolarization), m_tmin(other.m_tmin), m_tmax(
                other.m_tmax), m_xBmin(other.m_xBmin), m_y(other.m_y), m_pScaleModule(
                0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(0) {

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

std::string DVMPProcessModule::toString() const {
    return ProcessModule<DVMPObservableKinematic, DVMPObservableResult>::toString();
}

void DVMPProcessModule::resolveObjectDependencies() {
    ProcessModule<DVMPObservableKinematic, DVMPObservableResult>::resolveObjectDependencies();
}

void DVMPProcessModule::run() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Users should evaluate cross-sections through respective ObservableService");
}

void DVMPProcessModule::configure(const ElemUtils::Parameters &parameters) {
    ProcessModule<DVMPObservableKinematic, DVMPObservableResult>::configure(
            parameters);
}

void DVMPProcessModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    ProcessModule<DVMPObservableKinematic, DVMPObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for scales module
    it = subModulesData.find(DVMPScalesModule::DVMP_SCALES_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newDVMPScalesModule(
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
            DVMPXiConverterModule::DVMP_XI_CONVERTER_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newDVMPXiConverterModule(
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
                        DVMPConvolCoeffFunctionModule::DVMP_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

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
                        Partons::getInstance()->getModuleObjectFactory()->newDVMPConvolCoeffFunctionModule(
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

List<GPDType> DVMPProcessModule::getListOfAvailableGPDTypeForComputation() const {

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

std::vector<double> DVMPProcessModule::test() {

    std::vector<double> result;

    PARTONS::DVMPConvolCoeffFunctionModule* pDVMPCFFModel =
        PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVMPConvolCoeffFunctionModule(
                PARTONS::DVMPCFFGK06::classId);

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    pDVMPCFFModel->setGPDModule(pGPDModule);

    pDVMPCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    setConvolCoeffFunctionModule(pDVMPCFFModel);

    PARTONS::DVMPXiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVMPXiConverterModule(
                    PARTONS::DVMPXiConverterXBToXi::classId);
    setXiConverterModule(pXiConverterModule);

    PARTONS::DVMPScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVMPScalesModule(
                    PARTONS::DVMPScalesQ2Multiplier::classId);
   setScaleModule(pScalesModule);

    result.push_back(compute(1, 1, NumA::Vector3D(0.,0.,0.),PolarizationType::UNDEFINED, DVMPObservableKinematic(0.2, -0.1, 2., 6., M_PI/2., MesonType::PI0),
                        pDVMPCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    result.push_back(compute(1, 1, NumA::Vector3D(0.,0.,0.), PolarizationType::UNDEFINED, DVMPObservableKinematic(0.02, -0.5, 16., 1000., M_PI/4., MesonType::PI0),
                        pDVMPCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
               pScalesModule, 0);
    pScalesModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
         pXiConverterModule, 0);
    pXiConverterModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
        pGPDModule, 0);
    pGPDModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVMPCFFModel, 0);
    pDVMPCFFModel = 0;

    return result;
}

DVMPObservableResult DVMPProcessModule::compute(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization,
        PolarizationType::Type mesonPolarization,
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //set experimental conditions
    setExperimentalConditions(beamHelicity, beamCharge, targetPolarization,
            mesonPolarization);

    //compute CCF
    if (m_isCCFModuleDependent)
        computeConvolCoeffFunction(kinematic, mesonPolarization, gpdType);

    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    DVMPObservableResult result(kinematic);

    PhysicalType<double> value = CrossSection();

    //set value
    result.setValue(value);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

void DVMPProcessModule::resetPreviousKinematic() {

    m_dvcsConvolCoeffFunctionResult = DVMPConvolCoeffFunctionResult();
    m_lastCCFKinematics = DVMPConvolCoeffFunctionKinematic();
}

bool DVMPProcessModule::isPreviousCCFKinematicDifferent(
        const DVMPConvolCoeffFunctionKinematic& kinematic) const {

    return ((kinematic.getXi() != m_lastCCFKinematics.getXi())
            || (kinematic.getT() != m_lastCCFKinematics.getT())
            || (kinematic.getQ2() != m_lastCCFKinematics.getQ2())
            || (kinematic.getMuF2() != m_lastCCFKinematics.getMuF2())
            || (kinematic.getMuR2() != m_lastCCFKinematics.getMuR2())
            || (kinematic.getMesonType() != m_lastCCFKinematics.getMesonType()));
}

PhysicalType<double> DVMPProcessModule::CrossSection() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

DVMPScalesModule* DVMPProcessModule::getScaleModule() const {
    return m_pScaleModule;
}

void DVMPProcessModule::setScaleModule(DVMPScalesModule* pScaleModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pScaleModule,
            pScaleModule);
    m_pScaleModule = pScaleModule;

    if (m_pScaleModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "DVMPScalesModule is set to: "
                        << pScaleModule->getClassName());
    } else {
        info(__func__, "DVMPScalesModule is set to: 0");
    }
}

DVMPXiConverterModule* DVMPProcessModule::getXiConverterModule() const {
    return m_pXiConverterModule;
}

void DVMPProcessModule::setXiConverterModule(
        DVMPXiConverterModule* pXiConverterModule) {

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

DVMPConvolCoeffFunctionModule* DVMPProcessModule::getConvolCoeffFunctionModule() const {
    return m_pConvolCoeffFunctionModule;
}

void DVMPProcessModule::setConvolCoeffFunctionModule(
        DVMPConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {

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

void DVMPProcessModule::setKinematics(
        const DVMPObservableKinematic& kinematic) {

    m_xB = kinematic.getXB().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
    m_mesonType = kinematic.getMesonType();
}

void DVMPProcessModule::setExperimentalConditions(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization,
        PolarizationType::Type mesonPolarization) {

    m_beamHelicity = beamHelicity;
    m_beamCharge = beamCharge;
    m_targetPolarization = targetPolarization;
    m_mesonPolarization = mesonPolarization;
}

void DVMPProcessModule::initModule() {

    //run for mother
    ProcessModule<DVMPObservableKinematic, DVMPObservableResult>::initModule();

    //evaluate internal variables
    m_y = m_Q2 / (2 * m_xB * Constant::PROTON_MASS * m_E);
    m_xBmin = 2 * m_Q2 * m_E / Constant::PROTON_MASS / (4 * m_E * m_E - m_Q2);

    double nu = m_Q2 / (2 * Constant::PROTON_MASS * m_xB);

    double E_e_TAR = m_E;
    double E_eS_TAR = E_e_TAR - nu;

    double cosTheta_eS_TAR = 1. - m_Q2 / (2 * E_e_TAR * E_eS_TAR);
    double sinTheta_eS_TAR = sqrt(1. - pow(cosTheta_eS_TAR, 2));

    double s4 = pow(E_e_TAR - E_eS_TAR + Constant::PROTON_MASS, 2)
            - pow(E_e_TAR - E_eS_TAR * cosTheta_eS_TAR, 2)
            - pow(-E_eS_TAR * sinTheta_eS_TAR, 2);

    double m1_2 = -m_Q2;
    double m2_2 = pow(Constant::PROTON_MASS, 2);
    double m3_2 = pow(MesonType(m_mesonType).getMass(), 2);
    double m4_2 = pow(Constant::PROTON_MASS, 2);

    double E1cm = (s4 + m1_2 - m2_2) / (2 * sqrt(s4));
    double E3cm = (s4 + m3_2 - m4_2) / (2 * sqrt(s4));

    double p1cm = sqrt(pow(E1cm, 2) - m1_2);
    double p3cm = sqrt(pow(E3cm, 2) - m3_2);

    m_tmin = pow((m1_2 - m3_2 - m2_2 + m4_2) / (2 * sqrt(s4)), 2)
            - pow(p1cm - p3cm, 2);
    m_tmax = pow((m1_2 - m3_2 - m2_2 + m4_2) / (2 * sqrt(s4)), 2)
            - pow(p1cm + p3cm, 2);
}

void DVMPProcessModule::isModuleWellConfigured() {

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
                << m_tmin << " (DVMP kinematic limits)";
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

    //test kmeson type
    if (m_mesonType == MesonType::UNDEFINED) {
        ElemUtils::Formatter formatter;
        formatter << "Meson type in undefined";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of beam energy
    if (m_y < 0. || m_y > 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of y = " << m_y
                << " (lepton energy fraction) does not lay between 0 and 1";
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

void DVMPProcessModule::computeConvolCoeffFunction(
        const DVMPObservableKinematic& kinematic,
        PolarizationType::Type mesonPolarization,
        const List<GPDType>& gpdType) {

    //compute scales
    Scales scale = m_pScaleModule->compute(kinematic);

    //compute xi
    PhysicalType<double> xi = m_pXiConverterModule->compute(kinematic);

    //create ccf kinematics
    DVMPConvolCoeffFunctionKinematic ccfKinematics(xi, kinematic.getT(),
            kinematic.getQ2(), scale.getMuF2(), scale.getMuR2(),
            kinematic.getMesonType(), mesonPolarization);

    //check if different
    if (isPreviousCCFKinematicDifferent(ccfKinematics)) {

        //possible GPD types
        List<GPDType> gpdTypePossible = VectorUtils::intersection(gpdType,
                MesonType(kinematic.getMesonType()).getPossibleGPDTypes());

        //evaluate
        m_dvcsConvolCoeffFunctionResult =
                Partons::getInstance()->getServiceObjectRegistry()->getDVMPConvolCoeffFunctionService()->computeSingleKinematic(
                        ccfKinematics, m_pConvolCoeffFunctionModule,
                        gpdTypePossible);

        //set corresponding kinematics
        m_lastCCFKinematics = ccfKinematics;
    }
}

std::complex<double> DVMPProcessModule::getConvolCoeffFunctionValue(
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

void DVMPProcessModule::setConvolCoeffFunction(
        const DVMPConvolCoeffFunctionKinematic& kin,
        const DVMPConvolCoeffFunctionResult& result) {

    m_lastCCFKinematics = kin;
    m_dvcsConvolCoeffFunctionResult = result;
}

} /* namespace PARTONS */

