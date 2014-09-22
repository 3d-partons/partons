#include "CFFService.h"

#include <stdexcept>

#include "CFFModule.h"
#include "utils/fileUtils/FileExceptionType.h"
#include "utils/fileUtils/FileUtils.h"
#include "utils/logger/LoggerManager.h"
#include "utils/stringUtils/Formatter.h"
#include "utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
CFFService* CFFService::m_pInstance = 0;

CFFService::CFFService() :
        BaseObject("CFFService") {

}

CFFService* CFFService::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new CFFService();
    }

    return m_pInstance;
}

CFFService::~CFFService() {
    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

//TODO implementer
std::pair<double, double> CFFService::compute(CFFModule* pCFFModule,
        CFFInputData* _pCFFInputData, GPDModule* _pGPDModule, double MuF,
        double MuR, QCDOrderType qcdOrderType) {

    // Configure cff module
    pCFFModule->setGPDModule(_pGPDModule);
    pCFFModule->setQCDOrderType(&qcdOrderType);

    return pCFFModule->compute(_pCFFInputData->getXB(), _pCFFInputData->getT(),
            _pCFFInputData->getQ2(), MuF, MuR, qcdOrderType);
}

//TODO implementer
std::vector<CFFOutputData> compute(std::vector<CFFInputData> ListOfCFFInputData,
        GPDModule* _pGPDModule, double MuF, double MuR,
        QCDOrderType qcdOrderType) {
    std::vector<CFFOutputData> results;

    return results;
}

std::map<unsigned int, CFFInputData> CFFService::getMapOfCFFInputDataFromFile(
        const std::string & filePath) {
    // define a map<beanId, CFFInputData> to handle kinematic variables
    std::map<unsigned int, CFFInputData> data;

    if (FileUtils::isReadable(filePath)) {
        // retrieve a vector of file's lines
        std::vector<std::string> fileLines = FileUtils::readByLine(filePath);
        std::vector<std::string> splitedLine;
        // for each entry
        for (unsigned int i = 0; i != fileLines.size(); i++) {
            // split string character with space character
            splitedLine = StringUtils::split(fileLines[i], ' ');
            // if there is 4 entry that's we have all parameters
            if (!splitedLine.empty() && splitedLine.size() == 4) {
                // store a new CFFInputData object
                data.insert(
                        std::pair<unsigned int, CFFInputData>(
                                StringUtils::fromStringToInt(splitedLine[0]),
                                CFFInputData(
                                        StringUtils::fromStringToInt(
                                                splitedLine[0]),
                                        StringUtils::fromStringToDouble(
                                                splitedLine[1]),
                                        StringUtils::fromStringToDouble(
                                                splitedLine[2]),
                                        StringUtils::fromStringToDouble(
                                                splitedLine[3]))));
            } else {
                //TODO missing arg
            }
        }
    } else {
        //TODO faire une classe FileException pour mettre en forme le message et utiliser le formatter

        m_pLoggerManager->error(getClassName(), __func__,
                "UNREADABLE file: " + filePath);

        throw std::runtime_error(
                Formatter()
                        << FileExceptionType(FileExceptionType::UNREADABLE).toString()
                        << " filePath: " << filePath); // implicitly cast to std::string
    }

    return data;
}

std::vector<CFFInputData> CFFService::getListOfCFFInputDataFromFile(
        const std::string & filePath) {
    // define a map<beanId, CFFInputData> to handle kinematic variables
    std::vector<CFFInputData> data;

    if (FileUtils::isReadable(filePath)) {
        // retrieve a vector of file's lines
        std::vector<std::string> fileLines = FileUtils::readByLine(filePath);
        std::vector<std::string> splitedLine;
        // for each entry
        for (unsigned int i = 0; i != fileLines.size(); i++) {
            // split string character with space character
            splitedLine = StringUtils::split(fileLines[i], ' ');
            // if there is 4 entry that's we have all parameters
            if (!splitedLine.empty() && splitedLine.size() == 4) {
                // store a new CFFInputData object
                data.push_back(

                        CFFInputData(
                                StringUtils::fromStringToInt(splitedLine[0]),
                                StringUtils::fromStringToDouble(splitedLine[1]),
                                StringUtils::fromStringToDouble(splitedLine[2]),
                                StringUtils::fromStringToDouble(
                                        splitedLine[3])));
            } else {
                //TODO missing arg
            }
        }
    } else {
        //TODO faire une classe FileException pour mettre en forme le message et utiliser le formatter

        m_pLoggerManager->error(getClassName(), __func__,
                "UNREADABLE file: " + filePath);

        throw std::runtime_error(
                Formatter()
                        << FileExceptionType(FileExceptionType::UNREADABLE).toString()
                        << " filePath: " << filePath); // implicitly cast to std::string
    }

    return data;
}
