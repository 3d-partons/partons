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

#include <Qt/qsqlquery.h>
#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "ConvolCoeffFunctionKinematicDao.h"

class DVCSConvolCoeffFunctionResultList;
class DVCSConvolCoeffFunctionResult;

class ConvolCoeffFunctionResultDao: public BaseObject {
public:
    ConvolCoeffFunctionResultDao();
    virtual ~ConvolCoeffFunctionResultDao();

    int insert(double realPart, double imgPart, int gpdType,
            const std::string &computationModuleName, int kinematicId,
            int computationId) const;

    DVCSConvolCoeffFunctionResultList getResultListByComputationId(
            const int computationId) const;

private:
    ConvolCoeffFunctionKinematicDao m_convolCoeffFunctionKinematicDao;

    void getResultFromQuery(QSqlQuery &query,
            DVCSConvolCoeffFunctionResult &result) const;
};

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_H */
