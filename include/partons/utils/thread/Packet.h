#ifndef PACKET_H
#define PACKET_H

/**
 * @file Packet.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 13 January 2016
 * @version 1.0
 *
 * @class Packet
 *
 * @brief FIFO
 */

#include <cstddef>
#include <string>
#include <vector>

class Packet {
public:
    /**
     * Default constructor
     */
    Packet();

    /**
     * Default destructor
     */
    virtual ~Packet();

    ////////////////////////////////////////////////////////////
    /// \brief Append data to the end of the packet
    ///
    /// \param data        Pointer to the sequence of bytes to append
    /// \param sizeInBytes Number of bytes to append
    ///
    /// \see clear
    ///
    ////////////////////////////////////////////////////////////
    void append(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    /// Overloads of operator >> to read data from the packet
    ///
    ////////////////////////////////////////////////////////////
    Packet& operator >>(double& data);
    Packet& operator >>(unsigned int &data);
    Packet& operator >>(int &data);
    Packet& operator >>(unsigned short &data);
    Packet& operator >>(short &data);
    Packet& operator >>(const std::string &data);

    ////////////////////////////////////////////////////////////
    /// Overloads of operator << to write data into the packet
    ///
    ////////////////////////////////////////////////////////////
    Packet& operator <<(double data);
    Packet& operator <<(unsigned int data);
    Packet& operator <<(int data);
    Packet& operator <<(unsigned short data);
    Packet& operator <<(short data);
    Packet& operator <<(const std::string &data);

private:
    std::vector<char> m_data; ///< Data stored in the packet
    std::size_t m_readPos; ///< Current reading position in the packet
    bool m_isValid; ///< Reading state of the packet

    ////////////////////////////////////////////////////////////
    /// \brief Check if the packet can extract a given number of bytes
    ///
    /// This function updates accordingly the state of the packet.
    ///
    /// \param size Size to check
    ///
    /// \return True if \a size bytes can be read from the packet
    ///
    ////////////////////////////////////////////////////////////
    bool checkSize(std::size_t size);

    ////////////////////////////////////////////////////////////
    /// Disallow comparisons between GenericTypes
    ////////////////////////////////////////////////////////////
    bool operator ==(const Packet& right) const;
    bool operator !=(const Packet& right) const;
};

#endif /*  PACKET_H */
