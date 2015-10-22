#include "DVCSConvolCoeffFunctionService.h"

#include "../beans/automation/Task.h"
#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResultList.h"
#include "../BaseObjectRegistry.h"
#include "../modules/GPDModule.h"
#include "../ModuleObjectFactory.h"
#include "../utils/GenericType.h"
#include "../utils/ParameterList.h"
#include "../utils/stringUtils/Formatter.h"
#include "../utils/stringUtils/StringUtils.h"

//#include "GPDService.h"

const std::string DVCSConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL =
        "computeWithGPDModel";
const std::string DVCSConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL =
        "computeListWithGPDModel";

// Initialise [class]::classId with a unique name.
const unsigned int DVCSConvolCoeffFunctionService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
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
    if (StringUtils::equals(task.getFunctionName(),
            DVCSConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL)) {

        //create a GPDKinematic and init it with a list of parameters
        DVCSConvolCoeffFunctionKinematic kinematic;

        if (task.isAvailableParameterList("DVCSConvolCoeffFunctionKinematic")) {
            kinematic = DVCSConvolCoeffFunctionKinematic(
                    task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter()
                            << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                            << task.getFunctionName());
        }

        GPDModule* pGPDModule = 0;

        //TODO How to handle CFF module without GPD module ?

        if (task.isAvailableParameterList("GPDModule")) {
            pGPDModule = ModuleObjectFactory::newGPDModule(
                    task.getLastAvailableParameterList().get("id").toString());
            pGPDModule->configure(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule = 0;

        if (task.isAvailableParameterList("DVCSConvolCoeffFunctionModule")) {
            pDVCSConvolCoeffFunctionModule =
                    ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
                            task.getLastAvailableParameterList().get("id").toString());
            pDVCSConvolCoeffFunctionModule->configure(
                    task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter()
                            << "Missing object : <GPDEvolutionModule> for method "
                            << task.getFunctionName());
        }

        DVCSConvolCoeffFunctionResult result = computeWithGPDModel(
                pDVCSConvolCoeffFunctionModule, pGPDModule, kinematic);

        info(__func__,
                Formatter() << task.getFunctionName() << "("
                        << pDVCSConvolCoeffFunctionModule->getClassName()
                        << " , " << pGPDModule->getClassName() << ")" << '\n'
                        << result.toString());

    } else if (StringUtils::equals(task.getFunctionName(),
            DVCSConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL)) {

//create kinematic
        DVCSConvolCoeffFunctionKinematic kinematic;

        if (task.isAvailableParameterList("DVCSConvolCoeffFunctionKinematic")) {
            kinematic = DVCSConvolCoeffFunctionKinematic(
                    task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
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
                        ModuleObjectFactory::newDVCSConvolCoeffFunctionModule(
                                listOfParameterList[i].get("id").toString()));
                listOfModule[i]->configure(listOfParameterList[i]);
            }

        } else {
            throwException(__func__,
                    Formatter()
                            << "Missing object : <DVCSConvolCoeffFunctionModule> for method "
                            << task.getFunctionName());
        }

        GPDModule* pGPDModule = 0;

        //TODO How to handle CFF module without GPD module ?

        if (task.isAvailableParameterList("GPDModule")) {
            pGPDModule = ModuleObjectFactory::newGPDModule(
                    task.getLastAvailableParameterList().get("id").toString());
            pGPDModule->configure(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        DVCSConvolCoeffFunctionResultList results = computeListWithGPDModel(
                kinematic, listOfModule, pGPDModule);

        info(__func__,
                Formatter() << task.getFunctionName() << "(" << ")" << '\n'
                        << results.toString());
    } else {
        throwException(__func__,
                "unknown function name = " + task.getFunctionName());
    }
}

//TODO implementer
DVCSConvolCoeffFunctionResult DVCSConvolCoeffFunctionService::computeWithGPDModel(
        DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
        GPDModule* _pGPDModule,
        const DVCSConvolCoeffFunctionKinematic &kinematic,
        GPDType::Type gpdType) {

    // Configure cff module
    dvcsConvolCoeffFunctionModule->setGPDModule(_pGPDModule);

    //TODO call with xi no xB ?
    DVCSConvolCoeffFunctionResult result =
            dvcsConvolCoeffFunctionModule->compute(kinematic.getXi(),
                    kinematic.getT(), kinematic.getQ2(), kinematic.getMuF2(),
                    kinematic.getMuR2(), gpdType);

    result.setKinematic(kinematic);

    return result;
}

DVCSConvolCoeffFunctionResultList DVCSConvolCoeffFunctionService::computeListWithGPDModel(
        const DVCSConvolCoeffFunctionKinematic& kinematic,
        std::vector<DVCSConvolCoeffFunctionModule*> listOfDVCSConvolCoeffFunctionModule,
        GPDModule* _pGPDModule, GPDType::Type gpdType) {

    DVCSConvolCoeffFunctionResultList results;

    for (unsigned int i = 0; i != listOfDVCSConvolCoeffFunctionModule.size();
            i++) {
        results.add(
                computeWithGPDModel(listOfDVCSConvolCoeffFunctionModule[i],
                        _pGPDModule, kinematic, gpdType));
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

