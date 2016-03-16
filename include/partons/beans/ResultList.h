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

#include "Computation.h"
#include "List.h"

template<class T>
class ResultList: public List<T> {
public:
    ResultList() :
            List<T>() {
    }

    virtual ~ResultList() {
    }

    virtual void add(const T &data) {
        List<T>::add(data);
        this->m_data.back().getComputation().setDateTime(
                getComputation().getDateTime());
    }

    // ##### GETTERS & SETTERS #####

    const Computation& getComputation() const {
        return m_computation;
    }

    void setComputation(const Computation& computation) {
        m_computation = computation;
    }

private:

    Computation m_computation;
};

#endif /* RESULT_LIST_H */
