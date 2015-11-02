#ifndef OBSERVABLE_KINEMATIC_LIST_H
#define OBSERVABLE_KINEMATIC_LIST_H

/**
 * @file ObservableKinematicList.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ObservableKinematicList
 *
 * @brief
 */

#include <stddef.h>
#include <string>
#include <vector>

#include "ObservableKinematic.h"

class ObservableKinematicList: public BaseObject {
public:
    ObservableKinematicList();
    ObservableKinematicList(const std::string &kinematicFilePath);
    virtual ~ObservableKinematicList();

    size_t size() const;

    ObservableKinematic& operator[](size_t n);
    const ObservableKinematic& operator[](size_t n) const;

private:
    std::vector<ObservableKinematic> m_kinematicList;
};

#endif /* OBSERVABLE_KINEMATIC_LIST_H */
