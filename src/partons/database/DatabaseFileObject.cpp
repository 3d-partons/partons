#include "../../../include/partons/database/DatabaseFileObject.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/Partons.h"
#include "../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../include/partons/ServiceObjectRegistry.h"

DatabaseFileObject::DatabaseFileObject(const std::string& className) :
        DatabaseObject(className), m_pCryptographicHashService(0), m_storeDate(
                time(0)), m_file(ElemUtils::StringUtils::EMPTY), m_hashSum(
                ElemUtils::StringUtils::EMPTY), m_filePath(
                ElemUtils::StringUtils::EMPTY) {
    setCryptographicHashService();
}

DatabaseFileObject::~DatabaseFileObject() {
}

void DatabaseFileObject::setCryptographicHashService() {
    m_pCryptographicHashService =
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService();
}

const std::string& DatabaseFileObject::getFile() const {
    if ((getIndexId() != -1) && (m_file.empty())) {
        m_file = fillFile();
    } else if ((getIndexId() == -1) && (m_file.empty())
            && (!m_filePath.empty())) {
        m_file = ElemUtils::FileUtils::read(m_filePath);
    }

    //TODO how to handle empty file ?

    return m_file;
}

void DatabaseFileObject::setFile(const std::string& file) {
    m_file = file;
}

//TODO better handle error
const std::string& DatabaseFileObject::getHashSum() const {
    if ((m_hashSum.empty()) && (!m_file.empty())) {

        m_hashSum = m_pCryptographicHashService->generateSHA1HashSum(m_file);
    } else {
        warn(__func__, "Cannot compute Hash Sum; m_file is empty.");
    }

    return m_hashSum;
}

void DatabaseFileObject::setHashSum(const std::string& hashSum) {
    m_hashSum = hashSum;
}

time_t DatabaseFileObject::getStoreDate() const {
    return m_storeDate;
}

void DatabaseFileObject::setStoreDate(time_t storeDate) {
    m_storeDate = storeDate;
}

const std::string& DatabaseFileObject::getFilePath() const {
    return m_filePath;
}

void DatabaseFileObject::setFilePath(const std::string& filePath) {
    m_filePath = filePath;
}
