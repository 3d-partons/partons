#include "GPDKinematicList.h"

GPDKinematicList::GPDKinematicList() :
        BaseObject("GPDKinematicList") {
}

GPDKinematicList::~GPDKinematicList() {
}

size_t GPDKinematicList::size() const {
    return m_kinematicList.size();
}

void GPDKinematicList::add(const GPDKinematic& gpdKinematic) {
    m_kinematicList.push_back(gpdKinematic);
}

GPDKinematic& GPDKinematicList::operator [](size_t n) {
    return m_kinematicList[n];
}

const GPDKinematic& GPDKinematicList::operator [](size_t n) const {
    return m_kinematicList[n];
}
