#include "../../../../../include/partons/modules/convol_coeff_function/GAM2/GAM2CFFStandard.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../../../include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

#include <gsl/gsl_sf_dilog.h>
#include <gsl/gsl_sf_result.h>

namespace PARTONS {

const unsigned int GAM2CFFStandard::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GAM2CFFStandard("GAM2CFFStandard"));

GAM2CFFStandard::GAM2CFFStandard(const std::string &className) :
        GAM2ConvolCoeffFunctionModule(className), m_pRunningAlphaStrongModule(0) {

    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &GAM2ConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &GAM2ConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &GAM2ConvolCoeffFunctionModule::computePolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &GAM2ConvolCoeffFunctionModule::computePolarized));

    initFunctorsForIntegrations();
}

GAM2CFFStandard::GAM2CFFStandard(const GAM2CFFStandard &other) :
        GAM2ConvolCoeffFunctionModule(other) {

    if (other.m_pRunningAlphaStrongModule != 0) {
        m_pRunningAlphaStrongModule = m_pModuleObjectFactory->cloneModuleObject(
                other.m_pRunningAlphaStrongModule);
    } else {
        m_pRunningAlphaStrongModule = 0;
    }

    initFunctorsForIntegrations();
}

GAM2CFFStandard* GAM2CFFStandard::clone() const {
    return new GAM2CFFStandard(*this);
}

void GAM2CFFStandard::resolveObjectDependencies() {
    GAM2ConvolCoeffFunctionModule::resolveObjectDependencies();

    setIntegrator(NumA::IntegratorType1D::DEXP);

    m_pRunningAlphaStrongModule =
            Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                    RunningAlphaStrongStandard::classId);
}

void GAM2CFFStandard::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {

    GAM2ConvolCoeffFunctionModule::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    it = subModulesData.find(
            RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

        if (m_pRunningAlphaStrongModule != 0) {
            setRunningAlphaStrongModule(0);
            m_pRunningAlphaStrongModule = 0;
        }

        if (!m_pRunningAlphaStrongModule) {
            m_pRunningAlphaStrongModule =
                    Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
                            (it->second).getModuleClassName());
            info(__func__,
                    ElemUtils::Formatter()
                            << "Configure with RunningAlphaStrongModule = "
                            << m_pRunningAlphaStrongModule->getClassName());
            m_pRunningAlphaStrongModule->configure(
                    (it->second).getParameters());
        }
    }
}

void GAM2CFFStandard::initFunctorsForIntegrations() {
    m_pExampleIntegration = NumA::Integrator1D::newIntegrationFunctor(this,
            &GAM2CFFStandard::exampleIntegration);
}

GAM2CFFStandard::~GAM2CFFStandard() {

    if (m_pRunningAlphaStrongModule) {
        setRunningAlphaStrongModule(0);
        m_pRunningAlphaStrongModule = 0;
    }

    if (m_pExampleIntegration) {
        delete m_pExampleIntegration;
        m_pExampleIntegration = 0;
    }
}

void GAM2CFFStandard::initModule() {
    GAM2ConvolCoeffFunctionModule::initModule();
}

void GAM2CFFStandard::isModuleWellConfigured() {

    GAM2ConvolCoeffFunctionModule::isModuleWellConfigured();

    if (m_pGPDModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "GPDModule* is NULL");
    }

    if (m_qcdOrderType == PerturbativeQCDOrderType::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "QCDOrderType is UNDEFINED");
    }

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO
            && m_qcdOrderType != PerturbativeQCDOrderType::NLO) {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input, perturbative QCD order can only be LO or NLO. Here Order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    if (getMathIntegrator() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "MathIntegrationMode is UNDEFINED");
    }

    if (m_pRunningAlphaStrongModule == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "RunningAlphaStrongModule* is NULL");
    }
}

double epsilon = 10E-7; // infinitesimal part inserted 'by hand'
double CF = 4. / 3.;

// Trace \mathcal{A}, see Eq. 25
double GAM2CFFStandard::A(double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){
    return s * ( beta[0] * ( ee[0] * ek[0] - ee[1] * ek[1] ) + beta[2] * ( ee[1] * ek[1] - ee[2] * ek[2] ) ) / 8.;
}

