#ifndef OBSERVABLE_RESULT_LIST_H
#define OBSERVABLE_RESULT_LIST_H

/**
 * @file ObservableResultList.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 21 September 2015
 * @version 1.0
 *
 * @class ObservableResultList
 *
 * @brief
 */

#include <stddef.h>
#include <ctime>
#include <string>
#include <vector>

#include "ObservableResult.h"

class ObservableResultList {
public:
    ObservableResultList();
    virtual ~ObservableResultList();

    void add(ObservableResult observableResult);
    void add(const ObservableResultList &observableResultList);

    size_t size() const;

    ObservableResult& operator[](size_t n);
    const ObservableResult& operator[](size_t n) const;

    virtual std::string toString();

    time_t getDateTime() const;
    void setDateTime(time_t dateTime);

private:
    std::vector<ObservableResult> m_observableResultList;

    time_t m_dateTime;
};

#endif /* OBSERVABLE_RESULT_LIST_H */
