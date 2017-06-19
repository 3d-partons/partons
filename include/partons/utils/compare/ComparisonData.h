#ifndef COMPARISON_DATA_H
#define COMPARISON_DATA_H

/**
 * @file ComparisonData.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 24 February 2016
 * @version 1.0
 */

#include <NumA/utils/Differences.h>
#include <string>
/**
 * @class ComparisonData
 *
 * @brief Comparison report for single data point.
 *
 * This class stores a comparison result for a single data point.
 */
class ComparisonData {

public:

	/**
	 * Constructor.
	 * @param isPassed Boolean value indicating if the comparison is passed.
	 * @param variableName Name of variable.
	 * @param variableValue Value of variable.
	 * @param variableValueReference Reference value of variable.
	 * @param differences Difference.
	 * @param objectInfo Information on object being compared.
	 */
	ComparisonData(bool isPassed, const std::string& variableName,
			const std::string& variableValue,
			const std::string& variableValueReference,
			const NumA::Differences &differences,
			const std::string &objectInfo);

	/**
	 * Destructor.
	 */
	virtual ~ComparisonData();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values.
     * @return Pre-formatted characters string.
     */
	std::string toString() const;

	//********************************************************
	//*** SETTERS AND GETTERS ********************************
	//********************************************************

	/**
	 * Check if the comparison is passed.
	 */
	bool isIsComparisonPassed() const;

	/**
	 * Set if the comparison is passed.
	 */
	void setIsComparisonPassed(bool isComparisonPassed);

private:

	/**
	 * Boolean value indicating if the comparison is passed.
	 */
	bool m_isComparisonPassed;

	/**
	 * Name of variable.
	 */
	std::string m_variableName;

	/**
	 * Value of variable.
	 */
	std::string m_variableValue;

	/**
	 * Reference value of variable.
	 */
	std::string m_variableValueReference;

	/**
	 * Difference.
	 */
	NumA::Differences m_differences;

	/**
	 * Information on object being compared.
	 */
	std::string m_objectInfo;
};

#endif /* COMPARISON_DATA_H */
