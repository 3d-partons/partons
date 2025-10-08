#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <stddef.h>
#include <iostream>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/DVMPConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../../include/partons/modules/gpd/GPDGK16.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/utils/VectorUtils.h"

namespace PARTONS {

const std::string DVMPConvolCoeffFunctionModule::DVMP_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME =
        "DVMPConvolCoeffFunctionModule";

DVMPConvolCoeffFunctionModule::DVMPConvolCoeffFunctionModule(
        const std::string &className) :
        ConvolCoeffFunctionModule(className, ChannelType::DVMP), m_Q2(0.), m_mesonType(
                MesonType::UNDEFINED), m_mesonPolarization(
                PolarizationType::UNDEFINED), m_qcdOrderType(
                PerturbativeQCDOrderType::UNDEFINED) {
}

DVMPConvolCoeffFunctionModule::DVMPConvolCoeffFunctionModule(
        const DVMPConvolCoeffFunctionModule &other) :
        ConvolCoeffFunctionModule(other), m_Q2(other.m_Q2), m_mesonType(
                other.m_mesonType), m_mesonPolarization(
                other.m_mesonPolarization), m_qcdOrderType(other.m_qcdOrderType) {

    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
}

DVMPConvolCoeffFunctionModule::~DVMPConvolCoeffFunctionModule() {
}

std::string DVMPConvolCoeffFunctionModule::toString() const {
    return ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>::toString();
}

void DVMPConvolCoeffFunctionModule::resolveObjectDependencies() {
    ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>::resolveObjectDependencies();
}

void DVMPConvolCoeffFunctionModule::run() {

    try {

        //get service
        DVMPConvolCoeffFunctionService* pService =
                Partons::getInstance()->getServiceObjectRegistry()->getDVMPConvolCoeffFunctionService();

        //run until empty
        while (!(pService->isEmptyTaskQueue())) {

            //kinematics
            DVMPConvolCoeffFunctionKinematic kinematic;

            //list of GPD types
            List<GPDType> gpdTypeList;

            //set
            ElemUtils::Packet packet = pService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdTypeList;

            //debug information
            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            //object to be returned
            DVMPConvolCoeffFunctionResult result = compute(kinematic,
                    gpdTypeList);

            //helpful to sort later if kinematic is coming from database
            //TODO is used?
            result.setIndexId(kinematic.getIndexId());

            //add
            pService->add(result);

            //TODO useful to do a sleep ?
            // sf::sleep(sf::milliseconds(3));
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void DVMPConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>::configure(parameters);

    //check if available
    if (parameters.isAvailable(
            PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE)) {

        //try to set m_qcdOrderType by standard way
        try {
            m_qcdOrderType =
                    static_cast<PerturbativeQCDOrderType::Type>(parameters.getLastAvailable().toUInt());

        }
        //if an exception is raised it means that it's a string configuration value
        catch (const std::exception &e) {

            m_qcdOrderType = PerturbativeQCDOrderType(
                    parameters.getLastAvailable().getString()).getType();
        }

        //print info
        info(__func__,
                ElemUtils::Formatter()
                        << PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE
                        << " configured with value = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }
}

void DVMPConvolCoeffFunctionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>::prepareSubModules(subModulesData);
}

DVMPConvolCoeffFunctionResult DVMPConvolCoeffFunctionModule::compute(
        const DVMPConvolCoeffFunctionKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    DVMPConvolCoeffFunctionResult result(kinematic);

    //loop over GPD types
    for (size_t i = 0; i < gpdType.size(); i++) {

        //search for pointer to function associated to given GPD type
        m_it = m_listOfCFFComputeFunctionAvailable.find(gpdType[i]);

        //check if found
        if (m_it != m_listOfCFFComputeFunctionAvailable.end()) {

            //set GPD type
            setCurrentGPDType(gpdType[i]);

            //evaluate
            result.addResult(gpdType[i], ((*this).*(m_it->second))());

        } else {

            //throw error
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "GPD("
                            << GPDType(gpdType[i]).toString()
                            << ") is not available for this  model");
        }
    }

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<GPDType> DVMPConvolCoeffFunctionModule::getListOfAvailableGPDTypeForComputation() const {

    //object to be returned
    List<GPDType> listOfAvailableGPDTypeForComputation;

    //iterator
    std::map<GPDType::Type,
            std::complex<double> (DVMPConvolCoeffFunctionModule::*)()>::const_iterator it;

    //fill list
    for (it = m_listOfCFFComputeFunctionAvailable.begin();
            it != m_listOfCFFComputeFunctionAvailable.end(); it++) {
        listOfAvailableGPDTypeForComputation.add(it->first);
    }

    //if this CCF model is GPD model dependent we need to perform another intersection with GPDType available for this GPD model
    if (m_isGPDModuleDependent) {

        if (m_pGPDModule == 0) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "GPD module not set");
        }

        listOfAvailableGPDTypeForComputation = VectorUtils::intersection(
                listOfAvailableGPDTypeForComputation,
                m_pGPDModule->getListOfAvailableGPDTypeForComputation());
    }

    //return
    return listOfAvailableGPDTypeForComputation;
}

std::complex<double> DVMPConvolCoeffFunctionModule::computeUnpolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> DVMPConvolCoeffFunctionModule::computePolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> DVMPConvolCoeffFunctionModule::computeCFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::vector<double> DVMPConvolCoeffFunctionModule::test() {

    std::vector<double> result;

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    setGPDModule(pGPDModule);

    ElemUtils::Parameters parameters(
        PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
        PerturbativeQCDOrderType::LO);
    configure(parameters);

    convolCoeffFunctionResultToStdVector(compute(DVMPConvolCoeffFunctionKinematic(0.2, -0.1, 4., 4., 4., MesonType::PI0, PolarizationType::UNDEFINED),
                    getListOfAvailableGPDTypeForComputation()), result);
    convolCoeffFunctionResultToStdVector(compute(DVMPConvolCoeffFunctionKinematic(0.02, -0.5, 16., 16., 16., MesonType::PI0, PolarizationType::UNDEFINED),
                   getListOfAvailableGPDTypeForComputation()), result);

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
        pGPDModule, 0);
    pGPDModule = 0;

    return result;
}

void DVMPConvolCoeffFunctionModule::setKinematics(
        const DVMPConvolCoeffFunctionKinematic& kinematic) {

    ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>::setKinematics(kinematic);

    m_Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_mesonType = kinematic.getMesonType();
    m_mesonPolarization = kinematic.getMesonPolarization();
}

PerturbativeQCDOrderType::Type DVMPConvolCoeffFunctionModule::getQCDOrderType() const {
    return m_qcdOrderType;
}

void DVMPConvolCoeffFunctionModule::setQCDOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

void DVMPConvolCoeffFunctionModule::initModule() {
    ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>::initModule();
}

void DVMPConvolCoeffFunctionModule::isModuleWellConfigured() {

    ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>::isModuleWellConfigured();

    if (m_Q2 < 0) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Q2 = " << m_Q2
                        << " is not > 0.");
    }

    if (m_mesonType == MesonType::UNDEFINED) {
        warn(__func__,
                ElemUtils::Formatter() << "Meson type is "
                        << MesonType(m_mesonType).toString());
    }
}

} /* namespace PARTONS */
