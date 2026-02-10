#include "../../../../../include/partons/modules/process/GAM2/GAM2ProcessModule.h"

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
#include "../../../../../include/partons/modules/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/scales/GAM2/GAM2ScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/GAM2/GAM2XiConverterModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

#include "../../../../../include/partons/modules/convol_coeff_function/GAM2/GAM2CFFStandard.h"
#include "../../../../../include/partons/modules/gpd/GPDGK19.h"
#include "../../../../../include/partons/modules/scales/GAM2/GAM2ScalesMgg2Multiplier.h"
#include "../../../../../include/partons/modules/xi_converter/GAM2/GAM2XiConverterExact.h"

namespace PARTONS {

const std::string GAM2ProcessModule::GAM2_PROCESS_MODULE_CLASS_NAME =
        "GAM2ProcessModule";

GAM2ProcessModule::GAM2ProcessModule(const std::string &className) :
        ProcessModule(className, ChannelType::GAM2), m_t(0.), m_uPrim(0.), m_Mgg2(
                0.), m_E(0.), m_phi(0.), m_polG0(PolarizationType::UNDEFINED), m_polG1(
                PolarizationType::UNDEFINED), m_polG2(
                PolarizationType::UNDEFINED), m_tmin(0.), m_xi(0.), m_pScaleModule(
                0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(0) {
}

GAM2ProcessModule::~GAM2ProcessModule() {

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

GAM2ProcessModule::GAM2ProcessModule(const GAM2ProcessModule& other) :
        ProcessModule(other), m_t(other.m_t), m_uPrim(other.m_uPrim), m_Mgg2(
                other.m_Mgg2), m_E(other.m_E), m_phi(other.m_phi), m_polG0(
                other.m_polG0), m_polG1(other.m_polG1), m_polG2(other.m_polG2), m_tmin(
                other.m_tmin), m_xi(other.m_xi), m_pScaleModule(0), m_pXiConverterModule(
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

std::string GAM2ProcessModule::toString() const {
    ProcessModule<GAM2ObservableKinematic, GAM2ObservableResult>::toString();
    return ProcessModule<GAM2ObservableKinematic, GAM2ObservableResult>::toString();
}

void GAM2ProcessModule::resolveObjectDependencies() {
    ProcessModule<GAM2ObservableKinematic, GAM2ObservableResult>::resolveObjectDependencies();
}

void GAM2ProcessModule::run() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Users should evaluate cross-sections through respective ObservableService");
}

void GAM2ProcessModule::configure(const ElemUtils::Parameters &parameters) {
    ProcessModule<GAM2ObservableKinematic, GAM2ObservableResult>::configure(
            parameters);
}

void GAM2ProcessModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    ProcessModule<GAM2ObservableKinematic, GAM2ObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for scales module
    it = subModulesData.find(GAM2ScalesModule::GAM2_SCALES_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newGAM2ScalesModule(
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
            GAM2XiConverterModule::GAM2_XI_CONVERTER_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newGAM2XiConverterModule(
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
                        GAM2ConvolCoeffFunctionModule::GAM2_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

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
                        Partons::getInstance()->getModuleObjectFactory()->newGAM2ConvolCoeffFunctionModule(
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

List<GPDType> GAM2ProcessModule::getListOfAvailableGPDTypeForComputation() const {

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

std::vector<double> GAM2ProcessModule::test() {

    std::vector<double> result;

    PARTONS::GAM2ConvolCoeffFunctionModule* pGAM2CFFModel =
        PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGAM2ConvolCoeffFunctionModule(
                PARTONS::GAM2CFFStandard::classId);

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    pGAM2CFFModel->setGPDModule(pGPDModule);

    pGAM2CFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    setConvolCoeffFunctionModule(pGAM2CFFModel);

    PARTONS::GAM2XiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGAM2XiConverterModule(
                    PARTONS::GAM2XiConverterExact::classId);
    setXiConverterModule(pXiConverterModule);

    PARTONS::GAM2ScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGAM2ScalesModule(
                    PARTONS::GAM2ScalesMgg2Multiplier::classId);
   setScaleModule(pScalesModule);

    result.push_back(compute(PolarizationType::LIN_TRANS_X_PLUS, PolarizationType::LIN_TRANS_X_PLUS, PolarizationType::LIN_TRANS_X_PLUS, NumA::Vector3D(0.,0.,0.),
        GAM2ObservableKinematic(-0.1, -2., 4., 24., M_PI/3.),
                        pGAM2CFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    result.push_back(compute(PolarizationType::LIN_TRANS_X_PLUS, PolarizationType::LIN_TRANS_X_PLUS, PolarizationType::LIN_TRANS_X_PLUS, NumA::Vector3D(0.,0.,0.),
        GAM2ObservableKinematic( -0.2, -4., 16., 100., M_PI/6.),
                        pGAM2CFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());

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
            pGAM2CFFModel, 0);
    pGAM2CFFModel = 0;

    return result;
}

GAM2ObservableResult GAM2ProcessModule::compute(PolarizationType::Type polG0,
        PolarizationType::Type polG1, PolarizationType::Type polG2,
        NumA::Vector3D targetPolarization,
        const GAM2ObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //set experimental conditions
    setExperimentalConditions(polG0, polG1, polG2, targetPolarization);

    //compute CCF
    if (m_isCCFModuleDependent)
        computeConvolCoeffFunction(kinematic, gpdType);

    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    GAM2ObservableResult result(kinematic);

    PhysicalType<double> value = CrossSection();

    //set value
    result.setValue(value);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

PhysicalType<double> GAM2ProcessModule::CrossSection() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

void GAM2ProcessModule::resetPreviousKinematic() {

    m_dvcsConvolCoeffFunctionResult = GAM2ConvolCoeffFunctionResult();
    m_lastCCFKinematics = GAM2ConvolCoeffFunctionKinematic();
}

bool GAM2ProcessModule::isPreviousCCFKinematicDifferent(
        const GAM2ConvolCoeffFunctionKinematic& kinematic) const {

    return ((kinematic.getXi() != m_lastCCFKinematics.getXi())
            || (kinematic.getT() != m_lastCCFKinematics.getT())
            || (kinematic.getMuF2() != m_lastCCFKinematics.getMuF2())
            || (kinematic.getMuR2() != m_lastCCFKinematics.getMuR2())
            || (kinematic.getUPrim() != m_lastCCFKinematics.getUPrim())
            || (kinematic.getMgg2() != m_lastCCFKinematics.getMgg2())
            || (kinematic.getPolG0() != m_lastCCFKinematics.getPolG0())
            || (kinematic.getPolG1() != m_lastCCFKinematics.getPolG1())
            || (kinematic.getPolG2() != m_lastCCFKinematics.getPolG2()));
}

GAM2ScalesModule* GAM2ProcessModule::getScaleModule() const {
    return m_pScaleModule;
}

void GAM2ProcessModule::setScaleModule(GAM2ScalesModule* pScaleModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pScaleModule,
            pScaleModule);
    m_pScaleModule = pScaleModule;

    if (m_pScaleModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "GAM2ScalesModule is set to: "
                        << pScaleModule->getClassName());
    } else {
        info(__func__, "GAM2ScalesModule is set to: 0");
    }
}

GAM2XiConverterModule* GAM2ProcessModule::getXiConverterModule() const {
    return m_pXiConverterModule;
}

void GAM2ProcessModule::setXiConverterModule(
        GAM2XiConverterModule* pXiConverterModule) {

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

GAM2ConvolCoeffFunctionModule* GAM2ProcessModule::getConvolCoeffFunctionModule() const {
    return m_pConvolCoeffFunctionModule;
}

void GAM2ProcessModule::setConvolCoeffFunctionModule(
        GAM2ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {

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

void GAM2ProcessModule::setKinematics(
        const GAM2ObservableKinematic& kinematic) {

    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_uPrim =
            kinematic.getUPrim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Mgg2 = kinematic.getMgg2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
}

void GAM2ProcessModule::setExperimentalConditions(PolarizationType::Type polG0,
        PolarizationType::Type polG1, PolarizationType::Type polG2,
        NumA::Vector3D targetPolarization) {

    m_polG0 = polG0;
    m_polG1 = polG1;
    m_polG2 = polG2;
    m_targetPolarization = targetPolarization;
}

void GAM2ProcessModule::initModule() {

    //run for mother
    ProcessModule<GAM2ObservableKinematic, GAM2ObservableResult>::initModule();

    //evaluate internal variables

    if (m_pXiConverterModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pXiConverterModule is NULL pointer ; Use configure method to configure it");
    }

    m_xi =
            m_pXiConverterModule->compute(
                    GAM2ObservableKinematic(m_t, m_uPrim, m_Mgg2, m_E, m_phi)).getValue();
    m_tmin = -1 * pow(2 * m_xi * Constant::PROTON_MASS, 2)
            / (1. - pow(m_xi, 2));
}

void GAM2ProcessModule::isModuleWellConfigured() {

    //run for mother
    ProcessModule<GAM2ObservableKinematic, GAM2ObservableResult>::isModuleWellConfigured();

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
    if (m_t > m_tmin) {
        ElemUtils::Formatter formatter;
        formatter << " Input value of t = " << m_t << " greater than t_min = "
                << m_tmin << " (GAM2 kinematic limits)";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of u'
    if (m_uPrim > 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of u' = " << m_uPrim << " is not < 0";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of Mgg2
    if (m_Mgg2 < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of Mgg2 = " << m_Mgg2 << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of E
    if (m_E < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test pol0
    if (m_polG0 == PolarizationType::UNDEFINED) {
        ElemUtils::Formatter formatter;
        formatter << "Polarization of incoming photon is: "
                << PolarizationType(m_polG0).toString();
        warn(__func__, formatter.str());
    }

    //test pol1
    if (m_polG1 == PolarizationType::UNDEFINED) {
        ElemUtils::Formatter formatter;
        formatter << "Polarization of first outgoing photon is: "
                << PolarizationType(m_polG1).toString();
        warn(__func__, formatter.str());
    }

    //test pol2
    if (m_polG2 == PolarizationType::UNDEFINED) {
        ElemUtils::Formatter formatter;
        formatter << "Polarization of second outgoing photon is: "
                << PolarizationType(m_polG2).toString();
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

void GAM2ProcessModule::computeConvolCoeffFunction(
        const GAM2ObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //compute scales
    Scales scale = m_pScaleModule->compute(kinematic);

    //compute xi
    PhysicalType<double> xi = m_pXiConverterModule->compute(kinematic);

    //create ccf kinematics
    GAM2ConvolCoeffFunctionKinematic ccfKinematics(xi, kinematic.getT(),
            kinematic.getUPrim(), kinematic.getMgg2(), scale.getMuF2(),
            scale.getMuR2(), m_polG0, m_polG1, m_polG2, kinematic.getPhi());

    //check if different
    if (isPreviousCCFKinematicDifferent(ccfKinematics)) {

        //evaluate
        m_dvcsConvolCoeffFunctionResult = m_pConvolCoeffFunctionModule->compute(
                ccfKinematics, gpdType);

//                Partons::getInstance()->getServiceObjectRegistry()->getGAM2ConvolCoeffFunctionService()->computeSingleKinematic(
//                        ccfKinematics, m_pConvolCoeffFunctionModule, gpdType);

//set corresponding kinematics
        m_lastCCFKinematics = ccfKinematics;
    }
}

std::complex<double> GAM2ProcessModule::getConvolCoeffFunctionValue(
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

void GAM2ProcessModule::setConvolCoeffFunction(
        const GAM2ConvolCoeffFunctionKinematic& kin,
        const GAM2ConvolCoeffFunctionResult& result) {

    m_lastCCFKinematics = kin;
    m_dvcsConvolCoeffFunctionResult = result;
}

} /* namespace PARTONS */

