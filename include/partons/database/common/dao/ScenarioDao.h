#ifndef SCENARIO_DAO_H
#define SCENARIO_DAO_H

/**
 * @file ScenarioDao.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 13, 2015
 * @version 1.0
 *
 * @class ScenarioDao
 *
 * @brief Scenario information Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete scenario information from the database. This class in not intended to be used by regular users who should deal with ScenarioDaoService instead.
 */

#include <include/partons/BaseObject.h>
#include <string>

class Scenario;

class ScenarioDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ScenarioDao();

    /**
     * Destructor.
     */
    virtual ~ScenarioDao();

    /**
     * Insert scenario information into the database.
     * @param description Scenario description.
     * @param xmlFile Xml file scenario representation.
     * @param hashSum Hash sum of xml file.
     * @return Unique id of inserted row in the database.
     */
    int insertWithoutTransaction(const std::string &description,
            const std::string &xmlFile, const std::string &hashSum) const;

    /**
     * Retrieve unique id of row containing scenario information in the database by given hash sum.
     * @param hashSum Hash sum of scenario information to be selected.
     * @return Unique id of selected row in the database.
     */
    int getScenarioIdByHashSum(const std::string &hashSum) const;

    /**
     * Retrieve xml file scenario representation by given unique id of computation.
     * @param indexId Unique id of row containing scenario information to be selected.
     * @return String containing xml representation.
     */
    std::string getXMLFileByIndexId(const int indexId) const;

    /**
     * Retrieve unique id of row containing scenario information in the database by id of that containing computation information.
     * @param computationId  Unique id of row containing scenario information in the database.
     * @return Unique id of selected row in the database.
     */
    int getScenarioIdByComputationId(const int computationId) const;

    /**
     * Retrieve hash sum associated to scenario information stored in row in the database of unique id.
     * @param scenarioId Unique id of row containing scenario information in the database.
     * @return Associated hash sum.
     */
    std::string getHashSumById(const int scenarioId);

    /**
     * Try to find an entry in the database containing scenario information that matches with given unique id value.
     * @param scenarioId Unique id of database entry to be found.
     * @return Pointer to Scenario object filled with the retrieved data or null pointer if couldn't find it. If successful, Scenario object is automatically registered in ResourceManager.
     */
    Scenario* getScenarioById(const int scenarioId);

    /**
     * Update row containing scenario information in the database.
     * @param scenarioId Unique id of scenario information to be selected.
     * @param file Xml file scenario representation.
     * @param hashSum Hash sum of xml file.
     */
    void updateScenarioFile(const int scenarioId, const std::string& file,
            const std::string &hashSum);
};

#endif /* SCENARIO_DAO_H */
