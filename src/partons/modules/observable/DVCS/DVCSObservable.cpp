#include "../../../../../include/partons/modules/observable/DVCS/DVCSObservable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <iostream>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/DVCSObservableService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"

#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h"
#include "../../../../../include/partons/modules/gpd/GPDGK19.h"
#include "../../../../../include/partons/modules/process/DVCS/DVCSProcessGV08.h"
#include "../../../../../include/partons/modules/scales/DVCS/DVCSScalesQ2Multiplier.h"
#include "../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterXBToXi.h"

namespace PARTONS {

const std::string DVCSObservable::DVCS_OBSERVABLE_MODULE_CLASS_NAME =
        "DVCSObservableModule";

DVCSObservable::DVCSObservable(const std::string &className) :
        Observable(className, ChannelType::DVCS), m_pProcessModule(0) {
}

DVCSObservable::DVCSObservable(const DVCSObservable& other) :
        Observable(other) {

    if (other.m_pProcessModule != 0) {
        m_pProcessModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pProcessModule);
    } else {
        m_pProcessModule = 0;
    }
}

DVCSObservable::~DVCSObservable() {

    if (m_pProcessModule != 0) {
        setProcessModule(0);
        m_pProcessModule = 0;
    }
}

std::string DVCSObservable::toString() const {
    return Observable<DVCSObservableKinematic, DVCSObservableResult>::toString();
}

void DVCSObservable::resolveObjectDependencies() {
    Observable<DVCSObservableKinematic, DVCSObservableResult>::resolveObjectDependencies();
}

void DVCSObservable::run() {

    try {

        //get service
        DVCSObservableService* pDVCSObservableService =
                Partons::getInstance()->getServiceObjectRegistry()->getDVCSObservableService();

        //run until empty
        while (!(pDVCSObservableService->isEmptyTaskQueue())) {

            //kinematics
            DVCSObservableKinematic kinematic;

            //list of GPD types
            List<GPDType> gpdType;

            //set
            ElemUtils::Packet packet =
                    pDVCSObservableService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdType;

            //debug information
            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            //object to be returned
            DVCSObservableResult result = compute(kinematic, gpdType);

            //helpful to sort later if kinematic is coming from database
            result.setIndexId(kinematic.getIndexId());

            //add
            pDVCSObservableService->add(result);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void DVCSObservable::configure(const ElemUtils::Parameters &parameters) {
    Observable<DVCSObservableKinematic, DVCSObservableResult>::configure(
            parameters);
}

void DVCSObservable::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    Observable<DVCSObservableKinematic, DVCSObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for process module
    it = subModulesData.find(DVCSProcessModule::DVCS_PROCESS_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
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

DVCSObservableResult DVCSObservable::compute(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {



    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    DVCSObservableResult result(computeObservable(kinematic, gpdType),
            kinematic);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<GPDType> DVCSObservable::getListOfAvailableGPDTypeForComputation() const {
    return m_pProcessModule->getConvolCoeffFunctionModule()->getListOfAvailableGPDTypeForComputation();
}

std::vector<double> DVCSObservable::test() {

    std::vector<double> result;

    PARTONS::DVCSProcessModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
                    PARTONS::DVCSProcessGV08::classId);
    setProcessModule(pProcessModule);

    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModel =
        PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                PARTONS::DVCSCFFStandard::classId);

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    pDVCSCFFModel->setGPDModule(pGPDModule);

    pDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    pProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);

    PARTONS::DVCSXiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSXiConverterModule(
                    PARTONS::DVCSXiConverterXBToXi::classId);
    pProcessModule->setXiConverterModule(pXiConverterModule);

    PARTONS::DVCSScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSScalesModule(
                    PARTONS::DVCSScalesQ2Multiplier::classId);
    pProcessModule->setScaleModule(pScalesModule);

    result.push_back(compute(DVCSObservableKinematic(0.2, -0.1, 2., 6., M_PI/2.),
                        pDVCSCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    result.push_back(compute(DVCSObservableKinematic(0.02, -0.5, 16., 1000., M_PI/4.),
                        pDVCSCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());

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
            pDVCSCFFModel, 0);
    pDVCSCFFModel = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    return result;
}

void DVCSObservable::initModule() {
    Observable<DVCSObservableKinematic, DVCSObservableResult>::initModule();
}

void DVCSObservable::isModuleWellConfigured() {
    Observable<DVCSObservableKinematic, DVCSObservableResult>::isModuleWellConfigured();
}

DVCSProcessModule* DVCSObservable::getProcessModule() const {
    return m_pProcessModule;
}

void DVCSObservable::setProcessModule(DVCSProcessModule* pProcessModule) {

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
