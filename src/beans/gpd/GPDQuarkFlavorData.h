#ifndef GPD_QUARK_FLAVOR_DATA
#define GPD_QUARK_FLAVOR_DATA

/**
 * @file GPDQuarkFlavorData.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 30 July 2014
 * @version 1.0
 *
 * @class GPDQuarkFlavorData
 *
 * @brief
 */

#include <string>

#include "../QuarkFlavor.h"
#include "GPDComputeType.h"

//TODO GPD -> PartonDistributionQuarkFlavorData
class GPDQuarkFlavorData {

public:
    /**
     * @file GPDResultData.h
     * @brief
     * @author Bryan BERTHOU (CEA Saclay)
     * @date 24 July 2014
     * @version 1.0
     *
     * @class GPDResultData
     *
     * @brief
     */(GPDComputeType::Type _gpdComputeType,
            QuarkFlavor::Type _quarkFlavorType);

    virtual ~GPDQuarkFlavorData();

    std::string toString();
    std::string toStringGeneric();

    double getSea() const;

    void setSea(double hqSea);

    double getValence() const;

    void setValence(double hqVal);

    QuarkFlavor* getQuarkFlavor();

    void setQuarkFlavor(QuarkFlavor &_quarkFlavor);

    double getPartonDistributionMinus() const;
    void setPartonDistributionMinus(double partonDistributionMinus);
    double getPartonDistributionPlus() const;
    void setPartonDistributionPlus(double partonDistributionPlus);
    double getPartonDistribution() const;
    void setPartonDistribution(double partonDistribution);
    double getPartonDistributionSinglet() const;
    void setPartonDistributionSinglet(double partonDistributionSinglet);

private:
    GPDComputeType m_gpdComputeType;
    QuarkFlavor m_quarkFlavor;

    double m_partonDistribution;          ///< GPD(q), flavour singlet
    //TODO faire référence à la revue paraph 3.3.2 de Markus Diehl
    double m_partonDistributionMinus;   ///<
    double m_partonDistributionPlus;    ///<

    double valence;             ///< GPD(q), valence part
    double sea;             ///< GPD(q), sea part

    //TODO singlet -> devient pdPlus
    double m_partonDistributionSinglet;     ///< GPD(q), charge singlet part

    //TODO a voir
    double quark;
    double antiQuark;
};

#endif /* GPD_QUARK_FLAVOR_DATA */
