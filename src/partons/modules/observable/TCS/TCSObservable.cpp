#include "../../../../../include/partons/modules/observable/TCS/TCSObservable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <stddef.h>
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

    // check units

    //TODO
//    m_xB = kinematic.getXB().makeSameUnitAs(PhysicalUnit::NONE).getValue();
//    m_t = kinematic.getT().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
//    m_Q2 = kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
//    m_E = kinematic.getE().makeSameUnitAs(PhysicalUnit::GEV).getValue();
//    m_phi = kinematic.getPhi().makeSameUnitAs(PhysicalUnit::RAD).getValue();
}

void TCSObservable::initModule() {
    Observable<TCSObservableKinematic, TCSObservableResult>::initModule();
}

void TCSObservable::isModuleWellConfigured() {

    //run mother
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

std::vector<double> TCSObservable::serializeKinematicsAndGPDTypesToVector(
        const TCSObservableKinematic& kin, const List<GPDType>& list) const {

    std::vector<double> result;

    result.push_back(kin.getT().getValue());
    result.push_back(static_cast<double>(kin.getT().getUnit()));
    result.push_back(kin.getQ2Prim().getValue());
    result.push_back(static_cast<double>(kin.getQ2Prim().getUnit()));
    result.push_back(kin.getE().getValue());
    result.push_back(static_cast<double>(kin.getE().getUnit()));
    result.push_back(kin.getPhi().getValue());
    result.push_back(static_cast<double>(kin.getPhi().getUnit()));
    result.push_back(kin.getTheta().getValue());
    result.push_back(static_cast<double>(kin.getTheta().getUnit()));

    for (size_t i = 0; i < list.size(); i++) {
        result.push_back(static_cast<double>(list[i].getType()));
    }

    return result;

}

void TCSObservable::unserializeKinematicsAndGPDTypesFromVector(
        const std::vector<double>& vec, TCSObservableKinematic& kin,
        List<GPDType>& list) const {

    PhysicalType<double> t(vec.at(0),
            static_cast<PhysicalUnit::Type>(vec.at(1)));
    PhysicalType<double> Q2Prim(vec.at(2),
            static_cast<PhysicalUnit::Type>(vec.at(3)));
    PhysicalType<double> E(vec.at(4),
            static_cast<PhysicalUnit::Type>(vec.at(5)));
    PhysicalType<double> phi(vec.at(6),
            static_cast<PhysicalUnit::Type>(vec.at(7)));
    PhysicalType<double> theta(vec.at(8),
            static_cast<PhysicalUnit::Type>(vec.at(9)));

    kin = TCSObservableKinematic(t, Q2Prim, E, phi, theta);

    list.clear();

    for (size_t i = 0; i < vec.size(); i++) {
        list.add(GPDType(static_cast<GPDType::Type>(vec.at(i))));
    }
}

} /* namespace PARTONS */
