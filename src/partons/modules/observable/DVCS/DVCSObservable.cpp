#include "../../../../../include/partons/modules/observable/DVCS/DVCSObservable.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/Result.h"

namespace PARTONS {

DVCSObservable::DVCSObservable(const std::string &className,
        ObservableType::Type observableType) :
        Observable(className, ChannelType::DVCS), m_observableType(
                observableType), m_xB(0.), m_t(0.), m_Q2(0.), m_E(0.), m_phi(0.) {
}

DVCSObservable::DVCSObservable(const DVCSObservable& other) :
        Observable(other), m_observableType(other.m_observableType), m_xB(
                other.m_xB), m_t(other.m_t), m_Q2(other.m_Q2), m_E(other.m_E), m_phi(
                other.m_phi) {
}

DVCSObservable::~DVCSObservable() {
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
    Observable<DVCSObservableKinematic, DVCSObservableResult>::prepareSubModules(
            subModulesData);
}

DVCSObservableResult DVCSObservable::compute(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //reset kinematics (virtuality)
    setKinematics(kinematic);

    //execute last child function (virtuality)
    initModule();

    //execute last child function (virtuality)
    isModuleWellConfigured();

    //result
    DVCSObservableResult result;

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
                ElemUtils::Formatter() << "Unkown observable type: "
                        << ObservableType(m_observableType).toString());
    }

    //set rest information
    result.setComputationModuleName(getClassName());
    result.setKinematic(kinematic);

    //return
    return result;
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

double DVCSObservable::getXB() const {
    return m_xB;
}

void DVCSObservable::setXB(double xB) {
    m_xB = xB;
}

double DVCSObservable::getT() const {
    return m_t;
}

void DVCSObservable::setT(double t) {
    m_t = t;
}

double DVCSObservable::getQ2() const {
    return m_Q2;
}

void DVCSObservable::setQ2(double q2) {
    m_Q2 = q2;
}

double DVCSObservable::getE() const {
    return m_E;
}

void DVCSObservable::setE(double e) {
    m_E = e;
}

double DVCSObservable::getPhi() const {
    return m_phi;
}

void DVCSObservable::setPhi(double phi) {
    m_phi = phi;
}

} /* namespace PARTONS */
