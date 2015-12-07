#include "../../../include/partons/beans/KinematicUtils.h"

#include <stddef.h>
#include <stdexcept>
#include <vector>

#include "../../../include/partons/utils/fileUtils/FileUtils.h"
#include "../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../include/partons/utils/stringUtils/StringUtils.h"

List<ObservableKinematic> KinematicUtils::getObservableKinematicFromFile(
        const std::string& filePath) {

    List<ObservableKinematic> observableKinematicList;

    std::vector<std::string> kinematicString = FileUtils::readByLine(filePath);

    if (kinematicString.empty()) {
        throw std::runtime_error(
                Formatter()
                        << "(KinematicUtils::getObservableKinematicFromFile) Empty kinematic input file : "
                        << filePath);
    }

    for (size_t i = 0; i != kinematicString.size(); i++) {
        std::vector<std::string> kinematicValues = StringUtils::split(
                kinematicString[i], '|');
        if (kinematicValues.size() < 4) {
            throw std::runtime_error(
                    Formatter()
                            << "(KinematicUtils::getObservableKinematicFromFile) Missing column value in your kinematic input file : "
                            << filePath
                            << " ; You must provided 4 column : xB | t | Q2 | phi");
        }

        observableKinematicList.add(
                ObservableKinematic(kinematicValues[0], kinematicValues[1],
                        kinematicValues[2], kinematicValues[3]));

    }

    return observableKinematicList;
}
