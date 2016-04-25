#ifndef RESULT_LIST_H
#define RESULT_LIST_H

/**
 * @file ResultList.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 30, 2015
 * @version 1.0
 */

#include "Computation.h"
#include "List.h"

/**
 * @class ResultList
 *
 * @brief
 */
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
