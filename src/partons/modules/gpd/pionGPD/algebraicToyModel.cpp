#include "../../../../../include/partons/modules/gpd/pionGPD/algebraicToyModel.h"

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <cln/float.h>
#include <cln/float_class.h>
#include <cln/floatformat.h>
#include <cln/real.h>
#include <cmath>
#include <utility>

namespace PARTONS {

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
// Usign the assigned name "satyratedModel_Ding" we can make use of this module in subsequent codes. 
const unsigned int algebraicToyModel::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new algebraicToyModel("algebraicToyModel"));

algebraicToyModel::algebraicToyModel(const std::string &className) : PARTONS::GPDModule(className) 
{
    // Set reference factorization scale.
    m_MuF2_ref = 0.4;

    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
}

algebraicToyModel::algebraicToyModel(const algebraicToyModel& other) : PARTONS::GPDModule(other) 
{
    m2 = 0.16;
}

algebraicToyModel::~algebraicToyModel() 
{

}

algebraicToyModel* algebraicToyModel::clone() const 
{
    return new algebraicToyModel(*this);
}

void algebraicToyModel::resolveObjectDependencies() 
{

}

void algebraicToyModel::configure(const ElemUtils::Parameters &parameters) 
{
    PARTONS::GPDModule::configure(parameters);
}

void algebraicToyModel::isModuleWellConfigured() 
{
    PARTONS::GPDModule::isModuleWellConfigured();
}

void algebraicToyModel::initModule() 
{
    PARTONS::GPDModule::initModule();
}

