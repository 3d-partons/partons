#include "../../../../../include/partons/modules/convol_coeff_function/JET/JETConvolCoeffFunctionModule.h"

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
#include "../../../../../include/partons/services/JETConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../../include/partons/beans/JetType.h"
#include "../../../../../include/partons/modules/gpd/GPDGK16.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/utils/VectorUtils.h"

namespace PARTONS {

const std::string JETConvolCoeffFunctionModule::JET_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME =
        "JETConvolCoeffFunctionModule";

JETConvolCoeffFunctionModule::JETConvolCoeffFunctionModule(
        const std::string &className) :
        ConvolCoeffFunctionModule(className, ChannelType::JET), m_z(0.), m_qPerp2(0.), m_Q2(0.),
                m_jetType(JetType::UNDEFINED), m_qcdOrderType(PerturbativeQCDOrderType::UNDEFINED) {
}

JETConvolCoeffFunctionModule::JETConvolCoeffFunctionModule(
        const JETConvolCoeffFunctionModule &other) :
        ConvolCoeffFunctionModule(other), m_z(other.m_z), m_qPerp2(other.m_qPerp2), m_Q2(other.m_Q2),
                m_jetType(other.m_jetType), m_qcdOrderType(other.m_qcdOrderType) {

    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
}

JETConvolCoeffFunctionModule::~JETConvolCoeffFunctionModule() {
}

std::string JETConvolCoeffFunctionModule::toString() const {
    return ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>::toString();
}

void JETConvolCoeffFunctionModule::resolveObjectDependencies() {
    ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>::resolveObjectDependencies();
}

void JETConvolCoeffFunctionModule::run() {

    try {

        //get service
        JETConvolCoeffFunctionService* pService =
                Partons::getInstance()->getServiceObjectRegistry()->getJETConvolCoeffFunctionService();

        //run until empty
        while (!(pService->isEmptyTaskQueue())) {

            //kinematics
            JETConvolCoeffFunctionKinematic kinematic;

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
            JETConvolCoeffFunctionResult result = compute(kinematic,
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

void JETConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>::configure(parameters);

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

void JETConvolCoeffFunctionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>::prepareSubModules(subModulesData);
}

JETConvolCoeffFunctionResult JETConvolCoeffFunctionModule::compute(
        const JETConvolCoeffFunctionKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    JETConvolCoeffFunctionResult result(kinematic);

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

List<GPDType> JETConvolCoeffFunctionModule::getListOfAvailableGPDTypeForComputation() const {

    //object to be returned
    List<GPDType> listOfAvailableGPDTypeForComputation;

    //iterator
    std::map<GPDType::Type,
            std::complex<double> (JETConvolCoeffFunctionModule::*)()>::const_iterator it;

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

std::complex<double> JETConvolCoeffFunctionModule::computeUnpolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> JETConvolCoeffFunctionModule::computePolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> JETConvolCoeffFunctionModule::computeCFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::vector<double> JETConvolCoeffFunctionModule::test() {

    std::vector<double> result;

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    setGPDModule(pGPDModule);

    ElemUtils::Parameters parameters(
        PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
        PerturbativeQCDOrderType::LO);
    configure(parameters);

    convolCoeffFunctionResultToStdVector(compute(JETConvolCoeffFunctionKinematic(0.2, -0.1, 0.2, 2., 4., 4., 4., JetType::UP),
                        getListOfAvailableGPDTypeForComputation()), result);
    convolCoeffFunctionResultToStdVector(compute(JETConvolCoeffFunctionKinematic(0.01, -0.3, 0.6, 1., 40., 40., 40., JetType::GLUON),
                        getListOfAvailableGPDTypeForComputation()), result);

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
        pGPDModule, 0);
    pGPDModule = 0;

    return result;
}

void JETConvolCoeffFunctionModule::setKinematics(
        const JETConvolCoeffFunctionKinematic& kinematic) {

    ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>::setKinematics(kinematic);

    m_z = kinematic.getZ().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_qPerp2 = kinematic.getQPerp2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_jetType = kinematic.getJetType();
}

PerturbativeQCDOrderType::Type JETConvolCoeffFunctionModule::getQCDOrderType() const {
    return m_qcdOrderType;
}

void JETConvolCoeffFunctionModule::setQCDOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

void JETConvolCoeffFunctionModule::initModule() {
    ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>::initModule();
}

void JETConvolCoeffFunctionModule::isModuleWellConfigured() {

    ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>::isModuleWellConfigured();

    if (m_z < 0. || m_z > 1.) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of z = " << m_z
                        << " is not between 0. and 1.");
    }

    if (m_qPerp2 < 0.) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of qPerp2 = " << m_qPerp2
                        << " is not > 0.");
    }

    if (m_Q2 < 0.) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Q2 = " << m_Q2
                        << " is not > 0.");
    }

    if (m_jetType == JetType::UNDEFINED) {
        warn(__func__,
                ElemUtils::Formatter() << "Jet type is undefined");
    }
}

} /* namespace PARTONS */
