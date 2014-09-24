#ifndef GPD_QUARK_FLAVOR_DATA
#define GPD_QUARK_FLAVOR_DATA

/**
 * @file GPDQuarkFlavorData.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 30 Juil 2014
 * @version 1.0
 */

#include <string>

#include "../QuarkFlavor.h"
#include "GPDComputeType.h"

class GPDQuarkFlavorData {
    GPDComputeType m_gpdComputeType;
    QuarkFlavor m_quarkFlavor;

    //TODO changer le nom de cette variable
    double Hq;          ///< GPD(q), flavour singlet
    double valence;     ///< GPD(q), valence part
    double sea;         ///< GPD(q), sea part
    double singlet;     ///< GPD(q), charge singlet part

public:
    GPDQuarkFlavorData(GPDComputeType &_gpdComputeType,
            QuarkFlavor::Type _quarkFlavorType);

    virtual ~GPDQuarkFlavorData();

    std::string toString();

    double getHq() const;

    void setHq(double hq);

    double getSea() const;

    void setSea(double hqSea);

    double getSinglet() const;

    void setSinglet(double hqSinglet);

    double getValence() const;

    void setValence(double hqVal);

    QuarkFlavor* getQuarkFlavor();

    void setQuarkFlavor(QuarkFlavor &_quarkFlavor);
};

#endif /* GPD_QUARK_FLAVOR_DATA */
