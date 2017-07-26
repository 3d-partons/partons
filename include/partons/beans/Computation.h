#ifndef COMPUTATION_H
#define COMPUTATION_H

/**
 * @file Computation.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <ctime>

#include "../BaseObject.h"

namespace PARTONS {

/**
 * @class Computation
 *
 * @brief Class to store computation information.
 *
 * This object is used to store the computation information, like that indicating time when a given computation was performed.
 * It is used in particular when a given computation is performed via xml scenario and its result is stored in the database.
 * In this case the content of Computation class is stored in the corresponding database table, so it can be retrieved latter.
 */
class Computation: public BaseObject {

public:

    /**
     * Default constructor.
     */
    Computation();

    /**
     * Copy constructor.
     *
     * @param other Object to be copied.
     */
    Computation(const Computation &other);

    /**
     * Assignment constructor.
     *
     * @param indexId Index id to be set, see BaseObject::m_indexId.
     * @param dateTime Time associated to a given computation to be set.
     */
    Computation(int indexId, time_t dateTime);

    /**
     * Destructor.
     */
    virtual ~Computation();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get time associated to a given computation.
     */
    time_t getDateTime() const;

    /**
     * Set time associated to a given computation.
     */
    void setDateTime(time_t dateTime);

private:

    /**
     * Time associated to a given computation.
     */
    time_t m_dateTime;
};

} /* namespace PARTONS */

#endif /* COMPUTATION_H */
