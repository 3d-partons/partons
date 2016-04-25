#ifndef CRYPTOGRAPHIC_HASH_I_H
#define CRYPTOGRAPHIC_HASH_I_H

/**
 * @file CryptographicHashI.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 24, 2016
 * @version 1.0
 *
 * @class CryptographicHashI
 *
 * @brief
 */

#include <string>

#include "../../BaseObject.h"

class CryptographicHashI: public BaseObject {
public:
    CryptographicHashI(const std::string &className);
    virtual ~CryptographicHashI();

    virtual std::string generateSHA1HashSum(
            const std::string &string) const = 0;
};

#endif /* CRYPTOGRAPHIC_HASH_I_H */
