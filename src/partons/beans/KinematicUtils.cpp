#include "../../../include/partons/beans/KinematicUtils.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>

#include "../../../include/partons/beans/MesonPolarization.h"
#include "../../../include/partons/beans/MesonType.h"
#include "../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

KinematicUtils::KinematicUtils() :
        BaseObject("KinematicUtils") {
}

KinematicUtils::~KinematicUtils() {
}

List<GPDKinematic> KinematicUtils::getGPDKinematicFromFile(
        const std::string& filePath) {

    //result
    List<GPDKinematic> kinematicList;

    //check if readable
    if (ElemUtils::FileUtils::isReadable(filePath)) {

        //get file (by line)
        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        //check if not empty
        checkEmptyInputFile(__func__, kinematicString, filePath);

        //units
        std::vector<PhysicalUnit> kinematicUnits;

        //values
        std::vector<std::string> kinematicValues;

        //single result
        GPDKinematic kinematic;

        //loop over lines
        for (size_t i = 0; i != kinematicString.size(); i++) {

            //process if line is not empty.
            if (!kinematicString[i].empty()) {

                //trim
                ElemUtils::StringUtils::trimAll(kinematicString[i]);

                //if first check if with units
                if (i == 0 && kinematicString[i].at(0) == '#') {

                    //replace
                    ElemUtils::StringUtils::trimAll(kinematicString[i], "#");

                    //get units
                    kinematicUnits = getUnitsFromInputFileLine(__func__,
                            kinematicString[i]);

                    //check size
                    if (kinematicUnits.size() != 5) {
                        error(__func__,
                                ElemUtils::Formatter() << "Line " << i
                                        << " (units). Missing column value in your kinematic input file : "
                                        << filePath
                                        << " ; You must provided 5 column : x | xi | t | MuF2 | MuR2");

                    }

                    //continue
                    continue;
                }

                //split
                kinematicValues = ElemUtils::StringUtils::split(
                        kinematicString[i], '|');

                //check size
                if (kinematicValues.size() != 5) {

                    error(__func__,
                            ElemUtils::Formatter() << "Line " << i
                                    << ". Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 5 column : x | xi | t | MuF2 | MuR2");

                }

                //default units
                if (kinematicUnits.size() == 0) {

                    kinematicUnits.push_back(PhysicalUnit::NONE);
                    kinematicUnits.push_back(PhysicalUnit::NONE);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                }

                //single
                kinematic = GPDKinematic(
                        PhysicalType<double>(kinematicValues[0],
                                kinematicUnits[0]),
                        PhysicalType<double>(kinematicValues[1],
                                kinematicUnits[1]),
                        PhysicalType<double>(kinematicValues[2],
                                kinematicUnits[2]),
                        PhysicalType<double>(kinematicValues[3],
                                kinematicUnits[3]),
                        PhysicalType<double>(kinematicValues[4],
                                kinematicUnits[4]));

                kinematic.setIndexId(kinematicList.size());

                //add
                kinematicList.add(kinematic);
            }
        }
    } else {
        errorCannotOpenFile(__func__, filePath);
    }

    return kinematicList;
}

