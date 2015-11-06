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
#include <string>
#include <vector>

#include "ObservableResult.h"

class ObservableResultList: public Computation {
public:
    ObservableResultList();
    virtual ~ObservableResultList();

    void add(ObservableResult observableResult);
    void add(ObservableResultList observableResultList);

    size_t size() const;

    ObservableResult& operator[](size_t n);
    const ObservableResult& operator[](size_t n) const;

    virtual std::string toString();

private:
    std::vector<ObservableResult> m_observableResultList;
};

#endif /* OBSERVABLE_RESULT_LIST_H */
