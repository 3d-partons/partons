#ifndef LIST_H
#define LIST_H

/**
 * @file List.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2015
 * @version 1.0
 *
 * @class List
 *
 * @brief
 */

#include <stddef.h>
#include <iostream>
#include <vector>

#include "../utils/test/ComparisonReport.h"

class Tolerances;

template<class T> class List {
public:
    List() {

    }

    virtual ~List() {

    }

    void add(const T &data) {
        m_data.push_back(data);
    }

    T& operator[](size_t n) {
        return m_data[n];
    }

    const T& operator[](size_t n) const {
        return m_data[n];
    }

    size_t size() const {
        return m_data.size();
    }

    bool isEmpty() const {
        return (size() == 0) ? true : false;
    }

    ComparisonReport compare(const List<T> &other,
            const Tolerances &tolerances) const {

        if (this->isEmpty() && other.isEmpty()) {

        } else {
            if (this->size() == other.size()) {

                for (size_t i = 0; i != this->size(); i++) {
                    (this->m_data[i]).compare(other[i], tolerances);
                }

            } else {
                std::cerr << "Lists of are not equal in size" << std::endl;
            }

        }

        return ComparisonReport();
    }

private:
    std::vector<T> m_data;
};

#endif /* LIST_H */
