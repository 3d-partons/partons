#ifndef DOUBLE_DISTRIBUTION_MODULE_H
#define DOUBLE_DISTRIBUTION_MODULE_H

/**
 * @file DoubleDistributionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 July 2015
 * @version 1.0
 *
 * @class DoubleDistributionModule
 *
 * @brief
 */

#include <map>
#include <string>

#include "../beans/double_distribution/DoubleDistributionType.h"
#include "../beans/parton_distribution/PartonDistribution.h"
#include "../ModuleObject.h"

class DoubleDistributionResult;

class DoubleDistributionModule: public ModuleObject {
public:
    /**
     * Constructor.
     * See BaseObject class for more info about input parameter.
     *
     * @param className class's name of child class.
     */
    DoubleDistributionModule(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~DoubleDistributionModule();

    /**
     * Clone
     *
     * @return
     */
    virtual DoubleDistributionModule* clone() const = 0;

    virtual DoubleDistributionResult compute(double beta, double alpha,
            double t, double MuF2, double MuR2,
            DoubleDistributionType::Type doubleDistributionType);

    virtual PartonDistribution computeF();
    virtual PartonDistribution computeG();
    virtual PartonDistribution computeK();

protected:
    std::map<DoubleDistributionType::Type,
            PartonDistribution (DoubleDistributionModule::*)()> m_listOfAvailableDualDistributionFunctions;
    std::map<DoubleDistributionType::Type,
            PartonDistribution (DoubleDistributionModule::*)()>::iterator m_it;

    /**
     * Copy constructor
     *
     * @param other
     */
    DoubleDistributionModule(const DoubleDistributionModule &other);

    void preCompute(double beta, double alpha, double t, double MuF2,
            double MuR2, DoubleDistributionType::Type doubleDistributionType);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:
    double m_beta;
    double m_alpha;
    double m_t;
    double m_MuF2;
    double m_MuR2;

    DoubleDistributionType::Type m_doubleDistributionType;
};

#endif /* DOUBLE_DISTRIBUTION_MODULE_H */
