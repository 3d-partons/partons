#ifndef SCENARIO_DAO_SERVICE_H
#define SCENARIO_DAO_SERVICE_H

/**
 * @file ScenarioDaoService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 12, 2015
 * @version 1.0
 *
 * @class ScenarioDaoService
 *
 * @brief Scenario information Data Access Object (DAO) service.
 *
 * It deals with Scenario C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove scenario information from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */

#include <include/partons/database/common/dao/ScenarioDao.h>
#include <string>

class ResourceManager;
class Scenario;

class ScenarioDaoService: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ScenarioDaoService();

    /**
     * Destructor.
     */
    virtual ~ScenarioDaoService();

    /**
     * Insert into database a new Scenario object with transactions mechanisms.
     * @param scenario Reference to Scenario object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const Scenario &scenario) const;

    /**
     * Try to find unique id of database entry containing scenario information that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
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
     * Try to find an entry in the database containing scenario information that matches with given unique id value.
     * @param scenarioId Unique id of database entry to be found.
     * @return Pointer to Scenario object filled with the retrieved data or null pointer if couldn't find it. If successful, Scenario object is automatically registered in ResourceManager.
     */
    Scenario* getScenarioById(const int scenarioId);

    /**
     * Retrieve hash sum associated to scenario information stored in row in the database of unique id.
     * @param scenarioId  Unique id of row containing scenario information in the database.
     * @return Associated hash sum.
     */
    std::string getHashSumById(const int scenarioId);

    /**
     * Update row containing scenario information in the database.
     * @param scenarioId Unique id of scenario information to be selected.
     * @param file Xml file scenario representation.
     */
    void updateScenarioFile(const int scenarioId, const std::string &file);

private:

    /**
     * ScenarioDao object to perform database queries.
     */
    ScenarioDao m_scenarioDao;

    /**
     * Pointer to ResourceManager.
     */
    ResourceManager* m_pResourceManager;
};

#endif /* SCENARIO_DAO_SERVICE_H */
