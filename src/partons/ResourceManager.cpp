#include "../../include/partons/ResourceManager.h"

namespace PARTONS {

ResourceManager* ResourceManager::m_pInstance = 0;

ResourceManager* ResourceManager::getInstance() {
    if (!m_pInstance) {
        m_pInstance = new ResourceManager();
    }
    return m_pInstance;
}

Scenario* ResourceManager::getScenarioByHashSum(
        const std::string& hashSum) const {
    Scenario* pScenario = 0;

    std::map<std::string, std::pair<Scenario*, unsigned int> >::const_iterator it;

    it = m_scenarioResourceList.find(hashSum);

    if (it != m_scenarioResourceList.end()) {
        pScenario = (it->second).first;
    }

    return pScenario;
}

ResourceManager::ResourceManager() :
        BaseObject("ResourceManager") {
}

ResourceManager::~ResourceManager() {
//TODO Is automatically auto_ptr destroy ?

//    std::map<std::string, std::pair<std::auto_ptr<Scenario>, unsigned int> >::iterator it_scenarioResourceList;
//
//    for (it_scenarioResourceList = m_scenarioResourceList.begin();
//            it_scenarioResourceList != m_scenarioResourceList.end();
//            it_scenarioResourceList++) {
//        if ((it_scenarioResourceList->second).first) {
//            delete (it_scenarioResourceList->second).first;
//            (it_scenarioResourceList->second).first = 0;
//        }
//    }

}

//TODO how to handle counter when gives back pointer ?
//Scenario* ResourceManager::getScenario(const std::string &hashSum) {
//    std::map<std::string, std::pair<std::auto_ptr<Scenario>, unsigned int> >::iterator it_scenarioResourceList;
//    it_scenarioResourceList = m_scenarioResourceList.find(hashSum);
//
//    Scenario* pScenario = 0;
//
//    if (it_scenarioResourceList != m_scenarioResourceList.end()) {
//        pScenario = (it_scenarioResourceList->second).first.get();
//        (it_scenarioResourceList->second).second++;
//    }
//
//    return pScenario;
//}

Scenario* ResourceManager::registerScenario(Scenario* pScenario) {
    return registerScenario(
            pScenario->getDescription(),
            pScenario->getFilePath(), pScenario->getHashSum(),
            pScenario->getFile()
    );
}

//TODO how to handle counter when gives back pointer ?
Scenario* ResourceManager::registerScenario(
const std::string &description,
        const std::string &filePath, const std::string& hashSum,
        const std::string& file) {

    std::pair<
            std::map<std::string, std::pair<Scenario*, unsigned int> >::iterator,
            bool> isInserted = m_scenarioResourceList.insert(
            std::make_pair(hashSum,
                    std::make_pair(
                            (new Scenario(description,
                                    filePath, hashSum, file)), 1)));

    // If this scenario already exists in the map
    if (!(isInserted.second)) {
        // Just increment the counter by one.
        ((isInserted.first)->second).second++;
    }

    // Return a pointer to the scenario in memory.
    return ((isInserted.first)->second).first;
}

} /* namespace PARTONS */
