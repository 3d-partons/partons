#ifndef COLLINEAR_DISTRIBUTION_EVOLUTION_APFEL_H
#define COLLINEAR_DISTRIBUTION_EVOLUTION_APFEL_H

/**
 * @file CollinearDistributionEvolutionApfel.h
 * @author Valerio BERTONE (CEA Saclay)
 * @date July 27, 2020
 * @version 1.0
 */

#include "CollinearDistributionEvolutionModule.h"
#include "../../../../../include/partons/modules/collinear_distribution/CollinearDistributionModule.h"

#include <apfel/apfelxx.h>

namespace PARTONS {

class RunningAlphaStrongModule;

/**
 * @class CollinearDistributionEvolutionApfel
 */
class CollinearDistributionEvolutionApfel: public CollinearDistributionEvolutionModule {

public:

    /**
     * Settable parameters
     */
    static const std::string PARAM_NAME_THRESHOLDS;
    static const std::string PARAM_NAME_MASSES;

    static const std::string PARAM_NAME_SUBGRID_NODES;
    static const std::string PARAM_NAME_SUBGRID_LOWER_BOUNDS;
    static const std::string PARAM_NAME_SUBGRID_INTER_DEGREES;

    static const std::string PARAM_NAME_TAB_NODES;
    static const std::string PARAM_NAME_TAB_LOWER_BOUND;
    static const std::string PARAM_NAME_TAB_UPPER_BOUND;
    static const std::string PARAM_NAME_TAB_INTER_DEGREE;

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    CollinearDistributionEvolutionApfel(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionEvolutionApfel();

    virtual CollinearDistributionEvolutionApfel* clone() const;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData);

    // ##### GETTERS & SETTERS #####

    void setThresholds(const std::vector<double>& thresholds);
    void setMasses(const std::vector<double>& masses);

    void setSubgridNodes(const std::vector<int>& subgridNodes);
    void setSubgridLowerBounds(const std::vector<double>& subgridLowerBounds);
    void setSubgridInterDegrees(const std::vector<int>& subgridInterDegrees);

    void setTabNodes(const int& tabNodes);
    void setTabLowerBound(const double& tabLowerBound);
    void setTabUpperBound(const double& tabUpperBound);
    void setTabInterDegree(const int& tabInterDegree);

    std::vector<double> getThresholds() const;
    std::vector<double> getMasses() const;

    std::vector<int> getSubgridNodes() const;
    std::vector<double> getSubgridLowerBounds() const;
    std::vector<int> getSubgridInterDegrees() const;

    int getTabNodes() const;
    double getTabLowerBound() const;
    double getTabUpperBound() const;
    int getTabInterDegree() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    CollinearDistributionEvolutionApfel(const CollinearDistributionEvolutionApfel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PartonDistribution compute(CollinearDistributionModule* pCollinearDistributionModule);

    std::function<std::map<int, double>(double const&)> initialScaleDistributions(CollinearDistributionModule* pCollinearDistributionModule);

private:

    std::vector<double> m_thresholds;
    std::vector<double> m_masses;
    std::vector<int> m_subgridNodes;
    std::vector<double> m_subgridLowerBounds;
    std::vector<int> m_subgridInterDegrees;
    int m_tabNodes;
    double m_tabLowerBound;
    double m_tabUpperBound;
    int m_tabInterDegree;

    std::unique_ptr<apfel::Grid> m_g;
    std::unique_ptr<apfel::TabulateObject<apfel::Set<apfel::Operator>>> m_tabulatedOps;
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_EVOLUTION_APFEL_H */
