#ifndef SCENARIO_DAO_H
#define SCENARIO_DAO_H

/**
 * @file ScenarioDao.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 13, 2015
 * @version 1.0
 */

#include <string>

#include "../../../BaseObject.h"

/**
 * @class ScenarioDao
 *
 * @brief
 */
class ScenarioDao: public BaseObject {
public:
    ScenarioDao();
    virtual ~ScenarioDao();

    int insertWithoutTransaction(const std::string &description,
            const std::string &xmlFile, const std::string &hashSum) const;
    int getScnearioIdByHashSum(const std::string &hashSum) const;
    std::string getXMLFileByIndexId(const int indexId) const;
};

#endif /* SCENARIO_DAO_H */
