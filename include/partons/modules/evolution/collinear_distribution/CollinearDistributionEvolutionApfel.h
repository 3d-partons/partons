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
#include "../../active_flavors_thresholds/ActiveFlavorsThresholdsModule.h"

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
	static const std::string PARAM_NAME_SUBGRID_NODES;
	static const std::string PARAM_NAME_SUBGRID_LOWER_BOUNDS;
	static const std::string PARAM_NAME_SUBGRID_INTER_DEGREES;

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
	virtual void prepareSubModules(
			const std::map<std::string, BaseObjectData>& subModulesData);

	// ##### GETTERS & SETTERS #####

	void setSubgridNodes(const std::vector<int>& subgridNodes);
	void setSubgridLowerBounds(const std::vector<double>& subgridLowerBounds);
	void setSubgridInterDegrees(const std::vector<int>& subgridInterDegrees);

	std::vector<int> getSubgridNodes() const;
	std::vector<double> getSubgridLowerBounds() const;
	std::vector<int> getSubgridInterDegrees() const;

	virtual void setRunningAlphaStrongModule(RunningAlphaStrongModule* runningAlphaStrongModule);

protected:

	/**
	 * Copy constructor.
	 * @param other Object to be copied.
	 */
	CollinearDistributionEvolutionApfel(
			const CollinearDistributionEvolutionApfel &other);

	virtual void initModule();
	virtual void isModuleWellConfigured();

	virtual PartonDistribution compute(
			CollinearDistributionModule* pCollinearDistributionModule);

	std::function<std::map<int, double>(double const&, double const&)> initialScaleDistributions(
			CollinearDistributionModule* pCollinearDistributionModule);

private:

	std::vector<int> m_subgridNodes;
	std::vector<double> m_subgridLowerBounds;
	std::vector<int> m_subgridInterDegrees;
	std::shared_ptr<apfel::Grid> m_grid;
	std::map<int, apfel::DglapObjects> m_dglapobj;

	bool m_setApfelTables;	///< Switch used to generate Apfel tables.
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_EVOLUTION_APFEL_H */
