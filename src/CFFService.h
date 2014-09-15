class GPDModule;

class CFFInputData;
class CFFOutputData;

/**
 * @file CFFService.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 07 Août 2014
 * @version 1.0
 *
 * Last update : 21 Aout 2014
 *
 * @class CFFService
 * @brief \<singleton\> Use for handle and compute some pre-configured CFF modules.
 */

class CFFService {

    /**
     * Private pointer of this class for a unique instance
     */
    static CFFService* pInstance;

    /**
     * Private default constructor for a unique instance
     */
    CFFService();

public:
    /**
     * Share a unique pointer of this class
     */
    static CFFService* getInstance();
    /**
     * Default destructor
     */
    ~CFFService();

    /**
     *
     * @param _pCFFInputData
     * @return
     */
    CFFOutputData* compute(CFFInputData* _pCFFInputData,
            GPDModule* _pGPDModule);

    void configure();
};
