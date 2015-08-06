#ifndef PARTON_DISTRIBUTION_H
#define PARTON_DISTRIBUTION_H

#include <map>
#include <string>
#include <vector>

#include "../../BaseObject.h"
#include "../QuarkFlavor.h"
#include "GluonDistribution.h"
#include "QuarkDistribution.h"

class PartonDistribution: public BaseObject {
public:
    PartonDistribution();

    virtual ~PartonDistribution();

    void addQuarkDistribution(QuarkDistribution &quarkDistribution);
    const QuarkDistribution& getQuarkDistribution(
            QuarkFlavor::Type quarkFlavorType) const;

    std::vector<QuarkFlavor::Type> listTypeOfQuarkFlavor();
    unsigned int getQuarkDistributionsSize() const;

    const std::vector<QuarkDistribution> getVectorOfQuarkDistribution() const;

    double getSinglet();

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    const GluonDistribution& getGluonDistribution() const;
    void setGluonDistribution(const GluonDistribution &gluonDistribution);
    const std::map<QuarkFlavor::Type, QuarkDistribution>& getQuarkDistributions() const;
    void setQuarkDistributions(
            const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkDistributions);

private:
    std::map<QuarkFlavor::Type, QuarkDistribution> m_quarkDistributions;
    // std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it;

    GluonDistribution m_gluonDistribution;
};

#endif /* PARTON_DISTRIBUTION_H */
