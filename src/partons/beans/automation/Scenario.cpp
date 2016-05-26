#include "../../../../include/partons/beans/automation/Scenario.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../../include/partons/database/common/service/ScenarioDaoService.h"

Scenario::Scenario() :
        DatabaseFileObject("Scenario"), m_description(
                ElemUtils::StringUtils::EMPTY) {
}

Scenario::Scenario(const int indexId, const std::string &description,
        const time_t storeDate, const std::string& filePath,
        const std::string& hashSum, const std::string& file) :
        DatabaseFileObject("Scenario", indexId, storeDate, filePath, hashSum,
                file), m_description(description) {
}

Scenario::Scenario(const Scenario &other) :
        DatabaseFileObject(other) {
    m_description = other.m_description;
    m_tasks = other.m_tasks;
}

Scenario::~Scenario() {
}

const std::string& Scenario::getDescription() const {
    return m_description;
}

void Scenario::setDescription(const std::string& description) {
    m_description = description;
}

size_t Scenario::size() const {
    return m_tasks.size();
}

//TODO test range of i and throw exception
const Task& Scenario::getTask(unsigned int i) const {
    return m_tasks[i];
}

Task& Scenario::getTask(unsigned int i) {
    return m_tasks[i];
}

void Scenario::add(const Task& task) {
    m_tasks.push_back(task);

    // For the last inserted Task, update its Scenario pointers.
    m_tasks.back().setScenario(this);

    // For the last inserted Task, update its task index number.
    m_tasks.back().setScenarioTaskIndexNumber(this->size() - 1);
}

std::string Scenario::toString() const {
    ElemUtils::Formatter formatter;

    formatter << "[" << getClassName() << "]\n";
    formatter << DatabaseFileObject::toString() << "\n";

    formatter << "Description = " << m_description << '\n';

    for (size_t i = 0; i != size(); i++) {
        formatter << m_tasks[i].toString() << '\n';
    }

    return formatter.str();
}

const std::vector<Task>& Scenario::getTasks() const {
    return m_tasks;
}

void Scenario::setTasks(const std::vector<Task>& tasks) {
    m_tasks = tasks;
}

std::string Scenario::fillFile() const {
    ScenarioDaoService scenarioDaoService;
    return scenarioDaoService.getXMLFileByIndexId(getIndexId());
}

