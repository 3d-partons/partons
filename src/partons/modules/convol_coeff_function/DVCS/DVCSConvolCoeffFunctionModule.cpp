#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <iostream>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsQuarkMasses.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/DVCSConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

DVCSConvolCoeffFunctionModule::DVCSConvolCoeffFunctionModule(
        const std::string &className) :
        ConvolCoeffFunctionModule(className, ChannelType::DVCS), m_xi(0.), m_t(
                0.), m_Q2(0.), m_MuF2(0.), m_MuR2(0.), m_qcdOrderType(
                PerturbativeQCDOrderType::UNDEFINED), m_currentGPDComputeType(
                GPDType::UNDEFINED) {
}

DVCSConvolCoeffFunctionModule::DVCSConvolCoeffFunctionModule(
        const DVCSConvolCoeffFunctionModule &other) :
        ConvolCoeffFunctionModule(other) {

    m_xi = other.m_xi;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;

    m_qcdOrderType = other.m_qcdOrderType;
    m_currentGPDComputeType = other.m_currentGPDComputeType;

    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
}

DVCSConvolCoeffFunctionModule::~DVCSConvolCoeffFunctionModule() {
}

std::string DVCSConvolCoeffFunctionModule::toString() const {
    return ConvolCoeffFunctionModule<DVCSConvolCoeffFunctionKinematic,
            DVCSConvolCoeffFunctionResult>::toString();
}

void DVCSConvolCoeffFunctionModule::resolveObjectDependencies() {
    ConvolCoeffFunctionModule<DVCSConvolCoeffFunctionKinematic,
            DVCSConvolCoeffFunctionResult>::resolveObjectDependencies();
}



void DVCSConvolCoeffFunctionModule::run() {

//    try {
//        DVCSConvolCoeffFunctionService* pService =
//                Partons::getInstance()->getServiceObjectRegistry()->getDVCSConvolCoeffFunctionService();
//
//        while (!(pService->isEmptyTaskQueue())) {
//
//            DVCSConvolCoeffFunctionKinematic kinematic;
//            List<GPDType> gpdTypeList;
//
//            ElemUtils::Packet packet = pService->popTaskFormQueue();
//            packet >> kinematic;
//            packet >> gpdTypeList;
//
//            debug(__func__,
//                    ElemUtils::Formatter() << "objectId = " << getObjectId()
//                            << " " << kinematic.toString());
//
//            DVCSConvolCoeffFunctionResult result;
//            result.setKinematic(kinematic);
//            result.setComputationModuleName(getClassName());
//
//            //Helpful to sort later if kinematic is coming from database
//            result.setIndexId(kinematic.getIndexId());
//
//            for (unsigned int i = 0; i != gpdTypeList.size(); i++) {
//                result.addResult(gpdTypeList[i].getType(),
//                        compute(kinematic, gpdTypeList[i].getType()));
//            }
//
//            pService->add(result);
//
//            //TODO useful to do a sleep ?
//            // sf::sleep(sf::milliseconds(3));
//        }
//    } catch (std::exception &e) {
//        //TODO remove and improve
//        std::cerr << e.what() << std::endl;
//    }
}

void DVCSConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionModule<DVCSConvolCoeffFunctionKinematic,
    DVCSConvolCoeffFunctionResult>::configure(parameters);

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

void DVCSConvolCoeffFunctionModule::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    ConvolCoeffFunctionModule<DVCSConvolCoeffFunctionKinematic,
    DVCSConvolCoeffFunctionResult>::prepareSubModules(subModulesData);
}

void DVCSConvolCoeffFunctionModule::initModule() {
}

void DVCSConvolCoeffFunctionModule::isModuleWellConfigured() {

    // Test kinematic domain of Xi
    if (m_xi < 0 || m_xi > 1) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Xi = " << m_xi
                        << " do not lay between 0 and 1.");
    }

    // Test kinematic domain of t
    if (m_t > 0) {
        warn(__func__,
                ElemUtils::Formatter() << " Input value of t = " << m_t
                        << " is not <= 0.");
    }

    // Test kinematic domain of Q2
    if (m_Q2 < 0) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Q2 = " << m_Q2
                        << " is not > 0.");
    }

    if (isGPDModuleDependent() && m_pGPDModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pGPDModule is NULL");
    }

    if (m_pRunningAlphaStrongModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pRunningAlphaStrongModule is NULL");
    }

    if (m_pNfConvolCoeffFunction == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pNfConvolCoeffFunction is NULL");
    }
}

DVCSConvolCoeffFunctionResult DVCSConvolCoeffFunctionModule::compute(
        const DVCSConvolCoeffFunctionKinematic& kinematic,
        const List<GPDType>& gpdType) {
    return compute(kinematic.getXi().getValue(), kinematic.getT().getValue(),
            kinematic.getQ2().getValue(), kinematic.getMuF2().getValue(),
            kinematic.getMuR2().getValue());

    //reset kinematics (virtuality)
     setKinematics(kinematic);

     //set gpd type
     m_currentGPDComputeType = gpdType;

     // execute last child function (virtuality)
     initModule();

     // execute last child function (virtuality)
     isModuleWellConfigured();

       //object to be returned
       DVCSConvolCoeffFunctionResult result;

       //search for pointer to function associated to given GPD type
       m_it = m_listOfCFFComputeFunctionAvailable.find(gpdType);

       //check if found
       if (m_it != m_listOfCFFComputeFunctionAvailable.end()) {

           //evaluate
           result = ((*this).*(m_it->second))();

       } else {

           //throw error
           throw ElemUtils::CustomException(getClassName(), __func__,
                   ElemUtils::Formatter() << "GPD(" << GPDType(gpdType).toString()
                           << ") is not available for this  model");
       }

       //return
       return result;
}


std::complex<double> DVCSConvolCoeffFunctionModule::computeUnpolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> DVCSConvolCoeffFunctionModule::computePolarized() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

std::complex<double> DVCSConvolCoeffFunctionModule::computeCFF() {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Check your child implementation : " + getClassName());
}

void DVCSConvolCoeffFunctionModule::preCompute(const double xi, const double t,
        const double Q2, const double MuF2, const double MuR2,
        GPDType::Type gpdComputeType) {

    // set variables
    m_xi = xi;
    m_t = t;
    m_Q2 = Q2;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
    m_currentGPDComputeType = gpdComputeType;

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

PerturbativeQCDOrderType::Type DVCSConvolCoeffFunctionModule::getQCDOrderType() const {
    return m_qcdOrderType;
}

void DVCSConvolCoeffFunctionModule::setQCDOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

//TODO handle string for XML file and native type from C++ code ; see QCD_ORDER_TYPE test


List<GPDType> DVCSConvolCoeffFunctionModule::getListOfAvailableGPDTypeForComputation() const {

    std::map<GPDType::Type,
            std::complex<double> (DVCSConvolCoeffFunctionModule::*)()>::const_iterator it;
    List<GPDType> listOfAvailableGPDTypeForComputation;

    for (it = m_listOfCFFComputeFunctionAvailable.begin();
            it != m_listOfCFFComputeFunctionAvailable.end(); it++) {
        listOfAvailableGPDTypeForComputation.add(it->first);
    }

    return listOfAvailableGPDTypeForComputation;
}



} /* namespace PARTONS */
