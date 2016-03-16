#ifndef COMPUTATION_CONFIGURATION_H
#define COMPUTATION_CONFIGURATION_H

/**
 * @file ComputationConfiguration.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 11 March 2016
 * @version 1.0
 *
 * @class ComputationConfiguration
 *
 * @brief
 */

#include <string>

#include "../../database/DatabaseObject.h"

class ComputationConfiguration: public DatabaseObject {
public:
    ComputationConfiguration();
    ComputationConfiguration(const std::string &xmlFile,
            const std::string &md5);
    ComputationConfiguration(int indexId, const std::string &xmlFile,
            const std::string &md5);
    virtual ~ComputationConfiguration();

    // ##### GETTERS & SETTERS #####

    const std::string& getMd5() const;
    void setMd5(const std::string& md5);
    // mutable allow modification of member in const function
    const std::string& getXmlFile() const;
    void setXmlFile(const std::string& xmlFile);

private:
    // xml file member is empty when object created from database to save possible large memory space
    // user need to call ComputationConfigurationDaoService manually to get xml file by this object indexId
    std::string m_xmlFile;
    std::string m_md5;
};

#endif /* COMPUTATION_CONFIGURATION_H */
