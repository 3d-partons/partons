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

namespace PARTONS {

/**
 * @class List
 *
 * @brief STL-like container to store basic PARTONS objects.
 *
 * This class is a STL-like container created in order to store PARTONS objects that derivative from BaseObject class.
 * More precisely, it is a wrap of std::vector that provides additional functionalities, like sorting and serialization, all based on functions predefined in BaseObject class.
 * This example illustrates how to use this container:
 \code{.cpp}
 //define two lists of GPDResult objects
 List<GPDResult> list1;
 List<GPDResult> list2;

 //fill these lists with few objects

 //retrieve GPD service
 GPDService* pGPDService =
 Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel =
 Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
 MMS13Model::classId);

 //define list of GPD types to be computed
 List<GPDType> gpdTypeList;
 gpdTypeList.add(GPDType::H);

 //get four examples
 for (size_t i = 0; i < 4; i++) {

 //evaluate
 GPDResult gpdResult = pGPDService->computeGPDModel(
 GPDKinematic(-0.1 + 0.05 * i, 0.05, 0., 2., 2.), pGPDModel,
 gpdTypeList);

 //fill lists
 list1.add(gpdResult);
 list2.add(gpdResult);
 }

 //compare lists
 ComparisonReport comparisonReport1(NumA::Tolerances(0.001, 0.001));
 list1.compare(comparisonReport1, list2);

 //print comparison results
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Comparison report: " << comparisonReport1.toString());
 \endcode
 which gives via Logger:
 \code
 20-05-2017 11:37:21 [INFO] (example::main) Comparison report:
 [Environment setting]
 Linux partons 3.2.0-4-amd64 #1 SMP Debian 3.2.73-2+deb7u2 x86_64 GNU/Linux
 g++ (Debian 4.7.2-5) 4.7.2
 GSL 1.16
 ROOT 5.34.32
 CLN 1.3.4
 SFML 2.3.2
 QT 4.8.3
 PARTONS Revision: 728
 NumA++ Revision: 728

 [Objects compared]
 compared to

 [Tolerances]
 Absolute tolerance = 0.001
 Relative tolerance = 0.001

 Number of objects compared with test PASSED = 40

 Number of objects compared with test FAILED = 0
 \endcode
 */
template<class T> class List: public BaseObject {
public:

    /**
     * Constructor.
     */
    List() :
            BaseObject("List") {
    }

    /**
     * Assignment constructor.
     * @param stdVector Vector storing values to be assigned.
     */
    List(std::vector<T> &stdVector) :
            BaseObject("List") {
        for (unsigned int i = 0; i != stdVector.size(); i++) {
            this->add(stdVector[i]);
        }
    }

    /**
     * Destructor.
     */
    virtual ~List() {
    }

    /**
     * Add new element.
     */
    virtual void add(const T &data) {
        m_data.push_back(data);
    }

    /**
     * Add list of new elements.
     * @param list List of elements to be added.
     */
    void add(const List<T> &list) {
        for (size_t i = 0; i != list.size(); i++) {
            add(list[i]);
        }
    }

    /**
     * Add vector of new elements.
     * @param list Vector of elements to be added.
     */
    void add(const std::vector<T> &vector) {
        for (size_t i = 0; i != vector.size(); i++) {
            add(vector[i]);
        }
    }

    /**
     * Get reference to element of given index.
     * @param n Index of requested element.
     * @return Reference to requested element.
     */
    const T& get(size_t n) const {
        return m_data.at(n);
    }

    /**
     * Get reference to element of given index.
     * @param n Index of requested element.
     * @return Reference to requested element.
     */
    T& operator[](size_t n) {
        return m_data[n];
    }

    /**
     * Get reference to element of given index.
     * @param n Index of requested element.
     * @return Reference to requested element.
     */
    const T& operator[](size_t n) const {
        return m_data[n];
    }

    /**
     * Get size of this list.
     * @return Size of this list.
     */
    size_t size() const {
        return m_data.size();
    }

    /**
     * Check if list is empty.
     * @return True if list is empty, otherwise false.
     */
    bool isEmpty() const {
        return (size() == 0) ? true : false;
    }

    /**
     * Return a pre-formatted characters string for output visualization of class member's values.
     * @return A pre-formatted characters string.
     */
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

    /**
     * Sort elements of this list.
     */
    void sort() {
        std::sort(m_data.begin(), m_data.end());
    }

    /**
     * Get vector containing stored elements.
     * @return Vector containing stored elements
     */
    std::vector<T> getData() const {
        return m_data;
    }

    /**
     * Compare to other List and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to List to be compared.
     * @param comparisonMode Mode of comparison, see documentation of ComparisonMode class.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
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

    /**
     * Clear list.
     */
    void clear() {
        m_data.clear();
    }

    /**
     * Resize list to given size.
     * @param n Size to be set.
     */
    void resize(size_t n) {
        m_data.resize(n);
    }

    /**
     * Remove the last element from this list.
     */
    void removeLast() {
        m_data.pop_back();
    }

    /**
     * Get the last element of this list.
     * @return Requested element.
     */
    const T& getLast() const {

        if (size() > 0)
            return m_data[size() - 1];
        else
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "Cannot get the last object because the list is empty");
    }

    /**
     * Used to split a complex C++ object into a concatenation of simple types.
     * @param packet Target Packet.
     */
    void serialize(ElemUtils::Packet &packet) const {
        packet << static_cast<unsigned int>(size());

        for (size_t i = 0; i != size(); i++) {
            packet << m_data[i];
        }
    }

    /**
     * Used to rebuild a complex C++ object from a concatenation of simple type.
     * @param packet Input Packet.
     */
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

    /**
     * Elements of this list.
     */
    std::vector<T> m_data;

};

} /* namespace PARTONS */

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
template<class T>
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, PARTONS::List<T>& list) {
    list.serialize(packet);
    return packet;
}

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
template<class T>
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, PARTONS::List<T>& list) {
    list.unserialize(packet);
    return packet;
}

#endif /* LIST_H */
