#include <map>
#include <string>
#include <vector>

#include "../beans/cff/CFFInputData.h"
#include "../beans/cff/CFFOutputData.h"
#include "../beans/QCDOrderType.h"
#include "../BaseObject.h"

class CFFModule;
class GPDModule;

/**
 * @file CFFService.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 07 Août 2014
 * @version 1.0
 *
 * Last update : 22 September 2014
 *
 * @class CFFService
 * @brief \<singleton\> Use for handle and compute some pre-configured CFF modules.
 */

class CFFService: BaseObject {
public:
    /**
     * Share a unique pointer of this class
     */
    static CFFService* getInstance();
    /**
     * Default destructor
     */
    virtual ~CFFService();

    CFFOutputData* compute(CFFModule* cffModule, CFFInputData* _pCFFInputData,
            GPDModule* _pGPDModule, double MuF, double MuR,
            QCDOrderType qcdOrderType);

    std::vector<CFFOutputData> compute(
            std::vector<CFFInputData> ListOfCFFInputData,
            GPDModule* _pGPDModule, double MuF, double MuR,
            QCDOrderType qcdOrderType);

    std::map<unsigned int, CFFInputData> getMapOfCFFInputDataFromFile(
            const std::string & filePath);

    std::vector<CFFInputData> getListOfCFFInputDataFromFile(
            const std::string & filePath);

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
