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
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string TCSObservable::TCS_OBSERVABLE_MODULE_CLASS_NAME =
        "TCSObservableModule";

TCSObservable::TCSObservable(const std::string &className) :
        Observable(className, ChannelType::TCS), m_pProcessModule(0), m_xB(0.), m_t(
                0.), m_Q2Prim(0.), m_E(0.), m_phi(0.), m_theta(0.) {
}

TCSObservable::TCSObservable(const TCSObservable& other) :
        Observable(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2Prim(
                other.m_Q2Prim), m_E(other.m_E), m_phi(other.m_phi), m_theta(
                other.m_theta) {

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

    //reset kinematics (virtuality)
    setKinematics(kinematic);

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

void TCSObservable::setKinematics(const TCSObservableKinematic& kinematic) {

    // set variables
    m_xB = kinematic.getXB().makeSameUnitAs(PhysicalUnit::NONE).getValue();
    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_Q2Prim =
            kinematic.getQ2Prim().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
    m_theta = kinematic.getTheta().makeSameUnitAs(PhysicalUnit::RAD).getValue();
}

void TCSObservable::initModule() {
    Observable<TCSObservableKinematic, TCSObservableResult>::initModule();
}

void TCSObservable::isModuleWellConfigured() {

    //run mother
    Observable<TCSObservableKinematic, TCSObservableResult>::isModuleWellConfigured();

    //test kinematic domain of xB
    if (m_xB < 0. || m_xB > 1.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of xB = " << m_xB
                << " does not lay between 0 and 1";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of t
    if (m_t > 0.) {
        ElemUtils::Formatter formatter;
        formatter << " Input value of t = " << m_t << " is not < 0";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of Q2
    if (m_Q2Prim < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of Q2' = " << m_Q2Prim << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of E
    if (m_E < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " is not > 0";
        warn(__func__, formatter.str());
    }
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
