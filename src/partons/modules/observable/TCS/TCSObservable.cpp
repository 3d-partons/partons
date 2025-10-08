#include "../../../../../include/partons/modules/observable/TCS/TCSObservable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <iostream>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/TCSObservableService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"

#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSCFFStandard.h"
#include "../../../../../include/partons/modules/gpd/GPDGK19.h"
#include "../../../../../include/partons/modules/process/TCS/TCSProcessBDP01.h"
#include "../../../../../include/partons/modules/scales/TCS/TCSScalesQ2PrimMultiplier.h"
#include "../../../../../include/partons/modules/xi_converter/TCS/TCSXiConverterTauToXi.h"

namespace PARTONS {

const std::string TCSObservable::TCS_OBSERVABLE_MODULE_CLASS_NAME =
        "TCSObservableModule";

TCSObservable::TCSObservable(const std::string &className) :
        Observable(className, ChannelType::TCS), m_pProcessModule(0) {
}

TCSObservable::TCSObservable(const TCSObservable& other) :
        Observable(other) {

    if (other.m_pProcessModule != 0) {
        m_pProcessModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pProcessModule);
    } else {
        m_pProcessModule = 0;
    }
}

TCSObservable::~TCSObservable() {

    if (m_pProcessModule != 0) {
        setProcessModule(0);
        m_pProcessModule = 0;
    }
}

std::string TCSObservable::toString() const {
    return Observable<TCSObservableKinematic, TCSObservableResult>::toString();
}

void TCSObservable::resolveObjectDependencies() {
    Observable<TCSObservableKinematic, TCSObservableResult>::resolveObjectDependencies();
}

void TCSObservable::run() {

    try {

        //get service
        TCSObservableService* pTCSObservableService =
                Partons::getInstance()->getServiceObjectRegistry()->getTCSObservableService();

        //run until empty
        while (!(pTCSObservableService->isEmptyTaskQueue())) {

            //kinematics
            TCSObservableKinematic kinematic;

            //list of GPD types
            List<GPDType> gpdType;

            //set
            ElemUtils::Packet packet =
                    pTCSObservableService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdType;

            //debug information
            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            //object to be returned
            TCSObservableResult result = compute(kinematic, gpdType);

            //helpful to sort later if kinematic is coming from database
            result.setIndexId(kinematic.getIndexId());

            //add
            pTCSObservableService->add(result);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void TCSObservable::configure(const ElemUtils::Parameters &parameters) {
    Observable<TCSObservableKinematic, TCSObservableResult>::configure(
            parameters);
}

void TCSObservable::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    Observable<TCSObservableKinematic, TCSObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for process module
    it = subModulesData.find(TCSProcessModule::TCS_PROCESS_MODULE_CLASS_NAME);

    //check if there
    if (it != subModulesData.end()) {

        //check if already set
        if (m_pProcessModule) {

            setProcessModule(0);
            m_pProcessModule = 0;
        }

        //set
        if (!m_pProcessModule) {

            m_pProcessModule =
                    Partons::getInstance()->getModuleObjectFactory()->newTCSProcessModule(
                            (it->second).getModuleClassName());

            info(__func__,
                    ElemUtils::Formatter() << "Configured with ProcessModule = "
                            << m_pProcessModule->getClassName());

            m_pProcessModule->configure((it->second).getParameters());
            m_pProcessModule->prepareSubModules((it->second).getSubModules());
        }

    } else {

        //throw error
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << getClassName()
                        << " is ProcessModule dependent and you have not provided one");
    }
}

TCSObservableResult TCSObservable::compute(
        const TCSObservableKinematic& kinematic, const List<GPDType>& gpdType) {

    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    TCSObservableResult result(computeObservable(kinematic, gpdType),
            kinematic);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<GPDType> TCSObservable::getListOfAvailableGPDTypeForComputation() const {
    return m_pProcessModule->getConvolCoeffFunctionModule()->getListOfAvailableGPDTypeForComputation();
}

std::vector<double> TCSObservable::test() {

    std::vector<double> result;

    PARTONS::TCSProcessModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSProcessModule(
                    PARTONS::TCSProcessBDP01::classId);
    setProcessModule(pProcessModule);

    PARTONS::TCSConvolCoeffFunctionModule* pTCSCFFModel =
        PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSConvolCoeffFunctionModule(
                PARTONS::TCSCFFStandard::classId);

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    pTCSCFFModel->setGPDModule(pGPDModule);

    pTCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    pProcessModule->setConvolCoeffFunctionModule(pTCSCFFModel);

    PARTONS::TCSXiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSXiConverterModule(
                    PARTONS::TCSXiConverterTauToXi::classId);
    pProcessModule->setXiConverterModule(pXiConverterModule);

    PARTONS::TCSScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSScalesModule(
                    PARTONS::TCSScalesQ2PrimMultiplier::classId);
    pProcessModule->setScaleModule(pScalesModule);

    result.push_back(compute(TCSObservableKinematic( -0.1, 2., 6., M_PI/2.,M_PI/4.),
                        pTCSCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    result.push_back(compute(TCSObservableKinematic(-0.5, 16., 1000., M_PI/4., M_PI/2.),
                        pTCSCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());

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
            pTCSCFFModel, 0);
    pTCSCFFModel = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    return result;
}
    
void TCSObservable::initModule() {
    Observable<TCSObservableKinematic, TCSObservableResult>::initModule();
}

void TCSObservable::isModuleWellConfigured() {
    Observable<TCSObservableKinematic, TCSObservableResult>::isModuleWellConfigured();
}

TCSProcessModule* TCSObservable::getProcessModule() const {
    return m_pProcessModule;
}

void TCSObservable::setProcessModule(TCSProcessModule* pProcessModule) {

    m_pModuleObjectFactory->updateModulePointerReference(m_pProcessModule,
            pProcessModule);
    m_pProcessModule = pProcessModule;

    if (m_pProcessModule != 0) {
        info(__func__,
                ElemUtils::Formatter() << "ProcessModule is set to: "
                        << m_pProcessModule->getClassName());
    } else {
        info(__func__, "ProcessModule is set to: 0");
    }
}

} /* namespace PARTONS */
