#include "../../../../include/partons/modules/process/DVCSModule.h"

//#include <ElementaryUtils/parameters/GenericType.h>
//#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/Scale.h"
#include "../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../include/partons/modules/scale/Q2Multiplier.h"
#include "../../../../include/partons/modules/xb_to_xi/XBToXi.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

DVCSModule::DVCSModule(const std::string &className) :
        ProcessModule(className), m_phaseSpace(0.) {
    m_channel = ObservableChannel::DVCS;
}

DVCSModule::~DVCSModule() {

}

DVCSModule::DVCSModule(const DVCSModule& other) :
        ProcessModule(other) {
    m_phaseSpace = other.m_phaseSpace;

    m_dvcsConvolCoeffFunctionResult = other.m_dvcsConvolCoeffFunctionResult;
}

void DVCSModule::resolveObjectDependencies() {
    m_pScaleModule =
            Partons::getInstance()->getModuleObjectFactory()->newScaleModule(
                    Q2Multiplier::classId);

    m_pXiConverterModule =
            Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(
                    XBToXi::classId);
}

void DVCSModule::initModule() {
    //TODO implement

    debug(__func__, "Entered function.");
}

void DVCSModule::initModule(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    debug(__func__, "Entered function.");
}

void DVCSModule::isModuleWellConfigured() {
    if (m_pScaleModule == 0) {
        error(__func__,
                "m_pScaleModule is NULL pointer ; Use configure method to configure it");
    }

    if (m_pXiConverterModule == 0) {
        error(__func__,
                "m_pXiConverterModule is NULL pointer ; Use configure method to configure it");
    }
}

void DVCSModule::computeConvolCoeffFunction(double xB, double t, double Q2) {
    if (isPreviousKinematicsDifferent(xB, t, Q2)) {
        // Compute scale from Q2
        Scale scale = m_pScaleModule->compute(Q2);

        //TODO Is it possible to move that into the ProcessModule->compute() mother class ?
        m_dvcsConvolCoeffFunctionResult =
                static_cast<DVCSConvolCoeffFunctionModule*>(m_pConvolCoeffFunctionModule)->compute(
                        m_pXiConverterModule->compute(xB, t, Q2), t, Q2,
                        scale.getMuF2(), scale.getMuR2(), GPDType::ALL);
    }

    debug(__func__,
            ElemUtils::Formatter() << "m_dvcsConvolCoeffFunctionResult = "
                    << m_dvcsConvolCoeffFunctionResult.toString());

    m_xB = xB;
    m_t = t;
    m_Q2 = Q2;

    initModule();
    isModuleWellConfigured();
}

double DVCSModule::computeCrossSection(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization, double phi) {

    m_phi = phi;

    debug(__func__,
            ElemUtils::Formatter() << "beamHelicity = " << beamHelicity
                    << " beamCharge = " << beamCharge << " phi = " << m_phi);

    initModule(beamHelicity, beamCharge, targetPolarization);

    return CrossSectionBH(beamHelicity, beamCharge, targetPolarization)
            + CrossSectionVCS(beamHelicity, beamCharge, targetPolarization)
            + CrossSectionInterf(beamHelicity, beamCharge, targetPolarization);
}

std::complex<double> DVCSModule::getConvolCoeffFunctionValue(
        GPDType::Type gpdType) {
    std::complex<double> result = 0.;

    if (m_dvcsConvolCoeffFunctionResult.isAvailable(gpdType)) {
        result = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
    }

    return result;
}

void DVCSModule::configure(const ElemUtils::Parameters &parameters) {
    ProcessModule::configure(parameters);
}

void DVCSModule::setConvolCoeffFunctionModule(
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {
    if (pConvolCoeffFunctionModule) {
        if (pConvolCoeffFunctionModule->getChannel()
                == ObservableChannel::DVCS) {
            ProcessModule::setConvolCoeffFunctionModule(
                    pConvolCoeffFunctionModule);
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Cannot set ConvolCoeffFunctionModule, because its channel is different from ProcessModule : "
                            << ObservableChannel(
                                    pConvolCoeffFunctionModule->getChannel()).toString());
        }
    } else {
        error(__func__,
                "Cannot set ConvolCoeffFunctionModule, because it's a NULL pointer");
    }

}
