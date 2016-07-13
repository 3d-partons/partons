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

#include <include/partons/ServiceObject.h>
#include <string>

class Plot2DList;

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

#endif /* VIZUALISATION_SERVICE_H */
