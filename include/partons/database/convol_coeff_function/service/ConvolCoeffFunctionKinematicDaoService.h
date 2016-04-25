#ifndef CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H
#define CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H

/**
 * @file ConvolCoeffFunctionKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 *
 * @class ConvolCoeffFunctionKinematicDaoService
 *
 * @brief Its role is to deal with DVCSConvolCoeffFunctionKinematic C++ object and related tables from the database.
 * You can insert, select or remove results from database by many kind of stuff.
 * Therefore database services ensure the integrity of the database by using transaction and rollback mechanisms if something wrong append : the database will stay always in a stable state.
 * And they improve querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../dao/ConvolCoeffFunctionKinematicDao.h"

class ConvolCoeffFunctionKinematicDaoService: public BaseObject {
public:
    /**
     * Default constructor
     */
    ConvolCoeffFunctionKinematicDaoService();

    /**
     * Default destructor
     */
    virtual ~ConvolCoeffFunctionKinematicDaoService();

    /**
     * Insert into database a new DVCSConvolCoeffFunctionKinematic object with transactions mechanisms.
     *
     * @param ccfKinematic
     * @return unique id related to the new entry inserted into the database
     */
    int insert(const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * Insert into database a list of DVCSConvolCoeffFunctionKinematic objects with transactions mechanisms.
     *
     * @param ccfKinematicList
     * @return unique id related to the last entry inserted into the database
     */
    int insert(
            const List<DVCSConvolCoeffFunctionKinematic> &ccfKinematicList) const;

    /**
     * Try to find an entry in convol_coeff_function_kinematic table that match with xi, t, Q2, MuF2 and MuR2 values.
     *
     * @param ccfKinematic
     * @return unique id related to entry that match with DVCSConvolCoeffFunctionKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;

    /**
     * * Try to find an entry in convol_coeff_function_kinematic table that match with id value.
     *
     * @param id
     * @return DVCSConvolCoeffFunctionKinematic object fill with data from database tables or a default DVCSConvolCoeffFunctionKinematic object if couldn't find it.
     */
    DVCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Return a list of DVCSConvolCoeffFunctionKinematic objects from the database identified by a specific computation identifier.
     *
     * @param computationId
     * @return list of DVCSConvolCoeffFunctionKinematic objects.
     */
    List<DVCSConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

private:
    friend class ConvolCoeffFunctionResultDaoService; ///< allow ConvolCoeffFunctionResultDaoService to call private member function insertWithoutTransaction(...)

    ConvolCoeffFunctionKinematicDao m_convolCoeffFunctionKinematicDao; ///< reference to the right DAO object to perform database queries

    /**
     * Insert into the database a new DVCSConvolCoeffFunctionKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<DVCSConvolCoeffFunctionKinematic> object, because transactions are already performed earlier.
     *
     * @param ccfKinematic
     * @return unique id related to the new entry inserted into the database
     */
    int insertWithoutTransaction(
            const DVCSConvolCoeffFunctionKinematic &ccfKinematic) const;
};

#endif /* CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H */
