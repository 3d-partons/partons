#include "../../../include/partons/beans/KinematicUtils.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>
#include <iostream>
#include <stdexcept>
#include <vector>

List<ObservableKinematic> KinematicUtils::getObservableKinematicFromFile(
        const std::string& filePath) {

    List<ObservableKinematic> observableKinematicList;

    if (ElemUtils::FileUtils::isReadable(filePath)) {

        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        if (kinematicString.empty()) {
            throw std::runtime_error(
                    ElemUtils::Formatter()
                            << "(KinematicUtils::getObservableKinematicFromFile) Empty kinematic input file : "
                            << filePath);
        }

        for (size_t i = 0; i != kinematicString.size(); i++) {
            std::vector<std::string> kinematicValues =
                    ElemUtils::StringUtils::split(kinematicString[i], '|');
            if (kinematicValues.size() < 4) {
                throw std::runtime_error(
                        ElemUtils::Formatter()
                                << "(KinematicUtils::getObservableKinematicFromFile) Missing column value in your kinematic input file : "
                                << filePath
                                << " ; You must provided 4 column : xB | t | Q2 | phi");
            }

            observableKinematicList.add(
                    ObservableKinematic(kinematicValues[0], kinematicValues[1],
                            kinematicValues[2], kinematicValues[3]));

        }
    } else {
        //TODO print error with logger : cannot open file;

        std::cerr << "Cannot open file : " << filePath << std::endl;
    }

    return observableKinematicList;
}

List<GPDKinematic> KinematicUtils::getGPDKinematicFromFile(
        const std::string& filePath) {
    List<GPDKinematic> gpdKinematicList;

    if (ElemUtils::FileUtils::isReadable(filePath)) {

        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        if (kinematicString.empty()) {
            throw std::runtime_error(
                    ElemUtils::Formatter()
                            << "(KinematicUtils::getObservableKinematicFromFile) Empty kinematic input file : "
                            << filePath);
        }

        for (size_t i = 0; i != kinematicString.size(); i++) {
            std::vector<std::string> kinematicValues =
                    ElemUtils::StringUtils::split(kinematicString[i], '|');
            if (kinematicValues.size() < 5) {
                throw std::runtime_error(
                        ElemUtils::Formatter()
                                << "(KinematicUtils::getGPDKinematicFromFile) Missing column value in your kinematic input file : "
                                << filePath
                                << " ; You must provided 5 column : x | xi | t | MuF2 | MuR2");
            }

            gpdKinematicList.add(
                    GPDKinematic(kinematicValues[0], kinematicValues[1],
                            kinematicValues[2], kinematicValues[3],
                            kinematicValues[4]));

        }
    } else {
        //TODO print error with logger : cannot open file;

        std::cerr << "Cannot open file : " << filePath << std::endl;
    }

    return gpdKinematicList;
}
