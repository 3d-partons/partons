#ifndef LIST_H
#define LIST_H

/**
 * @file List.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 26, 2015
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
#include <stddef.h>
#include <algorithm>    // std::sort
#include <string>
#include <vector>

#include "../BaseObject.h"
#include "../utils/compare/ComparisonMode.h"
#include "../utils/compare/ComparisonReport.h"

/**
 * @class List
 *
 * @brief
 */
template<class T> class List: public BaseObject {
public:
    List() :
            BaseObject("List") {
    }

    List(std::vector<T> &stdVector) :
            BaseObject("List") {
        for (unsigned int i = 0; i != stdVector.size(); i++) {
            this->add(stdVector[i]);
        }
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
        ElemUtils::Formatter formatter;

        for (size_t i = 0; i != m_data.size(); i++) {
            formatter << "List[index] = " << i << '\n' << m_data[i].toString()
                    << '\n';
        }

        return formatter.str();
    }

    //TODO implement
//    void sort(const SortingMode &sortingMode) {
//        switch (sortingMode.getType()) {
//        case SortingMode::ASCENDING: {
//
//            break;
//        }
//        case SortingMode::DESCENDING: {
//            break;
//        }
//        }
//    }

    void sort() {
        std::sort(m_data.begin(), m_data.end());
    }

    std::vector<T> getData() const {
        return m_data;
    }

    void compare(ComparisonReport &rootComparisonReport,
            const List<T> &referenceObject,
            const ComparisonMode &comparisonMode = ComparisonMode::EQUAL,
            std::string parentObjectInfo = ElemUtils::StringUtils::EMPTY) const {

        if ((this->isEmpty()) && referenceObject.isEmpty()) {
            warn(__func__, ElemUtils::Formatter() << "Lists are empty");
        } else {
            // see : http://stackoverflow.com/a/9813792
            // TODO Don't work
            // reportList.setObjectTypeCompared(__PRETTY_FUNCTION__);

            switch (comparisonMode) {
            case ComparisonMode::INTERSECTION: {
                //TODO implement
                break;
            }
            default: {

                //TODO remove hardcoded value ; use properties file
                unsigned int batchSize = 1000;

                unsigned int i = 0;
                unsigned int j = 0;

                while (i != referenceObject.size()) {
                    j = 0;

                    while ((j != batchSize) && (i != referenceObject.size())) {
                        (this->m_data[i]).compare(rootComparisonReport,
                                referenceObject[i]);
                        i++;
                        j++;
                    }

                    info(__func__, rootComparisonReport.showComparisonStats());

                    if (rootComparisonReport.sizeOfComparedDataFailed() != 0) {
                        info(__func__,
                                rootComparisonReport.showComparedDataFailed());
                    }
                    rootComparisonReport.clearComparedData();
                }

                info(__func__, rootComparisonReport.showComparisonStats());

//                // equal comparison by default
//                if (this->size() == referenceObject.size()) {
//                    for (size_t i = 0; i != this->size(); i++) {
//                        (this->m_data[i]).compare(rootComparisonReport,
//                                referenceObject[i]);
//                    }
//                } else {
//                    warn(__func__,
//                            ElemUtils::Formatter()
//                                    << "Lists are not equal in size ; EQUAL comparison mode cannot be performed");
//                }
                break;
            }
            }

        }
    }

    void clear() {
        m_data.clear();
    }

    void resize(size_t n) {
        m_data.resize(n);
    }

    void removeLast() {
        m_data.pop_back();
    }

    const T& getLast() const {

        return (size() > 0) ?
                m_data[size() - 1] :
                throw ElemUtils::CustomException(getClassName(), __func__,
                        "Cannot get the last object because the list is empty");
    }

    void serialize(ElemUtils::Packet &packet) const {
        packet << static_cast<unsigned int>(size());

        for (size_t i = 0; i != size(); i++) {
            packet << m_data[i];
        }
    }

    void unserialize(ElemUtils::Packet &packet) {
        unsigned int packetSize;
        packet >> packetSize;

        for (size_t i = 0; i != packetSize; i++) {
            T data;
            packet >> data;
            add(data);
        }
    }

protected:
    std::vector<T> m_data;

};

template<class T>
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, List<T>& list) {
    list.serialize(packet);
    return packet;
}

template<class T>
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, List<T>& list) {
    list.unserialize(packet);
    return packet;
}

#endif /* LIST_H */
