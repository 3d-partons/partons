#include "../../../include/partons/services/ConvolCoeffFunctionService.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>

#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionResultDaoService.h"
#include "../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

const std::string ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL =
        "computeWithGPDModel";
const std::string ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL =
        "computeListWithGPDModel";

// Initialise [class]::classId with a unique name.
const unsigned int ConvolCoeffFunctionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new ConvolCoeffFunctionService(
                        "ConvolCoeffFunctionService"));

ConvolCoeffFunctionService::ConvolCoeffFunctionService(
        const std::string &className) :
        ServiceObject(className) {
}

ConvolCoeffFunctionService::~ConvolCoeffFunctionService() {
}

//TODO implement
void ConvolCoeffFunctionService::computeTask(Task &task) {

    ResultList<DVCSConvolCoeffFunctionResult> resultList;

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL)) {
        resultList.add(computeWithGPDModelTask(task));

    } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL)) {
        //TODO implement
        // resultList = computeListWithGPDModelTask(task);
    } else {
        error(__func__, "unknown function name = " + task.getFunctionName());
    }

    if (task.isStoreInDB()) {
        ConvolCoeffFunctionResultDaoService convolCoeffFunctionResultDaoService;
        int computationId = convolCoeffFunctionResultDaoService.insert(
                resultList);

        if (computationId != -1) {
            info(__func__,
                    ElemUtils::Formatter()
                            << "DVCSConvolCoeffFunctionResultList object has been stored in database with computation_id = "
                            << computationId);
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "DVCSConvolCoeffFunctionResultList object : insertion into database failed");
        }
    }
}

//TODO implementer
DVCSConvolCoeffFunctionResult ConvolCoeffFunctionService::computeWithGPDModel(
        const DVCSConvolCoeffFunctionKinematic &kinematic,
        ConvolCoeffFunctionModule* convolCoeffFunctionModule,
        GPDType::Type gpdType) const {

    DVCSConvolCoeffFunctionResult result = convolCoeffFunctionModule->compute(
            kinematic, gpdType);

    return result;
}

ConvolCoeffFunctionModule* ConvolCoeffFunctionService::configureConvolCoeffFunctionModule(
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
        GPDModule* pGPDModule) const {

    // set gpd module to dvcs convol coeff function module
    pConvolCoeffFunctionModule->setGPDModule(pGPDModule);

    return pConvolCoeffFunctionModule;
}

//
//ResultList<DVCSConvolCoeffFunctionResult> DVCSConvolCoeffFunctionService::computeListWithGPDModel(
//        const DVCSConvolCoeffFunctionKinematic& kinematic,
//        std::vector<DVCSConvolCoeffFunctionModule*> listOfDVCSConvolCoeffFunctionModule,
//        GPDType::Type gpdType) const {
//
//    ResultList<DVCSConvolCoeffFunctionResult> results;
//
//    for (unsigned int i = 0; i != listOfDVCSConvolCoeffFunctionModule.size();
//            i++) {
//        results.add(
//                computeWithGPDModel(kinematic,
//                        listOfDVCSConvolCoeffFunctionModule[i], gpdType));
//    }
//
//    return results;
//}

