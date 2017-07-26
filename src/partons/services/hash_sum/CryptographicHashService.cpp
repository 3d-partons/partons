#include "../../../../include/partons/services/hash_sum/CryptographicHashService.h"

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/hash_sum/Qt4CryptographicHash.h"

namespace PARTONS {


// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int CryptographicHashService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new CryptographicHashService("CryptographicHashService"));

CryptographicHashService::CryptographicHashService(const std::string &className) :
        ServiceObject(className), m_pCryptographicHashI(0) {
    m_pCryptographicHashI = new Qt4CryptographicHash();
}

CryptographicHashService::~CryptographicHashService() {
    if (m_pCryptographicHashI) {
        delete m_pCryptographicHashI;
        m_pCryptographicHashI = 0;
    }
}

void CryptographicHashService::computeTask(Task& task) {
    //TODO implement
}

std::string CryptographicHashService::generateSHA1HashSum(
        const std::string& string) const {
    return m_pCryptographicHashI->generateSHA1HashSum(string);
}


} /* namespace PARTONS */
