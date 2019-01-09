#include "../../../../../include/partons/modules/observable/DVCS/DVCSObservable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

namespace PARTONS {

DVCSObservable::DVCSObservable(const std::string &className,
        ObservableType::Type observableType) :
        Observable(className, ChannelType::DVCS), m_observableType(
                observableType), m_pProcessModule(0), m_xB(0.), m_t(0.), m_Q2(
                0.), m_E(0.), m_phi(0.) {
}

DVCSObservable::DVCSObservable(const DVCSObservable& other) :
        Observable(other), m_observableType(other.m_observableType), m_xB(
                other.m_xB), m_t(other.m_t), m_Q2(other.m_Q2), m_E(other.m_E), m_phi(
                other.m_phi) {

    if (other.m_pProcessModule != 0) {
        m_pProcessModule = other.m_pProcessModule->clone();
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

    //TODO

//    try {
//
//        // Retrieve DVCSObservable service
//        DVCSObservableService* pDVCSObservableService =
//                Partons::getInstance()->getServiceObjectRegistry()->getDVCSObservableService();
//
//        while (!(pDVCSObservableService->isEmptyTaskQueue())) {
//            DVCSObservableKinematic kinematic;
//            List<GPDType> gpdType;
//            ElemUtils::Packet packet = pDVCSObservableService->popTaskFormQueue();
//            packet >> kinematic;
//            packet >> gpdType;
//
//            info(__func__,
//                    ElemUtils::Formatter() << "objectId = " << getObjectId()
//                            << " " << kinematic.toString());
//
//            pDVCSObservableService->add(compute(kinematic, gpdType));
//        }
//
//    } catch (std::exception &e) {
//        std::cerr << e.what() << std::endl;
//    }

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
    it = subModulesData.find(DVCSProcessModule::PROCESS_MODULE_CLASS_NAME);

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

    //TODO gpdType

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //object to be returned
    DVCSObservableResult result(kinematic);

    //check if this observable is a phi dependent observable
    if (m_observableType == ObservableType::PHI) {

        result.setValue(computePhiDVCSObservable(kinematic));
        result.setObservableType(ObservableType::PHI);
    }
    //check if this observable is a Fourier observable (phi moment)
    else if (m_observableType == ObservableType::FOURIER) {

        result.setValue(computeFourierDVCSObservable(kinematic));
        result.setObservableType(ObservableType::FOURIER);
    }
    //check if this observable is an undefined-type observable
    else if (m_observableType == ObservableType::UNDEFINED) {

        result.setValue(computeOtherDVCSObservable(kinematic));
        result.setObservableType(ObservableType::UNDEFINED);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Unknown observable type: "
                        << ObservableType(m_observableType).toString());
    }

    //set module name
    result.setComputationModuleName(getClassName());

    //return
    return result;
}

List<GPDType> DVCSObservable::getListOfAvailableGPDTypeForComputation() const {
    return m_pProcessModule->getConvolCoeffFunctionModule()->getListOfAvailableGPDTypeForComputation();
}

void DVCSObservable::setKinematics(const DVCSObservableKinematic& kinematic) {

    // set variables
    m_xB = kinematic.getXB().getValue();
    m_t = kinematic.getT().getValue();
    m_Q2 = kinematic.getQ2().getValue();
    m_E = kinematic.getE().getValue();
    m_phi = kinematic.getPhi().getValue();
}

void DVCSObservable::initModule() {
    Observable<DVCSObservableKinematic, DVCSObservableResult>::initModule();
}

void DVCSObservable::isModuleWellConfigured() {

    //run mother
    Observable<DVCSObservableKinematic, DVCSObservableResult>::isModuleWellConfigured();

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
    if (m_Q2 < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of Q2 = " << m_Q2 << " is not > 0";
        warn(__func__, formatter.str());
    }

    //test kinematic domain of E
    if (m_E < 0.) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " is not > 0";
        warn(__func__, formatter.str());
    }
}

PhysicalType<double> DVCSObservable::computePhiDVCSObservable(
        const DVCSObservableKinematic& kinematic) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Nothing to do ; Must be implemented in daugther class");
    return PhysicalType<double>();
}

PhysicalType<double> DVCSObservable::computeFourierDVCSObservable(
        const DVCSObservableKinematic& kinematic) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Nothing to do ; Must be implemented in daugther class");
    return PhysicalType<double>();
}

PhysicalType<double> DVCSObservable::computeOtherDVCSObservable(
        const DVCSObservableKinematic& kinematic) {
    throw ElemUtils::CustomException(getClassName(), __func__,
            "Nothing to do ; Must be implemented in daugther class");
    return PhysicalType<double>();
}

ObservableType::Type DVCSObservable::getObservableType() const {
    return m_observableType;
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
