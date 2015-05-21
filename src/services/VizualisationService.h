#ifndef VIZUALISATION_SERVICE_H
#define VIZUALISATION_SERVICE_H

/**
 * @file VizualisationService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 06 May 2015
 * @version 1.0
 *
 * @class VizualisationService
 *
 * @brief
 */

#include <string>

#include "Service.h"

class Plot2DList;

class VizualisationService: public Service {
public:
    static const std::string ID; ///< Unique ID to self-register in the registry

    /**
     * A static function that provides a unique pointer of this class
     *
     * @return A unique pointer of this class
     */
    static VizualisationService* getInstance();

    /**
     * Default destructor
     */
    virtual ~VizualisationService();

    virtual void computeScenario(Scenario scenario);

    Plot2DList getplot2DFromSQLQuery(const std::string &sqlQuery);

private:
    static VizualisationService* pInstance; ///< Private pointer of this class for a unique instance

    /**
     * Private default constructor for a unique instance of this class
     */
    VizualisationService();
};

#endif /* VIZUALISATION_SERVICE_H */
