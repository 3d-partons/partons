#include "../../../include/partons/utils/PropertiesManager.h"

#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>

const std::string PropertiesManager::PARTONS_PROPERTIES_FILE_PATH =
        "partons.properties";

sf::Mutex PropertiesManager::m_mutex;

// Global static pointer used to ensure a single instance of the class.
PropertiesManager* PropertiesManager::pInstance = 0;

PropertiesManager::PropertiesManager() {
}

PropertiesManager* PropertiesManager::getInstance() {
    sf::Lock lock(m_mutex); // mutex.lock()

    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new PropertiesManager();
    }

    return pInstance;
} // mutex.unlock()

PropertiesManager::~PropertiesManager() {
    sf::Lock lock(m_mutex); // mutex.lock()

    if (pInstance != 0) {
        delete pInstance;
        pInstance = 0;
    }
} // mutex.unlock()

void PropertiesManager::init(const std::string &currentWorkingDirectoryPath) {
    sf::Lock lock(m_mutex); // mutex.lock()

    // std::cerr << "currentWorkingDirectoryPath: " << currentWorkingDirectoryPath << std::endl;
    m_iniFileParser.parse(
            currentWorkingDirectoryPath
                    + PropertiesManager::PARTONS_PROPERTIES_FILE_PATH);
} // mutex.unlock()

//TODO voir à ajouter en parametre la valeur de retour par défaut dans le cas ou EMPTY est une valeur possible
std::string PropertiesManager::getString(const std::string & section,
        const std::string & key) {
    sf::Lock lock(m_mutex); // mutex.lock()

    return m_iniFileParser.getString(section, key);
} // mutex.unlock()

//TODO voir à ajouter en parametre la valeur de retour par défaut dans le cas ou EMPTY est une valeur possible
std::string PropertiesManager::getString(const std::string & key) {
    sf::Lock lock(m_mutex); // mutex.lock()

    return m_iniFileParser.getString(key);
} // mutex.unlock()
