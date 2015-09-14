#ifndef NF_FUNCTIONEXAMPLE_H
#define NF_FUNCTIONEXAMPLE_H

/**
 * @file NfFunctionExample.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 August 2015
 * @version 1.0
 *
 * @class NfFunctionExample
 *
 * @brief
 */

#include <string>

#include "../ActiveFlavorsModule.h"

class NfFunctionExample: public ActiveFlavorsModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    NfFunctionExample(const std::string &className);
    virtual ~NfFunctionExample();

    virtual NfFunctionExample* clone() const;

protected:
    /**
     * Copy constructor
     */
    NfFunctionExample(const NfFunctionExample &other);

};

#endif /* NF_FUNCTIONEXAMPLE_H */
