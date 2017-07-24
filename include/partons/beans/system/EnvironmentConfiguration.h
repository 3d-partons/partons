#ifndef ENVIRONMENT_CONFIGURATION_H
#define ENVIRONMENT_CONFIGURATION_H

/**
 * @file EnvironmentConfiguration.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 09, 2016
 * @version 1.0
 */

#include <ctime>
#include <string>

#include "../../database/DatabaseFileObject.h"

namespace PARTONS {

/**
 * @class EnvironmentConfiguration
 *
 * @brief Class storing environment configuration information.
 *
 * This class is used to store the environment configuration information.
 * Such information can be set via a text file indicated by ```environment.configuration.file.path``` property in the main configuration file of PARTONS, namely ```partons.properties```.
 * It may look like that:
 \code
 Linux partons 3.2.0-4-amd64 #1 SMP Debian 3.2.73-2+deb7u2 x86_64 GNU/Linux
 g++ (Debian 4.7.2-5) 4.7.2
 GSL 1.16
 ROOT 5.34.32
 CLN 1.3.4
 SFML 2.3.2
 QT 4.8.3
 PARTONS Revision: 728
 NumA++ Revision: 728
 \endcode
 * When a computation is done via xml scenario and its result is stored in the database, the environment configuration information is associated to it.
 * This allows to check the environment configuration information for any result stored in the database.
 */
class EnvironmentConfiguration: public DatabaseFileObject {

public:

    /**
     * Default constructor.
     */
    EnvironmentConfiguration();

    /**
     * Assignment constructor.
     * @param indexId Unique id used when object is created from the database, see BaseObject::m_indexId.
     * @param storeDate Time of insertion into the database.
     * @param configuration String storing the configuration file.
     * @param hashSum Hash sum of file content.
     */
    EnvironmentConfiguration(int indexId, time_t storeDate,
            const std::string &configuration, const std::string &hashSum);

    /**
     * Destructor.
     */
    virtual ~EnvironmentConfiguration();

protected:

    virtual std::string fillFile() const;
};

} /* namespace PARTONS */

#endif /* ENVIRONMENT_CONFIGURATION_H */
