#include "../../../../include/partons/modules/process/DVCSModule.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/beans/Scale.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConstantCFFModel.h"
//#include "../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../include/partons/modules/scale/Q2Multiplier.h"
#include "../../../../include/partons/modules/xb_to_xi/XBToXi.h"
#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

DVCSModule::DVCSModule(const std::string &className) :
        ProcessModule(className), m_phaseSpace(0.), m_tmin(0.), m_tmax(0.), m_xBmin(
                0), m_y(0.), m_epsilon(0.) {
    m_channel = ObservableChannel::DVCS;
}

DVCSModule::~DVCSModule() {

}

DVCSModule::DVCSModule(const DVCSModule& other) :
        ProcessModule(other) {
    m_phaseSpace = other.m_phaseSpace;
    m_tmin = other.m_tmin;
    m_tmax = other.m_tmax;
    m_xBmin = other.m_xBmin;
    m_y = other.m_y;
    m_epsilon = other.m_epsilon;

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
    m_epsilon = 2 * m_xB * PROTON_MASS / sqrt(m_Q2);
    m_y = m_Q2 / (2 * m_xB * PROTON_MASS * m_E);
    double eps2 = m_epsilon * m_epsilon;
    double epsroot = sqrt(1 + eps2);
    double tfactor = -m_Q2 / (4 * m_xB * (1 - m_xB) + eps2);
    m_tmin = tfactor * (2 * (1 - m_xB) * (1 - epsroot) + eps2);
    m_tmax = tfactor * (2 * (1 - m_xB) * (1 + epsroot) + eps2);
    m_xBmin = 2 * m_Q2 * m_E / PROTON_MASS / (4 * m_E * m_E - m_Q2);

    debug(__func__, "Entered function.");
}

void DVCSModule::initModule(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    debug(__func__, "Entered function.");
}

void DVCSModule::isModuleWellConfigured() {
    // Test kinematic domain of xB
    if (m_xB < m_xBmin || m_xB > 1) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of xB = " << m_xB
                << " does not lay between xBmin = " << m_xBmin << " and 1.";
        warn(__func__, formatter.str());
    }

    // Test kinematic domain of t
    if (m_t > m_tmin || m_t < m_tmax) {
        ElemUtils::Formatter formatter;
        formatter << " Input value of t = " << m_t
                << " does not lay between t_max = " << m_tmax << " and t_min = "
                << m_tmin << " (DVCS kinematic limits).";
        warn(__func__, formatter.str());
    }

    // Test kinematic domain of Q2
    if (m_Q2 < 0) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of Q2 = " << m_Q2 << " is not > 0.";
        warn(__func__, formatter.str());
    }

    // Test kinematic domain of beam energy
    if (m_y < 0 || m_y > 1) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of y = " << m_y
                << " (lepton energy fraction) does not lay between 0 and 1.";
        warn(__func__, formatter.str());
    }

    if (m_pScaleModule == 0) {
        error(__func__,
                "m_pScaleModule is NULL pointer ; Use configure method to configure it");
    }

    if (m_pXiConverterModule == 0) {
        error(__func__,
                "m_pXiConverterModule is NULL pointer ; Use configure method to configure it");
    }

    debug(__func__, "Entered function.");
}

void DVCSModule::computeConvolCoeffFunction(double xB, double t, double Q2) {
    if (isPreviousKinematicsDifferent(xB, t, Q2)
            || (BaseObjectRegistry::getInstance()->getObjectClassIdByClassName(
                    m_pConvolCoeffFunctionModule->getClassName())
                    == DVCSConstantCFFModel::classId)) {
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
