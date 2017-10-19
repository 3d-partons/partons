#ifndef ACTIVE_FLAVORS_THRESHOLDS_CONSTANT_H
#define ACTIVE_FLAVORS_THRESHOLDS_CONSTANT_H

/**
 * @file ActiveFlavorsThresholdsConstant.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 August 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <stddef.h>
#include <string>

#include "ActiveFlavorsThresholdsModule.h"

namespace PARTONS {

/**
 * @class ActiveFlavorsThresholdsConstant
 *
 * @brief Fixed number of active quark flavors.
 *
 * This module defines one number of active quark flavors for whole range of factorization scale. The default number is 3 and can be changed by or by.
 */
class ActiveFlavorsThresholdsConstant: public ActiveFlavorsThresholdsModule {

public:

    static const std::string PARAM_NAME_N_FLAVORS;

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of this class.
     */
    ActiveFlavorsThresholdsConstant(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ActiveFlavorsThresholdsConstant();

    virtual ActiveFlavorsThresholdsConstant* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();

    /**
     * Set number of quark flavors.
     */
    void setNFlavors(size_t nFlavors);

    /**
     * Get number of quark flavors.
     */
    size_t getNFlavors() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ActiveFlavorsThresholdsConstant(
            const ActiveFlavorsThresholdsConstant &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Number of quark flavors.
     */
    size_t m_nFlavors;
};

} /* namespace PARTONS */

#endif /* ACTIVE_FLAVORS_THRESHOLDS_CONSTANT_H */
