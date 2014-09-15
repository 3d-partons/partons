#ifndef GPD_QUARK_FLAVOR_DATA
#define GPD_QUARK_FLAVOR_DATA

/**
 * @file GPDQuarkFlavorData.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 30 Juil 2014
 * @version 1.0
 */

#include <string>

struct GPDComputeType;

struct QuarkFlavor;

class GPDQuarkFlavorData {
    GPDComputeType* pGPDComputeType;
    QuarkFlavor* pQuarkFlavor;

    //TODO changer le nom de cette variable
    double Hq;          ///< GPD(q), flavour singlet
    double valence;     ///< GPD(q), valence part
    double sea;         ///< GPD(q), sea part
    double singlet;     ///< GPD(q), charge singlet part

public:
    GPDQuarkFlavorData(GPDComputeType* _pGPDComputeType,
            QuarkFlavor* _pQuarkFlavor);

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

    QuarkFlavor* getQuarkFlavor() const;

    void setQuarkFlavor(QuarkFlavor* quarkFlavor);
};

#endif /* GPD_QUARK_FLAVOR_DATA */
