#ifndef CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H
#define CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H

/**
 * @file ConvolCoeffFunctionResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 *
 * @class ConvolCoeffFunctionResultDaoService
 *
 * @brief
 */

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../common/service/CommonDaoService.h"
#include "../dao/ConvolCoeffFunctionResultDao.h"
#include "ConvolCoeffFunctionKinematicDaoService.h"

class DVCSConvolCoeffFunctionResultList;

class ConvolCoeffFunctionResultDaoService: public BaseObject {
public:
    ConvolCoeffFunctionResultDaoService();
    virtual ~ConvolCoeffFunctionResultDaoService();

    int insert(const DVCSConvolCoeffFunctionResult &result);
    int insert(const DVCSConvolCoeffFunctionResultList &resultList);

    DVCSConvolCoeffFunctionResultList getResultListByComputationId(
            const int computationId) const;

private:
    ConvolCoeffFunctionResultDao m_convolCoeffFunctionResultDao;

    ConvolCoeffFunctionKinematicDaoService m_convolCoeffFunctionKinematicDaoService;
    CommonDaoService m_commonDaoService;

    int insertWithoutTransaction(
            const DVCSConvolCoeffFunctionResult &result) const;
};

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H */
