#include "DVCSModule.h"

#include "../../utils/GenericType.h"
#include "../../utils/ParameterList.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../../utils/vector/Vector3D.h"
#include "../convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../observable/Observable.h"

//#include "../GPDModule.h"

const std::string DVCSModule::PARAMETER_NAME_BEAM_ENERGY = "beam_energy";

//TODO Remove duplicate kinematics
DVCSModule::DVCSModule(const std::string &className) :
        ObservableModule(className), /*m_xB(0.), m_t(0.), m_Q2(0.),*/ m_phi(0.), m_phiS(
                0.), m_phie(0.), m_E(0.), m_phaseSpace(0.), m_pObservable(0) {

}

DVCSModule::~DVCSModule() {

}

DVCSModule::DVCSModule(const DVCSModule& other) :
        ObservableModule(other) {
    /*
    m_xB = other.m_xB;
    m_t = other.m_t;
    m_Q2 = other.m_Q2;
    */

    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;

    m_phi = other.m_phi;
    m_phiS = other.m_phiS;
    m_phie = other.m_phie;

    m_E = other.m_E;
    m_phaseSpace = other.m_phaseSpace;

    if (other.m_pObservable != 0) {
        m_pObservable = other.m_pObservable->clone();
    } else {
        m_pObservable = 0;
    }

    m_dvcsConvolCoeffFunctionResult = other.m_dvcsConvolCoeffFunctionResult;
}

void DVCSModule::initModule() {
    //TODO implement

    debug(__func__, "Entered function.");
}

void DVCSModule::initModule(double beamHelicity, double beamCharge,
        Vector3D targetPolarization) {

    debug(__func__, "Entered function.");
}

void DVCSModule::isModuleWellConfigured() {
    //TODO implement

    debug(__func__, "Entered function.");
}

//TODO delete
double DVCSModule::xBToXi(double xB) {
    return xB / (2 - xB);
}

void DVCSModule::computeConvolCoeffFunction(double xB, double t, double Q2,
        double MuF2, double MuR2) {

    debug(__func__,
            Formatter() << "xB = " << xB << " t = " << t << " Q2 = " << Q2
                    << " MuF2 = " << MuF2 << " MuR2 = " << MuR2);

    if (isPreviousKinematicDifferent(xB, t, Q2)) {
        //TODO replace hard coded : convert xB to xi
        m_dvcsConvolCoeffFunctionResult =
                m_pDVCSConvolCoeffFunctionModule->compute(xBToXi(xB), t, Q2,
                        MuF2, MuR2, GPDType::ALL);
    }

    debug(__func__,
            Formatter() << "m_dvcsConvolCoeffFunctionResult = "
                    << m_dvcsConvolCoeffFunctionResult.toString());

    //TODO add the definition of m_E (beam energy)
    m_xB = xB;
    m_t = t;
    m_Q2 = Q2;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;

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
        Vector3D targetPolarization, double phi) {

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
