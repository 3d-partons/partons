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
#include "Service.h"

class CFFInputData;
class CFFModule;
class CFFOutputData;
class GPDModule;

class CFFService: public Service {
public:
    static const std::string ID; ///< Unique ID to self-register in the registry

    /**
     * Share a unique pointer of this class
     */
    static CFFService* getInstance();
    /**
     * Default destructor
     */
    virtual ~CFFService();

    virtual void computeScenario(Scenario scenario);

    virtual CFFOutputData computeWithGPDModel(CFFModule* cffModule,
            GPDModule* _pGPDModule, CFFInputData &cffInputData,
            const double MuF, const double MuR,
            PerturbativeQCDOrderType::Type qcdOrderType,
            GPDType::Type gpdType = GPDType::ALL);

    virtual CFFOutputData computeWithCFFModel(CFFModule* cffModule,
            CFFInputData &cffInputData, const double MuF, const double MuR,
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
    /**
     * Private pointer of this class for a unique instance
     */
    static CFFService* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    CFFService();
};

#endif /* CFF_SERVICE_H */
