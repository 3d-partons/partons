#ifndef QT4_CRYPTOGRAPHIC_HASH_H
#define QT4_CRYPTOGRAPHIC_HASH_H

/**
 * @file Qt4CryptographicHash.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 24, 2016
 * @version 1.0
 *
 * @class Qt4CryptographicHash
 *
 * @brief
 */

#include <string>

#include "CryptographicHashI.h"

class Qt4CryptographicHash: public CryptographicHashI {
public:
    Qt4CryptographicHash();
    virtual ~Qt4CryptographicHash();

    virtual std::string generateSHA1HashSum(const std::string &string) const;
};

#endif /* QT4_CRYPTOGRAPHIC_HASH_H */
