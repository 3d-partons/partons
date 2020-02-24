#ifndef COMPARISON_SERVICE_H
#define COMPARISON_SERVICE_H

#include <string>

#include "../ServiceObject.h"

namespace PARTONS {

/**
 * @class ComparisonService
 *
 * @brief Comparison service (incomplete)
 */
class ComparisonService: public ServiceObject {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor.
     */
    ComparisonService(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ComparisonService();

    virtual void computeTask(Task &task);
};

} /* namespace PARTONS */

#endif /* COMPARISON_SERVICE_H */
