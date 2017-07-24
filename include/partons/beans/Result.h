#ifndef RESULT_H
#define RESULT_H

/**
 * @file Result.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 12 November 2015
 * @version 1.0
 */

#include <string>

#include "system/ResultInfo.h"

namespace PARTONS {

/**
 * @class Result
 *
 * @brief Base class for all result-like classes.
 *
 * This class is used as a base (abstract) class for all classes storing result information, like e.g. GPDResult or ObservableResult.
 */
class Result: public BaseObject {

public:

    /**
     * Default constructor.
     * @param className Name of class to be associated to BaseObject (see BaseObject::m_className variable)
     */
    Result(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    Result(const Result &other);

    /**
     * Destructor.
     */
    virtual ~Result();

    virtual std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get information on this result.
     */
    const ResultInfo& getResultInfo() const;

    /**
     * Set information on this result.
     */
    void setResultInfo(const ResultInfo& resultInfo);

    /**
     * Get name of module used to evaluate this result.
     */
    const std::string& getComputationModuleName() const;

    /**
     * Set name of module used to evaluate this result.
     */
    void setComputationModuleName(const std::string& moduleName);

private:

    /**
     * Information on this result.
     */
    ResultInfo m_resultInfo;

    /**
     * Name of module used to evaluate this result.
     */
    std::string m_computationModuleName;
};

} /* namespace PARTONS */

#endif /* RESULT_H */
