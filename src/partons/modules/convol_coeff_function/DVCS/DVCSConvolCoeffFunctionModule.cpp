#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <stdexcept>
#include <utility>

#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../include/partons/modules/active_flavors/NfFunctionExample.h"
#include "../../../../../include/partons/modules/alphaS/RunningAlphaStrong.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"

DVCSConvolCoeffFunctionModule::DVCSConvolCoeffFunctionModule(
        const std::string &className) :
        ConvolCoeffFunctionModule(className), m_xi(0.), m_t(0.), m_Q2(0.), m_MuF2(
                0.), m_MuR2(0.), m_nf(0), m_qcdOrderType(
                PerturbativeQCDOrderType::UNDEFINED), m_currentGPDComputeType(
                GPDType::UNDEFINED), m_gpdType(GPDType::UNDEFINED), m_pRunningAlphaStrongModule(
                0), m_pNfConvolCoeffFunction(0) {
    m_channel = ObservableChannel::DVCS;
}

DVCSConvolCoeffFunctionModule::DVCSConvolCoeffFunctionModule(
        const DVCSConvolCoeffFunctionModule &other) :
        ConvolCoeffFunctionModule(other) {
    m_listOfCFFComputeFunctionAvailable =
            other.m_listOfCFFComputeFunctionAvailable;
    m_it = other.m_it;

    m_xi = other.m_xi;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;

    m_nf = other.m_nf;

    m_qcdOrderType = other.m_qcdOrderType;
    m_currentGPDComputeType = other.m_currentGPDComputeType;
    m_gpdType = other.m_gpdType;

    if (other.m_pRunningAlphaStrongModule != 0) {
        // RunningAlphaStrongModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pRunningAlphaStrongModule =
                (other.m_pRunningAlphaStrongModule)->clone();
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    if (other.m_pNfConvolCoeffFunction != 0) {
        // ActiveFlavorsModule is an abstract class, so it's impossible to use copy constructor to get a new instance of the object
        m_pNfConvolCoeffFunction = (other.m_pNfConvolCoeffFunction)->clone();
    } else {
        m_pNfConvolCoeffFunction = 0;
    }
}

DVCSConvolCoeffFunctionModule::~DVCSConvolCoeffFunctionModule() {
    //TODO Remove all pointers dependencies
}

void DVCSConvolCoeffFunctionModule::resolveObjectDependencies() {
    ConvolCoeffFunctionModule::resolveObjectDependencies();

    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrong::classId);

    m_pNfConvolCoeffFunction =
            Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsModule(
                    NfFunctionExample::classId);
}

//TODO implement
void DVCSConvolCoeffFunctionModule::initModule() {

    debug(__func__, ElemUtils::Formatter() << "executed");
}

void DVCSConvolCoeffFunctionModule::isModuleWellConfigured() {
    debug(__func__, ElemUtils::Formatter() << "executed");

    // Test kinematic domain of Xi
    if (m_xi < 0 || m_xi > 1) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Xi = " << m_xi
                        << " do not lay between 0 and 1.");
    }

    // Test kinematic domain of t
    if (m_t > 0) {
        warn(__func__,
                ElemUtils::Formatter() << " Input value of t = " << m_t
                        << " is not <= 0.");
    }

    // Test kinematic domain of Q2
    if (m_Q2 < 0) {
        warn(__func__,
                ElemUtils::Formatter() << "Input value of Q2 = " << m_Q2
                        << " is not > 0.");
    }

    if (m_pGPDModule == 0) {
        error(__func__, "m_pGPDModule is NULL");
    }

    if (m_pRunningAlphaStrongModule == 0) {
        error(__func__, "m_pRunningAlphaStrongModule is NULL");
    }

    if (m_pNfConvolCoeffFunction == 0) {
        error(__func__, "m_pNfConvolCoeffFunction is NULL");
    }
}

