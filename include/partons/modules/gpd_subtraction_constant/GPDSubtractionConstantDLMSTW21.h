/*
 * GPDSubtractionConstantDLMSTW21.h
 *
 *  Created on: Jan 8, 2021
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef GPD_SUBTRACTION_CONSTANT_DLMSTW21_H
#define GPD_SUBTRACTION_CONSTANT_DLMSTW21_H

#include <stddef.h>
#include <string>
#include <vector>

#include <ElementaryUtils/parameters/Parameters.h>

#include "../../modules/gpd_subtraction_constant/GPDSubtractionConstantModule.h"
#include "../../utils/type/PhysicalType.h"

namespace PARTONS {

/**
 * @class GPDSubtractionConstantDLMSTW21
 *
 * @brief Implementation of DLMSTW subtraction constant.
 *
 * This module implements the subtraction constant based on tripole Ansatz with parameters
 * extracted as described in 2021 paper by Dutrieux, Lorce, Moutarde, Sznajder, Trawinski and Wagner.
 */
class GPDSubtractionConstantDLMSTW21: public GPDSubtractionConstantModule {

public:

	static const std::string PARAMETER_NAME_REPLICA; ///< Name of parameter to set replica index via xml file.

	/**
	 * Unique ID to automatically register the class in the registry.
	 */
	static const unsigned int classId;

	/**
	 * Constructor.
	 * @param className Name of this class.
	 */
	GPDSubtractionConstantDLMSTW21(const std::string &className);

	/**
	 * Destructor.
	 */
	virtual ~GPDSubtractionConstantDLMSTW21();

	virtual void configure(const ElemUtils::Parameters &parameters);
	virtual GPDSubtractionConstantDLMSTW21* clone() const;

	/**
	 * Load parameters for a given replica index.
	 */
	void loadParameters(size_t replica, bool printInfo = true);

	/**
	 * Evaluate mean and uncertainty for a given vector of numbers. The procedure include removing of outliers.
	 */
	void getMeanAndUncertainty(const std::vector<double>& v, double& mean,
			double& unc) const;

protected:

	/** Copy constructor.
	 * @param other Object to be copied.
	 */
	GPDSubtractionConstantDLMSTW21(const GPDSubtractionConstantDLMSTW21& other);

	virtual PhysicalType<double> computeSubtractionConstant();

private:

	/**
	 * Evaluate mean from a given vector.
	 */
	double getMean(const std::vector<double>& v) const;

	/**
	 * Evaluate sigma from a given vector.
	 */
	double getSigma(const std::vector<double>& v) const;

	/**
	 * Remove outliers from a given vector using 3sigma rule.
	 */
	size_t removeOutliers(std::vector<double>& v) const;

	size_t m_replica; ///< Replica index.

	double m_d1g;	///< d1 for gluons at initial scale.
	double m_d1q;	///< d1 for light quarks at initial scale.

	double m_muF20;	///< Initial factorization scale.
	double m_M;		///< Parameter of multipole Ansatz.
	double m_alpha;	///< Parameter of multipole Ansatz.
};

} /* namespace PARTONS */

#endif /* GPD_SUBTRACTION_CONSTANT_DLMSTW21_H */
