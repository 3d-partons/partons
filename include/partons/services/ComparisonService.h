#include <string>

#include "../ServiceObject.h"
#include "../utils/compare/ComparisonMode.h"
#include "../utils/compare/ComparisonReportList.h"
#include "../utils/test/ComparisonReport.h"

class ComparisonService: public ServiceObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    ComparisonService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ComparisonService();

    virtual void computeTask(Task &task);

    template<class T>
    ComparisonReport compare(const T &lhs, const T &rhs,
            const Tolerances &tolerances) const {
        return lhs.compare(rhs, tolerances);
    }

    template<class T>
    ComparisonReportList compareList(const T &objectToCompare,
            const T &referenceObject, const Tolerances &tolerances,
            const ComparisonMode &comparisonMode) const {
        return objectToCompare.compare(referenceObject, tolerances,
                comparisonMode);
    }

private:

};
