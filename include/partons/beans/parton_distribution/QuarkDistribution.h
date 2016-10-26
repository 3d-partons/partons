#ifndef QUARK_DISTRIBUTION_H
#define QUARK_DISTRIBUTION_H

#include <string>

#include "../../BaseObject.h"
#include "../../utils/compare/ComparisonReport.h"
#include "../QuarkFlavor.h"

class QuarkDistribution: public BaseObject {
public:

    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION;
    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS;
    static const std::string QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS;

    //QuarkDistribution(); // Not needed because the other constructor uses default values now
    QuarkDistribution(const QuarkDistribution &other);
    //QuarkDistribution(QuarkFlavor::Type quarkFlavor); // Not needed because the other constructor uses default values now
    QuarkDistribution(QuarkFlavor::Type quarkFlavor = QuarkFlavor::UNDEFINED, double quarkDistribution = 0.,
            double quarkDistributionPlus = 0., double quarkDistributionMinus = 0.);
    virtual ~QuarkDistribution();

    virtual std::string toString() const;

    void compare(ComparisonReport &rootComparisonReport,
            const QuarkDistribution &referenceObject,
            std::string parentObjectInfo = "") const;

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
    QuarkFlavor getQuarkFlavor() const;
    void setQuarkFlavor(QuarkFlavor quarkFlavorType);

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
