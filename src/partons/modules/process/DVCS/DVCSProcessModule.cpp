#include "../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>

#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../include/partons/beans/Scales.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFConstant.h"
#include "../../../../../include/partons/modules/scales/ScalesModule.h"
#include "../../../../../include/partons/modules/xi_converter/XiConverterModule.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/ConvolCoeffFunctionService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

DVCSProcessModule::DVCSProcessModule(const std::string &className) :
        ProcessModule(className), m_phaseSpace(0.), m_tmin(0.), m_tmax(0.), m_xBmin(
                0), m_y(0.), m_epsilon(0.) {
    m_channel = ObservableChannel::DVCS;
}

DVCSProcessModule::~DVCSProcessModule() {
}

DVCSProcessModule::DVCSProcessModule(const DVCSProcessModule& other) :
        ProcessModule(other) {
    m_phaseSpace = other.m_phaseSpace;
    m_tmin = other.m_tmin;
    m_tmax = other.m_tmax;
    m_xBmin = other.m_xBmin;
    m_y = other.m_y;
    m_epsilon = other.m_epsilon;

    m_dvcsConvolCoeffFunctionResult = other.m_dvcsConvolCoeffFunctionResult;
}

void DVCSProcessModule::resolveObjectDependencies() {
}

void DVCSProcessModule::initModule() {
    m_epsilon = 2 * m_xB * Constant::PROTON_MASS / sqrt(m_Q2);
    m_y = m_Q2 / (2 * m_xB * Constant::PROTON_MASS * m_E);
    double eps2 = m_epsilon * m_epsilon;
    double epsroot = sqrt(1 + eps2);
    double tfactor = -m_Q2 / (4 * m_xB * (1 - m_xB) + eps2);
    m_tmin = tfactor * (2 * (1 - m_xB) * (1 - epsroot) + eps2);
    m_tmax = tfactor * (2 * (1 - m_xB) * (1 + epsroot) + eps2);
    m_xBmin = 2 * m_Q2 * m_E / Constant::PROTON_MASS / (4 * m_E * m_E - m_Q2);

    debug(__func__, "Entered function.");
}

void DVCSProcessModule::initModule(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    debug(__func__, "Entered function.");
}

void DVCSProcessModule::isModuleWellConfigured() {

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

    // Test kinematic domain of E
    if (m_E < 0) {
        ElemUtils::Formatter formatter;
        formatter << "Input value of E = " << m_E << " is not > 0.";
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
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pScaleModule is NULL pointer ; Use configure method to configure it");
    }

    if (m_pXiConverterModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "m_pXiConverterModule is NULL pointer ; Use configure method to configure it");
    }

    debug(__func__, "Entered function.");
}

void DVCSProcessModule::computeConvolCoeffFunction(double xB, double t,
        double Q2, double E, const List<GPDType> & gpdType) {
    if (isPreviousKinematicsDifferent(xB, t, Q2)
            || (BaseObjectRegistry::getInstance()->getObjectClassIdByClassName(
                    m_pConvolCoeffFunctionModule->getClassName())
                    == DVCSCFFConstant::classId)) {
        // Compute scale from Q2
        Scales scale = m_pScaleModule->compute(Q2);

        //TODO Is it possible to move that into the ProcessModule->compute() mother class ?
        //TODO propagate change
//        m_dvcsConvolCoeffFunctionResult = m_pConvolCoeffFunctionModule->compute(
//                m_pXiConverterModule->compute(xB, t, Q2), t, Q2,
//                scale.getMuF2(), scale.getMuR2(), gpdType);

        //m_dvcsConvolCoeffFunctionResult = DVCSConvolCoeffFunctionResult();

        m_dvcsConvolCoeffFunctionResult =
                Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService()->computeForOneCCFModel(
                        DVCSConvolCoeffFunctionKinematic(
                                m_pXiConverterModule->compute(xB, t, Q2), t, Q2,
                                scale.getMuF2(), scale.getMuR2()),
                        m_pConvolCoeffFunctionModule, gpdType);
//
//        for(unsigned int i=0;;)
//        {
//            m_dvcsConvolCoeffFunctionResult.add(gpdType, m_pConvolCoeffFunctionModule->compute(
//                    m_pXiConverterModule->compute(xB, t, Q2), t, Q2,
//                    scale.getMuF2(), scale.getMuR2(), gpdType));
//        }
    }

    debug(__func__,
            ElemUtils::Formatter() << "m_dvcsConvolCoeffFunctionResult = "
                    << m_dvcsConvolCoeffFunctionResult.toString());

    m_xB = xB;
    m_t = t;
    m_Q2 = Q2;

    m_E = E;

    initModule();
    isModuleWellConfigured();
}

double DVCSProcessModule::computeCrossSection(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization, double phi) {
    return computeCrossSection(beamHelicity, beamCharge, targetPolarization,
            phi, DVCSSubProcessType::ALL);
}

double DVCSProcessModule::computeCrossSection(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization, double phi,
        DVCSSubProcessType::Type processType) {

    m_phi = phi;

    debug(__func__,
            ElemUtils::Formatter() << "beamHelicity = " << beamHelicity
                    << " beamCharge = " << beamCharge << " phi = " << m_phi);

    initModule(beamHelicity, beamCharge, targetPolarization);

    double result = 0.;

    if (processType == DVCSSubProcessType::ALL
            || processType == DVCSSubProcessType::DVCS) {
        result += CrossSectionVCS(beamHelicity, beamCharge, targetPolarization);
    }

    if (processType == DVCSSubProcessType::ALL
            || processType == DVCSSubProcessType::BH) {
        result += CrossSectionBH(beamHelicity, beamCharge, targetPolarization);
    }

    if (processType == DVCSSubProcessType::ALL
            || processType == DVCSSubProcessType::INT) {
        result += CrossSectionInterf(beamHelicity, beamCharge,
                targetPolarization);
    }

    return result;
}

std::complex<double> DVCSProcessModule::getConvolCoeffFunctionValue(
        GPDType::Type gpdType) {
    std::complex<double> result = std::complex<double>(0., 0.);

    try {
        if (m_dvcsConvolCoeffFunctionResult.isAvailable(gpdType)) {
            result = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
        }
    } catch (std::exception &e) {
        // Nothing to do
        // If gpdType is not available for this CCFResult then continue and return 0.
    }

    return result;
}

void DVCSProcessModule::configure(const ElemUtils::Parameters &parameters) {
    ProcessModule::configure(parameters);
}

void DVCSProcessModule::setConvolCoeffFunctionModule(
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {
    if (pConvolCoeffFunctionModule) {
        if (pConvolCoeffFunctionModule->getChannel()
                == ObservableChannel::DVCS) {
            ProcessModule::setConvolCoeffFunctionModule(
                    pConvolCoeffFunctionModule);
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Cannot set ConvolCoeffFunctionModule, because its channel is different from ProcessModule : "
                            << ObservableChannel(
                                    pConvolCoeffFunctionModule->getChannel()).toString());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Cannot set ConvolCoeffFunctionModule, because it's a NULL pointer");
    }

}

} /* namespace PARTONS */

