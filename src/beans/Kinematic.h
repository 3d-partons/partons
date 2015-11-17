#ifndef KINEMATIC_H
#define KINEMATIC_H

/**
 * @file Kinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 17 November 2015
 * @version 1.0
 *
 * @class Kinematic
 *
 * @brief
 */

#include <string>

#include "../BaseObject.h"

class Kinematic: public BaseObject {
public:
    Kinematic();
    Kinematic(const std::string &className);
    virtual ~Kinematic();

    virtual std::string toString() const;

    // use by std::sort function
    bool operator <(const Kinematic &other) const;

    unsigned int getId() const;
    void setId(unsigned int id);

private:
    unsigned int m_id;
};

#endif /* KINEMATIC_H */
