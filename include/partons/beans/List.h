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
#include <string>
#include <vector>

#include "../BaseObject.h"
#include "../utils/compare/ComparisonMode.h"
#include "../utils/compare/ComparisonReport.h"
#include "../utils/stringUtils/Formatter.h"
#include "SortingMode.h"

namespace NumA {
class Tolerances;
} /* namespace NumA */

template<class T> class List: public BaseObject {
public:
    List() :
            BaseObject("List") {
    }

    virtual ~List() {
    }

    virtual void add(const T &data) {
        m_data.push_back(data);
    }

    void add(const List<T> &list) {
        for (size_t i = 0; i != list.size(); i++) {
            add(list[i]);
        }
    }

    void add(const std::vector<T> &vector) {
        for (size_t i = 0; i != vector.size(); i++) {
            add(vector[i]);
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

    //TODO implement
    void sort(const SortingMode &sortingMode) {
        switch (sortingMode.getType()) {
        case SortingMode::ASCENDING: {

            break;
        }
        case SortingMode::DESCENDING: {
            break;
        }
        }
    }

    void compare(ComparisonReport &rootComparisonReport,
            const List<T> &referenceObject, const NumA::Tolerances &tolerances,
            const ComparisonMode &comparisonMode = ComparisonMode::EQUAL,
            std::string parentObjectInfo = "") const {

        rootComparisonReport.setTolerances(tolerances);

        if ((this->isEmpty()) && referenceObject.isEmpty()) {
            warn(__func__, Formatter() << "Lists are empty");
        } else {
            // see : http://stackoverflow.com/a/9813792
            // TODO Don't work
            // reportList.setObjectTypeCompared(__PRETTY_FUNCTION__);

            switch (comparisonMode) {
            case ComparisonMode::INTERSECTION:
                break;
            default:
                // equal comparison by default
                if (this->size() == referenceObject.size()) {
                    //reportList.setNumberOfComparedObjet(this->size());

                    for (size_t i = 0; i != this->size(); i++) {
                        // reportList.add(
//                                (this->m_data[i]).compare(referenceObject[i],
//                                        tolerances));

                        (this->m_data[i]).compare(rootComparisonReport,
                                referenceObject[i], tolerances);
                    }
                } else {
                    warn(__func__,
                            Formatter() << "Lists are not equal in size");
                }
                break;
            }

        }
    }

    void clear() {
        m_data.clear();
    }

protected:
    std::vector<T> m_data;
};

#endif /* LIST_H */
