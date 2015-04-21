#include "PropertiesManager.h"

#include "../Partons.h"

const std::string PropertiesManager::PARTONS_PROPERTIES_FILE_PATH =
        "data/partons.properties";

// Global static pointer used to ensure a single instance of the class.
PropertiesManager* PropertiesManager::pInstance = 0;

PropertiesManager::PropertiesManager() {
}

PropertiesManager* PropertiesManager::getInstance() {
    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new PropertiesManager();
    }

    return pInstance;
}

PropertiesManager::~PropertiesManager() {
    if (pInstance != 0) {
        delete pInstance;
        pInstance = 0;
    }
}

void PropertiesManager::init(const std::string &currentWorkingDirectoryPath) {
    m_iniFileParser.parse(Partons::getInstance()->getCurrentWorkingDirectory()
            + PropertiesManager::PARTONS_PROPERTIES_FILE_PATH);
}

//TODO voir à ajouter en parametre la valeur de retour par défaut dans le cas ou EMPTY est une valeur possible
std::string PropertiesManager::getString(const std::string & section,
        const std::string & key) {
    return m_iniFileParser.getString(section, key);
}

//TODO voir à ajouter en parametre la valeur de retour par défaut dans le cas ou EMPTY est une valeur possible
std::string PropertiesManager::getString(const std::string & key) {
    return m_iniFileParser.getString(key);
}
