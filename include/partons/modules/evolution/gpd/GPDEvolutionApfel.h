#ifndef GPD_EVOLUTION_APFEL_H
#define GPD_EVOLUTION_APFEL_H

/**
 * @file GPDEvolutionApfel.h
 * @author Valerio BERTONE (CEA Saclay)
 * @date September 25, 2020
 * @version 1.0
 */

#include "GPDEvolutionModule.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"

#include <apfel/apfelxx.h>
#include <functional>

namespace PARTONS {

class RunningAlphaStrongModule;

/**
 * @class GPDEvolutionApfel
 */
class GPDEvolutionApfel: public GPDEvolutionModule {

public:

    /**
     * Settable parameters
     */

	/*
	 *TODO: give a description of what these parameters means
	 */

    static const std::string PARAM_NAME_THRESHOLDS;             // Threshold of quarks entering the loops and modifying the running
    static const std::string PARAM_NAME_MASSES;                 // Masses of the quarks

    // Descritpion of the Grid in x variable
    static const std::string PARAM_NAME_SUBGRID_NODES;          // Number of node per subgrid
    static const std::string PARAM_NAME_SUBGRID_LOWER_BOUNDS;   // Lower value for each subgrid
    static const std::string PARAM_NAME_SUBGRID_INTER_DEGREES;  // Interpolation degree in each subgrid

    // Description of the Q^2 grid
    static const std::string PARAM_NAME_TAB_NODES;              //
    static const std::string PARAM_NAME_TAB_LOWER_BOUND;        // Lower bound which
    static const std::string PARAM_NAME_TAB_UPPER_BOUND;        // Upper bound for which the code will be able to evolve
    static const std::string PARAM_NAME_TAB_INTER_DEGREE;       // Interpolation degree in Q^2

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    GPDEvolutionApfel(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDEvolutionApfel();

    virtual GPDEvolutionApfel* clone() const;
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

    void setPreviousXi(const double& xi_prev);

    std::vector<double> getThresholds() const;
    std::vector<double> getMasses() const;

    std::vector<int> getSubgridNodes() const;
    std::vector<double> getSubgridLowerBounds() const;
    std::vector<int> getSubgridInterDegrees() const;

    int getTabNodes() const;
    double getTabLowerBound() const;
    double getTabUpperBound() const;
    int getTabInterDegree() const;
    double getPreviousXi() const;

    std::shared_ptr<apfel::Grid> getGrid() const;
    std::function<double(double const&)> getAlphasFunc() const;
    std::shared_ptr<apfel::TabulateObject<apfel::Set<apfel::Distribution>>> getTabulatedGDPs() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDEvolutionApfel(const GPDEvolutionApfel &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PartonDistribution compute(GPDModule* pGPDModule, const GPDType::Type &type);

    std::function<std::map<int, double>(double const&, double const&)> initialScaleDistributions(GPDModule* pGPDModule);

private:

    void initializeGrid();

    std::vector<double> m_thresholds;
    std::vector<double> m_masses;
    std::vector<int> m_subgridNodes;
    std::vector<double> m_subgridLowerBounds;
    std::vector<int> m_subgridInterDegrees;
    int m_tabNodes;
    double m_tabLowerBound;
    double m_tabUpperBound;
    int m_tabInterDegree;
    double m_xi_prev;

    std::shared_ptr<apfel::Grid> m_g;
    std::function<double(double const&)> m_as;
    std::shared_ptr<apfel::TabulateObject<apfel::Set<apfel::Distribution>>> m_tabulatedGpds;
};

} /* namespace PARTONS */

#endif /* GPD_EVOLUTION_APFEL_H */
