#include "ScenarioManager.h"

#include <cmath>
#include <map>

#include "beans/kinematic/GPDKinematic.h"
#include "modules/GPDModule.h"
#include "ModuleObjectFactory.h"
#include "services/ServiceFunctionNames.h"
#include "ServiceObject.h"
#include "ServiceObjectRegistry.h"
#include "utils/module/hadron_structure/GPDUtils.h"
#include "utils/parser/xml/Attributs.h"
#include "utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
ScenarioManager* ScenarioManager::pInstance = 0;

ScenarioManager::ScenarioManager() {
}

ScenarioManager* ScenarioManager::getInstance() {
    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new ScenarioManager();
    }

    return pInstance;
}

ScenarioManager::~ScenarioManager() {
    if (pInstance != 0) {
        delete pInstance;
        pInstance = 0;
    }
}

void ScenarioManager::playScenario(const std::string &scenarioFilePath) {
    // parse XML file
    analyse(scenarioFilePath);

    ServiceObject* pServiceObject = ServiceObjectRegistry::get(
            m_scenario.getServiceName());

    pServiceObject->computeScenario(m_scenario);

//    // tant que la liste des taches a effectuees n'est pas vide
//    while (!m_taskQueue.empty()) {
//        // on regarde quel service doit executer quelle tache
//
//        // on récupére le premier element de la file d'attente
//        sf::Packet currentPacket = m_taskQueue.front();
//        // et on le supprime de la file d'attente
//        m_taskQueue.pop();
//        unsigned int serviceId = 0;
//        currentPacket >> serviceId;
//
//        // on selection le service en function de l'id que l'on a recupéré du fichier XML
//        m_it = m_mapOfAvailableService.find(serviceId);
//        if (m_it != m_mapOfAvailableService.end()) {
//            (m_it->second)->computeTask(currentPacket);
//        } else {
//            throw std::runtime_error(
//                    "[ScenarioService::playScenario] service ID unknow");
//        }
//    }
}

//TODO refactoriser
//TODO passer les chaine de caractere en variable final static
void ScenarioManager::startElement(std::string tagName, Attributs attributes,
        std::string tagValue) {

    if (StringUtils::equals(tagName, "scenario")) {
        std::string scenarioId = attributes.getStringValueOf("id");
        std::string scenarioDescription = attributes.getStringValueOf(
                "description");

        m_scenario.setId(scenarioId);
        m_scenario.setDescription(scenarioDescription);
    }

    if (StringUtils::equals(tagName, "operation")) {
        std::string serviceClassName = attributes.getStringValueOf("service");
        std::string functionName = attributes.getStringValueOf("method");

        m_scenario.setServiceName(serviceClassName);
        m_scenario.setFunctionName(ServiceFunctionNames(functionName));
    }

    if (StringUtils::equals(tagName, "GPDKinematic")) {

        //TODO quand est-ce que l'on fait la conversion des données utilisateurs vers les services ?
        //TODO comment selectionne-t-on la bonne conversion xB to xi ?

        double x = attributes.getDoubleValueOf("x");
        double xi = GPDUtils::convertXBToXi(attributes.getDoubleValueOf("xB"));
        double t = attributes.getDoubleValueOf("t");
        double MuF = sqrt(attributes.getDoubleValueOf("MuF2"));
        double MuR = sqrt(attributes.getDoubleValueOf("MuR2"));

        GPDKinematic* pGPDKinematic = new GPDKinematic(x, xi, t, MuF, MuR);

        m_scenario.addFunctionArg(tagName, static_cast<void*>(pGPDKinematic));
    }

    if (StringUtils::equals(tagName, "GPDModule")) {
        std::string gpdModuleId = attributes.getStringValueOf("id");

        GPDModule* pGPDModule = ModuleObjectFactory::newGPDModule(gpdModuleId);

        m_scenario.addFunctionArg(tagName, static_cast<void*>(pGPDModule));
    }
}
