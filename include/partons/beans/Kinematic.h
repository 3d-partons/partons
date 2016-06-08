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

    int getId() const;
    void setId(int id);
    const std::string& getHashSum() const;
    void setHashSum(const std::string& hashSum) const;

protected:
    virtual void updateHashSum() const = 0;

private:
    int m_id;
   mutable std::string m_hashSum;
};

#endif /* KINEMATIC_H */
