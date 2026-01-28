#include "../../../../../../include/partons/modules/observable/other/elastic/ElasticFFGMnorm.h"

#include <NumA/linear_algebra/vector/Vector3D.h>
#include <gsl/gsl_integration.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/beans/Scales.h"
#include "../../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"
#include "../../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

const unsigned int ElasticFFGMnorm::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ElasticFFGMnorm("ElasticFFGMnorm"));

ElasticFFGMnorm::ElasticFFGMnorm(const std::string &className) :
        DVCSObservable(className) {
}

ElasticFFGMnorm::ElasticFFGMnorm(
        const ElasticFFGMnorm& other) :
        DVCSObservable(other) {
}

ElasticFFGMnorm::~ElasticFFGMnorm() {
}

ElasticFFGMnorm* ElasticFFGMnorm::clone() const {
    return new ElasticFFGMnorm(*this);
}

double ElasticFFGMnorm::integrand(double logX, void* params){

    auto parameters = static_cast<ElasticFFGMnorm::Parameters*>(params);

    double x = exp(logX);

    PartonDistribution pd = parameters->m_pGPDModule->compute(
            GPDKinematic(x, parameters->m_xi, parameters->m_t, parameters->m_muF2, parameters->m_muR2), 
            parameters->m_gpdType);

    constexpr double chargeU = 2/3.; 
    constexpr double chargeD = -1/3.; 

    return 
        x * (
            chargeU * pd.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionMinus() + 
            chargeD * pd.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionMinus()
        ); 
}

PhysicalType<double> ElasticFFGMnorm::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

        //evaluate
        List<GPDType> gpdList;
        gpdList.add(GPDType::H);

        //parameters
        Parameters parameters;

        parameters.m_pGPDModule = m_pProcessModule->getConvolCoeffFunctionModule()->getGPDModule();
        parameters.m_xi = 1.E-8;
        parameters.m_t = -1 * kinematic.getQ2().makeSameUnitAs(PhysicalUnit::GEV2).getValue();
        parameters.m_muF2 = parameters.m_pGPDModule->getMuF2Ref(); 
        parameters.m_muR2 = parameters.m_muF2; 

        //integration
        gsl_integration_workspace* w = gsl_integration_workspace_alloc (1000);

        double F1, F2, error;

        constexpr double c_xMin = 1.E-8;
        constexpr double c_xMax = 1. - 1.E-4;

        gsl_function F;
        F.function = &integrand;
        F.params = &parameters;

        parameters.m_gpdType = GPDType::H;
        gsl_integration_qags(&F, log(c_xMin), log(c_xMax), 0., 1.E-3, 1000, w, &F1, &error);

        parameters.m_gpdType = GPDType::E;
        gsl_integration_qags(&F, log(c_xMin), log(c_xMax), 0., 1.E-3, 1000, w, &F2, &error);

        gsl_integration_workspace_free(w);

        //return
        double& t = parameters.m_t;

        double GM = F1 + F2;
//        double GE = F1 + t * pow(2 * Constant::PROTON_MASS, -2) * F2;
        double GD = pow(1. - t/0.71, -2);

        constexpr double mup = 2.793;
//        constexpr double mun = -1.913;

        double result = GM / (mup * GD); 

        return PhysicalType<double>(result, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
