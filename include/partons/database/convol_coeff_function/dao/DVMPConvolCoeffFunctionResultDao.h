#ifndef DVMP_CONVOL_COEFF_FUNCTION_RESULT_DAO_H
#define DVMP_CONVOL_COEFF_FUNCTION_RESULT_DAO_H

/**
 * @file DVMPConvolCoeffFunctionResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>

#include "../../../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "ConvolCoeffFunctionResultDao.h"
#include "DVMPConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class DVMPConvolCoeffFunctionResultDao
 *
 * @brief DVMP Compton form factor (CFF) result Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete CFF results from the database.
 */
class DVMPConvolCoeffFunctionResultDao: public ConvolCoeffFunctionResultDao {

public:

    /**
     * Default constructor.
     */
    DVMPConvolCoeffFunctionResultDao();

    /**
     * Destructor.
     */
    virtual ~DVMPConvolCoeffFunctionResultDao();

    /**
     * Insert and associate row in the database representing complex number (result for given GPD type) to that representing DVMPConvolCoeffFunctionResult.
     * @param realPart Real part of CFF value.
     * @param imgPart Imaginary part of CFF value.
     * @param gpdType GPD type, see GPDType::Type.
     * @param ccfResultId Unique id of row in the database representing DVMPConvolCoeffFunctionResult object.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoDVMPCCFResultComplex(const double realPart,
            const double imgPart, const GPDType::Type gpdType,
            const int ccfResultId) const;

    /**
     * Retrieve list of CFF results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of DVMPConvolCoeffFunctionResult objects containing CFF results retrieved from the database.
     */
    List<DVMPConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:

    /**
     * Fill List of DVMPConvolCoeffFunctionResult objects from given query.
     * @param resultList List of DVMPConvolCoeffFunctionResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillConvolCoeffFunctionResultList(
            List<DVMPConvolCoeffFunctionResult> &resultList,
            QSqlQuery& query) const;

    /**
     * Kinematic DAO.
     */
    DVMPConvolCoeffFunctionKinematicDao m_dvmpConvolCoeffFunctionKinematicDao;
};

} /* namespace PARTONS */

#endif /* DVMP_CONVOL_COEFF_FUNCTION_RESULT_DAO_H */
