#ifndef CRYPTOGRAPHIC_HASH_SERVICE_H
#define CRYPTOGRAPHIC_HASH_SERVICE_H

/**
 * @file CryptographicHashService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 24, 2016
 * @version 1.0
 *
 * @class CryptographicHashService
 *
 * @brief
 */

#include <string>

#include "../../ServiceObject.h"

class CryptographicHashI;

class CryptographicHashService: public ServiceObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    CryptographicHashService(const std::string &className);
    virtual ~CryptographicHashService();

    virtual void computeTask(Task &task);

    std::string generateSHA1HashSum(const std::string &string) const;

private:
    CryptographicHashI* m_pCryptographicHashI;
};

#endif /* CRYPTOGRAPHIC_HASH_SERVICE_H */
