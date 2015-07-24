#include "DVCSConvolCoeffFunctionService.h"

#include "../beans/convolCoeffFunction/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../beans/convolCoeffFunction/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../BaseObjectRegistry.h"
#include "../modules/convolCoeffFunction/DVCS/DVCSConvolCoeffFunctionModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int DVCSConvolCoeffFunctionService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSConvolCoeffFunctionService(
                        "CoefficientFunctionService"));

DVCSConvolCoeffFunctionService::DVCSConvolCoeffFunctionService(
        const std::string &className) :
        ServiceObject(className) {

}

DVCSConvolCoeffFunctionService::~DVCSConvolCoeffFunctionService() {
}

//TODO implement
void DVCSConvolCoeffFunctionService::computeTask(const Task &task) {

}

//TODO implementer
DVCSConvolCoeffFunctionResult DVCSConvolCoeffFunctionService::computeWithGPDModel(
        DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
        GPDModule* _pGPDModule, DVCSConvolCoeffFunctionKinematic &kinematic,
        PerturbativeQCDOrderType::Type qcdOrderType, GPDType::Type gpdType) {

    // Configure cff module
    dvcsConvolCoeffFunctionModule->setGPDModule(_pGPDModule);
    dvcsConvolCoeffFunctionModule->setQCDOrderType(qcdOrderType);

    //TODO call with xi no xB ?
    DVCSConvolCoeffFunctionResult result =
            dvcsConvolCoeffFunctionModule->compute(kinematic.getXi(),
                    kinematic.getT(), kinematic.getQ2(), kinematic.getMuF2(),
                    kinematic.getMuR2(), gpdType);

    return result;
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
DVCSConvolCoeffFunctionResult DVCSConvolCoeffFunctionService::computeWithCFFModel(
        DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
        DVCSConvolCoeffFunctionKinematic &kinematic,
        PerturbativeQCDOrderType &qcdOrderType) {

    return DVCSConvolCoeffFunctionResult();
}
