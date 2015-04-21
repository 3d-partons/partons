#ifndef PROPERTIES_MANAGER_H
#define PROPERTIES_MANAGER_H

/**
 * @file PropertiesManager.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 21 April 2015
 * @version 1.0
 *
 * @class PropertiesManager
 * @brief \<singleton\> Use for handle properties from configuration's file.
 */

#include <string>

#include "parser/IniFileParser.h"

class PropertiesManager {
public:

    /**
     * A static function that provides a unique pointer of this class
     *
     * @return A unique pointer of this class
     */
    static PropertiesManager* getInstance();

    /**
     * Default destructor
     */
    virtual ~PropertiesManager();

    void init(const std::string &currentWorkingDirectoryPath);

    std::string getString(const std::string & section, const std::string & key);
    std::string getString(const std::string & key);

private:
    static const std::string PARTONS_PROPERTIES_FILE_PATH;

    static PropertiesManager* pInstance; ///< Private pointer of this class for a unique instance

    /**
     * Private default constructor for a unique instance of this class
     */
    PropertiesManager();

    IniFileParser m_iniFileParser;
};

#endif /* PROPERTIES_MANAGER_H */