// Eq. 49
std::complex <double> GAM2CFFStandard::M3M(double s, double x, double xi,
                          std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){
    std::complex <double> result (0., 0.);

    std::complex <double> aux (0., 0.);

    aux += std::log( std::complex<double>( -(x - xi) * beta[2], epsilon) ) - std::log( std::complex<double>( (x + xi) * beta[0], epsilon) );
    aux *= 2. * ( x + xi ) * beta[0] + ( x - xi ) * beta[2];
    aux += ( x + xi ) * beta[0] + ( x - xi ) * beta[2];
    aux *= 0.5 * s * ( beta[0] * ee[1] * ek[1] - beta[0] * ee[0] * ek[0] + beta[2] * ee[2] * ek[2] + beta[2] * ee[1] * ek[1] );
    aux *= ( x + xi ) * beta[0];
    aux /= ( x + xi ) * beta[0] + ( x - xi ) * beta[2];
    aux /= ( x + xi ) * beta[0] + ( x - xi ) * beta[2];

    result += aux;

    aux = - std::log( std::complex<double>( -(x - xi) * beta[2], epsilon) ) + std::log( std::complex<double>( (x + xi) * beta[0], epsilon) );
    aux *= ( x + xi ) * beta[0] + 2. * ( x - xi ) * beta[2];
    aux += ( x + xi ) * beta[0] + ( x - xi ) * beta[2];
    aux *= 0.5 * s * ( beta[0] * ee[1] * ek[1] + beta[0] * ee[0] * ek[0] - beta[2] * ee[2] * ek[2] + beta[2] * ee[1] * ek[1] );
    aux *= ( x - xi ) * beta[2];
    aux /= ( x + xi ) * beta[0] + ( x - xi ) * beta[2];
    aux /= ( x + xi ) * beta[0] + ( x - xi ) * beta[2];

    result -= aux;

    aux = - ( x - xi ) * beta[2] * std::log( std::complex<double>( ( x - xi ) * beta[2] / 2. / xi, -epsilon) );
    aux -= ( x + xi ) * beta[0] * std::log( std::complex<double>( - ( x + xi ) * beta[0] / 2. / xi, -epsilon) );
    aux /= ( x + xi ) * beta[0] + ( x - xi ) * beta[2];
    aux += 1.;
    aux *= A(s, beta, ee, ek);

    result += aux;

    result *= - CF * m_pRunningAlphaStrongModule->compute(m_MuF2) / 4. / Constant::PI / s / s;
    result /= std::complex<double>( (x + xi) * beta[0], epsilon) * std::complex<double>( (x - xi) * beta[2], -epsilon);

    return result;
}

// Trace structures in amplitudes 4.L and 5.L - Eqs. 50 and 51
// Tr_4/5L_Fnab stands by the function F(n, a, b, ...)

double GAM2CFFStandard::Tr_4L_F210(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

     double result = beta[0] * ( ( 2. * xi * beta[2] ) + 2. * ( 2. * xi * beta[0] ) ) - 2. * beta[1] * ( ( 2. * xi * beta[2] ) + ( 2. * xi * beta[1] ) );
     result *= -s * ( ee[0] * ek[0] - ee[1] * ek[1] );

     result += beta[2] * (( 2. * xi * beta[2] ) * s * ( ee[1] * ek[1] - 3. * ee[2] * ek[2] ) + 4. * ek[0] * ek[1] * ek[2] );

     result *= -2. * s;

     return result;
}

double GAM2CFFStandard::Tr_4L_F201(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = 2. * beta[1] * ee[1] * ek[1] - 2. * beta[1] * ee[0] * ek[0];
    result += 3. * beta[2] * ee[2] * ek[2] - 9. * beta[2] * ee[1] * ek[1] + 4. * beta[2] * ee[0] * ek[0];
    result *= beta[0];

    result += beta[0] * beta[0] * ( ee[1] * ek[1] - ee[0] * ek[0] );

    result += 2. * beta[1] * beta[2] * ( ee[2] * ek[2] - ee[1] * ek[1] );

    result *= 2. * s * s;

    return result;
}