List<DVCSConvolCoeffFunctionKinematic> KinematicUtils::getDVCSCCFKinematicFromFile(
        const std::string& filePath) {

    //result
    List<DVCSConvolCoeffFunctionKinematic> kinematicList;

    //check if readable
    if (ElemUtils::FileUtils::isReadable(filePath)) {

        //get file (by line)
        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        //check if not empty
        checkEmptyInputFile(__func__, kinematicString, filePath);

        //units
        std::vector<PhysicalUnit> kinematicUnits;

        //values
        std::vector<std::string> kinematicValues;

        //single result
        DVCSConvolCoeffFunctionKinematic kinematic;

        //loop over lines
        for (size_t i = 0; i != kinematicString.size(); i++) {

            //process if line is not empty.
            if (!kinematicString[i].empty()) {

                //trim
                ElemUtils::StringUtils::trimAll(kinematicString[i]);

                //if first check if with units
                if (i == 0 && kinematicString[i].at(0) == '#') {

                    //replace
                    ElemUtils::StringUtils::trimAll(kinematicString[i], "#");

                    //get units
                    kinematicUnits = getUnitsFromInputFileLine(__func__,
                            kinematicString[i]);

                    //check size
                    if (kinematicUnits.size() != 5) {
                        error(__func__,
                                ElemUtils::Formatter() << "Line " << i
                                        << " (units). Missing column value in your kinematic input file : "
                                        << filePath
                                        << " ; You must provided 5 column : xi | t | Q2 | MuF2 | MuR2");
                    }

                    //continue
                    continue;
                }

                //split
                kinematicValues = ElemUtils::StringUtils::split(
                        kinematicString[i], '|');

                //check size
                if (kinematicValues.size() != 5) {
                    error(__func__,
                            ElemUtils::Formatter() << "Line " << i
                                    << ". Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 5 column : xi | t | Q2 | MuF2 | MuR2");
                }

                //default
                if (kinematicUnits.size() == 0) {

                    kinematicUnits.push_back(PhysicalUnit::NONE);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                }

                //single
                kinematic = DVCSConvolCoeffFunctionKinematic(
                        PhysicalType<double>(kinematicValues[0],
                                kinematicUnits[0]),
                        PhysicalType<double>(kinematicValues[1],
                                kinematicUnits[1]),
                        PhysicalType<double>(kinematicValues[2],
                                kinematicUnits[2]),
                        PhysicalType<double>(kinematicValues[3],
                                kinematicUnits[3]),
                        PhysicalType<double>(kinematicValues[4],
                                kinematicUnits[4]));

                kinematic.setIndexId(kinematicList.size());

                //add
                kinematicList.add(kinematic);
            }
        }
    } else {
        errorCannotOpenFile(__func__, filePath);
    }

    return kinematicList;
}

List<TCSConvolCoeffFunctionKinematic> KinematicUtils::getTCSCCFKinematicFromFile(
        const std::string& filePath) {

    //result
    List<TCSConvolCoeffFunctionKinematic> kinematicList;

    //check if readable
    if (ElemUtils::FileUtils::isReadable(filePath)) {

        //get file (by line)
        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        //check if not empty
        checkEmptyInputFile(__func__, kinematicString, filePath);

        //units
        std::vector<PhysicalUnit> kinematicUnits;

        //values
        std::vector<std::string> kinematicValues;

        //single result
        TCSConvolCoeffFunctionKinematic kinematic;

        //loop over lines
        for (size_t i = 0; i != kinematicString.size(); i++) {

            //process if line is not empty.
            if (!kinematicString[i].empty()) {

                //trim
                ElemUtils::StringUtils::trimAll(kinematicString[i]);

                //if first check if with units
                if (i == 0 && kinematicString[i].at(0) == '#') {

                    //replace
                    ElemUtils::StringUtils::trimAll(kinematicString[i], "#");

                    //get units
                    kinematicUnits = getUnitsFromInputFileLine(__func__,
                            kinematicString[i]);

                    //check size
                    if (kinematicUnits.size() != 5) {
                        error(__func__,
                                ElemUtils::Formatter() << "Line " << i
                                        << " (units). Missing column value in your kinematic input file : "
                                        << filePath
                                        << " ; You must provided 5 column : xi | t | Q2' | MuF2 | MuR2");
                    }

                    //continue
                    continue;
                }

                //split
                kinematicValues = ElemUtils::StringUtils::split(
                        kinematicString[i], '|');

                //check size
                if (kinematicValues.size() != 5) {
                    error(__func__,
                            ElemUtils::Formatter() << "Line " << i
                                    << ". Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 5 column : xi | t | Q2' | MuF2 | MuR2");
                }

                //default
                if (kinematicUnits.size() == 0) {

                    kinematicUnits.push_back(PhysicalUnit::NONE);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                }

                //single
                kinematic = TCSConvolCoeffFunctionKinematic(
                        PhysicalType<double>(kinematicValues[0],
                                kinematicUnits[0]),
                        PhysicalType<double>(kinematicValues[1],
                                kinematicUnits[1]),
                        PhysicalType<double>(kinematicValues[2],
                                kinematicUnits[2]),
                        PhysicalType<double>(kinematicValues[3],
                                kinematicUnits[3]),
                        PhysicalType<double>(kinematicValues[4],
                                kinematicUnits[4]));

                kinematic.setIndexId(kinematicList.size());

                //add
                kinematicList.add(kinematic);
            }
        }
    } else {
        errorCannotOpenFile(__func__, filePath);
    }

    return kinematicList;
}

