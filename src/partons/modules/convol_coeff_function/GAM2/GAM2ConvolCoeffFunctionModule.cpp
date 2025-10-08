#include "../../../../../include/partons/modules/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionModule.h"

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
//#include "../../../../../include/partons/services/GAM2ConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../../include/partons/modules/gpd/GPDGK16.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../include/partons/utils/VectorUtils.h"

namespace PARTONS {

const std::string GAM2ConvolCoeffFunctionModule::GAM2_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME =
        "GAM2ConvolCoeffFunctionModule";

GAM2ConvolCoeffFunctionModule::GAM2ConvolCoeffFunctionModule(
        const std::string &className) :
        ConvolCoeffFunctionModule(className, ChannelType::GAM2), m_uPrim(0.), m_Mgg2(
                0.), m_polG0(PolarizationType::UNDEFINED), m_polG1(
                PolarizationType::UNDEFINED), m_polG2(
                PolarizationType::UNDEFINED), m_phi(0.), m_qcdOrderType(
                PerturbativeQCDOrderType::UNDEFINED) {
}

GAM2ConvolCoeffFunctionModule::GAM2ConvolCoeffFunctionModule(
        const GAM2ConvolCoeffFunctionModule &other) :
        ConvolCoeffFunctionModule(other), m_uPrim(other.m_uPrim), m_Mgg2(
                other.m_Mgg2), m_polG0(other.m_polG0), m_polG1(other.m_polG1), m_polG2(
                other.m_polG2), m_phi(other.m_phi), m_qcdOrderType(
                other.m_qcdOrderType) {

    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
}

GAM2ConvolCoeffFunctionModule::~GAM2ConvolCoeffFunctionModule() {
}

std::string GAM2ConvolCoeffFunctionModule::toString() const {
    return ConvolCoeffFunctionModule<GAM2ConvolCoeffFunctionKinematic,
            GAM2ConvolCoeffFunctionResult>::toString();
}

void GAM2ConvolCoeffFunctionModule::resolveObjectDependencies() {
    ConvolCoeffFunctionModule<GAM2ConvolCoeffFunctionKinematic,
            GAM2ConvolCoeffFunctionResult>::resolveObjectDependencies();
}

void GAM2ConvolCoeffFunctionModule::run() {

    /*
     try {

     //get service
     GAM2ConvolCoeffFunctionService* pService =
     Partons::getInstance()->getServiceObjectRegistry()->getGAM2ConvolCoeffFunctionService();

     //run until empty
     while (!(pService->isEmptyTaskQueue())) {

     //kinematics
     GAM2ConvolCoeffFunctionKinematic kinematic;

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
     GAM2ConvolCoeffFunctionResult result = compute(kinematic,
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
     */
}

void GAM2ConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionModule<GAM2ConvolCoeffFunctionKinematic,
            GAM2ConvolCoeffFunctionResult>::configure(parameters);

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

void GAM2ConvolCoeffFunctionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ConvolCoeffFunctionModule<GAM2ConvolCoeffFunctionKinematic,
            GAM2ConvolCoeffFunctionResult>::prepareSubModules(subModulesData);
}

GAM2ConvolCoeffFunctionResult GAM2ConvolCoeffFunctionModule::compute(
        const GAM2ConvolCoeffFunctionKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    GAM2ConvolCoeffFunctionResult result(kinematic);

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

List<GPDType> GAM2ConvolCoeffFunctionModule::getListOfAvailableGPDTypeForComputation() const {

    //object to be returned
    List<GPDType> listOfAvailableGPDTypeForComputation;

    //iterator
    std::map<GPDType::Type,
            std::complex<double> (GAM2ConvolCoeffFunctionModule::*)()>::const_iterator it;

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

std::complex<double> GAM2ConvolCoeffFunctionModule::computeUnpolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> GAM2ConvolCoeffFunctionModule::computePolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> GAM2ConvolCoeffFunctionModule::computeCFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::vector<double> GAM2ConvolCoeffFunctionModule::test() {

    std::vector<double> result;

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    setGPDModule(pGPDModule);

    ElemUtils::Parameters parameters(
        PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
        PerturbativeQCDOrderType::LO);
    configure(parameters);

    convolCoeffFunctionResultToStdVector(compute(GAM2ConvolCoeffFunctionKinematic(0.2, -0.1, -3., 4., 4., 4.,
        PolarizationType::LIN_TRANS_X_PLUS, PolarizationType::LIN_TRANS_X_PLUS, PolarizationType::LIN_TRANS_X_PLUS, M_PI/4.),
                        getListOfAvailableGPDTypeForComputation()), result);
    convolCoeffFunctionResultToStdVector(compute(GAM2ConvolCoeffFunctionKinematic(0.02, -0.5, -5., 16., 16., 16.,
        PolarizationType::LIN_TRANS_X_MINUS, PolarizationType::LIN_TRANS_X_PLUS, PolarizationType::LIN_TRANS_X_PLUS, M_PI/16.),
                        getListOfAvailableGPDTypeForComputation()), result);

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
        pGPDModule, 0);
    pGPDModule = 0;

    return result;
}

void GAM2ConvolCoeffFunctionModule::setKinematics(
        const GAM2ConvolCoeffFunctionKinematic& kinematic) {

    ConvolCoeffFunctionModule<GAM2ConvolCoeffFunctionKinematic,
            GAM2ConvolCoeffFunctionResult>::setKinematics(kinematic);

    m_uPrim =
            kinematic.getUPrim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Mgg2 = kinematic.getMgg2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_polG0 = kinematic.getPolG0();
    m_polG1 = kinematic.getPolG1();
    m_polG2 = kinematic.getPolG2();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
}

PerturbativeQCDOrderType::Type GAM2ConvolCoeffFunctionModule::getQCDOrderType() const {
    return m_qcdOrderType;
}

void GAM2ConvolCoeffFunctionModule::setQCDOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

void GAM2ConvolCoeffFunctionModule::initModule() {
    ConvolCoeffFunctionModule<GAM2ConvolCoeffFunctionKinematic,
            GAM2ConvolCoeffFunctionResult>::initModule();
}

void GAM2ConvolCoeffFunctionModule::isModuleWellConfigured() {

    ConvolCoeffFunctionModule<GAM2ConvolCoeffFunctionKinematic,
            GAM2ConvolCoeffFunctionResult>::isModuleWellConfigured();

    if (m_uPrim > 0.) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of u' = " << m_uPrim
                        << " is not < 0.");
    }

    if (m_Mgg2 < 0.) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Mgg2 = " << m_Mgg2
                        << " is not > 0.");
    }

    if (PolarizationType(m_polG0).getType() == PolarizationType::UNDEFINED) {
        warn(__func__, "Polarization of incoming photon is undefined");
    }

    if (PolarizationType(m_polG1).getType() == PolarizationType::UNDEFINED) {
        warn(__func__, "Polarization of first outgoing photon is undefined");
    }

    if (PolarizationType(m_polG2).getType() == PolarizationType::UNDEFINED) {
        warn(__func__, "Polarization of second outgoing photon is undefined");
    }
}

} /* namespace PARTONS */
