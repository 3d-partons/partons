#include "../../../../include/partons/beans/system/EnvironmentConfiguration.h"

#include <ElementaryUtils/string_utils/MD5.h>
#include <iostream>

EnvironmentConfiguration::EnvironmentConfiguration() :
        DatabaseObject("EnvironmentConfiguration"), m_storeDate(time(0)), m_configuration(
                ""), m_md5("") {
}

EnvironmentConfiguration::EnvironmentConfiguration(
        const std::string &configuration, const std::string &md5) :
        DatabaseObject("EnvironmentConfiguration"), m_storeDate(time(0)), m_configuration(
                configuration), m_md5(md5) {
}

EnvironmentConfiguration::EnvironmentConfiguration(int indexId,
        time_t storeDate, const std::string& configuration,
        const std::string& md5) :
        DatabaseObject("EnvironmentConfiguration", indexId), m_storeDate(
                storeDate), m_configuration(configuration), m_md5(md5) {
}

EnvironmentConfiguration::~EnvironmentConfiguration() {

}

const std::string& EnvironmentConfiguration::getMd5() const {
    return m_md5;
}

void EnvironmentConfiguration::setMd5(const std::string& md5) {
    m_md5 = md5;
}

time_t EnvironmentConfiguration::getStoreDate() const {
    return m_storeDate;
}

void EnvironmentConfiguration::setStoreDate(time_t storeDate) {
    m_storeDate = storeDate;
}

std::string EnvironmentConfiguration::getConfiguration() const {
    ElemUtils::MD5 md5;
    std::cerr << md5.convertToBitsString(m_configuration) << std::endl;
    return m_configuration;
}

void EnvironmentConfiguration::setConfiguration(
        const std::string& configuration) {
    m_configuration = configuration;
}
