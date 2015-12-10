#ifndef DIFFERENCES_H
#define DIFFERENCES_H

/**
 * @file Differences.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 10 December 2015
 * @version 1.0
 *
 * @class Differences
 *
 * @brief
 */

#include <string>

class Differences {
public:
    Differences();
    Differences(const double absoluteDifference,
            const double relativeDifference);
    virtual ~Differences();

    std::string toString() const;

    double getAbsoluteDifference() const;
    double getRelativeDifference() const;

    void setAbsoluteDifference(double absoluteDifference);
    void setRelativeDifference(double relativeDifference);

private:
    double m_absoluteDifference;
    double m_relativeDifference;
};

#endif /* DIFFERENCES_H */
