#ifndef GLUON_DISTRIBUTION_H
#define GLUON_DISTRIBUTION_H

#include <string>

class GluonDistribution {
public:
    static const std::string GLUON_DISTRIBUTION_DB_COLUMN_NAME;

    GluonDistribution();
    GluonDistribution(double gluonDistribution);
    virtual ~GluonDistribution();

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    double getGluonDistribution() const;
    void setGluonDistribution(double gluonDistribution);

private:
    double m_gluonDistribution;
};

#endif /* GLUON_DISTRIBUTION_H */
