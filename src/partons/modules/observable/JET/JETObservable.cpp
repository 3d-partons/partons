#include "../../../../../include/partons/modules/observable/JET/JETObservable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <iostream>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/modules/convol_coeff_function/JET/JETConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/process/JET/JETProcessModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/JETObservableService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/ServiceObjectTyped.h"

// #include "../../../../../include/partons/modules/convol_coeff_function/JET/JETCFFStandard.h"
// #include "../../../../../include/partons/modules/gpd/GPDGK19.h"
// #include "../../../../../include/partons/modules/process/JET/JETProcessGV08.h"
// #include "../../../../../include/partons/modules/scales/JET/JETScalesQ2Multiplier.h"
// #include "../../../../../include/partons/modules/xi_converter/JET/JETXiConverterXBToXi.h"

namespace PARTONS {

const std::string JETObservable::JET_OBSERVABLE_MODULE_CLASS_NAME =
        "JETObservableModule";

JETObservable::JETObservable(const std::string &className) :
        Observable(className, ChannelType::JET), m_pProcessModule(0) {
}

JETObservable::JETObservable(const JETObservable& other) :
        Observable(other) {

    if (other.m_pProcessModule != 0) {
        m_pProcessModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pProcessModule);
    } else {
        m_pProcessModule = 0;
    }
}

JETObservable::~JETObservable() {

    if (m_pProcessModule != 0) {
        setProcessModule(0);
        m_pProcessModule = 0;
    }
}

std::string JETObservable::toString() const {
    return Observable<JETObservableKinematic, JETObservableResult>::toString();
}

void JETObservable::resolveObjectDependencies() {
    Observable<JETObservableKinematic, JETObservableResult>::resolveObjectDependencies();
}

void JETObservable::run() {

    try {

        //get service
        JETObservableService* pJETObservableService =
                Partons::getInstance()->getServiceObjectRegistry()->getJETObservableService();

        //run until empty
        while (!(pJETObservableService->isEmptyTaskQueue())) {

            //kinematics
            JETObservableKinematic kinematic;

            //list of GPD types
            List<GPDType> gpdType;

            //set
            ElemUtils::Packet packet =
                    pJETObservableService->popTaskFormQueue();
            packet >> kinematic;
            packet >> gpdType;

            //debug information
            debug(__func__,
                    ElemUtils::Formatter() << "objectId = " << getObjectId()
                            << " " << kinematic.toString());

            //object to be returned
            JETObservableResult result = compute(kinematic, gpdType);

            //helpful to sort later if kinematic is coming from database
            result.setIndexId(kinematic.getIndexId());

            //add
            pJETObservableService->add(result);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void JETObservable::configure(const ElemUtils::Parameters &parameters) {
    Observable<JETObservableKinematic, JETObservableResult>::configure(
            parameters);
}

void JETObservable::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    //run for mother
    Observable<JETObservableKinematic, JETObservableResult>::prepareSubModules(
            subModulesData);

    //iterator
    std::map<std::string, BaseObjectData>::const_iterator it;

    //search for process module
    it = subModulesData.find(JETProcessModule::JET_PROCESS_MODULE_CLASS_NAME);

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
                    Partons::getInstance()->getModuleObjectFactory()->newJETProcessModule(
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

JETObservableResult JETObservable::compute(
        const JETObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {



    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    JETObservableResult result(computeObservable(kinematic, gpdType),
            kinematic);

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<GPDType> JETObservable::getListOfAvailableGPDTypeForComputation() const {
    return m_pProcessModule->getConvolCoeffFunctionModule()->getListOfAvailableGPDTypeForComputation();
}

std::vector<double> JETObservable::test() {

    std::vector<double> result;

    // PARTONS::JETProcessModule* pProcessModule =
    //         PARTONS::Partons::getInstance()->getModuleObjectFactory()->newJETProcessModule(
    //                 PARTONS::JETProcessGV08::classId);
    // setProcessModule(pProcessModule);
    //
    // PARTONS::JETConvolCoeffFunctionModule* pJETCFFModel =
    //     PARTONS::Partons::getInstance()->getModuleObjectFactory()->newJETConvolCoeffFunctionModule(
    //             PARTONS::JETCFFStandard::classId);
    //
    // GPDModule *pGPDModule =
    //         Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
    //             GPDGK16::classId);
    // pJETCFFModel->setGPDModule(pGPDModule);
    //
    // pJETCFFModel->configure(
    //         ElemUtils::Parameter(
    //                 PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
    //                 PARTONS::PerturbativeQCDOrderType::LO));
    //
    // pProcessModule->setConvolCoeffFunctionModule(pJETCFFModel);
    //
    // PARTONS::JETXiConverterModule* pXiConverterModule =
    //         PARTONS::Partons::getInstance()->getModuleObjectFactory()->newJETXiConverterModule(
    //                 PARTONS::JETXiConverterXBToXi::classId);
    // pProcessModule->setXiConverterModule(pXiConverterModule);
    //
    // PARTONS::JETScalesModule* pScalesModule =
    //         PARTONS::Partons::getInstance()->getModuleObjectFactory()->newJETScalesModule(
    //                 PARTONS::JETScalesQ2Multiplier::classId);
    // pProcessModule->setScaleModule(pScalesModule);
    //
    // result.push_back(compute(JETObservableKinematic(0.2, -0.1, 2., 6., M_PI/2.),
    //                     pJETCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    // result.push_back(compute(JETObservableKinematic(0.02, -0.5, 16., 1000., M_PI/4.),
    //                     pJETCFFModel->getListOfAvailableGPDTypeForComputation()).getValue().getValue());
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //            pScalesModule, 0);
    // pScalesModule = 0;
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //      pXiConverterModule, 0);
    // pXiConverterModule = 0;
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //     pGPDModule, 0);
    // pGPDModule = 0;
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //         pJETCFFModel, 0);
    // pJETCFFModel = 0;
    //
    // PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
    //         pProcessModule, 0);
    // pProcessModule = 0;

    return result;
}

void JETObservable::initModule() {
    Observable<JETObservableKinematic, JETObservableResult>::initModule();
}

void JETObservable::isModuleWellConfigured() {
    Observable<JETObservableKinematic, JETObservableResult>::isModuleWellConfigured();
}

JETProcessModule* JETObservable::getProcessModule() const {
    return m_pProcessModule;
}

void JETObservable::setProcessModule(JETProcessModule* pProcessModule) {

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
