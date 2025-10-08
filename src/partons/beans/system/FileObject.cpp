#include "../../../../include/partons/beans/system/FileObject.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {


FileObject::FileObject() :
        BaseObject("FileObject"), m_pCryptographicHashService(0), m_file(
                ElemUtils::StringUtils::EMPTY), m_hashSum(
                ElemUtils::StringUtils::EMPTY), m_filePath(
                ElemUtils::StringUtils::EMPTY) {
    m_pCryptographicHashService =
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService();
}

FileObject::FileObject(const std::string& className,
            const std::string& filePath,
            const std::string& hashSum, const std::string& file) :
    BaseObject(className), m_pCryptographicHashService(0), m_filePath(filePath), m_hashSum(hashSum), m_file(
            file) {
    m_pCryptographicHashService =
        Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService();
}

FileObject::FileObject(const FileObject &other) :
        BaseObject(other) {

    m_filePath = other.m_filePath;
    m_file = other.m_file;
    m_hashSum = other.m_hashSum;

    if (other.m_pCryptographicHashService) {
        m_pCryptographicHashService = other.m_pCryptographicHashService;
    } else {
        m_pCryptographicHashService = 0;
    }
}

FileObject::~FileObject() {
    // Nothing to do.
}

const std::string& FileObject::getFile() const {
    return m_file;
}

void FileObject::setFile(const std::string& file) {
    m_file = file;
}

const std::string& FileObject::getFilePath() const {
    return m_filePath;
}

void FileObject::setFilePath(const std::string& filePath) {
    m_filePath = filePath;
}

const std::string& FileObject::getHashSum() const {
    return m_hashSum;
}

void FileObject::setHashSum(const std::string& hashSum) {
    m_hashSum = hashSum;
}

} /* namespace PARTONS */
