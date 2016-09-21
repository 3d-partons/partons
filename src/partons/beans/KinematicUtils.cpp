#include "../../../include/partons/beans/KinematicUtils.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>

KinematicUtils::KinematicUtils() :
        BaseObject("KinematicUtils") {
}

KinematicUtils::~KinematicUtils() {
}

List<ObservableKinematic> KinematicUtils::getObservableKinematicFromFile(
        const std::string& filePath) {

    List<ObservableKinematic> observableKinematicList;

    if (ElemUtils::FileUtils::isReadable(filePath)) {

        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        //TODO est-ce que le vector à une taille > 0 s'il y a un retour chariot dans le fichier et que celui-ci est vide ?
        checkEmptyInputFile("getObservableKinematicFromFile", kinematicString,
                filePath);

        ObservableKinematic kinematic;

        for (size_t i = 0; i != kinematicString.size(); i++) {

            // process if line is not empty.
            if (!kinematicString[i].empty()) {

                std::vector<std::string> kinematicValues =
                        ElemUtils::StringUtils::split(kinematicString[i], '|');
                if (kinematicValues.size() < 4) {

                    error("getObservableKinematicFromFile",
                            ElemUtils::Formatter()
                                    << "Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 4 column : xB | t | Q2 | phi");
                }

                kinematic = ObservableKinematic(kinematicValues[0],
                        kinematicValues[1], kinematicValues[2],
                        kinematicValues[3]);
                kinematic.setIndexId(observableKinematicList.size());

                observableKinematicList.add(kinematic);
            }

        }
    } else {
        errorCannotOpenFile("getObservableKinematicFromFile", filePath);
    }

    return observableKinematicList;
}

List<GPDKinematic> KinematicUtils::getGPDKinematicFromFile(
        const std::string& filePath) {
    List<GPDKinematic> gpdKinematicList;

    if (ElemUtils::FileUtils::isReadable(filePath)) {

        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        checkEmptyInputFile("getGPDKinematicFromFile", kinematicString,
                filePath);

        GPDKinematic kinematic;

        for (size_t i = 0; i != kinematicString.size(); i++) {

            // process if line is not empty.
            if (!kinematicString[i].empty()) {

                std::vector<std::string> kinematicValues =
                        ElemUtils::StringUtils::split(kinematicString[i], '|');
                if (kinematicValues.size() < 5) {
                    error("getGPDKinematicFromFile",
                            ElemUtils::Formatter() << "Line " << i
                                    << ". Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 5 column : x | xi | t | MuF2 | MuR2");
                }

                kinematic = GPDKinematic(kinematicValues[0], kinematicValues[1],
                        kinematicValues[2], kinematicValues[3],
                        kinematicValues[4]);
                kinematic.setIndexId(gpdKinematicList.size());

                gpdKinematicList.add(kinematic);
            }

        }
    } else {
        errorCannotOpenFile("getGPDKinematicFromFile", filePath);
    }

    return gpdKinematicList;
}

List<DVCSConvolCoeffFunctionKinematic> KinematicUtils::getCCFKinematicFromFile(
        const std::string& filePath) {
    List<DVCSConvolCoeffFunctionKinematic> kinematicList;

    if (ElemUtils::FileUtils::isReadable(filePath)) {

        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        checkEmptyInputFile("getCCFKinematicFromFile", kinematicString,
                filePath);

        DVCSConvolCoeffFunctionKinematic kinematic;

        for (size_t i = 0; i != kinematicString.size(); i++) {

            // process if line is not empty.
            if (!kinematicString[i].empty()) {

                std::vector<std::string> kinematicValues =
                        ElemUtils::StringUtils::split(kinematicString[i], '|');
                if (kinematicValues.size() < 5) {

                    error("getCCFKinematicFromFile",
                            ElemUtils::Formatter() << "Line " << i
                                    << ". Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 5 column : xi | t | Q2 | MuF2 | MuR2");
                }

                kinematic = DVCSConvolCoeffFunctionKinematic(kinematicValues[0],
                        kinematicValues[1], kinematicValues[2],
                        kinematicValues[3], kinematicValues[4]);
                kinematic.setIndexId(kinematicList.size());

                kinematicList.add(kinematic);
            }

        }
    } else {
        errorCannotOpenFile("getCCFKinematicFromFile", filePath);
    }

    return kinematicList;
}

void KinematicUtils::error(const std::string &funcName,
        const std::string &msg) {
    throw ElemUtils::CustomException(getClassName(), __func__, msg);
}

void KinematicUtils::errorCannotOpenFile(const std::string &funcName,
        const std::string &msg) {
    error(funcName, ElemUtils::Formatter() << "Cannot open file " << msg);
}

void KinematicUtils::checkEmptyInputFile(const std::string &funcName,
        const std::vector<std::string> &kinematicString,
        const std::string &filePath) {

    if (kinematicString.empty()) {
        error(__func__,
                ElemUtils::Formatter() << "Empty kinematic input file : "
                        << filePath);
    }
}