DVCSConvolCoeffFunctionResult DVCSConvolCoeffFunctionModule::compute(
        const double xi, const double t, const double Q2, const double MuF2,
        const double MuR2, GPDType::Type gpdComputeType) {
    preCompute(xi, t, Q2, MuF2, MuR2, gpdComputeType);

    DVCSConvolCoeffFunctionResult dvcsConvolCoeffFunctionResult;

    switch (m_gpdType) {
    case GPDType::ALL: {

        for (m_it = m_listOfCFFComputeFunctionAvailable.begin();
                m_it != m_listOfCFFComputeFunctionAvailable.end(); m_it++) {
            m_currentGPDComputeType = m_it->first;

            debug(__func__,
                    ElemUtils::Formatter() << "m_currentGPDComputeType = "
                            << GPDType(m_currentGPDComputeType).toString());

            // call function storef in the base class map
            dvcsConvolCoeffFunctionResult.add(m_currentGPDComputeType,
                    ((*this).*(m_it->second))());
        }
        break;
    }
    default: {
        m_it = m_listOfCFFComputeFunctionAvailable.find(m_gpdType);
        if (m_it != m_listOfCFFComputeFunctionAvailable.end()) {
            m_currentGPDComputeType = m_gpdType;
            // call function storef in the base class map
            dvcsConvolCoeffFunctionResult.add(m_currentGPDComputeType,
                    ((*this).*(m_it->second))());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Cannot run computation for this value of GPDType = "
                            << GPDType(m_gpdType).toString());
        }
        break;
    }
    }

    dvcsConvolCoeffFunctionResult.setComputationModuleName(getClassName());
    dvcsConvolCoeffFunctionResult.setKinematic(
            DVCSConvolCoeffFunctionKinematic(m_xi, m_t, m_Q2, m_MuF2, m_MuR2));

    debug(__func__, dvcsConvolCoeffFunctionResult.toString());

    return dvcsConvolCoeffFunctionResult;
}

std::complex<double> DVCSConvolCoeffFunctionModule::computeUnpolarized() {
    throw std::runtime_error(
            "[CFFModule::computeUnpolarized] check your child implementation : "
                    + getClassName());
}

std::complex<double> DVCSConvolCoeffFunctionModule::computePolarized() {
    throw std::runtime_error(
            "[CFFModule::computeUnpolarized] check your child implementation : "
                    + getClassName());
}

std::complex<double> DVCSConvolCoeffFunctionModule::computeCFF() {
    throw std::runtime_error(
            "[CFFModule::computeCFF] check your child implementation : "
                    + getClassName());
}

void DVCSConvolCoeffFunctionModule::preCompute(const double xi, const double t,
        const double Q2, const double MuF2, const double MuR2,
        GPDType::Type gpdComputeType) {

    debug(__func__,
            ElemUtils::Formatter() << "enter preCompute() function ...");

    m_xi = xi;
    m_t = t;
    m_Q2 = Q2;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
    m_gpdType = gpdComputeType;

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

PerturbativeQCDOrderType::Type DVCSConvolCoeffFunctionModule::getQCDOrderType() const {
    return m_qcdOrderType;
}

void DVCSConvolCoeffFunctionModule::setQCDOrderType(
        PerturbativeQCDOrderType::Type qcdOrderType) {
    m_qcdOrderType = qcdOrderType;
}

//TODO handle string for XML file and native type from C++ code ; see QCD_ORDER_TYPE test
void DVCSConvolCoeffFunctionModule::configure(
        const ElemUtils::Parameters &parameters) {

    if (parameters.isAvailable(
            PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE)) {
        // try to set m_qcdOrderType by standard way
        try {
            m_qcdOrderType =
                    static_cast<PerturbativeQCDOrderType::Type>(parameters.getLastAvailable().toUInt());

        } catch (const std::exception &e) {
            // if an exception is raised it means that it's a string configuration value
            m_qcdOrderType = PerturbativeQCDOrderType(
                    parameters.getLastAvailable().toString()).getType();
        }

        info(__func__,
                ElemUtils::Formatter()
                        << PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE
                        << " configured with value = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    ConvolCoeffFunctionModule::configure(parameters);
}
