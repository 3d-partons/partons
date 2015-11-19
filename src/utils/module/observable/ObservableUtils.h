/**
 * @file ObservableUtils.h
 * @author: Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date 18 November 2015
 * @version 1.0
 *
 * @class ObservableUtils
 *
 * @brief Compare various observables within given tolerances.
 */

#ifndef OBSERVABLEUTILS_H_
#define OBSERVABLEUTILS_H_
class ObservableResult;
class ObservableResultList;
class ObservableResultListReport;
class ObservableResultReport;
class Tolerances;

class ObservableUtils {
public:
    static ObservableResultListReport compareObservableResultsLists(
            const ObservableResultList& lhsObservableResultList,
            const ObservableResultList& rhsObservableResultList,
            const Tolerances& tolerances);

    static ObservableResultReport compareObservableResults(
            const ObservableResult& lhsObservableResult,
            const ObservableResult& rhsObservableResult,
            const Tolerances& tolerances);
};

#endif /* OBSERVABLEUTILS_H_ */