double GAM2CFFStandard::Tr_4L_F211(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = -3. * ee[2] * ek[2] + 3. * ee[1] * ek[1] + 2. * ee[0] * ek[0];
    result *= beta[1] * beta[2];

    result += 2. * beta[1] * beta[1] * ( ee[0] * ek[0] - ee[1] * ek[1] );

    result += beta[0] * beta[2] * ( ee[0] * ek[0] - ee[2] * ek[2] );

    result *= -2. * s * s;

    return result;
}

double GAM2CFFStandard::Tr_4L_F220(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = -ee[2] * ek[2] * ( 2. * xi * beta[2] ) * s + ee[0] * ek[0] * ( 2. * xi * beta[2] ) * s + 4. * ek[0] * ek[1] * ek[2];
    result *= beta[2];

    result += beta[1] * ( 2. * xi * beta[2] ) * s * ( ee[0] * ek[0] - ee[1] * ek[1] );

    result *= 2. * s;

    return result;
}

double GAM2CFFStandard::Tr_4L_F221(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = -beta[1] * ee[1] * ek[1] + beta[1] * ee[0] * ek[0] - beta[2] * ee[2] * ek[2] + beta[2] * ee[0] * ek[0];
    result *= 2. * beta[1] * s * s;

    return result;
}

double GAM2CFFStandard::Tr_4L_F100(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = -beta[0] * ee[1] * ek[1] + beta[0] * ee[0] * ek[0] - 2. * beta[1] * ee[1] * ek[1];
    result += 2. * beta[1] * ee[0] * ek[0] - 3. * beta[2] * ee[2] * ek[2];
    result += 3. * beta[2] * ee[1] * ek[1] - 2. * beta[2] * ee[0] * ek[0];

    result *= 2. * s;

    return result;
}

double GAM2CFFStandard::Tr_4L_F110(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = 3. * beta[1] * ee[1] * ek[1] - 3. * beta[1] * ee[0] * ek[0];
    result += beta[2] * ee[2] * ek[2] - beta[2] * ee[0] * ek[0];

    result *= 2. * s;

    return result;
}

double GAM2CFFStandard::Tr_4L_G(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = -beta[0] * ee[1] * ek[1] + beta[0] * ee[0] * ek[0];
    result += -beta[2] * ee[2] * ek[2] + beta[2] * ee[1] * ek[1];

    result *= 2. * beta[2] * s * s;

    return result;
}

double GAM2CFFStandard::Tr_5L_F201(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = 2. * beta[0] * ee[2] * ek[2] - 5. * beta[0] * ee[1] * ek[1];
    result += 3. * beta[0] * ee[0] * ek[0] + beta[1] * ee[2] * ek[2] - beta[1] * ee[1] * ek[1];
    result *= 2. * s;

    return result;
}

double GAM2CFFStandard::Tr_5L_F210(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = 2. * ek[1] * ( 2. * ek[0] * ek[2] - ee[1] * ( 2. * xi * beta[2] ) * s );
    result += -6. * ek[2] * ( 2. * ek[0] * ek[1] - ee[2] * ( 2. * xi * beta[2] ) * s );

    return result;
}

double GAM2CFFStandard::Tr_5L_F211(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = -3. * beta[0] * ee[2] * ek[2] + 2. * beta[0] * ee[1] * ek[1];
    result += beta[0] * ee[0] * ek[0] - 7. * beta[1] * ee[2] * ek[2] + 5. * beta[1] * ee[1] * ek[1];
    result *= -s;

    return result;
}

double GAM2CFFStandard::Tr_5L_F220(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = 2. * ek[2] * ( 2. * ek[0] * ek[1] - ee[2] * ( 2. * xi * beta[2] ) * s );
    result += -2. * ek[0] * ( ee[0] * ( 2. * xi * beta[2] ) * s + 2. * ek[1] * ek[2] );

    return result;
}

double GAM2CFFStandard::Tr_5L_F221(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = -2. * beta[1] * ee[2] * ek[2] * s;
    result += 2. * beta[1] * ee[0] * ek[0] * s;

    return result;
}

double GAM2CFFStandard::Tr_5L_F100(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = 3. * ee[0] * ek[1] - 3. * ee[2] * ek[2] - 2. * ee[0] * ek[0];
    result *= 4.;

    return result;
}

