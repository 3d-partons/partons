#include "../../../include/partons/modules/ProcessModule.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../include/partons/modules/ConvolCoeffFunctionModule.h"
#include "../../../include/partons/modules/scale/ScaleModule.h"
#include "../../../include/partons/modules/xb_to_xi/XiConverterModule.h"

const std::string ProcessModule::PARAMETER_NAME_BEAM_ENERGY = "beam_energy";

ProcessModule::ProcessModule(const std::string &className) :
        ModuleObject(className), m_isCCFModuleDependent(true), m_phi(0.), m_phiS(
                0.), m_phie(0.), m_xB(0.), m_t(0.), m_Q2(0.), m_E(0.), m_pScaleModule(
                0), m_pXiConverterModule(0), m_pConvolCoeffFunctionModule(0), m_channel(
                ObservableChannel::UNDEFINED) {
}

ProcessModule::ProcessModule(const ProcessModule &other) :
        ModuleObject(other) {
    m_isCCFModuleDependent = other.m_isCCFModuleDependent;

    m_channel = other.m_channel;

    m_xB = other.m_xB;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;

    m_E = other.m_E;

    m_phi = other.m_phi;
    m_phiS = other.m_phiS;
    m_phie = other.m_phie;

    if (other.m_pScaleModule != 0) {
        m_pScaleModule = other.m_pScaleModule->clone();
    } else {
        m_pScaleModule = 0;
    }

    if (other.m_pXiConverterModule != 0) {
        m_pXiConverterModule = other.m_pXiConverterModule->clone();
    } else {
        m_pXiConverterModule = 0;
    }

    if (other.m_pConvolCoeffFunctionModule != 0) {
        m_pConvolCoeffFunctionModule =
                other.m_pConvolCoeffFunctionModule->clone();
    } else {
        m_pConvolCoeffFunctionModule = 0;
    }
}

ProcessModule::~ProcessModule() {
}

void ProcessModule::configure(const ElemUtils::Parameters &parameters) {

    if (parameters.isAvailable(ProcessModule::PARAMETER_NAME_BEAM_ENERGY)) {
        try {
            m_E = parameters.getLastAvailable().toDouble();

            info(__func__,
                    ElemUtils::Formatter()
                            << ProcessModule::PARAMETER_NAME_BEAM_ENERGY
                            << " configured with value = " << m_E);
        } catch (const std::exception &e) {
            ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "For parameter "
                            << ProcessModule::PARAMETER_NAME_BEAM_ENERGY << " "
                            << e.what());
        }
    }

    ModuleObject::configure(parameters);
}

/*--------------------------- Function SetBeamEnergy(EBeam) ----------------------------*
 | Sets the beam energy in the laboratory frame.                                        |
 *--------------------------------------------------------------------------------------*/
void ProcessModule::setBeamEnergy(double EBeam) {
    m_E = EBeam;
}

//TODO vérifier que la précision du double ne fausse pas le résultat
bool ProcessModule::isPreviousKinematicsDifferent(double xB, double t,
        double Q2) {
    bool result = false;
    if (xB != m_xB || t != m_t || Q2 != m_Q2) {
        result = true;
    }
    return result;
}

void ProcessModule::resetPreviousKinematics() {
    m_xB = 0.;
    m_t = 0.;
    m_Q2 = 0.;
}

void ProcessModule::setConvolCoeffFunctionModule(
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) {

    m_pConvolCoeffFunctionModule = pConvolCoeffFunctionModule;

    info(__func__,
            ElemUtils::Formatter() << "ConvolCoeffFunctionModule is set to : "
                    << pConvolCoeffFunctionModule->getClassName());

    resetPreviousKinematics();
}

void ProcessModule::setPScaleModule(ScaleModule* pScaleModule) {
    m_pScaleModule = pScaleModule;
}

void ProcessModule::setPXiConverterModule(
        XiConverterModule* pXiConverterModule) {
    m_pXiConverterModule = pXiConverterModule;
}

bool ProcessModule::isCCFModuleDependent() const {
    return m_isCCFModuleDependent;
}

void ProcessModule::isCCFModuleDependent(bool isCcfModuleDependent) {
    m_isCCFModuleDependent = isCcfModuleDependent;
}

ConvolCoeffFunctionModule* ProcessModule::getConvolCoeffFunctionModule() const {
    return m_pConvolCoeffFunctionModule;
}
