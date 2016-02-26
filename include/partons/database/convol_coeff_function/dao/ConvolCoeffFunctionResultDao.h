#ifndef CONVOL_COEFF_FUNCTION_RESULT_DAO_H
#define CONVOL_COEFF_FUNCTION_RESULT_DAO_H

/**
 * @file ConvolCoeffFunctionResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 *
 * @class ConvolCoeffFunctionResultDao
 *
 * @brief
 */

#include <QSqlQuery>
#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../beans/ResultList.h"
#include "../../common/dao/ComplexDao.h"
#include "ConvolCoeffFunctionKinematicDao.h"

class ConvolCoeffFunctionResultDao: public BaseObject {
public:
    ConvolCoeffFunctionResultDao();
    virtual ~ConvolCoeffFunctionResultDao();

    int insert(const std::string &computationModuleName, const int kinematicId,
            const int computationId) const;

    ResultList<DVCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

    int insertIntoCCFResultComplex(const int gpdTypeId, const int ccfResultId,
            const int complexId) const;

private:
    ConvolCoeffFunctionKinematicDao m_convolCoeffFunctionKinematicDao;
    ComplexDao m_complexDao;

    void fillConvolCoeffFunctionResultList(
            ResultList<DVCSConvolCoeffFunctionResult> &resultList,
            QSqlQuery& query) const;

    void fillConvolCoeffFunctionResult(
            DVCSConvolCoeffFunctionResult &convolCoeffFunctionResult) const;
};

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_H */
