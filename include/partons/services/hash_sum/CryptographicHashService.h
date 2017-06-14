#ifndef CRYPTOGRAPHIC_HASH_SERVICE_H
#define CRYPTOGRAPHIC_HASH_SERVICE_H

/**
 * @file CryptographicHashService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 24, 2016
 * @version 1.0
 */

#include <string>

#include "../../ServiceObject.h"

class CryptographicHashI;

/**
 * @class CryptographicHashService
 *
 * @brief Service for generation of hash values.
 *
 * This service is used to generate hash values for input strings of characters. The default generator is based on Qt4 library, see Qt4CryptographicHash class.
 * The usage of this service is demonstrated by the following example:
 \code{.cpp}
 //retrieve CryptographicHashService
 CryptographicHashService* pCryptographicHashService = Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService();

 //generate hash value for given string of characters
 std::string inputString = "the quick brown fox jumps over the lazy dog";
 std::string hashValue = pCryptographicHashService->generateSHA1HashSum(inputString);

 //print result
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "SHA1 hash sum for string '" << inputString << "' is " << hashValue);
 \endcode
 which gives via Logger:
 \code
 14-06-2017 09:54:08 [INFO] (example::main) SHA1 hash sum for string 'the quick brown fox jumps over the lazy dog' is 16312751ef9307c3fd1afbcb993cdc80464ba0f1
 \endcode
 */
class CryptographicHashService: public ServiceObject {

public:

	/**
	 * Unique ID to automatically register the class in the registry.
	 */
	static const unsigned int classId;

	/**
	 * Constructor.
	 * @param className Name of class.
	 */
	CryptographicHashService(const std::string &className);

	/**
	 * Destructor.
	 */
	virtual ~CryptographicHashService();

	virtual void computeTask(Task &task);

	/**
	 * Generate SHA-1 hash value for a given string of characters.
	 * @param string Input string.
	 */
	std::string generateSHA1HashSum(const std::string &string) const;

private:

	/**
	 * Pointer to object used to generate hash values.
	 */
	CryptographicHashI* m_pCryptographicHashI;
};

#endif /* CRYPTOGRAPHIC_HASH_SERVICE_H */
