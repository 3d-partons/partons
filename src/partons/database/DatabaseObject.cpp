#include "../../../include/partons/database/DatabaseObject.h"

DatabaseObject::DatabaseObject(const std::string &className) :
        BaseObject(className), m_indexId(-1) {
}

DatabaseObject::DatabaseObject(const std::string &className, int indexId) :
        BaseObject(className), m_indexId(indexId) {
}

DatabaseObject::~DatabaseObject() {
}

int DatabaseObject::getIndexId() const {
    return m_indexId;
}

void DatabaseObject::setIndexId(int indexId) {
    m_indexId = indexId;
}
