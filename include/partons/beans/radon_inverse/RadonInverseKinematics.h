#ifndef RADONINVERSEKINEMATIC_H
#define RADONINVERSEKINEMATIC_H

/**
 * @file RadonInverseKinematic.h
 * @author: Cédric Mezrag (ANL)
 * @date 17 November 2015
 * @version 0.1
 *
 * @class RadonInverseKinematic
 *
 * @brief
 */

#include <string>

#include "../BaseObject.h"

namespace PARTONS {

class RadonInverseKinematic: public BaseObject {
public:
    RadonInverseKinematic();
    RadonInverseKinematic(const std::string &className);
    virtual ~RadonInverseKinematic();

    virtual std::string toString() const;

    // use by std::sort function
    bool operator <(const Kinematic &other) const;

    int getId() const;
    void setId(int id);

private:
    int m_id;
};

} /* namespace PARTONS */

#endif /* RADONINVERSEKINEMATIC_H */
