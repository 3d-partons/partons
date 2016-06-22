#ifndef KINEMATIC_UTILS_H
#define KINEMATIC_UTILS_H

/**
 * @file KinematicUtils.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 27, 2015
 * @version 1.0
 *
 * @class KinematicUtils
 *
 * @brief
 */

#include <string>

#include "convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "gpd/GPDKinematic.h"
#include "List.h"
#include "observable/ObservableKinematic.h"

class KinematicUtils {
public:
    static List<ObservableKinematic> getObservableKinematicFromFile(
            const std::string &filePath);

    static List<GPDKinematic> getGPDKinematicFromFile(
            const std::string &filePath);

    static List<DVCSConvolCoeffFunctionKinematic> getCCFKinematicFromFile(
            const std::string &filePath);
};

#endif /* KINEMATICUTILS_H_ */