DVCSConvolCoeffFunctionResult ConvolCoeffFunctionService::computeWithGPDModelTask(
        Task& task) const {

    //create a kinematic and init it with a list of parameters
    DVCSConvolCoeffFunctionKinematic kinematic;

    if (task.isAvailableParameters("DVCSConvolCoeffFunctionKinematic")) {
        kinematic = DVCSConvolCoeffFunctionKinematic(
                task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule =
            newConvolCoeffFunctionModule(task);

    DVCSConvolCoeffFunctionResult result = computeWithGPDModel(kinematic,
            pConvolCoeffFunctionModule);

    return result;
}

//ResultList<DVCSConvolCoeffFunctionResult> DVCSConvolCoeffFunctionService::computeListWithGPDModelTask(
//        Task& task) const {
//    //create kinematic
//    DVCSConvolCoeffFunctionKinematic kinematic;
//
//    if (task.isAvailableParameters("DVCSConvolCoeffFunctionKinematic")) {
//        kinematic = DVCSConvolCoeffFunctionKinematic(
//                task.getLastAvailableParameters());
//    } else {
//        error(__func__,
//                ElemUtils::Formatter()
//                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
//                        << task.getFunctionName());
//    }
//
//    std::vector<DVCSConvolCoeffFunctionModule*> listOfModule;
//
//    if (task.isAvailableParameters("DVCSConvolCoeffFunctionModule")) {
//        std::vector<ElemUtils::Parameters> listOfParameterList =
//                task.getListOfLastAvailableParameters(
//                        "DVCSConvolCoeffFunctionModule");
//
//        for (unsigned int i = 0; i != listOfParameterList.size(); i++) {
//            listOfModule.push_back(
//                    m_pModuleObjectFactory->newDVCSConvolCoeffFunctionModule(
//                            listOfParameterList[i].get(ModuleObject::CLASS_NAME).toString()));
//            listOfModule[i]->configure(listOfParameterList[i]);
//        }
//
//    } else {
//        error(__func__,
//                ElemUtils::Formatter()
//                        << "Missing object : <DVCSConvolCoeffFunctionModule> for method "
//                        << task.getFunctionName());
//    }
//
//    GPDModule* pGPDModule = 0;
//
//    //TODO How to handle CFF module without GPD module ?
//
//    if (task.isAvailableParameters("GPDModule")) {
//        pGPDModule =
//                m_pModuleObjectFactory->newGPDModule(
//                        task.getLastAvailableParameters().get(
//                                ModuleObject::CLASS_NAME).toString());
//        pGPDModule->configure(task.getLastAvailableParameters());
//    } else {
//        error(__func__,
//                ElemUtils::Formatter()
//                        << "Missing object : <GPDModule> for method "
//                        << task.getFunctionName());
//    }
//
//    ResultList<DVCSConvolCoeffFunctionResult> results = computeListWithGPDModel(
//            kinematic, listOfModule);
//
//    return results;
//}

ResultList<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionService::computeManyKinematicOneModel(
        List<DVCSConvolCoeffFunctionKinematic> &kinematics,
        ConvolCoeffFunctionModule* convolCoeffFunctionModule) const {

    ResultList<DVCSConvolCoeffFunctionResult> results;

    for (size_t i = 0; i != kinematics.size(); i++) {
        results.add(
                computeWithGPDModel(kinematics[i], convolCoeffFunctionModule));
    }

    return results;
}

ConvolCoeffFunctionModule* ConvolCoeffFunctionService::newConvolCoeffFunctionModule(
        const Task& task) const {
    GPDModule* pGPDModule = 0;

    //TODO How to handle CFF module without GPD module ?

    if (task.isAvailableParameters("GPDModule")) {
        pGPDModule =
                m_pModuleObjectFactory->newGPDModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pGPDModule->configure(task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <GPDModule> for method "
                        << task.getFunctionName());
    }

    DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule = 0;

    if (task.isAvailableParameters("DVCSConvolCoeffFunctionModule")) {
        pDVCSConvolCoeffFunctionModule =
                m_pModuleObjectFactory->newDVCSConvolCoeffFunctionModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pDVCSConvolCoeffFunctionModule->configure(
                task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <GPDEvolutionModule> for method "
                        << task.getFunctionName());
    }

    return configureConvolCoeffFunctionModule(pDVCSConvolCoeffFunctionModule,
            pGPDModule);
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
//            splitedLine = ElemUtils::StringUtils::split(fileLines[i], ' ');
//            // if there is 4 entry that's we have all parameters
//            if (!splitedLine.empty() && splitedLine.size() == 4) {
//                // store a new CFFInputData object
//                data.insert(
//                        std::pair<unsigned int, CFFInputData>(
//                                ElemUtils::StringUtils::fromStringToInt(splitedLine[0]),
//                                CFFInputData(
//                                        ElemUtils::StringUtils::fromStringToInt(
//                                                splitedLine[0]),
//                                        ElemUtils::StringUtils::fromStringToDouble(
//                                                splitedLine[1]),
//                                        ElemUtils::StringUtils::fromStringToDouble(
//                                                splitedLine[2]),
//                                        ElemUtils::StringUtils::fromStringToDouble(
//                                                splitedLine[3]))));
//            } else {
//                //TODO missing arg
//            }
//        }
//    } else {
//        //TODO faire une classe FileException pour mettre en forme le message et utiliser le formatter
//
//        error( __func__,
//                "UNREADABLE file: " + filePath);
//
//        throw std::runtime_error(
//                ElemUtils::Formatter()
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
//            splitedLine = ElemUtils::StringUtils::split(fileLines[i], ' ');
//            // if there is 4 entry that's we have all parameters
//            if (!splitedLine.empty() && splitedLine.size() == 4) {
//                // store a new CFFInputData object
//                data.push_back(
//
//                        CFFInputData(
//                                ElemUtils::StringUtils::fromStringToInt(splitedLine[0]),
//                                ElemUtils::StringUtils::fromStringToDouble(splitedLine[1]),
//                                ElemUtils::StringUtils::fromStringToDouble(splitedLine[2]),
//                                ElemUtils::StringUtils::fromStringToDouble(
//                                        splitedLine[3])));
//            } else {
//                //TODO missing arg
//            }
//        }
//    } else {
//        //TODO faire une classe FileException pour mettre en forme le message et utiliser le formatter
//
//        error( __func__,
//                "UNREADABLE file: " + filePath);
//
//        throw std::runtime_error(
//                ElemUtils::Formatter()
//                        << FileExceptionType(FileExceptionType::UNREADABLE).toString()
//                        << " filePath: " << filePath); // implicitly cast to std::string
//    }
//
//    return data;
//}

