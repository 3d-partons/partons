#ifndef DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H
#define DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H

/**
 * @file DVMPConvolCoeffFunctionKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../dao/DVMPConvolCoeffFunctionKinematicDao.h"
#include "ConvolCoeffFunctionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class DVMPConvolCoeffFunctionKinematicDaoService
 *
 * @brief DVMP Compton form factor (CFF) kinematics Data Access Object (DAO) service.
 *
 * It deals with ConvolCoeffFunctionKinematic C++ object and related tables from the database.
 */
class DVMPConvolCoeffFunctionKinematicDaoService: public ConvolCoeffFunctionKinematicDaoService {

public:

    /**
     * Default constructor.
     */
    DVMPConvolCoeffFunctionKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~DVMPConvolCoeffFunctionKinematicDaoService();

    /**
     * Insert into database a new DVMPConvolCoeffFunctionKinematic object with transactions mechanisms.
     *
     * @param ccfKinematic Reference to DVMPConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DVMPConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Insert into database a list of DVMPConvolCoeffFunctionKinematic objects with transactions mechanisms.
     *
     * @param ccfKinematicList Reference to List of DVMPConvolCoeffFunctionKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(
            const List<DVMPConvolCoeffFunctionKinematic> &ccfKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given DVMPConvolCoeffFunctionKinematic kinematics.
     *
     * @param ccfKinematic Reference to DVMPConvolCoeffFunctionKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with DVMPConvolCoeffFunctionKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const DVMPConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Try to find an entry in the database containing CFF kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return DVMPConvolCoeffFunctionKinematic object filled with the retrieved data or a default GPDKinematic object if couldn't find it.
     */
    DVMPConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Try to find all entries in the database containing CFF kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return  List of DVMPConvolCoeffFunctionKinematic objects that matches with computation id.
     */
    List<DVMPConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Try to find unique id of database entry containing CFF kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    virtual int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow DVMPConvolCoeffFunctionResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class DVMPConvolCoeffFunctionResultDaoService;

    /**
     *DVMP ConvolCoeffFunctionKinematicDao object to perform database queries.
     */
    DVMPConvolCoeffFunctionKinematicDao m_dvmpConvolCoeffFunctionKinematicDao;

    /**
     * Insert into the database a new DVMPConvolCoeffFunctionKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<DVMPConvolCoeffFunctionKinematic> object, because transactions are already performed earlier.
     *
     * @param ccfKinematic Reference to DVMPConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(
            const DVMPConvolCoeffFunctionKinematic &ccfKinematic) const;
};

} /* namespace PARTONS */

#endif /* DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H */
