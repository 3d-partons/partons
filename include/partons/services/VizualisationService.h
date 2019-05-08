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
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class VizualisationService
 *
 * @brief Visualization service (incomplete)
 */
class VizualisationService: public ServiceObject {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor.
     */
    VizualisationService(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~VizualisationService();

    virtual void computeTask(Task &task);

private:

    /**
     * Get 2D plot list from SQL query.
     */
    Plot2DList getPlot2DFromSQLQuery(const std::string &sqlQuery);
};

} /* namespace PARTONS */

#endif /* VIZUALISATION_SERVICE_H */
