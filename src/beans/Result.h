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

#include <ctime>
#include <string>

class Result {
public:
    Result();
    virtual ~Result();

    time_t getComputationDateTime() const;
    void setComputationDateTime(time_t computationDateTime);
    const std::string& getComputationModuleName() const;
    void setComputationModuleName(const std::string& moduleName);

private:
    std::string m_computationModuleName;
    time_t m_computationDateTime;
};

#endif /* RESULT_H */
