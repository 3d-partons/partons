#ifndef DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H
#define DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H

/**
 * @file DVCSConvolCoeffFunctionKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../dao/DVCSConvolCoeffFunctionKinematicDao.h"
#include "ConvolCoeffFunctionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionKinematicDaoService
 *
 * @brief DVCS Compton form factor (CFF) kinematics Data Access Object (DAO) service.
 *
 * It deals with ConvolCoeffFunctionKinematic C++ object and related tables from the database.
 */
class DVCSConvolCoeffFunctionKinematicDaoService: public ConvolCoeffFunctionKinematicDaoService {

public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionKinematicDaoService();

    /**
     * Insert into database a new DVCSConvolCoeffFunctionKinematic object with transactions mechanisms.
     *
     * @param ccfKinematic Reference to DVCSConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Insert into database a list of DVCSConvolCoeffFunctionKinematic objects with transactions mechanisms.
     *
     * @param ccfKinematicList Reference to List of DVCSConvolCoeffFunctionKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(
            const List<DVCSConvolCoeffFunctionKinematic> &ccfKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given DVCSConvolCoeffFunctionKinematic kinematics.
     *
     * @param ccfKinematic Reference to DVCSConvolCoeffFunctionKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with DVCSConvolCoeffFunctionKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Try to find an entry in the database containing CFF kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return DVCSConvolCoeffFunctionKinematic object filled with the retrieved data or a default GPDKinematic object if couldn't find it.
     */
    DVCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Try to find all entries in the database containing CFF kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return  List of DVCSConvolCoeffFunctionKinematic objects that matches with computation id.
     */
    List<DVCSConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Try to find unique id of database entry containing CFF kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    virtual int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow DVCSConvolCoeffFunctionResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class DVCSConvolCoeffFunctionResultDaoService;

    /**
     *DVCS ConvolCoeffFunctionKinematicDao object to perform database queries.
     */
    DVCSConvolCoeffFunctionKinematicDao m_dvcsConvolCoeffFunctionKinematicDao;

    /**
     * Insert into the database a new DVCSConvolCoeffFunctionKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<DVCSConvolCoeffFunctionKinematic> object, because transactions are already performed earlier.
     *
     * @param ccfKinematic Reference to DVCSConvolCoeffFunctionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(
            const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H */