List<DVMPConvolCoeffFunctionKinematic> KinematicUtils::getDVMPCCFKinematicFromFile(
        const std::string& filePath) {

    //result
    List<DVMPConvolCoeffFunctionKinematic> kinematicList;

    //check if readable
    if (ElemUtils::FileUtils::isReadable(filePath)) {

        //get file (by line)
        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        //check if not empty
        checkEmptyInputFile(__func__, kinematicString, filePath);

        //units
        std::vector<PhysicalUnit> kinematicUnits;

        //values
        std::vector<std::string> kinematicValues;

        //single result
        DVMPConvolCoeffFunctionKinematic kinematic;

        //loop over lines
        for (size_t i = 0; i != kinematicString.size(); i++) {

            //process if line is not empty.
            if (!kinematicString[i].empty()) {

                //trim
                ElemUtils::StringUtils::trimAll(kinematicString[i]);

                //if first check if with units
                if (i == 0 && kinematicString[i].at(0) == '#') {

                    //replace
                    ElemUtils::StringUtils::trimAll(kinematicString[i], "#");

                    //get units
                    kinematicUnits = getUnitsFromInputFileLine(__func__,
                            kinematicString[i]);

                    //check size
                    if (kinematicUnits.size() != 7) {
                        error(__func__,
                                ElemUtils::Formatter() << "Line " << i
                                        << " (units). Missing column value in your kinematic input file : "
                                        << filePath
                                        << " ; You must provided 7 column : xi | t | Q2 | MuF2 | MuR2 | mesonType | mesonPolarization");
                    }

                    //continue
                    continue;

                }

                //split
                kinematicValues = ElemUtils::StringUtils::split(
                        kinematicString[i], '|');

                //check size
                if (kinematicValues.size() != 7) {
                    error(__func__,
                            ElemUtils::Formatter() << "Line " << i
                                    << ". Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 7 column : xi | t | Q2 | MuF2 | MuR2 | mesonType | mesonPolarization");
                }

                //default
                if (kinematicUnits.size() == 0) {

                    kinematicUnits.push_back(PhysicalUnit::NONE);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::UNDEFINED);
                    kinematicUnits.push_back(PhysicalUnit::UNDEFINED);
                }

                //single
                kinematic = DVMPConvolCoeffFunctionKinematic(
                        PhysicalType<double>(kinematicValues[0],
                                kinematicUnits[0]),
                        PhysicalType<double>(kinematicValues[1],
                                kinematicUnits[1]),
                        PhysicalType<double>(kinematicValues[2],
                                kinematicUnits[2]),
                        PhysicalType<double>(kinematicValues[3],
                                kinematicUnits[3]),
                        PhysicalType<double>(kinematicValues[4],
                                kinematicUnits[4]),
                        MesonType::fromString(kinematicValues[5]),
                        MesonPolarization::fromString(kinematicValues[6]));

                kinematic.setIndexId(kinematicList.size());

                //add
                kinematicList.add(kinematic);
            }
        }
    } else {
        errorCannotOpenFile(__func__, filePath);
    }

    return kinematicList;
}

List<DVCSObservableKinematic> KinematicUtils::getDVCSObservableKinematicFromFile(
        const std::string& filePath) {

    //result
    List<DVCSObservableKinematic> kinematicList;

    //check if readable
    if (ElemUtils::FileUtils::isReadable(filePath)) {

        //get file (by line)
        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        //check if not empty
        checkEmptyInputFile(__func__, kinematicString, filePath);

        //units
        std::vector<PhysicalUnit> kinematicUnits;

        //values
        std::vector<std::string> kinematicValues;

        //single result
        DVCSObservableKinematic kinematic;

        //loop over lines
        for (size_t i = 0; i != kinematicString.size(); i++) {

            //process if line is not empty.
            if (!kinematicString[i].empty()) {

                //trim
                ElemUtils::StringUtils::trimAll(kinematicString[i]);

                //if first check if with units
                if (i == 0 && kinematicString[i].at(0) == '#') {

                    //replace
                    ElemUtils::StringUtils::trimAll(kinematicString[i], "#");

                    //get units
                    kinematicUnits = getUnitsFromInputFileLine(__func__,
                            kinematicString[i]);

                    //check size
                    if (kinematicUnits.size() != 5) {
                        error(__func__,
                                ElemUtils::Formatter() << "Line " << i
                                        << " (units). Missing column value in your kinematic input file : "
                                        << filePath
                                        << " ; You must provided 5 column : xB | t | Q2 | E | phi");

                    }

                    //continue
                    continue;
                }

                //split
                kinematicValues = ElemUtils::StringUtils::split(
                        kinematicString[i], '|');

                //check size
                if (kinematicValues.size() != 5) {
                    error(__func__,
                            ElemUtils::Formatter() << "Line " << i
                                    << ". Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 5 column : xB | t | Q2 | E | phi");

                }

                //default
                if (kinematicUnits.size() == 0) {

                    kinematicUnits.push_back(PhysicalUnit::NONE);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV);
                    kinematicUnits.push_back(PhysicalUnit::DEG);
                }

                //single
                kinematic = DVCSObservableKinematic(
                        PhysicalType<double>(kinematicValues[0],
                                kinematicUnits[0]),
                        PhysicalType<double>(kinematicValues[1],
                                kinematicUnits[1]),
                        PhysicalType<double>(kinematicValues[2],
                                kinematicUnits[2]),
                        PhysicalType<double>(kinematicValues[3],
                                kinematicUnits[3]),
                        PhysicalType<double>(kinematicValues[4],
                                kinematicUnits[4]));

                kinematic.setIndexId(kinematicList.size());

                //add
                kinematicList.add(kinematic);
            }
        }
    } else {
        errorCannotOpenFile(__func__, filePath);
    }

    return kinematicList;
}

