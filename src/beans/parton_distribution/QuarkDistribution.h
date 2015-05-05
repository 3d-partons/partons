#ifndef QUARK_DISTRIBUTION_H
#define QUARK_DISTRIBUTION_H

#include <string>

#include "../QuarkFlavor.h"

class QuarkDistribution {
public:

    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION;
    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS;
    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS;

    QuarkDistribution(QuarkFlavor::Type quarkFlavor);
    QuarkDistribution(QuarkFlavor::Type quarkFlavor, double quarkDistribution,
            double quarkDistributionPlus, double quarkDistributionMinus);
    virtual ~QuarkDistribution();

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    double getAntiQuark() const;
    void setAntiQuark(double antiQuark);
    double getQuarkDistribution() const;
    void setQuarkDistribution(double quarkDistribution);
    double getQuarkDistributionPlus() const;
    void setQuarkDistributionPlus(double quarkDistributionPlus);
    double getQuarkDistributionMinus() const;
    void setQuarkDistributionMinus(double quarkDistributionMinus);
    QuarkFlavor getQuarkFlavor() const;
    void setQuarkFlavor(QuarkFlavor quarkFlavor);
    double getQuark() const;
    void setQuark(double quark);

private:
    QuarkFlavor m_quarkFlavor;

    double m_quarkDistribution;          ///<
    //TODO faire référence à la revue paraph 3.3.2 de Markus Diehl
    double m_quarkDistributionPlus;    ///<
    double m_quarkDistributionMinus;   ///<

    //TODO a voir
    double quark;
    double antiQuark;
};

#endif /* QUARK_DISTRIBUTION_H */
