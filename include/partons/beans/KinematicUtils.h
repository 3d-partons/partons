/*
 * KinematicUtils.h
 *
 *  Created on: Nov 27, 2015
 *      Author: debian
 */

#ifndef KINEMATICUTILS_H_
#define KINEMATICUTILS_H_

#include <string>

#include "gpd/GPDKinematic.h"
#include "List.h"
#include "observable/ObservableKinematic.h"

class KinematicUtils {
public:
    static List<ObservableKinematic> getObservableKinematicFromFile(
            const std::string &filePath);

    static List<GPDKinematic> getGPDKinematicFromFile(
            const std::string &filePath);
};

#endif /* KINEMATICUTILS_H_ */
