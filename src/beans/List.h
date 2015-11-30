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
#include <string>
#include <vector>

#include "../utils/stringUtils/Formatter.h"
#include "../utils/test/ComparisonReport.h"
#include "ComparisonMode.h"

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

    void add(const List<T> &list) {
        for (size_t i = 0; i != list.size(); i++) {
            m_data.push_back(list[i]);
        }
    }

    const T& get(size_t n) const {
        return m_data.at(n);
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

    std::string toString() {
        Formatter formatter;

        for (size_t i = 0; i != m_data.size(); i++) {
            formatter << m_data[i].toString() << '\n';
        }

        return formatter.str();
    }

    ComparisonReport compare(const List<T> &other, const Tolerances &tolerances,
            const ComparisonMode &comparisonMode) const {

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
