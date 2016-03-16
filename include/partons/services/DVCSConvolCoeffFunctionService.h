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
#include <vector>

#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../beans/ResultList.h"
#include "../modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../ServiceObject.h"

class GPDModule;

class DVCSConvolCoeffFunctionService: public ServiceObject {
public:
    static const std::string FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL;
    static const std::string FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    DVCSConvolCoeffFunctionService(const std::string &className);
    /**
     * Default destructor
     */
    virtual ~DVCSConvolCoeffFunctionService();

    virtual void computeTask(Task &task);

    ResultList<DVCSConvolCoeffFunctionResult> computeManyKinematicOneModel(
            DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
            GPDModule* _pGPDModule,
            List<DVCSConvolCoeffFunctionKinematic> &kinematics) const;

    virtual DVCSConvolCoeffFunctionResult computeWithGPDModel(
            DVCSConvolCoeffFunctionModule* dvcsConvolCoeffFunctionModule,
            GPDModule* _pGPDModule,
            const DVCSConvolCoeffFunctionKinematic &kinematic,
            GPDType::Type gpdType = GPDType::ALL) const;

    virtual ResultList<DVCSConvolCoeffFunctionResult> computeListWithGPDModel(
            const DVCSConvolCoeffFunctionKinematic &kinematic,
            std::vector<DVCSConvolCoeffFunctionModule*> listOfDVCSConvolCoeffFunctionModule,
            GPDModule* _pGPDModule, GPDType::Type gpdType = GPDType::ALL) const;

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

private:
    //TODO improve object copy
    DVCSConvolCoeffFunctionResult computeWithGPDModelTask(Task &task) const;
    ResultList<DVCSConvolCoeffFunctionResult> computeListWithGPDModelTask(
            Task &task) const;
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H */