List<TCSObservableKinematic> KinematicUtils::getTCSObservableKinematicFromFile(
        const std::string& filePath) {

    //result
    List<TCSObservableKinematic> kinematicList;

    //check if readable
    if (ElemUtils::FileUtils::isReadable(filePath)) {

        //get file (by line)
        std::vector<std::string> kinematicString =
                ElemUtils::FileUtils::readByLine(filePath);

        //check if not empty
        checkEmptyInputFile(__func__, kinematicString, filePath);

        //units
        std::vector<PhysicalUnit> kinematicUnits;

        //values
        std::vector<std::string> kinematicValues;

        //single result
        TCSObservableKinematic kinematic;

        //loop over lines
        for (size_t i = 0; i != kinematicString.size(); i++) {

            //process if line is not empty.
            if (!kinematicString[i].empty()) {

                //trim
                ElemUtils::StringUtils::trimAll(kinematicString[i]);

                //if first check if with units
                if (i == 0 && kinematicString[i].at(0) == '#') {

                    //replace
                    ElemUtils::StringUtils::trimAll(kinematicString[i], "#");

                    //get units
                    kinematicUnits = getUnitsFromInputFileLine(__func__,
                            kinematicString[i]);

                    //check size
                    if (kinematicUnits.size() != 5) {
                        error(__func__,
                                ElemUtils::Formatter() << "Line " << i
                                        << " (units). Missing column value in your kinematic input file : "
                                        << filePath
                                        << " ; You must provided 5 column : t | Q2' | E | phi | theta");

                    }

                    //continue
                    continue;
                }

                //split
                kinematicValues = ElemUtils::StringUtils::split(
                        kinematicString[i], '|');

                //check size
                if (kinematicValues.size() != 5) {
                    error(__func__,
                            ElemUtils::Formatter() << "Line " << i
                                    << ". Missing column value in your kinematic input file : "
                                    << filePath
                                    << " ; You must provided 5 column : t | Q2' | E | phi | theta");

                }

                //default
                if (kinematicUnits.size() == 0) {

                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV2);
                    kinematicUnits.push_back(PhysicalUnit::GEV);
                    kinematicUnits.push_back(PhysicalUnit::DEG);
                    kinematicUnits.push_back(PhysicalUnit::DEG);
                }

                //single
                kinematic = TCSObservableKinematic(
                        PhysicalType<double>(kinematicValues[0],
                                kinematicUnits[0]),
                        PhysicalType<double>(kinematicValues[1],
                                kinematicUnits[1]),
                        PhysicalType<double>(kinematicValues[2],
                                kinematicUnits[2]),
                        PhysicalType<double>(kinematicValues[3],
                                kinematicUnits[3]),
                        PhysicalType<double>(kinematicValues[4],
                                kinematicUnits[4]));

                kinematic.setIndexId(kinematicList.size());

                //add
                kinematicList.add(kinematic);
            }
        }
    } else {
        errorCannotOpenFile(__func__, filePath);
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

std::vector<PhysicalUnit> KinematicUtils::getUnitsFromInputFileLine(
        const std::string &funcName, const std::string &line) const {

    //result
    std::vector<PhysicalUnit> units;

    //split
    std::vector<std::string> unitStrings = ElemUtils::StringUtils::split(line,
            '|');

    //get results
    for (size_t i = 0; i < unitStrings.size(); i++) {
        units.push_back(PhysicalUnit(unitStrings.at(i)));
    }

    return units;
}

} /* namespace PARTONS */
