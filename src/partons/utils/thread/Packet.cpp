#include "../../../../include/partons/utils/thread/Packet.h"

#include <cstring>

Packet::Packet() :
        m_readPos(0), m_isValid(true) {
}

Packet::~Packet() {
}

void Packet::append(const void* data, std::size_t sizeInBytes) {
    if (data && (sizeInBytes > 0)) {
        std::size_t start = m_data.size();
        m_data.resize(start + sizeInBytes);
        std::memcpy(&m_data[start], data, sizeInBytes);
    }
}

Packet& Packet::operator >>(double &data) {
    if (checkSize(sizeof(data))) {
        data = *reinterpret_cast<const double*>(&m_data[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}

Packet& Packet::operator >>(unsigned int &data) {
    if (checkSize(sizeof(data))) {
        data = *reinterpret_cast<const unsigned int*>(&m_data[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}

Packet& Packet::operator <<(double data) {
    append(&data, sizeof(data));
    return *this;
}

Packet& Packet::operator <<(unsigned int data) {
    append(&data, sizeof(data));
    return *this;
}

bool Packet::checkSize(std::size_t size) {
    m_isValid = m_isValid && (m_readPos + size <= m_data.size());

    return m_isValid;
}

Packet& Packet::operator >>(int& data) {
    if (checkSize(sizeof(data))) {
        data = *reinterpret_cast<const int*>(&m_data[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}

Packet& Packet::operator >>(unsigned short & data) {
    if (checkSize(sizeof(data))) {
        data = *reinterpret_cast<const unsigned short*>(&m_data[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}

Packet& Packet::operator >>(short & data) {
    if (checkSize(sizeof(data))) {
        data = *reinterpret_cast<const short*>(&m_data[m_readPos]);
        m_readPos += sizeof(data);
    }

    return *this;
}

Packet& Packet::operator <<(int data) {
    append(&data, sizeof(data));
    return *this;
}

Packet& Packet::operator <<(unsigned short data) {
    append(&data, sizeof(data));
    return *this;
}

Packet& Packet::operator <<(short data) {
    append(&data, sizeof(data));
    return *this;
}

Packet& Packet::operator >>(const std::string& data) {

}

Packet& Packet::operator <<(const std::string& data) {
}
