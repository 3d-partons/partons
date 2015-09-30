#ifndef DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H
#define DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H

/**
 * @file DVCSConvolCoeffFunctionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 Août 2014
 * @version 1.0
 *
 * @class DVCSConvolCoeffFunctionService
 *
 * @brief \<singleton\> Use for handle and compute some pre-configured CFF modules.
 */

#include <string>

#include "../beans/gpd/GPDType.h"
#include "../beans/PerturbativeQCDOrderType.h"
#include "../ServiceObject.h"

class DVCSConvolCoeffFunctionKinematic;
class DVCSConvolCoeffFunctionModule;
class DVCSConvolCoeffFunctionResult;
class GPDModule;

class DVCSConvolCoeffFunctionService: public ServiceObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    DVCSConvolCoeffFunctionService(const std::string &className);
    /**
     * Default destructor
     */
    virtual ~DVCSConvolCoeffFunctionService();

    virtual void computeTask(const Task &task);

    virtual DVCSConvolCoeffFunctionResult computeWithGPDModel(
            DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
            GPDModule* _pGPDModule, DVCSConvolCoeffFunctionKinematic &kinematic,
            GPDType::Type gpdType = GPDType::ALL);

    virtual DVCSConvolCoeffFunctionResult computeWithCFFModel(
            DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
            DVCSConvolCoeffFunctionKinematic &kinematic,
            PerturbativeQCDOrderType &qcdOrderType);

//    std::vector<DVCSConvolCoeffFunctionResult> compute(
//            std::vector<CFFInputData> ListOfCFFInputData,
//            GPDModule* _pGPDModule, double MuF, double MuR,
//            QCDOrderType qcdOrderType);
//
//    std::map<unsigned int, CFFInputData> getMapOfCFFInputDataFromFile(
//            const std::string & filePath);
//
//    std::vector<CFFInputData> getListOfCFFInputDataFromFile(
//            const std::string & filePath);

};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H */