double GAM2CFFStandard::Tr_5L_F110(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = ee[0] * ek[0] - ee[2] * ek[2];
    result *= -4.;

    return result;
}

double GAM2CFFStandard::Tr_5L_G(double xi, double s, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    double result = -beta[0] * ee[1] * ek[1] + beta[0] * ee[0] * ek[0];
    result += -beta[2] * ee[2] * ek[2] + beta[2] * ee[1] * ek[1];
    result *= -2. * s;

    return result;
}

// Complex dilogarithm

std::complex<double> Li2 (std::complex<double> z){

    double Re = real(z);
    double Im = imag(z);

    double r = sqrt( Re * Re + Im * Im);
    double theta = acos( Re / r);
    if ( Im < 0 ) theta *= -1.;

    gsl_sf_result res_re, res_im;

    gsl_sf_complex_dilog_e(Re, Im, &res_re, &res_im);

    return std::complex<double> ((res_re).val, (res_im).val);

}

// Functions F(n, a, b, ...)

std::complex<double> GAM2CFFStandard::F100(std::complex<double> a, std::complex<double> b, std::complex<double> c){
    std::complex<double> result (0., 0.);

    result += std::log( b/a ) * std::log( (a + c) / a );
    result += -std::log( ( (a + c) * (b + c) ) / (a*b) ) * std::log( (a + b + c) / a );
    result += Li2( -(c/a) ) + Li2( -(c/b) );
    result += -Li2( -( ( c * (a + b + c) ) / (a*b) ) );
    result /= -c;

    return result;
}

std::complex<double> GAM2CFFStandard::F110(std::complex<double> a, std::complex<double> b, std::complex<double> c){
    std::complex<double> result (0., 0.);
    std::complex<double> aux (0., 0.);

    aux = -c + (b + c) * std::log( (b + c) / b );
    aux *= a * std::log( a / b );

    result += aux;

    aux = c * (a + b + c);
    aux += - a * (b + c) * std::log( ( (a + c) * (b + c) ) / (a*b) );
    aux *= std::log( (a + b + c) / b );

    result += aux;

    aux = Li2( -(c/a) ) + Li2( -(c/b) );
    aux += -Li2( -( ( c * (a + b + c) ) / (a*b) ) );
    aux *= a * (b + c);

    result += aux;

    result /= c * c * ( b + c );

    return result;
}

std::complex<double> GAM2CFFStandard::F210(std::complex<double> a, std::complex<double> b, std::complex<double> c){
    std::complex<double> result (0., 0.);
    result += std::log( a + b + c ) - std::log( a );
    result /= b  * ( b + c);

    return result;
}

std::complex<double> GAM2CFFStandard::F211(std::complex<double> a, std::complex<double> b, std::complex<double> c){
    std::complex<double> result (0., 0.);
    std::complex<double> aux (0., 0.);

    aux = Li2(-(c/a)) + Li2( b / (b + c) ) - Li2( -( c * (a + b + c)) / (a*b) );
    aux *= -6. * (a + c) * (b + c);

    result += aux;

    aux = -( c * (a + b + 2. * c) ) + (a + c) * (b + c)* std::log( ( (a + c) * (b + c) ) / (a*b) );
    aux *= 6. * std::log(a + b + c);

    result += aux;

    aux = Constant::PI * Constant::PI + 6. * std::log( b + c );
    aux += 6. * ( -1. + std::log( c / b ) ) * std::log( (b + c) / b );
    aux += - 3. * std::pow( std::log( (b + c) / b ), 2. );
    aux *= (a + c) * (b + c);

    result += aux;

    aux = c + (b + c) * ( std::log(b) - std::log( b + c ) );
    aux *= 6. * ( a + c ) * std::log(a);

    result += aux;

    aux = -a - (a + c) * std::log( (a + c) / a );
    aux *= 6. * (b + c) * std::log(b);

    result += aux;

    result /= 6. * c * c * (a + c) * (b + c);

    return result;
}

std::complex<double> GAM2CFFStandard::F220(std::complex<double> a, std::complex<double> b, std::complex<double> c){
    std::complex<double> result (0., 0.);

    result = std::log(b) - std::log(b + c) + std::log( (b + c) / b );
    result *= - a * (b + c);
    result += c * c;
    result *= b + c;

    result += a * c * c * ( std::log(a) - std::log( a + b + c ) );

    result /= b * c * c;
    result /= (b + c) * (b + c);

    return result;
}

