#include "../../../../../include/partons/modules/process/JET/JETProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/beans/Scales.h"
#include "../../../../../include/partons/modules/convol_coeff_function/JET/JETConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/scales/ScalesModule.h"
#include "../../../../../include/partons/modules/scales/JET/JETScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/JET/JETXiConverterModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/JETConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

#include "../../../../../include/partons/modules/convol_coeff_function/JET/JETCFFStandard.h"
#include "../../../../../include/partons/modules/gpd/GPDGK19.h"
#include "../../../../../include/partons/modules/scales/JET/JETScalesMQPerp2zQ2.h"
#include "../../../../../include/partons/modules/xi_converter/JET/JETXiConverterXBToXi.h"

namespace PARTONS {

const std::string JETProcessModule::JET_PROCESS_MODULE_CLASS_NAME =
        "JETProcessModule";

JETProcessModule::JETProcessModule(const std::string &className) :
        ProcessModule(className, ChannelType::JET),
        m_xB(0.), m_t(0.), m_z(0.), m_qPerp2(0.), m_Q2(0.), m_E(0.), m_phi(0.), m_jetType(JetType::UNDEFINED),
        m_beamHelicity(0.), m_beamCharge(0.),
        m_xBmin(0.), m_y(0.),
        m_pScaleModule(0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(0) {
}

JETProcessModule::~JETProcessModule() {

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

JETProcessModule::JETProcessModule(const JETProcessModule& other) :
        ProcessModule(other),
        m_xB(other.m_xB), m_t(other.m_t), m_z(other.m_z), m_qPerp2(other.m_qPerp2), m_Q2(other.m_Q2), m_E(other.m_E), m_phi(other.m_phi), m_jetType(other.m_jetType),
        m_beamHelicity(other.m_beamHelicity), m_beamCharge(other.m_beamCharge),
        m_xBmin(other.m_xBmin), m_y(other.m_y),
        m_pScaleModule(0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(0) {

    m_lastCCFKinematics = other.m_lastCCFKinematics;
    m_jetConvolCoeffFunctionResult = other.m_jetConvolCoeffFunctionResult;

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

void JETProcessModule::run() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Users should evaluate cross-sections through respective ObservableService");
}

void JETProcessModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    ProcessModule<JETObservableKinematic, JETObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for scales module
    it = subModulesData.find(JETScalesModule::JET_SCALES_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newJETScalesModule(
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
            JETXiConverterModule::JET_XI_CONVERTER_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newJETXiConverterModule(
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
                        JETConvolCoeffFunctionModule::JET_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

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
                        Partons::getInstance()->getModuleObjectFactory()->newJETConvolCoeffFunctionModule(
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

JETObservableResult JETProcessModule::compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization,
            const JETObservableKinematic& kinematic,
            const List<GPDType>& gpdType) {

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
    JETObservableResult result(kinematic);

    //set value
    result.setValue(CrossSection());

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

PhysicalType<double> JETProcessModule::CrossSection() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

List<GPDType> JETProcessModule::getListOfAvailableGPDTypeForComputation() const {

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

std::vector<double> JETProcessModule::test() {

    std::vector<double> result;
    //
    // PARTONS::JETConvolCoeffFunctionModule* pJETCFFModel =
    //     PARTONS::Partons::getInstance()->getModuleObjectFactory()->newJETConvolCoeffFunctionModule(
    //             PARTONS::JETCFFStandard::classId);
    //
    // GPDModule *pGPDModule =
    //         Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
    //             GPDGK16::classId);
    // pJETCFFModel->setGPDModule(pGPDModule);
    //
    // pJETCFFModel->configure(
    //         ElemUtils::Parameter(
    //                 PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
    //                 PARTONS::PerturbativeQCDOrderType::LO));
    //
    // setConvolCoeffFunctionModule(pJETCFFModel);
    //
    // PARTONS::JETXiConverterModule* pXiConverterModule =
    //         PARTONS::Partons::getInstance()->getModuleObjectFactory()->newJETXiConverterModule(
    //                 PARTONS::JETXiConverterTauToXi::classId);
    // setXiConverterModule(pXiConverterModule);
    //
    // PARTONS::JETScalesModule* pScalesModule =
    //         PARTONS::Partons::getInstance()->getModuleObjectFactory()->newJETScalesModule(
    //                 PARTONS::JETScalesQ2PrimMultiplier::classId);
    // setScaleModule(pScalesModule);
    //
    // result.push_back(compute(1, NumA::Vector3D(0., 0., 0.), JETObservableKinematic( -0.1, 2., 6., M_PI/2.,M_PI/4.),
    //                     pJETCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    // result.push_back(compute(1, NumA::Vector3D(0., 0., 0.), JETObservableKinematic(-0.5, 16., 1000., M_PI/4., M_PI/2.),
    //                     pJETCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //            pScalesModule, 0);
    // pScalesModule = 0;
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //      pXiConverterModule, 0);
    // pXiConverterModule = 0;
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //     pGPDModule, 0);
    // pGPDModule = 0;
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //         pJETCFFModel, 0);
    // pJETCFFModel = 0;
    //
    return result;
}

void JETProcessModule::resetPreviousKinematic() {

    m_jetConvolCoeffFunctionResult.clear();
    m_lastCCFKinematics = JETConvolCoeffFunctionKinematic();
}

bool JETProcessModule::isPreviousCCFKinematicDifferent(
        const JETConvolCoeffFunctionKinematic& kinematic) const {

    return ((kinematic.getXi() != m_lastCCFKinematics.getXi())
            || (kinematic.getT() != m_lastCCFKinematics.getT())
            || (kinematic.getZ() != m_lastCCFKinematics.getZ())
            || (kinematic.getQPerp2() != m_lastCCFKinematics.getQPerp2())
            || (kinematic.getQ2() != m_lastCCFKinematics.getQ2())
            || (kinematic.getMuF2() != m_lastCCFKinematics.getMuF2())
            || (kinematic.getMuR2() != m_lastCCFKinematics.getMuR2())
            || (kinematic.getJetType() != m_lastCCFKinematics.getJetType()));
}

JETScalesModule* JETProcessModule::getScaleModule() const {
    return m_pScaleModule;
}

void JETProcessModule::setScaleModule(JETScalesModule* pScaleModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pScaleModule,
            pScaleModule);
    m_pScaleModule = pScaleModule;

    if (m_pScaleModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "JETScalesModule is set to: "
                        << pScaleModule->getClassName());
    } else {
        info(__func__, "JETScalesModule is set to: 0");
    }
}

JETXiConverterModule* JETProcessModule::getXiConverterModule() const {
    return m_pXiConverterModule;
}

void JETProcessModule::setXiConverterModule(
        JETXiConverterModule* pXiConverterModule) {

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

JETConvolCoeffFunctionModule* JETProcessModule::getConvolCoeffFunctionModule() const {
    return m_pConvolCoeffFunctionModule;
}

void JETProcessModule::setConvolCoeffFunctionModule(
        JETConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {

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

void JETProcessModule::setKinematics(const JETObservableKinematic& kinematic) {

    m_xB = kinematic.getXB().getValue();
    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_z = kinematic.getZ().getValue();
    m_qPerp2 = kinematic.getQPerp2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
    m_jetType = kinematic.getJetType();
}

void JETProcessModule::setExperimentalConditions(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization) {

    m_beamHelicity = beamHelicity;
    m_beamCharge = beamCharge;
    m_targetPolarization = targetPolarization;
}

void JETProcessModule::initModule() {

    //run for mother
    ProcessModule<JETObservableKinematic, JETObservableResult>::initModule();

    //evaluate internal variables
    m_y = m_Q2 / (2 * m_xB * Constant::PROTON_MASS * m_E);
    m_xBmin = 2 * m_Q2 * m_E / Constant::PROTON_MASS / (4 * m_E * m_E - m_Q2);
}

void JETProcessModule::isModuleWellConfigured() {

    //run for mother
    ProcessModule<JETObservableKinematic, JETObservableResult>::isModuleWellConfigured();

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

    // //test kinematic domain of t
    // if (m_t > m_tmin || m_t < m_tmax) {
    //     ElemUtils::Formatter formatter;
    //     formatter << " Input value of t = " << m_t
    //             << " does not lay between t_max = " << m_tmax << " and t_min = "
    //             << m_tmin << " (DVCS kinematic limits)";
    //     warn(__func__, formatter.str());
    // }

    //test z
    if (m_z < 0. || m_z > 1.) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of z = " << m_z
                        << " is not between 0. and 1.");
    }

    //test qPerp2
    if (m_qPerp2 < 0.) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of qPerp2 = " << m_qPerp2
                        << " is not > 0.");
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

    //test jet type
    if (m_jetType == JetType::UNDEFINED) {
        warn(__func__,
                ElemUtils::Formatter() << "Jet type is undefined");
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

void JETProcessModule::computeConvolCoeffFunction(
        const JETObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //compute scales
    Scales scale = m_pScaleModule->compute(kinematic);

    //compute xi
    PhysicalType<double> xi = m_pXiConverterModule->compute(kinematic);

    //create ccf kinematics
    JETConvolCoeffFunctionKinematic ccfKinematics(xi, kinematic.getT(), kinematic.getZ(), kinematic.getQPerp2(),
            kinematic.getQ2(), scale.getMuF2(), scale.getMuR2(),kinematic.getJetType(), JetCFFType::UNDEFINED);

    //check if different
    if (isPreviousCCFKinematicDifferent(ccfKinematics)) {

        //clear
        m_jetConvolCoeffFunctionResult.clear();

        //evaluate
        ccfKinematics.setJetCFFType(JetCFFType::LL);
        m_jetConvolCoeffFunctionResult.insert(std::make_pair(
            JetCFFType::LL,
            Partons::getInstance()->getServiceObjectRegistry()->getJETConvolCoeffFunctionService()->
                computeSingleKinematic(ccfKinematics, m_pConvolCoeffFunctionModule, gpdType)
            ));

        ccfKinematics.setJetCFFType(JetCFFType::TL);
        m_jetConvolCoeffFunctionResult.insert(std::make_pair(
            JetCFFType::TL,
            Partons::getInstance()->getServiceObjectRegistry()->getJETConvolCoeffFunctionService()->
                computeSingleKinematic(ccfKinematics, m_pConvolCoeffFunctionModule, gpdType)
            ));

        ccfKinematics.setJetCFFType(JetCFFType::TT1);
        m_jetConvolCoeffFunctionResult.insert(std::make_pair(
            JetCFFType::TT1,
            Partons::getInstance()->getServiceObjectRegistry()->getJETConvolCoeffFunctionService()->
                computeSingleKinematic(ccfKinematics, m_pConvolCoeffFunctionModule, gpdType)
            ));

        ccfKinematics.setJetCFFType(JetCFFType::TT2);
        m_jetConvolCoeffFunctionResult.insert(std::make_pair(
            JetCFFType::TT2,
            Partons::getInstance()->getServiceObjectRegistry()->getJETConvolCoeffFunctionService()->
                computeSingleKinematic(ccfKinematics, m_pConvolCoeffFunctionModule, gpdType)
            ));

        //set corresponding kinematics
        m_lastCCFKinematics = ccfKinematics;
    }
}

} /* namespace PARTONS */

