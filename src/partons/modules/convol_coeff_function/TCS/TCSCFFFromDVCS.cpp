#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSCFFFromDVCS.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/List.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int TCSCFFFromDVCS::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCFFFromDVCS("TCSCFFFromDVCS"));

TCSCFFFromDVCS::TCSCFFFromDVCS(const std::string &className) :
        TCSConvolCoeffFunctionModule(className), m_pDVCSConvolCoeffFunctionModule(
                0) {

    //relate GPD types with functions to be used
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &TCSConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &TCSConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &TCSConvolCoeffFunctionModule::computePolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &TCSConvolCoeffFunctionModule::computePolarized));

//TODO to be checked
//this module does not depend on GPDs
// setIsGPDModuleDependent(false);
}

TCSCFFFromDVCS::TCSCFFFromDVCS(const TCSCFFFromDVCS &other) :
        TCSConvolCoeffFunctionModule(other) {

    if (other.m_pDVCSConvolCoeffFunctionModule != 0) {
        m_pDVCSConvolCoeffFunctionModule =
                m_pModuleObjectFactory->cloneModuleObject(
                        other.m_pDVCSConvolCoeffFunctionModule);
    } else {
        m_pDVCSConvolCoeffFunctionModule = 0;
    }
}

TCSCFFFromDVCS::~TCSCFFFromDVCS() {

    if (m_pDVCSConvolCoeffFunctionModule != 0) {
        setDVCSConvolCoeffFunctionModule(0);
        m_pDVCSConvolCoeffFunctionModule = 0;
    }
}

void TCSCFFFromDVCS::resolveObjectDependencies() {
    TCSConvolCoeffFunctionModule::resolveObjectDependencies();
}

TCSCFFFromDVCS* TCSCFFFromDVCS::clone() const {
    return new TCSCFFFromDVCS(*this);
}

void TCSCFFFromDVCS::initModule() {
    TCSConvolCoeffFunctionModule::initModule();
}

void TCSCFFFromDVCS::isModuleWellConfigured() {

    //run for mother
    TCSConvolCoeffFunctionModule::isModuleWellConfigured();

    //check if m_pDVCSConvolCoeffFunctionModule set
    if (m_pDVCSConvolCoeffFunctionModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pDVCSConvolCoeffFunctionModule is NULL");
    }
}

std::complex<double> TCSCFFFromDVCS::computeUnpolarized() {

    /*
     * Calculate TCS Compton Form Factors using results obtained for DVCS
     * For unpolarized CFFs we have:
     *      In LO     CFF_TCS = CFF_DVCS*
     *      In NLO    CFF_TCS = CFF_DVCS* - i PI Q2 d/dQ2 CFF_DVCS*
     */

    const double step = 1e-7; // differentiation step

    //set module and pQCD order
    m_pDVCSConvolCoeffFunctionModule->setGPDModule(m_pGPDModule);
    m_pDVCSConvolCoeffFunctionModule->setQCDOrderType(m_qcdOrderType); //TODO was NLO set by hand !!!

    //currect GPD type
    List<GPDType> gpdTypeList;
    gpdTypeList.add(GPDType(m_currentGPDComputeType));

    //perform evaluation for DVCS
    DVCSConvolCoeffFunctionResult dvcsResult =
            m_pDVCSConvolCoeffFunctionModule->compute(
                    DVCSConvolCoeffFunctionKinematic(m_xi, m_t, m_Q2Prim,
                            m_MuF2, m_MuR2), gpdTypeList);

    //result
    std::complex<double> TCSCFF;

    //LO
    if (m_qcdOrderType == PerturbativeQCDOrderType::LO) {
        TCSCFF = std::conj(dvcsResult.getResult(m_currentGPDComputeType));
    }
    //NLO TODO comparison between GPDType and PerturbativeQCDOrderType !!!
    else if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {

        TCSCFF = std::conj(dvcsResult.getResult(m_currentGPDComputeType));

        DVCSConvolCoeffFunctionResult dvcsResult1 =
                m_pDVCSConvolCoeffFunctionModule->compute(
                        DVCSConvolCoeffFunctionKinematic(m_xi, m_t,
                                m_Q2Prim + step, m_MuF2, m_MuR2), gpdTypeList);

        TCSCFF += -Constant::PI * std::complex<double>(0., 1.) * m_Q2Prim
                * (std::conj(dvcsResult1.getResult(m_currentGPDComputeType))
                        - TCSCFF) / step;
    }
    return TCSCFF;
}

std::complex<double> TCSCFFFromDVCS::computePolarized() {

//    /*
//     * Calculate TCS Compton Form Factors using results obtained for DVCS
//     * For polarized CFFs we have:
//     *      In LO     CFF_TCS = - CFF_DVCS*
//     *      In NLO    CFF_TCS = - (CFF_DVCS* - i PI Q2 d/dQ2 CFF_DVCS*)
//     */
//
//    const double step = 1e-7; // differentiation step
//
//    m_pDVCSConvolCoeffFunctionModule->setGPDModule(m_pGPDModule);
//    m_pDVCSConvolCoeffFunctionModule->setGPDModule(m_pGPDModule);
//    m_pDVCSConvolCoeffFunctionModule->setQCDOrderType(
//            PerturbativeQCDOrderType::NLO);
//    std::complex<double> TCSCFF;
//    TCSCFF = std::conj(
//            m_pDVCSConvolCoeffFunctionModule->compute(m_xi, m_t, m_Q2, m_MuF2,
//                    m_MuR2, m_currentGPDComputeType));
//    TCSCFF += -Constant::PI * std::complex<double>(0., 1.) * m_Q2
//            * (std::conj(
//                    m_pDVCSConvolCoeffFunctionModule->compute(m_xi, m_t,
//                            m_Q2 + step, m_MuF2, m_MuR2,
//                            m_currentGPDComputeType)) - TCSCFF) / step;
//    return -TCSCFF; // minus sign

    return std::complex<double>(0., 0.);
}

void TCSCFFFromDVCS::setDVCSConvolCoeffFunctionModule(
        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) {
    m_pModuleObjectFactory->updateModulePointerReference(
            m_pDVCSConvolCoeffFunctionModule, pDVCSConvolCoeffFunctionModule);
    m_pDVCSConvolCoeffFunctionModule = pDVCSConvolCoeffFunctionModule;
}

DVCSConvolCoeffFunctionModule* TCSCFFFromDVCS::getDVCSConvolCoeffFunctionModule() const {
    return m_pDVCSConvolCoeffFunctionModule;
}

void TCSCFFFromDVCS::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother class
    TCSConvolCoeffFunctionModule::prepareSubModules(subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for DVCSConvolCoeffFunctionModule
    it =
            subModulesData.find(
                    DVCSConvolCoeffFunctionModule::DVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        //if already set destroy
        if (m_pDVCSConvolCoeffFunctionModule != 0) {
            setDVCSConvolCoeffFunctionModule(0);
            m_pDVCSConvolCoeffFunctionModule = 0;
        }

        //set
        if (!m_pDVCSConvolCoeffFunctionModule) {

            //get
            m_pDVCSConvolCoeffFunctionModule =
                    Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                            (it->second).getModuleClassName());

            //info
            info(__func__,
                    ElemUtils::Formatter()
                            << "Configure with DVCSConvolCoeffFunctionModule = "
                            << m_pDVCSConvolCoeffFunctionModule->getClassName());

            //configure
            m_pDVCSConvolCoeffFunctionModule->configure(
                    (it->second).getParameters());
        }
    }
}

} /* namespace PARTONS */
