#include "../../../../../include/partons/modules/observable/DVMP/DVMPObservable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <iostream>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/DVMPObservableService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"

#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06.h"
#include "../../../../../include/partons/modules/gpd/GPDGK19.h"
#include "../../../../../include/partons/modules/process/DVMP/DVMPProcessGK06.h"
#include "../../../../../include/partons/modules/scales/DVMP/DVMPScalesQ2Multiplier.h"
#include "../../../../../include/partons/modules/xi_converter/DVMP/DVMPXiConverterXBToXi.h"

namespace PARTONS {

const std::string DVMPObservable::DVMP_OBSERVABLE_MODULE_CLASS_NAME =
        "DVMPObservableModule";

DVMPObservable::DVMPObservable(const std::string &className) :
        Observable(className, ChannelType::DVMP), m_pProcessModule(0) {
}

DVMPObservable::DVMPObservable(const DVMPObservable& other) :
        Observable(other) {

    if (other.m_pProcessModule != 0) {
        m_pProcessModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pProcessModule);
    } else {
        m_pProcessModule = 0;
    }
}

DVMPObservable::~DVMPObservable() {

    if (m_pProcessModule != 0) {
        setProcessModule(0);
        m_pProcessModule = 0;
    }
}

std::string DVMPObservable::toString() const {
    return Observable<DVMPObservableKinematic, DVMPObservableResult>::toString();
}

void DVMPObservable::resolveObjectDependencies() {
    Observable<DVMPObservableKinematic, DVMPObservableResult>::resolveObjectDependencies();
}

void DVMPObservable::run() {

    try {

        //get service
        DVMPObservableService* pDVMPObservableService =
                Partons::getInstance()->getServiceObjectRegistry()->getDVMPObservableService();

        //run until empty
        while (!(pDVMPObservableService->isEmptyTaskQueue())) {

            //kinematics
            DVMPObservableKinematic kinematic;

            //list of GPD types
            List<GPDType> gpdType;

            //set
            ElemUtils::Packet packet =
                    pDVMPObservableService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdType;

            //debug information
            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            //object to be returned
            DVMPObservableResult result = compute(kinematic, gpdType);

            //helpful to sort later if kinematic is coming from database
            result.setIndexId(kinematic.getIndexId());

            //add
            pDVMPObservableService->add(result);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void DVMPObservable::configure(const ElemUtils::Parameters &parameters) {
    Observable<DVMPObservableKinematic, DVMPObservableResult>::configure(
            parameters);
}

void DVMPObservable::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    Observable<DVMPObservableKinematic, DVMPObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for process module
    it = subModulesData.find(DVMPProcessModule::DVMP_PROCESS_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newDVMPProcessModule(
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

DVMPObservableResult DVMPObservable::compute(
        const DVMPObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {



    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    DVMPObservableResult result(computeObservable(kinematic, gpdType),
            kinematic);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<GPDType> DVMPObservable::getListOfAvailableGPDTypeForComputation() const {
    return m_pProcessModule->getConvolCoeffFunctionModule()->getListOfAvailableGPDTypeForComputation();
}

std::vector<double> DVMPObservable::test() {

    std::vector<double> result;

    PARTONS::DVMPProcessModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVMPProcessModule(
                    PARTONS::DVMPProcessGK06::classId);
    setProcessModule(pProcessModule);

    PARTONS::DVMPConvolCoeffFunctionModule* pDVMPCFFModel =
        PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVMPConvolCoeffFunctionModule(
                PARTONS::DVMPCFFGK06::classId);

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    pDVMPCFFModel->setGPDModule(pGPDModule);

    pDVMPCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    pProcessModule->setConvolCoeffFunctionModule(pDVMPCFFModel);

    PARTONS::DVMPXiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVMPXiConverterModule(
                    PARTONS::DVMPXiConverterXBToXi::classId);
    pProcessModule->setXiConverterModule(pXiConverterModule);

    PARTONS::DVMPScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVMPScalesModule(
                    PARTONS::DVMPScalesQ2Multiplier::classId);
    pProcessModule->setScaleModule(pScalesModule);

    result.push_back(compute(DVMPObservableKinematic(0.2, -0.1, 2., 6., M_PI/2., MesonType::PI0),
                        pDVMPCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    result.push_back(compute(DVMPObservableKinematic(0.02, -0.5, 16., 1000., M_PI/4., MesonType::PI0),
                        pDVMPCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());

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
            pDVMPCFFModel, 0);
    pDVMPCFFModel = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    return result;
}

void DVMPObservable::initModule() {
    Observable<DVMPObservableKinematic, DVMPObservableResult>::initModule();
}

void DVMPObservable::isModuleWellConfigured() {
    Observable<DVMPObservableKinematic, DVMPObservableResult>::isModuleWellConfigured();
}

DVMPProcessModule* DVMPObservable::getProcessModule() const {
    return m_pProcessModule;
}

void DVMPObservable::setProcessModule(DVMPProcessModule* pProcessModule) {

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
