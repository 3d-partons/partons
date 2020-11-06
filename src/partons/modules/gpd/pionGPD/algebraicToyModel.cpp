#include "../../../../../include/partons/modules/gpd/pionGPD/algebraicToyModel.h"

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/logger/LoggerManager.h>

#include <cmath>

namespace PARTONS {

// With this line we "create" the name for our GPD module. This will be integrated into the factory and thus partons knows about it.
// Usign the assigned name "satyratedModel_Ding" we can make use of this module in subsequent codes. 
const unsigned int algebraicToyModel::classId =
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new algebraicToyModel("algebraicToyModel"));


// This is how partons know about the different methods available for a given class?
algebraicToyModel::algebraicToyModel(const std::string &className) : PARTONS::GPDModule(className) 
{
    //Relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));
   
    /* m_listGPDComputeTypeAvailable.insert(
        std::make_pair(PARTONS::GPDType::E, &PARTONS::GPDModule::computeE));*/
}


algebraicToyModel::algebraicToyModel(const algebraicToyModel& other) : PARTONS::GPDModule(other) 
{

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

    if ( m_x >= m_xi )
    {
        // Quark distributions
        QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
        // QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
        
        quarkDistributionUp.setQuarkDistribution(
            7.5 * pow(1 - m_x, 2.) * ( pow(m_x,2.) - pow(m_xi,2.) ) / pow( 1 - pow(m_xi,2.) , 2.)
            );
        // quarkDistributionDown.setQuarkDistribution(m_xi);

        partonDistribution.addQuarkDistribution(quarkDistributionUp);
        // partonDistribution.addQuarkDistribution(quarkDistributionDown);
    
    } else
    {
        // Quark distributions
        QuarkDistribution quarkDistributionUp(QuarkFlavor::UP);
        // QuarkDistribution quarkDistributionDown(QuarkFlavor::DOWN);
        
        quarkDistributionUp.setQuarkDistribution(
            7.5 * ( 1 - m_x ) * ( pow(m_xi,2) - pow(m_x,2) ) * ( m_x + 2 * m_x * m_xi + pow(m_xi,2) ) / ( pow(m_xi,3) * pow(1+m_xi,2) )
            );
        // quarkDistributionDown.setQuarkDistribution(m_xi);

        partonDistribution.addQuarkDistribution(quarkDistributionUp);
        // partonDistribution.addQuarkDistribution(quarkDistributionDown);
    }

    // Gluon distributions 
    GluonDistribution gluonDistribution(0.);
    partonDistribution.setGluonDistribution(gluonDistribution);

    return partonDistribution;
}

}
