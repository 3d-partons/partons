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
public:
	GenericData() {
	}
	;

	const std::map<std::string, std::string>& getDataMap() const {
		return m_dataMap;
	}

	void setDataMap(const std::map<std::string, std::string>& dataMap) {
		this->m_dataMap = dataMap;
	}

	const std::vector<std::string>& getDataVector() const {
		return m_dataVector;
	}

	void setDataVector(const std::vector<std::string>& dataVector) {
		this->m_dataVector = dataVector;
	}

private:
	std::vector<std::string> m_dataVector;
	std::map<std::string, std::string> m_dataMap;
};

#endif /* GENERIC_DATA_H */
