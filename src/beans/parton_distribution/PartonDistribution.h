#ifndef PARTON_DISTRIBUTION_H
#define PARTON_DISTRIBUTION_H

#include <map>
#include <string>
#include <vector>

#include "../QuarkFlavor.h"
#include "GluonDistribution.h"
#include "QuarkDistribution.h"

class PartonDistribution {
public:
    PartonDistribution();
    virtual ~PartonDistribution();

    void addQuarkDistribution(QuarkDistribution &quarkDistribution);
    const QuarkDistribution& getQuarkDistribution(QuarkFlavor::Type quarkFlavorType) const;

    std::vector<QuarkFlavor::Type> listTypeOfQuarkFlavor();
    unsigned int getQuarkDistributionsSize() const;

    std::vector<QuarkDistribution> getVectorOfQuarkDistribution() const;

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    const GluonDistribution& getGluonDistribution() const;
    void setGluonDistribution(const GluonDistribution& gluonDistribution);
    const std::map<QuarkFlavor::Type, QuarkDistribution>& getQuarkDistributions() const;
    void setQuarkDistributions(
            const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkDistributions);
    double getSinglet() const;
    void setSinglet(double singlet);

private:
    std::map<QuarkFlavor::Type, QuarkDistribution> m_quarkDistributions;
    // std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it;

    GluonDistribution m_gluonDistribution;
    double m_singlet;
};

#endif /* PARTON_DISTRIBUTION_H */
