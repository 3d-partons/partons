#ifndef QUARK_DISTRIBUTION_H
#define QUARK_DISTRIBUTION_H

#include <string>

#include "../../BaseObject.h"
#include "../QuarkFlavor.h"

class ComparisonReport;
class Tolerances;

class QuarkDistribution: public BaseObject {
public:

    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION;
    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS;
    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS;

    QuarkDistribution();
    QuarkDistribution(QuarkFlavor::Type quarkFlavor);
    QuarkDistribution(QuarkFlavor::Type quarkFlavor, double quarkDistribution,
            double quarkDistributionPlus, double quarkDistributionMinus);
    virtual ~QuarkDistribution();

    virtual std::string toString() const;

    ComparisonReport compare(const QuarkDistribution& referenceObject,
            const Tolerances& tolerances) const;

    // ##### GETTERS & SETTERS #####

    double getAntiQuark() const;
    void setAntiQuark(double antiQuark);
    double getQuarkDistribution() const;
    void setQuarkDistribution(double quarkDistribution);
    double getQuarkDistributionPlus() const;
    void setQuarkDistributionPlus(double quarkDistributionPlus);
    double getQuarkDistributionMinus() const;
    void setQuarkDistributionMinus(double quarkDistributionMinus);
    double getQuark() const;
    void setQuark(double quark);
    QuarkFlavor::Type getQuarkFlavor() const;
    void setQuarkFlavor(QuarkFlavor::Type quarkFlavorType);

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
