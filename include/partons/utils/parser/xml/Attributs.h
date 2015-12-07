#ifndef ATTRIBUTS_H
#define ATTRIBUTS_H

/**
 * @file Attributs.h
 * @author Adrien KIELB, Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 January 2014
 * @version 1.0
 *
 * @class Attributs
 * @brief
 */

#include <map>
#include <string>

class Attributs {
public:
    Attributs();
    virtual ~Attributs();

    void add(const std::string &key, const std::string &value);

    std::string getStringValueOf(const std::string &key);
    int getIntValueOf(const std::string &key);
    double getDoubleValueOf(const std::string &key);
    bool getBooleanValueOf(const std::string &key);

    bool isInt(std::string);

    std::string toString();

    bool isAvailable(const std::string &key) const;

private:
    std::map<std::string, std::string> m_attributes;
};

#endif /* ATTRIBUTS_H */
