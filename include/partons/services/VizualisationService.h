#ifndef VIZUALISATION_SERVICE_H
#define VIZUALISATION_SERVICE_H

/**
 * @file VizualisationService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date May 06, 2015
 * @version 1.0
 */

#include <string>

#include "../ServiceObject.h"

namespace PARTONS {

class Plot2DList;

/**
 * @class VizualisationService
 *
 * @brief
 */
class VizualisationService: public ServiceObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     *
     * @param className
     */
    VizualisationService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~VizualisationService();

    virtual void computeTask(Task &task);

    Plot2DList getplot2DFromSQLQuery(const std::string &sqlQuery);

private:
};

} /* namespace PARTONS */

#endif /* VIZUALISATION_SERVICE_H */
