#include "../../../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionModule.h"

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
#include "../../../../../include/partons/services/DDVCSConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/utils/VectorUtils.h"

namespace PARTONS {

const std::string DDVCSConvolCoeffFunctionModule::DDVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME =
        "DDVCSConvolCoeffFunctionModule";

DDVCSConvolCoeffFunctionModule::DDVCSConvolCoeffFunctionModule(
        const std::string &className) :
        ConvolCoeffFunctionModule(className, ChannelType::DDVCS), m_Q2(0.), m_Q2Prim(
                0.), m_eta(0.), m_qcdOrderType(
                PerturbativeQCDOrderType::UNDEFINED) {
}

DDVCSConvolCoeffFunctionModule::DDVCSConvolCoeffFunctionModule(
        const DDVCSConvolCoeffFunctionModule &other) :
        ConvolCoeffFunctionModule(other), m_Q2(other.m_Q2), m_Q2Prim(
                other.m_Q2Prim), m_eta(other.m_eta), m_qcdOrderType(
                other.m_qcdOrderType) {

    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
}

DDVCSConvolCoeffFunctionModule::~DDVCSConvolCoeffFunctionModule() {
}

std::string DDVCSConvolCoeffFunctionModule::toString() const {
    return ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>::toString();
}

void DDVCSConvolCoeffFunctionModule::resolveObjectDependencies() {
    ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>::resolveObjectDependencies();
}

void DDVCSConvolCoeffFunctionModule::run() {

    try {

        //get service
        DDVCSConvolCoeffFunctionService* pService =
                Partons::getInstance()->getServiceObjectRegistry()->getDDVCSConvolCoeffFunctionService();

        //run until empty
        while (!(pService->isEmptyTaskQueue())) {

            //kinematics
            DDVCSConvolCoeffFunctionKinematic kinematic;

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
            DDVCSConvolCoeffFunctionResult result = compute(kinematic,
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

void DDVCSConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>::configure(parameters);

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

void DDVCSConvolCoeffFunctionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>::prepareSubModules(subModulesData);
}

DDVCSConvolCoeffFunctionResult DDVCSConvolCoeffFunctionModule::compute(
        const DDVCSConvolCoeffFunctionKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    DDVCSConvolCoeffFunctionResult result(kinematic);

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

List<GPDType> DDVCSConvolCoeffFunctionModule::getListOfAvailableGPDTypeForComputation() const {

    //object to be returned
    List<GPDType> listOfAvailableGPDTypeForComputation;

    //iterator
    std::map<GPDType::Type,
            std::complex<double> (DDVCSConvolCoeffFunctionModule::*)()>::const_iterator it;

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

std::complex<double> DDVCSConvolCoeffFunctionModule::computeUnpolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> DDVCSConvolCoeffFunctionModule::computePolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> DDVCSConvolCoeffFunctionModule::computeCFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

void DDVCSConvolCoeffFunctionModule::setKinematics(
        const DDVCSConvolCoeffFunctionKinematic& kinematic) {

    ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>::setKinematics(kinematic);

    m_Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_eta = kinematic.getEta().makeSameUnitAs(PhysicalUnit::NONE).getValue();
}

PerturbativeQCDOrderType::Type DDVCSConvolCoeffFunctionModule::getQCDOrderType() const {
    return m_qcdOrderType;
}

void DDVCSConvolCoeffFunctionModule::setQCDOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

void DDVCSConvolCoeffFunctionModule::initModule() {
    ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>::initModule();
}

void DDVCSConvolCoeffFunctionModule::isModuleWellConfigured() {

    ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>::isModuleWellConfigured();

    if (m_Q2 < 0) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Q2 = " << m_Q2
                        << " is not > 0.");
    }

    if (m_Q2Prim < 0) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Q2' = " << m_Q2Prim
                        << " is not > 0.");
    }

    if (m_eta < 0. || m_eta > 1.) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of eta = " << m_eta
                        << " do not lay between 0 and 1.");
    }
}

} /* namespace PARTONS */
