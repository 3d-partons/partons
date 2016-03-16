#include "../../../include/partons/services/DVCSConvolCoeffFunctionService.h"

#include <stddef.h>

#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionResultDaoService.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/utils/GenericType.h"
#include "../../../include/partons/utils/ParameterList.h"
#include "../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../include/partons/utils/stringUtils/StringUtils.h"

const std::string DVCSConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL =
        "computeWithGPDModel";
const std::string DVCSConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL =
        "computeListWithGPDModel";

// Initialise [class]::classId with a unique name.
const unsigned int DVCSConvolCoeffFunctionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new DVCSConvolCoeffFunctionService(
                        "DVCSConvolCoeffFunctionService"));

DVCSConvolCoeffFunctionService::DVCSConvolCoeffFunctionService(
        const std::string &className) :
        ServiceObject(className) {
}

DVCSConvolCoeffFunctionService::~DVCSConvolCoeffFunctionService() {
}

//TODO implement
void DVCSConvolCoeffFunctionService::computeTask(Task &task) {

    ResultList<DVCSConvolCoeffFunctionResult> resultList;

    if (StringUtils::equals(task.getFunctionName(),
            DVCSConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL)) {
        resultList.add(computeWithGPDModelTask(task));

    } else if (StringUtils::equals(task.getFunctionName(),
            DVCSConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL)) {
        resultList = computeListWithGPDModelTask(task);
    } else {
        error(__func__, "unknown function name = " + task.getFunctionName());
    }

    if (task.isStoreInDB()) {
        ConvolCoeffFunctionResultDaoService convolCoeffFunctionResultDaoService;
        int computationId = convolCoeffFunctionResultDaoService.insert(
                resultList);

        if (computationId != -1) {
            info(__func__,
                    Formatter()
                            << "DVCSConvolCoeffFunctionResultList object has been stored in database with computation_id = "
                            << computationId);
        } else {
            error(__func__,
                    Formatter()
                            << "DVCSConvolCoeffFunctionResultList object : insertion into database failed");
        }
    }
}

//TODO implementer
DVCSConvolCoeffFunctionResult DVCSConvolCoeffFunctionService::computeWithGPDModel(
        DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
        GPDModule* _pGPDModule,
        const DVCSConvolCoeffFunctionKinematic &kinematic,
        GPDType::Type gpdType) const {

    // set gpd module to dvcs convol coeff function module
    dvcsConvolCoeffFunctionModule->setGPDModule(_pGPDModule);

    DVCSConvolCoeffFunctionResult result =
            dvcsConvolCoeffFunctionModule->compute(kinematic, gpdType);

    return result;
}

ResultList<DVCSConvolCoeffFunctionResult> DVCSConvolCoeffFunctionService::computeListWithGPDModel(
        const DVCSConvolCoeffFunctionKinematic& kinematic,
        std::vector<DVCSConvolCoeffFunctionModule*> listOfDVCSConvolCoeffFunctionModule,
        GPDModule* _pGPDModule, GPDType::Type gpdType) const {

    ResultList<DVCSConvolCoeffFunctionResult> results;

    for (unsigned int i = 0; i != listOfDVCSConvolCoeffFunctionModule.size();
            i++) {
        results.add(
                computeWithGPDModel(listOfDVCSConvolCoeffFunctionModule[i],
                        _pGPDModule, kinematic, gpdType));
    }

    return results;
}

DVCSConvolCoeffFunctionResult DVCSConvolCoeffFunctionService::computeWithGPDModelTask(
        Task& task) const {

    //create a kinematic and init it with a list of parameters
    DVCSConvolCoeffFunctionKinematic kinematic;

    if (task.isAvailableParameterList("DVCSConvolCoeffFunctionKinematic")) {
        kinematic = DVCSConvolCoeffFunctionKinematic(
                task.getLastAvailableParameterList());
    } else {
        error(__func__,
                Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    GPDModule* pGPDModule = 0;

    //TODO How to handle CFF module without GPD module ?

    if (task.isAvailableParameterList("GPDModule")) {
        pGPDModule = m_pModuleObjectFactory->newGPDModule(
                task.getLastAvailableParameterList().get("id").toString());
        pGPDModule->configure(task.getLastAvailableParameterList());
    } else {
        error(__func__,
                Formatter() << "Missing object : <GPDModule> for method "
                        << task.getFunctionName());
    }

    DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule = 0;

    if (task.isAvailableParameterList("DVCSConvolCoeffFunctionModule")) {
        pDVCSConvolCoeffFunctionModule =
                m_pModuleObjectFactory->newDVCSConvolCoeffFunctionModule(
                        task.getLastAvailableParameterList().get("id").toString());
        pDVCSConvolCoeffFunctionModule->configure(
                task.getLastAvailableParameterList());
    } else {
        error(__func__,
                Formatter()
                        << "Missing object : <GPDEvolutionModule> for method "
                        << task.getFunctionName());
    }

    DVCSConvolCoeffFunctionResult result = computeWithGPDModel(
            pDVCSConvolCoeffFunctionModule, pGPDModule, kinematic);

    return result;
}

ResultList<DVCSConvolCoeffFunctionResult> DVCSConvolCoeffFunctionService::computeListWithGPDModelTask(
        Task& task) const {
    //create kinematic
    DVCSConvolCoeffFunctionKinematic kinematic;

    if (task.isAvailableParameterList("DVCSConvolCoeffFunctionKinematic")) {
        kinematic = DVCSConvolCoeffFunctionKinematic(
                task.getLastAvailableParameterList());
    } else {
        error(__func__,
                Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    std::vector<DVCSConvolCoeffFunctionModule*> listOfModule;

    if (task.isAvailableParameterList("DVCSConvolCoeffFunctionModule")) {
        std::vector<ParameterList> listOfParameterList =
                task.getListOfLastAvailableParameterList(
                        "DVCSConvolCoeffFunctionModule");

        for (unsigned int i = 0; i != listOfParameterList.size(); i++) {
            listOfModule.push_back(
                    m_pModuleObjectFactory->newDVCSConvolCoeffFunctionModule(
                            listOfParameterList[i].get("id").toString()));
            listOfModule[i]->configure(listOfParameterList[i]);
        }

    } else {
        error(__func__,
                Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionModule> for method "
                        << task.getFunctionName());
    }

    GPDModule* pGPDModule = 0;

    //TODO How to handle CFF module without GPD module ?

    if (task.isAvailableParameterList("GPDModule")) {
        pGPDModule = m_pModuleObjectFactory->newGPDModule(
                task.getLastAvailableParameterList().get("id").toString());
        pGPDModule->configure(task.getLastAvailableParameterList());
    } else {
        error(__func__,
                Formatter() << "Missing object : <GPDModule> for method "
                        << task.getFunctionName());
    }

    ResultList<DVCSConvolCoeffFunctionResult> results = computeListWithGPDModel(
            kinematic, listOfModule, pGPDModule);

    return results;
}

ResultList<DVCSConvolCoeffFunctionResult> DVCSConvolCoeffFunctionService::computeManyKinematicOneModel(
        DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
        GPDModule* _pGPDModule,
        List<DVCSConvolCoeffFunctionKinematic>& kinematics) const {

    ResultList<DVCSConvolCoeffFunctionResult> results;

    for (size_t i = 0; i != kinematics.size(); i++) {
        results.add(
                computeWithGPDModel(dvcsConvolCoeffFunctionModule, _pGPDModule,
                        kinematics[i]));
    }

    return results;
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
//        error( __func__,
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
//        error( __func__,
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

