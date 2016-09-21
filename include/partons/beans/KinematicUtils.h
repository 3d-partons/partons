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
#include <vector>

#include "convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "gpd/GPDKinematic.h"
#include "List.h"
#include "observable/ObservableKinematic.h"

class KinematicUtils: public BaseObject {
public:
    KinematicUtils();
    virtual ~KinematicUtils();

    List<ObservableKinematic> getObservableKinematicFromFile(
            const std::string &filePath);

    List<GPDKinematic> getGPDKinematicFromFile(const std::string &filePath);

    List<DVCSConvolCoeffFunctionKinematic> getCCFKinematicFromFile(
            const std::string &filePath);

private:
    void error(const std::string &funcName, const std::string &msg);
    void errorCannotOpenFile(const std::string &funcName,
            const std::string &msg);
    void checkEmptyInputFile(const std::string &funcName,
            const std::vector<std::string> &kinematicString,
            const std::string &filePath);
};

#endif /* KINEMATICUTILS_H_ */
