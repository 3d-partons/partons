#include "../../../include/partons/modules/GPDModule.h"

#include <math.h>
#include <stdexcept>
#include <utility>

#include "../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../include/partons/modules/evolution/GPDEvolutionModule.h"
#include "../../../include/partons/utils/GenericType.h"
#include "../../../include/partons/utils/ParameterList.h"
#include "../../../include/partons/utils/stringUtils/Formatter.h"

const std::string GPDModule::GPD_TYPE = "GPD_MODULE_GPD_TYPE";

GPDModule::GPDModule(const std::string &className) :
        ModuleObject(className), m_x(0.), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(
                0.), m_gpdType(GPDType::ALL), m_MuF2_ref(0.), m_nf(0), m_pGPDEvolutionModule(
                0) {
}

GPDModule::GPDModule(const GPDModule &other) :
        ModuleObject(other) {
    m_x = other.m_x;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;

    m_gpdType = other.m_gpdType;

    m_MuF2_ref = other.m_MuF2_ref;
    m_nf = other.m_nf;

    if (other.m_pGPDEvolutionModule != 0) {
        m_pGPDEvolutionModule = other.m_pGPDEvolutionModule->clone();
    } else {
        m_pGPDEvolutionModule = 0;
    }

    m_listGPDComputeTypeAvailable = other.m_listGPDComputeTypeAvailable;
    m_it = other.m_it;

//		if (other.m_pPDFModule != 0) {
//			m_pPDFModule = other.m_pPDFModule->clone();
//		}
//    else
//    {
//        m_pPDFModule = 0;
//    }
}

GPDModule::~GPDModule() {
    //TODO Remove GPDEvolution pointer
}

void GPDModule::configure(ParameterList parameters) {
    if (parameters.isAvailable(GPDModule::GPD_TYPE)) {
        m_gpdType =
                static_cast<GPDType::Type>(parameters.getLastAvailable().toUInt());
    }
}

//TODO implement
void GPDModule::initModule() {
    debug(__func__, Formatter() << "executed");
}

//TODO implement
void GPDModule::isModuleWellConfigured() {
    debug(__func__, Formatter() << "executed");

    // Test variable range

    if (fabs(m_x) > 1.) {
        error(__func__,
                "Longitudinal momentum fraction should be in [-1., +1.]");
    }

    if (m_xi > 1. || m_xi < 0.) {
        error(__func__, "Skewness should be in [0., +1.]");
    }

    if (m_t > 0.) {
        error(__func__, "Nucleon momentum transfer should be <= 0.");
    }

    if (m_MuF2 < 0.) {
        error(__func__, "Square of factorization scale should be > 0.");
    }
}

void GPDModule::preCompute(double x, double xi, double t, double MuF,
        double MuR, GPDType::Type gpdType) {
    m_x = x;
    m_xi = xi;
    m_t = t;
    m_MuF2 = MuF;
    m_MuR2 = MuR;
    m_gpdType = gpdType;

    debug(__func__,
            Formatter() << "x = " << m_x << "    xi = " << m_xi << "    t = "
                    << m_t << " GeV2    MuF = " << m_MuF2 << " GeV    MuR = "
                    << m_MuR2 << " GeV");

    // execute last child function (virtuality)
    initModule();

    // execute last child function (virtuality)
    isModuleWellConfigured();
}

GPDResult GPDModule::compute(const GPDKinematic &kinematic,
        GPDType::Type gpdType, bool evolution) {
    return compute(kinematic.getX(), kinematic.getXi(), kinematic.getT(),
            kinematic.getMuF2(), kinematic.getMuR2(), gpdType, evolution);
}

GPDResult GPDModule::compute(double x, double xi, double t, double MuF2,
        double MuR2, GPDType::Type gpdType, bool evolution) {

    preCompute(x, xi, t, MuF2, MuR2, gpdType);

    if (evolution) {
        evolution = false;

        if (m_pGPDEvolutionModule != 0) {
            if (m_pGPDEvolutionModule->isRunnable(m_MuF2, m_MuF2_ref,
                    GPDEvolutionModule::RELATIVE)) {
                evolution = true;
            } else {
                warn(__func__,
                        "Evolution is not runnable : out of relative test");
            }
        } else {
            //TODO exception pas de module d'evolution
        }
    }

    GPDResult gpdResult;
    switch (m_gpdType) {
    case GPDType::ALL: {
        for (m_it = m_listGPDComputeTypeAvailable.begin();
                m_it != m_listGPDComputeTypeAvailable.end(); m_it++) {

            PartonDistribution partonDistribution;

            if (evolution) {
                partonDistribution = m_pGPDEvolutionModule->compute(m_x, m_xi,
                        m_t, m_MuF2, m_MuR2, this, (m_it->first));
            } else {
                partonDistribution = ((*this).*(m_it->second))();
            }

            gpdResult.addPartonDistribution(m_it->first, partonDistribution);
        }
        break;
    }
    default: {
        m_it = m_listGPDComputeTypeAvailable.find(m_gpdType);
        if (m_it != m_listGPDComputeTypeAvailable.end()) {
            PartonDistribution partonDistribution;

            if (evolution) {
                partonDistribution = m_pGPDEvolutionModule->compute(m_x, m_xi,
                        m_t, m_MuF2, m_MuR2, this, (m_it->first));
            } else {
                partonDistribution = ((*this).*(m_it->second))();
            }

            gpdResult.addPartonDistribution(m_it->first, partonDistribution);
        } else {
            throwException(__func__,
                    Formatter() << "GPD(" << GPDType(m_gpdType).toString()
                            << ") is not available for this GPD model");
        }
        break;
    }
    }

    gpdResult.setComputationModuleName(getClassName());
    gpdResult.setKinematic(GPDKinematic(x, xi, t, MuF2, MuR2));

    debug(__func__, Formatter() << gpdResult.toString());

    return gpdResult;
}

//TODO implement
PartonDistribution GPDModule::computeH() {
    throw std::runtime_error("");
}

PartonDistribution GPDModule::computeE() {
    throw std::runtime_error("");
}

PartonDistribution GPDModule::computeHt() {
    throw std::runtime_error("");
}

PartonDistribution GPDModule::computeEt() {
    throw std::runtime_error("");
}

unsigned int GPDModule::getNf() const {
    return m_nf;
}

void GPDModule::setNf(unsigned int nf) {
    m_nf = nf;
}

const GPDEvolutionModule* GPDModule::getEvolQcdModule() const {
    return m_pGPDEvolutionModule;
}

void GPDModule::setEvolQcdModule(GPDEvolutionModule* pEvolQcdModule) {
    m_pGPDEvolutionModule = pEvolQcdModule;
    if (m_pGPDEvolutionModule != 0) {
        m_pGPDEvolutionModule->setGpdModule(this);
    }
}

double GPDModule::getMuF2Ref() const {
    return m_MuF2_ref;
}

std::string GPDModule::toString() {
    return ModuleObject::toString();
}

double GPDModule::getMuF2() const {
    return m_MuF2;
}

void GPDModule::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

double GPDModule::getMuR2() const {
    return m_MuR2;
}

void GPDModule::setMuR2(double muR2) {
    m_MuR2 = muR2;
}

double GPDModule::getT() const {
    return m_t;
}

void GPDModule::setT(double t) {
    m_t = t;
}

double GPDModule::getX() const {
    return m_x;
}

void GPDModule::setX(double x) {
    m_x = x;
}

double GPDModule::getXi() const {
    return m_xi;
}

void GPDModule::setXi(double xi) {
    m_xi = xi;
}
