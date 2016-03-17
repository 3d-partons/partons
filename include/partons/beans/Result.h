#ifndef RESULT_H
#define RESULT_H

/**
 * @file Result.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 12 November 2015
 * @version 1.0
 *
 * @class Result
 *
 * @brief
 */

#include <string>

#include "Computation.h"

class Result: public DatabaseObject {
public:
    Result(const std::string &className);
    virtual ~Result();

    // ##### GETTERS & SETTERS #####

    const std::string& getComputationModuleName() const;
    void setComputationModuleName(const std::string& moduleName);
    const Computation& getComputation() const;
    Computation& getComputation();
    void setComputation(const Computation& computation);

private:
    Computation m_computation;

    std::string m_computationModuleName;
};

#endif /* RESULT_H */
