#ifndef RESULT_LIST_H
#define RESULT_LIST_H

/**
 * @file ResultList.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 November 2015
 * @version 1.0
 *
 * @class ResultList
 *
 * @brief
 */

#include <ctime>

#include "List.h"

template<class T>
class ResultList: public List<T> {
public:
    ResultList() :
            List<T>(), m_computationDateTime(time(0)) {
    }

    ~ResultList() {
    }

    time_t getComputationDateTime() const {
        return m_computationDateTime;
    }

    void setComputationDateTime(time_t computationDateTime) {
        m_computationDateTime = computationDateTime;
    }

private:
    time_t m_computationDateTime;
};

#endif /* RESULT_LIST_H */
