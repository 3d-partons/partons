#include "../../../../../include/partons/modules/observable/DDVCS/DDVCSObservable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <iostream>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/process/DDVCS/DDVCSProcessModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/DDVCSObservableService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"

#include "../../../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSCFFStandard.h"
#include "../../../../../include/partons/modules/gpd/GPDGK19.h"
#include "../../../../../include/partons/modules/process/DDVCS/DDVCSProcessDMSW22.h"
#include "../../../../../include/partons/modules/scales/DDVCS/DDVCSScalesVirtualitiesSum.h"
#include "../../../../../include/partons/modules/xi_converter/DDVCS/DDVCSXiConverterTNeglected.h"

namespace PARTONS {

const std::string DDVCSObservable::DDVCS_OBSERVABLE_MODULE_CLASS_NAME =
        "DDVCSObservableModule";

DDVCSObservable::DDVCSObservable(const std::string &className) :
        Observable(className, ChannelType::DDVCS), m_pProcessModule(0) {
}

DDVCSObservable::DDVCSObservable(const DDVCSObservable& other) :
        Observable(other) {

    if (other.m_pProcessModule != 0) {
        m_pProcessModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pProcessModule);
    } else {
        m_pProcessModule = 0;
    }
}

DDVCSObservable::~DDVCSObservable() {

    if (m_pProcessModule != 0) {
        setProcessModule(0);
        m_pProcessModule = 0;
    }
}

std::string DDVCSObservable::toString() const {
    return Observable<DDVCSObservableKinematic, DDVCSObservableResult>::toString();
}

void DDVCSObservable::resolveObjectDependencies() {
    Observable<DDVCSObservableKinematic, DDVCSObservableResult>::resolveObjectDependencies();
}

void DDVCSObservable::run() {

    try {

        //get service
        DDVCSObservableService* pDDVCSObservableService =
                Partons::getInstance()->getServiceObjectRegistry()->getDDVCSObservableService();

        //run until empty
        while (!(pDDVCSObservableService->isEmptyTaskQueue())) {

            //kinematics
            DDVCSObservableKinematic kinematic;

            //list of GPD types
            List<GPDType> gpdType;

            //set
            ElemUtils::Packet packet =
                    pDDVCSObservableService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdType;

            //debug information
            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            //object to be returned
            DDVCSObservableResult result = compute(kinematic, gpdType);

            //helpful to sort later if kinematic is coming from database
            result.setIndexId(kinematic.getIndexId());

            //add
            pDDVCSObservableService->add(result);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void DDVCSObservable::configure(const ElemUtils::Parameters &parameters) {
    Observable<DDVCSObservableKinematic, DDVCSObservableResult>::configure(
            parameters);
}

void DDVCSObservable::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    Observable<DDVCSObservableKinematic, DDVCSObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for process module
    it = subModulesData.find(DDVCSProcessModule::DDVCS_PROCESS_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newDDVCSProcessModule(
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

DDVCSObservableResult DDVCSObservable::compute(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {



    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    DDVCSObservableResult result(computeObservable(kinematic, gpdType),
            kinematic);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<GPDType> DDVCSObservable::getListOfAvailableGPDTypeForComputation() const {
    return m_pProcessModule->getConvolCoeffFunctionModule()->getListOfAvailableGPDTypeForComputation();
}

std::vector<double> DDVCSObservable::test() {

    std::vector<double> result;

    PARTONS::DDVCSProcessModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDDVCSProcessModule(
                    PARTONS::DDVCSProcessDMSW22::classId);
    setProcessModule(pProcessModule);

    PARTONS::DDVCSConvolCoeffFunctionModule* pDDVCSCFFModel =
        PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDDVCSConvolCoeffFunctionModule(
                PARTONS::DDVCSCFFStandard::classId);

    GPDModule *pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                GPDGK16::classId);
    pDDVCSCFFModel->setGPDModule(pGPDModule);

    pDDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    pProcessModule->setConvolCoeffFunctionModule(pDDVCSCFFModel);

    PARTONS::DDVCSXiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDDVCSXiConverterModule(
                    PARTONS::DDVCSXiConverterTNeglected::classId);
    pProcessModule->setXiConverterModule(pXiConverterModule);

    PARTONS::DDVCSScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDDVCSScalesModule(
                    PARTONS::DDVCSScalesVirtualitiesSum::classId);
    pProcessModule->setScaleModule(pScalesModule);

    result.push_back(compute(DDVCSObservableKinematic(0.1, -0.1, 2., 0.5, 12., M_PI/2., M_PI/3., M_PI/4.),
                        pDDVCSCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    result.push_back(compute(DDVCSObservableKinematic(0.02, -0.5, 9., 2., 1000., M_PI/4.,M_PI/3.,M_PI/2.),
                        pDDVCSCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());

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
            pDDVCSCFFModel, 0);
    pDDVCSCFFModel = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    return result;
}

void DDVCSObservable::initModule() {
    Observable<DDVCSObservableKinematic, DDVCSObservableResult>::initModule();
}

void DDVCSObservable::isModuleWellConfigured() {
    Observable<DDVCSObservableKinematic, DDVCSObservableResult>::isModuleWellConfigured();
}

DDVCSProcessModule* DDVCSObservable::getProcessModule() const {
    return m_pProcessModule;
}

void DDVCSObservable::setProcessModule(DDVCSProcessModule* pProcessModule) {

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
