#ifndef PARTON_DISTRIBUTION_H
#define PARTON_DISTRIBUTION_H

#include <map>
#include <string>
#include <vector>

#include "../../database/DatabaseObject.h"
#include "../../utils/compare/ComparisonReport.h"
#include "../List.h"
#include "../QuarkFlavor.h"
#include "GluonDistribution.h"
#include "QuarkDistribution.h"

class ComparisonReport;

class PartonDistribution: public DatabaseObject {
public:
    PartonDistribution();

    virtual ~PartonDistribution();

    void addQuarkDistribution(const QuarkDistribution &quarkDistribution);
    void addQuarkDistribution(QuarkDistribution &quarkDistribution);

    const QuarkDistribution& getQuarkDistribution(
            QuarkFlavor::Type quarkFlavorType) const;

    std::vector<QuarkFlavor::Type> listTypeOfQuarkFlavor();
    unsigned int getQuarkDistributionsSize() const;

    List<QuarkDistribution> getListOfQuarkDistribution() const;

    double getSinglet();

    virtual std::string toString() const;

    void compare(ComparisonReport &rootComparisonReport,
            const PartonDistribution &referenceObject,
            std::string parentObjectInfo = "") const;

    // ##### GETTERS & SETTERS #####

    const GluonDistribution& getGluonDistribution() const;
    void setGluonDistribution(const GluonDistribution &gluonDistribution);
    const std::map<QuarkFlavor::Type, QuarkDistribution>& getQuarkDistributions() const;
    void setQuarkDistributions(
            const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkDistributions);

private:
    std::map<QuarkFlavor::Type, QuarkDistribution> m_quarkDistributions;
    // std::map<QuarkFlavor::Type, QuarkDistribution>::iterator m_it;

    GluonDistribution m_gluonDistribution;
};

#endif /* PARTON_DISTRIBUTION_H */
