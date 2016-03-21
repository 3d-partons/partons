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
    //TODO documentation about mutable
    mutable std::string m_xmlFile;
    mutable std::string m_md5;
};

#endif /* COMPUTATION_CONFIGURATION_H */
