#ifndef GENERIC_DATA_H
#define GENERIC_DATA_H

/**
 * @file GenericData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 28 juil. 2014
 * @version 1.0
 */

#include <map>
#include <string>
#include <vector>

class GenericData {
protected:
    std::vector<std::string> dataVector;
    std::map<std::string, std::string> dataMap;

public:
    GenericData() {
    }
    ;

    const std::map<std::string, std::string>& getDataMap() const {
        return dataMap;
    }

    void setDataMap(const std::map<std::string, std::string>& dataMap) {
        this->dataMap = dataMap;
    }

    const std::vector<std::string>& getDataVector() const {
        return dataVector;
    }

    void setDataVector(const std::vector<std::string>& dataVector) {
        this->dataVector = dataVector;
    }
};

#endif /* GENERIC_DATA_H */