std::complex<double> GAM2CFFStandard::F221(std::complex<double> a, std::complex<double> b, std::complex<double> c){
    std::complex<double> result (0., 0.);
    std::complex<double> aux (0., 0.);

    aux = 3. * c * c + a * (b + c) * Constant::PI * Constant::PI;
    aux *= - (a + c)*(b + c);

    result += aux;

    aux = 3. * a * a + a * c;
    aux += -c * c;
    aux += 2. * a * ( a + c ) * std::log( (a + c) / a );
    aux *= 3. * std::pow( b + c , 2. ) * std::log(b);

    result += aux;

    aux = -( c * ( 2. * b + 3. * c ) ) - 2. * std::pow( b + c, 2. ) * ( std::log(b) - std::log(b + c) );

    result += aux;

    aux = -9. * a * (a + c);
    aux *= std::pow(b + c, 2. ) * std::log(b + c);

    result += aux;

    aux = -3. * a * (a + c) * std::pow(b + c, 2.);
    aux *= ( -3. + 2. * std::log( c/b ) ) * std::log( (b + c) / b );
    result += aux;

    aux = 3. * a * (a + c) * std::pow(b + c, 2.);
    aux += std::pow( std::log( (b + c) / b ), 2. );

    result += aux;

    aux = c * std::pow(b + c, 2.) + a * a * ( 2. * b + 3. * c );
    aux += a * ( 2. * b * b + 6. * b * c + 5. * c * c);
    aux *= 3. * c * std::log( a + b + c);

    result += aux;

    aux = -2. * a * (a + c);
    aux *= std::pow(b + c, 2.);
    aux *= std::log( ( (a + c) * (b + c) ) / (a*b) );
    aux *= 3. * c * std::log( a + b + c);

    result += aux;

    result /= (a + c) * std::pow(b + c, 2.);

    aux = Li2( -(c/a) ) + Li2( b / (b + c) );
    aux += -Li2( - ( ( c * (a + b + c) ) / (a*b) ) );
    aux *= 6. * a;

    result += aux;

    result /= 3. * c * c * c;

    return result;
}

std::complex<double> GAM2CFFStandard::G(std::complex<double> a, std::complex<double> b, std::complex<double> c){
    std::complex<double> result (0., 0.);
    std::complex<double> aux(0., 0.);

    result += - (a + c) * Constant::PI * Constant::PI;

    result += - 3. * c * std::pow( log( -b ), 2.);

    result += - 6. * c * log(b);

    result += 3. * c * std::pow( log( -a - b - c), 2. );

    result += 6.* (a + c) * log( -b ) * log( (a + c) / a );

    result += 6. * a * log(-a) * log( (b + c) / b );

    aux = 2. * c * log(-a) + (a + c) * ( -2. * log(c/b) + log( (b + c) / b ) );
    aux *= 3. * log( (b + c) / b );

    result += aux;

    result += 6. * (a + c) * log( -a - b - c ) * log( ( (a + c) * (b + c) ) / (a*b) );

    result += 6. * c * log( a + b + c );

    aux = Li2( -(c/a) ) + Li2( b / (b + c) ) - Li2( -( ( c * (a + b + c) ) / (a*b) ) );
    aux *= 6. * (a + c);

    result += aux;

    return result;

}

// Amplitudes 4.L and 4.R, see Eqs. 50 and 51

