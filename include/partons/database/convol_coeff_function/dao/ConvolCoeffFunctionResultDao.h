#ifndef CONVOL_COEFF_FUNCTION_RESULT_DAO_H
#define CONVOL_COEFF_FUNCTION_RESULT_DAO_H

/**
 * @file ConvolCoeffFunctionResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 *
 * @class ConvolCoeffFunctionResultDao
 *
 * @brief
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../beans/List.h"
#include "ConvolCoeffFunctionKinematicDao.h"

class ConvolCoeffFunctionResultDao: public BaseObject {
public:
    ConvolCoeffFunctionResultDao();
    virtual ~ConvolCoeffFunctionResultDao();

    int insert(const std::string &computationModuleName, const int kinematicId,
            const int computationId) const;

    List<DVCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

    int insertIntoCCFResultComplex(const int realPart, const int imgPart,
            const int gpdTypeId, const int ccfResultId) const;

private:
    ConvolCoeffFunctionKinematicDao m_convolCoeffFunctionKinematicDao;

    void fillConvolCoeffFunctionResultList(
            List<DVCSConvolCoeffFunctionResult> &resultList,
            QSqlQuery& query) const;
};

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_H */
