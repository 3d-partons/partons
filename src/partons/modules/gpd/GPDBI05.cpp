#include "../../../../include/partons/modules/gpd/GPDBI05.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <utility>
#include <gsl/gsl_integration.h>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {

const std::string GPDBI05::PARAM_NAME_SET_NAME = "set_name";
const std::string GPDBI05::PARAM_NAME_MEMBER = "member";

const unsigned int GPDBI05::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDBI05("GPDBI05"));

GPDBI05::GPDBI05(const std::string &className) :
        GPDModule(className), m_Forward(nullptr), m_setName("UNDEFINED"),
	m_member(0) {

    m_MuF2_ref = 4.;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
}

GPDBI05::GPDBI05(const GPDBI05& other) :
        GPDModule(other) {

    m_Forward = nullptr; 
    m_setName = other.m_setName;
    m_member = other.m_member;
}

GPDBI05::~GPDBI05() {

    if (m_Forward == nullptr) {

        delete m_Forward;
        m_Forward = nullptr;
    }
}

void GPDBI05::configure(const ElemUtils::Parameters &parameters){

    GPDModule::configure(parameters);

    if (parameters.isAvailable(GPDBI05::PARAM_NAME_SET_NAME)) {
        m_setName = parameters.getLastAvailable().getString();
	info(__func__, ElemUtils::Formatter() << GPDBI05::PARAM_NAME_SET_NAME
	     << " configured with value = " << getSetName());
    }

    if (parameters.isAvailable(GPDBI05::PARAM_NAME_MEMBER)) {
        m_member = parameters.getLastAvailable().toInt();
	info(__func__, ElemUtils::Formatter() << GPDBI05::PARAM_NAME_MEMBER
	     << " configured with value = " << getMember());
    }
}

GPDBI05* GPDBI05::clone() const {
    return new GPDBI05(*this);
}

void GPDBI05::initModule() {

    GPDModule::initModule();

    LHAPDF::setVerbosity(0);

    if (m_Forward == nullptr) {
        m_Forward = LHAPDF::mkPDF("cteq6l1", 0);
    }
}

void GPDBI05::setSetName(const std::string& setname) {
    m_setName = setname;
}

void GPDBI05::setMember(int member) {
    m_member = member;
}

const std::string& GPDBI05::getSetName() const {
    return m_setName;
}

int GPDBI05::getMember() const {
    return m_member;
}

double GPDBI05::getQuarkForward(double x, QuarkFlavor::Type q, bool isValence) const {

    if(m_Forward == nullptr){
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Pointer to PDF set is null");
    }

    double absX = fabs(x);
    int sgnX = (x <= 0.)?(-1):(1);

    switch(q){

        case QuarkFlavor::UP:{
            
            if(isValence){
                 if(x <= 0.) return 0.;
                 return (m_Forward->xfxQ2(2, absX, m_MuF2_ref) - m_Forward->xfxQ2(-2, absX, m_MuF2_ref)) / absX;
            }else{
                 return sgnX * m_Forward->xfxQ2(-2, absX, m_MuF2_ref) / absX;
            } 
        }

        case QuarkFlavor::DOWN:{
            
            if(isValence){
                 if(x <= 0.) return 0.;
                 return (m_Forward->xfxQ2(1, absX, m_MuF2_ref) - m_Forward->xfxQ2(-1, absX, m_MuF2_ref)) / absX;
            }else{
                 return sgnX * m_Forward->xfxQ2(-1, absX, m_MuF2_ref) / absX;
            } 
        }

        case QuarkFlavor::STRANGE:{
            
            if(isValence){
                 return 0.;
            }else{
                 return sgnX * m_Forward->xfxQ2(-3, absX, m_MuF2_ref) / absX;
            } 
        }

        default:{
            throw ElemUtils::CustomException(getClassName(), __func__,
                "Unknown quark type");
        }
    }
}

double GPDBI05::getGluonForward(double x) const {

    if(m_Forward == nullptr){
        throw ElemUtils::CustomException(getClassName(), __func__,
            "Pointer to PDF set is null");
    }

    double absX = fabs(x);

    return m_Forward->xfxQ2(21, absX, m_MuF2_ref);
} 

double GPDBI05::profile(double beta, double alpha, int n) const {
   return (pow(2,-1 - 2*n)*pow(-pow(alpha,2) + pow(1 - fabs(beta),2),n)*pow(1 - fabs(beta),-1 - 2*n)*tgamma(2 + 2*n))/pow(tgamma(1 + n),2); 
}

double GPDBI05::tDependace(double t) const {
    return exp(2.5 * t);
}

