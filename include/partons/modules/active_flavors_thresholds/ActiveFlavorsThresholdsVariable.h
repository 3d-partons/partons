#ifndef ACTIVE_FLAVORS_THRESHOLDS_VARIABLE_H
#define ACTIVE_FLAVORS_THRESHOLDS_VARIABLE_H

/**
 * @file ActiveFlavorsThresholdsVariable.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 October 2020
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <stddef.h>
#include <string>

#include "ActiveFlavorsThresholdsModule.h"

namespace PARTONS {

/**
 * @class ActiveFlavorsThresholdsVariable
 *
 * @brief Variable number of active quark flavors.
 */
class ActiveFlavorsThresholdsVariable: public ActiveFlavorsThresholdsModule {

public:

    static const std::string PARAM_NAME_THRESHOLDS;

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of this class.
     */
    ActiveFlavorsThresholdsVariable(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ActiveFlavorsThresholdsVariable();

    virtual ActiveFlavorsThresholdsVariable* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();

    /**
     * Set Thresholds.
     */
    void setThresholds(std::vector<double> thresholds);

    /**
     * Get number of quark flavors.
     */
    std::vector<double> getThresholds() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ActiveFlavorsThresholdsVariable(const ActiveFlavorsThresholdsVariable &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Thresholds
     */
    std::vector<double> m_thresholds;
};

} /* namespace PARTONS */

#endif /* ACTIVE_FLAVORS_THRESHOLDS_VARIABLE_H */