std::complex<double> GAM2CFFStandard::M4L(double s, double x, double xi,
                  std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

        std::complex<double> result (0., 0.);

        result += F210( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_4L_F210(xi, s, beta, ee, ek);

        // I did not define F201, since it differs from F210 just in the interchange of 1st and 3rd argument
        result += (x + xi) * F210( (x + xi) * beta[1], ( 2. * xi * beta[2] ), (x+xi) * beta[0] ) *
        Tr_4L_F201(xi, s, beta, ee, ek);

        result += (x + xi) * F211( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_4L_F211(xi, s, beta, ee, ek);

        result += F220( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_4L_F220(xi, s, beta, ee, ek);

        result += (x + xi) * F221( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_4L_F221(xi, s, beta, ee, ek);

        result += s * F100( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_4L_F100(xi, s, beta, ee, ek);

        result += s * F110( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_4L_F110(xi, s, beta, ee, ek);

        result *= - CF * m_pRunningAlphaStrongModule->compute(m_MuF2) / 4. / Constant::PI;
        result /= std::pow(s, 3);
        result /= std::complex<double>( beta[2], -epsilon);

       return result;
}

std::complex<double> GAM2CFFStandard::M5L(double s, double x, double xi,
                  std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

        std::complex<double> result (0., 0.);

        result = log( std::complex<double>( xi - x, epsilon * beta[2] ) / xi );
        result *= A(s, beta, ee, ek) / beta[2];
        result /= std::complex<double>( (x + xi) * beta[0], epsilon );

        result += F210( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_5L_F210(xi, s, beta, ee, ek);

        // I did not define F201, since it differs from F210 just in the interchange of 1st and 3rd argument
        result += (x + xi) * F210( (x + xi) * beta[1], ( 2. * xi * beta[2] ), (x+xi) * beta[0] ) *
        Tr_5L_F201(xi, s, beta, ee, ek);

        result += (x + xi) * F211( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_5L_F211(xi, s, beta, ee, ek);

        result += F220( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_5L_F220(xi, s, beta, ee, ek);

        result += (x + xi) * F221( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_5L_F221(xi, s, beta, ee, ek);

        result += s * F100( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_5L_F100(xi, s, beta, ee, ek);

        result += s * F110( (x + xi) * beta[0], ( 2. * xi * beta[2] ), (x+xi) * beta[1] ) *
        Tr_5L_F110(xi, s, beta, ee, ek);

        result *= - CF * m_pRunningAlphaStrongModule->compute(m_MuF2) / 4. / Constant::PI;
        result /= std::pow(s, 2);
        result /= xi;

       return result;
}

std::complex<double> compute_vector_permutation(double x, double xi, std::vector<double> beta, std::vector<double> ee, std::vector<double> ek){

    return 0.;
}

std::complex<double> GAM2CFFStandard::computeUnpolarized() {

    //variables to use
    m_currentGPDComputeType;
    m_pRunningAlphaStrongModule;
    m_pGPDModule;
    m_qcdOrderType;
    m_xi;
    m_t;
    m_uPrim;
    m_Mgg2;
    m_MuF2;
    m_MuR2;
    m_polG0;
    m_polG1;
    m_polG2;

    double tPrim = m_t - m_Mgg2 + m_uPrim;
    double tau = 2. * m_xi / ( 1. + m_xi );
    double M2 = Constant::PROTON_MASS * Constant::PROTON_MASS;
    double s = ( m_Mgg2 - m_t ) / tau / ( 1. + m_xi );
    double pt2 = - ( m_uPrim * tPrim ) / ( m_uPrim + tPrim );
    double alpha = m_uPrim / ( m_uPrim + tPrim );
    double alphabar = 1 - alpha;

    std::vector<double> emptyParameters;

    double resultA = integrate(m_pExampleIntegration, -1, 1.,
            emptyParameters);
    double resultB = m_pRunningAlphaStrongModule->compute(m_MuF2);

    return std::complex<double>(resultA, resultB);
}

std::complex<double> GAM2CFFStandard::computePolarized() {
    return std::complex<double>();
}

RunningAlphaStrongModule* GAM2CFFStandard::getRunningAlphaStrongModule() const {
    return m_pRunningAlphaStrongModule;
}

void GAM2CFFStandard::setRunningAlphaStrongModule(
        RunningAlphaStrongModule* pRunningAlphaStrongModule) {
    m_pModuleObjectFactory->updateModulePointerReference(
            m_pRunningAlphaStrongModule, pRunningAlphaStrongModule);
    m_pRunningAlphaStrongModule = pRunningAlphaStrongModule;
}

double GAM2CFFStandard::exampleIntegration(double x,
        std::vector<double> params) {
    return m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType).getQuarkDistribution(QuarkFlavor::UP).getQuarkDistribution();
}

} /* namespace PARTONS */
