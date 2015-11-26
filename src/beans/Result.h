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

#include "../BaseObject.h"

class Result: public BaseObject {
public:
    Result(const std::string &className);
    virtual ~Result();

    time_t getComputationDateTime() const;
    const std::string& getComputationModuleName() const;
    int getId() const;

    void setComputationDateTime(time_t computationDateTime);
    void setComputationModuleName(const std::string& moduleName);
    void setId(int id);

private:
    std::string m_computationModuleName;
    time_t m_computationDateTime;

    int m_id;
};

#endif /* RESULT_H */
