#include "../../../../include/partons/beans/automation/ComputationConfiguration.h"

ComputationConfiguration::ComputationConfiguration() :
        DatabaseObject("ComputationConfiguration"), m_xmlFile(""), m_md5("") {
}

ComputationConfiguration::ComputationConfiguration(const std::string &xmlFile,
        const std::string &md5) :
        DatabaseObject("ComputationConfiguration"), m_xmlFile(xmlFile), m_md5(
                md5) {
}

ComputationConfiguration::ComputationConfiguration(int indexId,
        const std::string& xmlFile, const std::string& md5) :
        DatabaseObject("ComputationConfiguration", indexId), m_xmlFile(xmlFile), m_md5(
                md5) {
}

ComputationConfiguration::~ComputationConfiguration() {
}

const std::string& ComputationConfiguration::getXmlFile() const {
    return m_xmlFile;
}

void ComputationConfiguration::setXmlFile(const std::string& xmlFile) {
    m_xmlFile = xmlFile;
}

const std::string& ComputationConfiguration::getMd5() const {
    return m_md5;
}

void ComputationConfiguration::setMd5(const std::string& md5) {
    m_md5 = md5;
}
