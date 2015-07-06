#ifndef CFF_SERVICE_H
#define CFF_SERVICE_H

/**
 * @file CFFService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 Août 2014
 * @version 1.0
 *
 * @class CFFService
 *
 * @brief \<singleton\> Use for handle and compute some pre-configured CFF modules.
 */

#include <string>

#include "../beans/gpd/GPDType.h"
#include "../beans/PerturbativeQCDOrderType.h"
#include "../ServiceObject.h"

class Scenario;
class CFFInputData;
class CoefficientFunctionModule;
class CFFOutputData;
class GPDModule;

class CoefficientFunctionService: public ServiceObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    CoefficientFunctionService(const std::string &className);
    /**
     * Default destructor
     */
    virtual ~CoefficientFunctionService();

    virtual void computeTask(const Task &task);

    virtual CFFOutputData computeWithGPDModel(
            CoefficientFunctionModule* cffModule, GPDModule* _pGPDModule,
            CFFInputData &cffInputData, const double MuF, const double MuR,
            PerturbativeQCDOrderType::Type qcdOrderType, GPDType::Type gpdType =
                    GPDType::ALL);

    virtual CFFOutputData computeWithCFFModel(
            CoefficientFunctionModule* cffModule, CFFInputData &cffInputData,
            const double MuF, const double MuR,
            PerturbativeQCDOrderType &qcdOrderType);

//    std::vector<CFFOutputData> compute(
//            std::vector<CFFInputData> ListOfCFFInputData,
//            GPDModule* _pGPDModule, double MuF, double MuR,
//            QCDOrderType qcdOrderType);
//
//    std::map<unsigned int, CFFInputData> getMapOfCFFInputDataFromFile(
//            const std::string & filePath);
//
//    std::vector<CFFInputData> getListOfCFFInputDataFromFile(
//            const std::string & filePath);

private:

};

#endif /* CFF_SERVICE_H */
