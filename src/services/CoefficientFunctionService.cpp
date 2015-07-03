#include "CoefficientFunctionService.h"

#include "../beans/cff/CFFInputData.h"
#include "../beans/cff/CFFOutputData.h"
#include "../beans/Scenario.h"
#include "../BaseObjectRegistry.h"
#include "../modules/CoefficientFunctionModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int CoefficientFunctionService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new CoefficientFunctionService("CoefficientFunctionService"));

CoefficientFunctionService::CoefficientFunctionService(
        const std::string &className) :
        ServiceObject(className) {

}

CoefficientFunctionService::~CoefficientFunctionService() {
}

//TODO implement
void CoefficientFunctionService::computeScenario(Scenario scenario) {

}

//TODO implementer
CFFOutputData CoefficientFunctionService::computeWithGPDModel(
        CoefficientFunctionModule* cffModule, GPDModule* _pGPDModule,
        CFFInputData &cffInputData, const double MuF, const double MuR,
        PerturbativeQCDOrderType::Type qcdOrderType,
        GPDType::Type gpdComputeType) {

    // Configure cff module
    cffModule->setGpdModule(_pGPDModule);
    cffModule->setQcdOrderType(qcdOrderType);

    CFFOutputData cffOutputData = cffModule->compute(cffInputData.getXB(),
            cffInputData.getT(), cffInputData.getQ2(), MuF, MuR,
            gpdComputeType);

    return cffOutputData;
}
//
////TODO implementer
//std::vector<CFFOutputData> compute(std::vector<CFFInputData> ListOfCFFInputData,
//        GPDModule* _pGPDModule, double MuF, double MuR,
//        QCDOrderType qcdOrderType) {
//    std::vector<CFFOutputData> results;
//
//    return results;
//}
//
//std::map<unsigned int, CFFInputData> CFFService::getMapOfCFFInputDataFromFile(
//        const std::string & filePath) {
//    // define a map<beanId, CFFInputData> to handle kinematic variables
//    std::map<unsigned int, CFFInputData> data;
//
//    if (FileUtils::isReadable(filePath)) {
//        // retrieve a vector of file's lines
//        std::vector<std::string> fileLines = FileUtils::readByLine(filePath);
//        std::vector<std::string> splitedLine;
//        // for each entry
//        for (unsigned int i = 0; i != fileLines.size(); i++) {
//            // split string character with space character
//            splitedLine = StringUtils::split(fileLines[i], ' ');
//            // if there is 4 entry that's we have all parameters
//            if (!splitedLine.empty() && splitedLine.size() == 4) {
//                // store a new CFFInputData object
//                data.insert(
//                        std::pair<unsigned int, CFFInputData>(
//                                StringUtils::fromStringToInt(splitedLine[0]),
//                                CFFInputData(
//                                        StringUtils::fromStringToInt(
//                                                splitedLine[0]),
//                                        StringUtils::fromStringToDouble(
//                                                splitedLine[1]),
//                                        StringUtils::fromStringToDouble(
//                                                splitedLine[2]),
//                                        StringUtils::fromStringToDouble(
//                                                splitedLine[3]))));
//            } else {
//                //TODO missing arg
//            }
//        }
//    } else {
//        //TODO faire une classe FileException pour mettre en forme le message et utiliser le formatter
//
//        m_pLoggerManager->error(getClassName(), __func__,
//                "UNREADABLE file: " + filePath);
//
//        throw std::runtime_error(
//                Formatter()
//                        << FileExceptionType(FileExceptionType::UNREADABLE).toString()
//                        << " filePath: " << filePath); // implicitly cast to std::string
//    }
//
//    return data;
//}
//
//std::vector<CFFInputData> CFFService::getListOfCFFInputDataFromFile(
//        const std::string & filePath) {
//    // define a map<beanId, CFFInputData> to handle kinematic variables
//    std::vector<CFFInputData> data;
//
//    if (FileUtils::isReadable(filePath)) {
//        // retrieve a vector of file's lines
//        std::vector<std::string> fileLines = FileUtils::readByLine(filePath);
//        std::vector<std::string> splitedLine;
//        // for each entry
//        for (unsigned int i = 0; i != fileLines.size(); i++) {
//            // split string character with space character
//            splitedLine = StringUtils::split(fileLines[i], ' ');
//            // if there is 4 entry that's we have all parameters
//            if (!splitedLine.empty() && splitedLine.size() == 4) {
//                // store a new CFFInputData object
//                data.push_back(
//
//                        CFFInputData(
//                                StringUtils::fromStringToInt(splitedLine[0]),
//                                StringUtils::fromStringToDouble(splitedLine[1]),
//                                StringUtils::fromStringToDouble(splitedLine[2]),
//                                StringUtils::fromStringToDouble(
//                                        splitedLine[3])));
//            } else {
//                //TODO missing arg
//            }
//        }
//    } else {
//        //TODO faire une classe FileException pour mettre en forme le message et utiliser le formatter
//
//        m_pLoggerManager->error(getClassName(), __func__,
//                "UNREADABLE file: " + filePath);
//
//        throw std::runtime_error(
//                Formatter()
//                        << FileExceptionType(FileExceptionType::UNREADABLE).toString()
//                        << " filePath: " << filePath); // implicitly cast to std::string
//    }
//
//    return data;
//}

//TODO implement
CFFOutputData CoefficientFunctionService::computeWithCFFModel(
        CoefficientFunctionModule* cffModule, CFFInputData &cffInputData,
        const double MuF, const double MuR,
        PerturbativeQCDOrderType &qcdOrderType) {

    return CFFOutputData();
}
