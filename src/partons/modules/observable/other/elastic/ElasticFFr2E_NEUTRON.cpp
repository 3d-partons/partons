#include "../../../../../../include/partons/modules/observable/other/elastic/ElasticFFr2E_NEUTRON.h"

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

const unsigned int ElasticFFr2E_NEUTRON::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ElasticFFr2E_NEUTRON("ElasticFFr2E_NEUTRON"));

ElasticFFr2E_NEUTRON::ElasticFFr2E_NEUTRON(const std::string &className) :
        DVCSObservable(className) {
}

ElasticFFr2E_NEUTRON::ElasticFFr2E_NEUTRON(
        const ElasticFFr2E_NEUTRON& other) :
        DVCSObservable(other) {
}

ElasticFFr2E_NEUTRON::~ElasticFFr2E_NEUTRON() {
}

ElasticFFr2E_NEUTRON* ElasticFFr2E_NEUTRON::clone() const {
    return new ElasticFFr2E_NEUTRON(*this);
}

double ElasticFFr2E_NEUTRON::integrand(double logX, void* params){

    auto parameters = static_cast<ElasticFFr2E_NEUTRON::Parameters*>(params);

    double x = exp(logX);

    PartonDistribution pd = parameters->m_pGPDModule->compute(
            GPDKinematic(x, parameters->m_xi, parameters->m_t, parameters->m_muF2, parameters->m_muR2), 
            parameters->m_gpdType);

    constexpr double chargeU = 2/3.; 
    constexpr double chargeD = -1/3.; 

    return 
        x * (
            chargeU * pd.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionMinus() + 
            chargeD * pd.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionMinus()
        ); 
}

PhysicalType<double> ElasticFFr2E_NEUTRON::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

        //evaluate
        List<GPDType> gpdList;
        gpdList.add(GPDType::H);

        //parameters
        Parameters parameters;

        parameters.m_pGPDModule = m_pProcessModule->getConvolCoeffFunctionModule()->getGPDModule();
        parameters.m_xi = 1.E-8;
        parameters.m_muF2 = parameters.m_pGPDModule->getMuF2Ref(); 
        parameters.m_muR2 = parameters.m_muF2; 

        //integration
        gsl_integration_workspace* w = gsl_integration_workspace_alloc (1000);

        double F1[2], F2[2], error;

        constexpr double c_xMin = 1.E-8;
        constexpr double c_xMax = 1. - 1.E-4;

        gsl_function F;
        F.function = &integrand;
        F.params = &parameters;

        constexpr double c_dt = 1.E-3;

        parameters.m_t = 0.; 

        parameters.m_gpdType = GPDType::H;
        gsl_integration_qags(&F, log(c_xMin), log(c_xMax), 0., 1.E-3, 1000, w, &F1[0], &error);

//        parameters.m_gpdType = GPDType::E;
//        gsl_integration_qags(&F, log(c_xMin), log(c_xMax), 0., 1.E-3, 1000, w, &F2[0], &error);

        parameters.m_t = -1 * c_dt; 

        parameters.m_gpdType = GPDType::H;
        gsl_integration_qags(&F, log(c_xMin), log(c_xMax), 0., 1.E-3, 1000, w, &F1[1], &error);

        parameters.m_gpdType = GPDType::E;
        gsl_integration_qags(&F, log(c_xMin), log(c_xMax), 0., 1.E-3, 1000, w, &F2[1], &error);

        gsl_integration_workspace_free(w);

        //return

        double GE[2] = {
            F1[0], 
            F1[1] + (-1 * c_dt)  * pow(2 * Constant::PROTON_MASS, -2) * F2[1],
        };

        double result = -6 * (GE[1] - GE[0]) / c_dt; 

        return PhysicalType<double>(result * pow(Constant::CONV_GEVm1_TO_FM, 2), PhysicalUnit::FM2);
}

} /* namespace PARTONS */
