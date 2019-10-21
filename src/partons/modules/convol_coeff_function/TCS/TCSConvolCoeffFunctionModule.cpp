#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSConvolCoeffFunctionModule.h"

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
#include "../../../../../include/partons/services/TCSConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/utils/VectorUtils.h"

namespace PARTONS {

const std::string TCSConvolCoeffFunctionModule::TCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME =
        "TCSConvolCoeffFunctionModule";

TCSConvolCoeffFunctionModule::TCSConvolCoeffFunctionModule(
        const std::string &className) :
        ConvolCoeffFunctionModule(className, ChannelType::TCS), m_Q2Prim(0.), m_qcdOrderType(
                PerturbativeQCDOrderType::UNDEFINED) {
}

TCSConvolCoeffFunctionModule::TCSConvolCoeffFunctionModule(
        const TCSConvolCoeffFunctionModule &other) :
        ConvolCoeffFunctionModule(other), m_Q2Prim(other.m_Q2Prim), m_qcdOrderType(
                other.m_qcdOrderType) {

    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
}

TCSConvolCoeffFunctionModule::~TCSConvolCoeffFunctionModule() {
}

std::string TCSConvolCoeffFunctionModule::toString() const {
    return ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>::toString();
}

void TCSConvolCoeffFunctionModule::resolveObjectDependencies() {
    ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>::resolveObjectDependencies();
}

void TCSConvolCoeffFunctionModule::run() {

    try {

        //get service
        TCSConvolCoeffFunctionService* pService =
                Partons::getInstance()->getServiceObjectRegistry()->getTCSConvolCoeffFunctionService();

        //run until empty
        while (!(pService->isEmptyTaskQueue())) {

            //kinematics
            TCSConvolCoeffFunctionKinematic kinematic;

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
            TCSConvolCoeffFunctionResult result = compute(kinematic,
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

void TCSConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>::configure(parameters);

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

void TCSConvolCoeffFunctionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>::prepareSubModules(subModulesData);
}

TCSConvolCoeffFunctionResult TCSConvolCoeffFunctionModule::compute(
        const TCSConvolCoeffFunctionKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    TCSConvolCoeffFunctionResult result(kinematic);

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

List<GPDType> TCSConvolCoeffFunctionModule::getListOfAvailableGPDTypeForComputation() const {

    //object to be returned
    List<GPDType> listOfAvailableGPDTypeForComputation;

    //iterator
    std::map<GPDType::Type,
            std::complex<double> (TCSConvolCoeffFunctionModule::*)()>::const_iterator it;

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

std::complex<double> TCSConvolCoeffFunctionModule::computeUnpolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> TCSConvolCoeffFunctionModule::computePolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> TCSConvolCoeffFunctionModule::computeCFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

void TCSConvolCoeffFunctionModule::setKinematics(
        const TCSConvolCoeffFunctionKinematic& kinematic) {

    ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>::setKinematics(kinematic);

    m_Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
}

PerturbativeQCDOrderType::Type TCSConvolCoeffFunctionModule::getQCDOrderType() const {
    return m_qcdOrderType;
}

void TCSConvolCoeffFunctionModule::setQCDOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

void TCSConvolCoeffFunctionModule::initModule() {
    ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>::initModule();
}

void TCSConvolCoeffFunctionModule::isModuleWellConfigured() {

    ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>::isModuleWellConfigured();

    if (m_Q2Prim < 0) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Q2' = " << m_Q2Prim
                        << " is not > 0.");
    }
}

} /* namespace PARTONS */
