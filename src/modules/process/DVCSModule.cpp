#include "DVCSModule.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../beans/Scale.h"
#include "../../ModuleObjectFactory.h"
#include "../../utils/GenericType.h"
#include "../../utils/ParameterList.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../observable/Observable.h"
#include "../scale/LambdaQ2Scale.h"

const std::string DVCSModule::PARAMETER_NAME_BEAM_ENERGY = "beam_energy";

DVCSModule::DVCSModule(const std::string &className) :
        ProcessModule(className), m_phi(0.), m_phiS(0.), m_phie(0.), m_phaseSpace(
                0.), m_pObservable(0), m_pDVCSConvolCoeffFunctionModule(0), m_pScaleModule(
                0) {

}

DVCSModule::~DVCSModule() {

}

DVCSModule::DVCSModule(const DVCSModule& other) :
        ProcessModule(other) {
    m_phi = other.m_phi;
    m_phiS = other.m_phiS;
    m_phie = other.m_phie;

    m_phaseSpace = other.m_phaseSpace;

    if (other.m_pObservable != 0) {
        m_pObservable = other.m_pObservable->clone();
    } else {
        m_pObservable = 0;
    }

    if (other.m_pDVCSConvolCoeffFunctionModule != 0) {
        m_pDVCSConvolCoeffFunctionModule =
                other.m_pDVCSConvolCoeffFunctionModule->clone();
    } else {
        m_pDVCSConvolCoeffFunctionModule = 0;
    }

    if (other.m_pScaleModule != 0) {
        m_pScaleModule = other.m_pScaleModule->clone();
    } else {
        m_pScaleModule = 0;
    }

    m_dvcsConvolCoeffFunctionResult = other.m_dvcsConvolCoeffFunctionResult;
}

void DVCSModule::init() {
    m_pScaleModule = ModuleObjectFactory::newScaleModule(
            LambdaQ2Scale::classId);
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
        throwException(__func__,
                "m_pScaleModule is NULL pointer ; Use configure method to configure it");
    }
}

//TODO delete
double DVCSModule::xBToXi(double xB) {
    return xB / (2 - xB);
}

void DVCSModule::computeConvolCoeffFunction(double xB, double t, double Q2) {
    if (isPreviousKinematicDifferent(xB, t, Q2)) {
        // Compute scale from Q2
        Scale scale = m_pScaleModule->compute(Q2);

        //TODO replace hard coded : convert xB to xi
        m_dvcsConvolCoeffFunctionResult =
                m_pDVCSConvolCoeffFunctionModule->compute(xBToXi(xB), t, Q2,
                        scale.getMuF2(), scale.getMuR2(), GPDType::ALL);
    }

    debug(__func__,
            Formatter() << "m_dvcsConvolCoeffFunctionResult = "
                    << m_dvcsConvolCoeffFunctionResult.toString());

    m_xB = xB;
    m_t = t;
    m_Q2 = Q2;

    initModule();
    isModuleWellConfigured();
}

/*--------------------------- Function SetBeamEnergy(EBeam) ----------------------------*
 | Sets the beam energy in the laboratory frame.                                        |
 *--------------------------------------------------------------------------------------*/
void DVCSModule::SetBeamEnergy(double EBeam) {
    m_E = EBeam;
}

double DVCSModule::computeCrossSection(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization, double phi) {

    m_phi = phi;

    debug(__func__,
            Formatter() << "beamHelicity = " << beamHelicity << " beamCharge = "
                    << beamCharge << " phi = " << m_phi);

    initModule(beamHelicity, beamCharge, targetPolarization);

    return CrossSectionBH(beamHelicity, beamCharge, targetPolarization)
            + CrossSectionVCS(beamHelicity, beamCharge, targetPolarization)
            + CrossSectionInterf(beamHelicity, beamCharge, targetPolarization);
}

//TODO vérifier que la précision du double ne fausse pas le résultat
bool DVCSModule::isPreviousKinematicDifferent(double xB, double t, double Q2) {
    bool result = false;
    if (xB != m_xB || t != m_t || Q2 != m_Q2) {
        result = true;
    }
    return result;
}

DVCSConvolCoeffFunctionModule* DVCSModule::getDVCSConvolCoeffFunctionModule() const {
    return m_pDVCSConvolCoeffFunctionModule;
}
void DVCSModule::setDVCSConvolCoeffFunctionModule(
        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) {
    m_pDVCSConvolCoeffFunctionModule = pDVCSConvolCoeffFunctionModule;
}

std::complex<double> DVCSModule::getConvolCoeffFunctionValue(
        GPDType::Type gpdType) {
    std::complex<double> result = 0.;

    if (m_dvcsConvolCoeffFunctionResult.isAvailable(gpdType)) {
        result = m_dvcsConvolCoeffFunctionResult.getLastAvailable();
    }

    return result;
}

void DVCSModule::configure(ParameterList parameters) {
    if (parameters.isAvailable(DVCSModule::PARAMETER_NAME_BEAM_ENERGY)) {
        m_E = parameters.getLastAvailable().toDouble();

        info(__func__,
                Formatter() << DVCSModule::PARAMETER_NAME_BEAM_ENERGY
                        << " configured with value = " << m_E);
    }
}
