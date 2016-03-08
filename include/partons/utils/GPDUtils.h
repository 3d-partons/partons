#include <NumA/utils/Interval.h>
#include <string>

#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDResult.h"
#include "../beans/List.h"
#include "../beans/ResultList.h"

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
            const ResultList<GPDResult> &gpdResultList);

    static ResultList<GPDResult> getResultListFromDatabase(int computationId);
};
