#ifndef GPD_KINEMATIC_LIST_H
#define GPD_KINEMATIC_LIST_H

/**
 * @file GPDKinematicList.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 13 November 2015
 * @version 1.0
 *
 * @class GPDKinematicList
 *
 * @brief
 */

#include <stddef.h>
#include <vector>

#include "GPDKinematic.h"

class GPDKinematicList: public BaseObject {
public:
    GPDKinematicList();
    virtual ~GPDKinematicList();

    size_t size() const;

    GPDKinematic& operator[](size_t n);
    const GPDKinematic& operator[](size_t n) const;

    void add(const GPDKinematic &gpdKinematic);

private:
    std::vector<GPDKinematic> m_kinematicList;
};

#endif /* GPD_KINEMATIC_LIST_H */