PARTONS::PartonDistribution algebraicToyModel::computeH() 
{
    PARTONS::PartonDistribution partonDistribution;

    // Quark distributions
    QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
    QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistributionStrange(QuarkFlavor::STRANGE);

    // u-quark
    double uVal, uValM;
    
    c  = -m_t*pow(1 - m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));
    cM = -m_t*pow(1 + m_x, 2.)/(4*m2*(1 - pow(m_xi,2)));

    if ( m_t == 0)                                                                                                                                  // zero momentum transfer
    {
         if ( m_x >= m_xi )                                                                                                                         // DGLAP
        {        
            uVal  = 15 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.);
            uValM = 0.;  

        } else if ( m_x >= (-1)*m_xi )                                                                                                              // ERBL
        {   
            uVal  = 7.5 * ( 1 - m_x ) * ( pow(m_xi,2) - pow(m_x,2) ) * ( m_x + 2 * m_x * m_xi + pow(m_xi,2) ) / ( pow(m_xi,3) * pow(1+m_xi,2) );
            uValM = 7.5 * ( 1 + m_x ) * ( pow(m_xi,2) - pow(m_x,2) ) * ( -m_x + 2 * -m_x * m_xi + pow(m_xi,2) ) / ( pow(m_xi,3) * pow(1+m_xi,2) );

        } else                                                                                                                                      // DGLAP-AntiQuark
        {
            uVal = 0.;
            uValM = 15 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.);
        }
    }else                                                                                                                                           // non-vanishing momentum transfer
    {
         if ( m_x >= m_xi )                                                                                                                         // DGLAP
        {        
            uVal  = 7.5 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * c) * atanh(c/(1+c)))/((1 + c) * sqrt(c/(1 + c))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + c,2) );
            uValM = 0.;
    
        } else if ( m_x >= (-1)*m_xi )                                                                                                              // ERBL
        {   
            uVal  = -3.75 * (pow(m_xi,2) - pow(m_x,2)) * ( sqrt(c * (c + 1)) * ( -c*m_xi*(1 - pow(m_xi,2))*(1 - m_x) 
                * (pow(m_xi,4) + 6*m_xi*(1-m_x)*pow(m_x,2)-6*pow(m_xi,3)*(1-m_x)+pow(m_xi,2)*(4-3*(3-m_x)*m_x)+m_x*(4-m_x*(8-5*m_x))) 
                + pow(c,2)*pow(1-pow(m_xi,2),2)*(pow(m_xi,3)*(6*m_xi-2)+3*pow(m_x,4)-4*m_xi*pow(m_x,3)-6*(m_xi-1)*m_xi*pow(m_x,2)+2*m_xi*(pow(m_xi,2)-1)*m_x)
                + pow(m_x-1,3)*(pow(m_xi,5)+3*pow(m_xi,4)*(m_x-1)+pow(m_xi,3)*(2-5*m_x)+2*m_xi*m_x))
                + (1-2*c)*pow( c*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))+pow(m_xi,2)*(m_x-1),2)
                *(atanh(sqrt(c/(1+c)))-atanh(sqrt(c/(1+c))*(pow(m_xi,2)-m_x)/(m_xi*(1-m_x)))))
                / (pow(1+c,2.5)*pow(1-pow(m_xi,2),1.5)*pow(1-m_x,2)*sqrt(c*(1-pow(m_xi,2)))*pow(pow(m_xi,2)+c*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))/pow(1-m_x,2),2));
            uValM = -3.75 * (pow(m_xi,2) - pow(m_x,2)) * ( sqrt(cM * (cM + 1)) * ( -cM*m_xi*(1 - pow(m_xi,2))*(1 + m_x) 
                * (pow(m_xi,4) + 6*m_xi*(1+m_x)*pow(m_x,2)-6*pow(m_xi,3)*(1+m_x)+pow(m_xi,2)*(4-3*(3+m_x)*(-1)*m_x)-m_x*(4+m_x*(8+5*m_x))) 
                + pow(cM,2)*pow(1-pow(m_xi,2),2)*(pow(m_xi,3)*(6*m_xi-2)+3*pow(m_x,4)-4*m_xi*pow(-m_x,3)-6*(m_xi-1)*m_xi*pow(m_x,2)+2*m_xi*(pow(m_xi,2)-1)*(-1)*m_x)
                + pow(-m_x-1,3)*(pow(m_xi,5)+3*pow(m_xi,4)*(-m_x-1)+pow(m_xi,3)*(2+5*m_x)+2*m_xi*(-1)*m_x))
                + (1-2*cM)*pow( cM*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))+pow(m_xi,2)*(-m_x-1),2)
                *(atanh(sqrt(cM/(1+cM)))-atanh(sqrt(cM/(1+cM))*(pow(m_xi,2)-m_x)/(-m_xi*(1+m_x)))))
                / (pow(1+cM,2.5)*pow(1-pow(m_xi,2),1.5)*pow(1+m_x,2)*sqrt(cM*(1-pow(m_xi,2)))*pow(pow(m_xi,2)+cM*(1-pow(m_xi,2))*(pow(m_xi,2)-pow(m_x,2))/pow(1+m_x,2),2));

        } else                                                                                                                                      // DGLAP-AntiQuark
        {
            uVal  = 0.;
            uValM = 7.5 * pow(1 + m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) * (3 + ((1 - 2 * cM) * atanh(cM/(1+cM)))/((1 + cM) * sqrt(cM/(1 + cM))) )
                / ( pow( 1 - pow(m_xi,2.) , 2.) * pow(1 + cM,2) );
        }
    }

    double uSea  = 0.;
    double uSeaM = 0.;

    quarkDistributionUp.setQuarkDistribution(uVal + uSea);

        // Singlet distribution
        quarkDistributionUp.setQuarkDistributionPlus(uVal - uValM);
        // Nonsinglet distributiion
        quarkDistributionUp.setQuarkDistributionMinus(uVal + uSea + uValM + uSeaM);
        
    // s-quark    
    double dVal = 0.;
    double dSea = 0.;

    quarkDistributionDown.setQuarkDistribution(dVal + dSea);

        // Singlet distribution

    // s-quark
    double sVal = 0.;
    double sSea = 0.;

    quarkDistributionStrange.setQuarkDistribution(sVal + sSea);

    partonDistribution.addQuarkDistribution(quarkDistributionUp);
    partonDistribution.addQuarkDistribution(quarkDistributionDown);
    partonDistribution.addQuarkDistribution(quarkDistributionStrange);

        // Singlet distribution

    // Gluon distributions 
    GluonDistribution gluonDistribution(0.);
    partonDistribution.setGluonDistribution(gluonDistribution);

    return partonDistribution;
}

}
