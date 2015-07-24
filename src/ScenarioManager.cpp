#include "ScenarioManager.h"

#include <iostream>

#include "ServiceObject.h"
#include "ServiceObjectRegistry.h"
#include "utils/parser/xml/Attributs.h"
#include "utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
ScenarioManager* ScenarioManager::pInstance = 0;

ScenarioManager::ScenarioManager() :
        XMLParser() {
}

ScenarioManager* ScenarioManager::getInstance() {
    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new ScenarioManager();
    }

    return pInstance;
}

ScenarioManager::~ScenarioManager() {
    if (pInstance) {
        delete pInstance;
        pInstance = 0;
    }
}

void ScenarioManager::playScenario(const std::string &scenarioFilePath) {
    // parse XML file
    analyse(scenarioFilePath);
//
//    ServiceObject* pServiceObject = ServiceObjectRegistry::get(
//            m_scenario.getServiceName());
//
//    pServiceObject->computeTask(m_scenario);

    for (unsigned int i = 0; i < m_scenario.size(); i++) {
        Task task = m_scenario.getTask(i);
        ServiceObjectRegistry::get(task.getServiceName())->computeTask(task);
    }

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

void ScenarioManager::startElement(const std::string &elementName,
        Attributs attributes, const std::string &elementData) {
    std::cerr << "StartElementName = " << elementName << std::endl;
    std::cerr << "Attributs : \n" << attributes.toString() << std::endl;

    if (StringUtils::equals(elementName, "scenario")) {
        std::string scenarioId = attributes.getStringValueOf("id");
        std::string scenarioDescription = attributes.getStringValueOf(
                "description");

        m_scenario.setId(scenarioId);
        m_scenario.setDescription(scenarioDescription);
    }

    if (StringUtils::equals(elementName, "task")) {
        m_task = Task();

        m_task.setServiceName(attributes.getStringValueOf("service"));
        m_task.setFunctionName(attributes.getStringValueOf("method"));
    }

    if (StringUtils::equals(elementName, "GPDKinematic")) {
        m_task.addParameter(elementName, "x", attributes.getStringValueOf("x"));
        m_task.addParameter(elementName, "xi",
                attributes.getStringValueOf("xB"));
        m_task.addParameter(elementName, "t", attributes.getStringValueOf("t"));
        m_task.addParameter(elementName, "MuF",
                attributes.getStringValueOf("MuF2"));
        m_task.addParameter(elementName, "MuR",
                attributes.getStringValueOf("MuR2"));

        //TODO quand est-ce que l'on fait la conversion des données utilisateurs vers les services ?
        //TODO comment selectionne-t-on la bonne conversion xB to xi ?

//        double x = attributes.getDoubleValueOf("x");
//        //TODO supprimer la conversion code en dur de xB to xi
//        double xi = GPDUtils::convertXBToXi(attributes.getDoubleValueOf("xB"));
//        double t = attributes.getDoubleValueOf("t");
//        double MuF = sqrt(attributes.getDoubleValueOf("MuF2"));
//        double MuR = sqrt(attributes.getDoubleValueOf("MuR2"));
//
//        GPDKinematic* pGPDKinematic = new GPDKinematic(x, xi, t, MuF, MuR);
//
//        m_scenario.addFunctionArg(tagName, static_cast<void*>(pGPDKinematic));
    }
}

void ScenarioManager::endElement(const std::string& elementName) {
    std::cerr << "EndElementName = " << elementName << std::endl;

    if (StringUtils::equals(elementName, "task")) {
        m_scenario.add(m_task);
    }
}
//TODO refactoriser
//TODO passer les chaine de caractere en variable final static
//void ScenarioManager::startElement(std::string tagName, Attributs attributes,
//        std::string tagValue) {
//
//    if (StringUtils::equals(tagName, "scenario")) {
//        std::string scenarioId = attributes.getStringValueOf("id");
//        std::string scenarioDescription = attributes.getStringValueOf(
//                "description");
//
//        m_scenario.setId(scenarioId);
//        m_scenario.setDescription(scenarioDescription);
//    }
//
//    if (StringUtils::equals(tagName, "operation")) {
//        std::string serviceClassName = attributes.getStringValueOf("service");
//        std::string functionName = attributes.getStringValueOf("method");
//
//        m_scenario.setServiceName(serviceClassName);
//        m_scenario.setFunctionName(ServiceFunctionNames(functionName));
//    }
//
//    if (StringUtils::equals(tagName, "GPDKinematic")) {
//
//        //TODO quand est-ce que l'on fait la conversion des données utilisateurs vers les services ?
//        //TODO comment selectionne-t-on la bonne conversion xB to xi ?
//
//        double x = attributes.getDoubleValueOf("x");
//        double xi = GPDUtils::convertXBToXi(attributes.getDoubleValueOf("xB"));
//        double t = attributes.getDoubleValueOf("t");
//        double MuF = sqrt(attributes.getDoubleValueOf("MuF2"));
//        double MuR = sqrt(attributes.getDoubleValueOf("MuR2"));
//
//        GPDKinematic* pGPDKinematic = new GPDKinematic(x, xi, t, MuF, MuR);
//
//        m_scenario.addFunctionArg(tagName, static_cast<void*>(pGPDKinematic));
//    }
//
//    if (StringUtils::equals(tagName, "GPDModule")) {
//        std::string gpdModuleId = attributes.getStringValueOf("id");
//
//        GPDModule* pGPDModule = ModuleObjectFactory::newGPDModule(gpdModuleId);
//
//        m_scenario.addFunctionArg(tagName, static_cast<void*>(pGPDModule));
//    }
//}