double GPDBI05::integrationFunction(double beta, void* par){

    IntegrationParameters* params = static_cast<IntegrationParameters*>(par);

    double alpha = params->m_x/params->m_xi - beta/params->m_xi; 

    if(params->m_isGluon){
        return params->m_pClass->profile(beta, alpha, 2) * params->m_pClass->getGluonForward(beta);
    }else{
        return params->m_pClass->profile(beta, alpha, 1) * params->m_pClass->getQuarkForward(beta, params->m_quarkType, params->m_isValence);
    }
}

double GPDBI05::integrate(IntegrationParameters* params) const {

    gsl_function F;
    F.function = integrationFunction;
    F.params = params;

    constexpr size_t workspace_size = 1000;
    gsl_integration_workspace* workspace = gsl_integration_workspace_alloc(workspace_size);

    double min, max;
    int status; double thisValue; double thisError;
    double result;

    // avoid singularity
    double eps = (params->m_xi < 1.E-6)?(1.E-2 * params->m_xi):(1.E-8);

   if(params->m_x < -1 * params->m_xi){

        min = -((-params->m_x + params->m_xi)/( 1. + params->m_xi));
        max = -(( params->m_x + params->m_xi)/(-1. + params->m_xi));

        status = gsl_integration_qag(&F, min, max, 1.E-3, 1.E-3, workspace_size, GSL_INTEG_GAUSS15, workspace, &result, &thisError);
    }
    else if(params->m_x > params->m_xi){

        min = -(( params->m_x - params->m_xi)/(-1. + params->m_xi));
        max = -((-params->m_x - params->m_xi)/( 1. + params->m_xi));

        status = gsl_integration_qag(&F, min, max, 1.E-3, 1.E-3, workspace_size, GSL_INTEG_GAUSS15, workspace, &result, &thisError);
    }
    else{

        min = -((-params->m_x + params->m_xi)/(1. + params->m_xi)); 
        max = -eps;

        status = gsl_integration_qag(&F, min, max, 1.E-3, 1.E-3, workspace_size, GSL_INTEG_GAUSS15, workspace, &result, &thisError);

        min = eps; 
        max = -((-params->m_x - params->m_xi)/(1. + params->m_xi)); 

        status = gsl_integration_qag(&F, min, max, 1.E-3, 1.E-3, workspace_size, GSL_INTEG_GAUSS15, workspace, &thisValue, &thisError);

        result += thisValue;
    }

    gsl_integration_workspace_free(workspace);

    return result / params->m_xi;
}

PartonDistribution GPDBI05::computeH() {

    //prepare
    IntegrationParameters params;

    params.m_pClass = this;
    params.m_xi = m_xi;

    //t
    double tDep = tDependace(m_t);

    //g
    params.m_isGluon = true;
    params.m_x = m_x;
    double Hg = integrate(&params) * tDep;

    //u
    params.m_isGluon = false;
    params.m_quarkType = QuarkFlavor::UP;

    params.m_isValence = true;
    params.m_x = m_x;
    double HuVal = integrate(&params) * tDep;

    params.m_isValence = true;
    params.m_x = -1 * m_x;
    double HuValMx = integrate(&params) * tDep;

    params.m_isValence = false;
    params.m_x = m_x;
    double HuSea = integrate(&params) * tDep;

    //d
    params.m_isGluon = false;
    params.m_quarkType = QuarkFlavor::DOWN;

    params.m_isValence = true;
    params.m_x = m_x;
    double HdVal = integrate(&params) * tDep;

    params.m_isValence = true;
    params.m_x = -1 * m_x;
    double HdValMx = integrate(&params) * tDep;

    params.m_isValence = false;
    params.m_x = m_x;
    double HdSea = integrate(&params) * tDep;

    //s
    params.m_isGluon = false;
    params.m_quarkType = QuarkFlavor::STRANGE;

    params.m_isValence = false;
    params.m_x = m_x;
    double Hs = integrate(&params) * tDep;

    // result
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    // g
    GluonDistribution gluonDistribution(Hg);

    // u 
    quarkDistribution_u.setQuarkDistribution(HuVal + HuSea);
    quarkDistribution_u.setQuarkDistributionPlus(HuVal - HuValMx + 2 * HuSea);
    quarkDistribution_u.setQuarkDistributionMinus(HuVal + HuValMx);

    // d 
    quarkDistribution_d.setQuarkDistribution(HdVal + HdSea);
    quarkDistribution_d.setQuarkDistributionPlus(HdVal - HdValMx + 2 * HdSea);
    quarkDistribution_d.setQuarkDistributionMinus(HdVal + HdValMx);

    // s 
    quarkDistribution_s.setQuarkDistribution(Hs);
    quarkDistribution_s.setQuarkDistributionPlus(2 * Hs);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

} /* namespace PARTONS */
