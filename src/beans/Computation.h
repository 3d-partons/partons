#ifndef COMPUTATION_H
#define COMPUTATION_H

/**
 * @file ObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 *
 * @class ObservableResult
 *
 * @brief
 */

#include <ctime>

class Computation {
public:
    Computation();
    virtual ~Computation();

    const time_t getDateTime() const;
    void setDateTime(time_t dateTime);

private:
    time_t m_dateTime;
};

#endif /* COMPUTATION_H */
