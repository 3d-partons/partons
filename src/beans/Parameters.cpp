#include "Parameters.h"

#include <utility>

//#include <vector>

Parameters::Parameters() {

}

Parameters::Parameters(unsigned int key, GenericData* value) {
    add(key, value);
}

Parameters::Parameters(unsigned int key, std::vector<GenericData*> values) {
    add(key, values);
}

Parameters::~Parameters() {

}

void Parameters::add(unsigned int key, GenericData* value) {
    std::vector<GenericData*> tempListOfGenericData;
    tempListOfGenericData.push_back(value);

    m_parameters.insert(
            std::make_pair<unsigned int, std::vector<GenericData*> >(key,
                    tempListOfGenericData));
}

void Parameters::add(unsigned int key, std::vector<GenericData*> values) {
    m_parameters.insert(
            std::make_pair<unsigned int, std::vector<GenericData*> >(key,
                    values));
}

GenericData* Parameters::getUnique(unsigned int key) {

    GenericData* pGenericData = 0;

    std::vector<GenericData*> listOfGenericData = get(key);

    if (listOfGenericData.size() > 0) {
        pGenericData = listOfGenericData[0];
    }

    return pGenericData;
}

std::vector<GenericData*> Parameters::get(unsigned int key) {

    std::vector<GenericData*> listOfGenericData;

    m_it = m_parameters.find(key);
    if (m_it != m_parameters.end()) {
        listOfGenericData = m_it->second;
    }

    return listOfGenericData;
}

void Parameters::clear() {
    m_parameters.clear();
}
