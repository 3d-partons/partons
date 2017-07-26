#ifndef GPD_UTILS_H
#define GPD_UTILS_H

#include <NumA/utils/Interval.h>
#include <string>

#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDResult.h"
#include "../beans/List.h"

namespace PARTONS {

class GPDUtils {
public:
    static List<GPDKinematic> getKinematicListByIntervals(
            const NumA::Interval<double> &xInterval,
            const NumA::Interval<double> &xiInterval,
            const NumA::Interval<double> &tInterval,
            const NumA::Interval<double> &MuFInterval,
            const NumA::Interval<double> &MuRInterval);

    static List<GPDKinematic> getKinematicListFromFile(
            const std::string &filePath);

    static List<GPDKinematic> getKinematicListFromResultList(
            const List<GPDResult> &gpdResultList);

    static List<GPDResult> getResultListFromDatabase(int computationId);
};

} /* namespace PARTONS */

#endif /* GPD_UTILS_H */
