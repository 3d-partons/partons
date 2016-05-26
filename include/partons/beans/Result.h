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

#include "../database/DatabaseObject.h"
#include "system/ResultInfo.h"

class Result: public DatabaseObject {
public:
    Result(const std::string &className);

    Result(const Result &other);

    virtual ~Result();

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    const std::string& getComputationModuleName() const;
    void setComputationModuleName(const std::string& moduleName);
    const ResultInfo& getResultInfo() const;
    void setResultInfo(const ResultInfo& resultInfo);

private:
    ResultInfo m_resultInfo;

    std::string m_computationModuleName;
};

#endif /* RESULT_H */
