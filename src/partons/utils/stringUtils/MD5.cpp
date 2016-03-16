#include "../../../../include/partons/utils/stringUtils/MD5.h"

#include <stddef.h>
#include <algorithm>
#include <cmath>
#include <sstream>

const int MD5::INIT_A = 0x67452301;
const int MD5::INIT_B = 0xEFCDAB89;
const int MD5::INIT_C = 0x98BADCFE;
const int MD5::INIT_D = 0x10325476;

std::vector<int> MD5::makeShiftAmts() {
    std::vector<int> shiftAmts(16, 0);

    shiftAmts.push_back(7);
    shiftAmts.push_back(12);
    shiftAmts.push_back(17);
    shiftAmts.push_back(22);

    shiftAmts.push_back(5);
    shiftAmts.push_back(9);
    shiftAmts.push_back(14);
    shiftAmts.push_back(20);

    shiftAmts.push_back(4);
    shiftAmts.push_back(11);
    shiftAmts.push_back(16);
    shiftAmts.push_back(23);

    shiftAmts.push_back(6);
    shiftAmts.push_back(10);
    shiftAmts.push_back(15);
    shiftAmts.push_back(21);

    return shiftAmts;
}

std::vector<int> MD5::makeTableT() {

    std::vector<int> tableT(64, 0);

    double power = pow(2, 32);
    for (int i = 0; i < tableT.size(); i++) {
        tableT[i] = floor(fabs(sin(i + 1)) * power);
    }

    return tableT;
}

MD5::MD5() :
        m_shiftAmountsPerRound(MD5::makeShiftAmts()), m_sinusOfInteger(
                makeTableT()) {
}

MD5::~MD5() {
    // TODO Auto-generated destructor stub
}

std::string MD5::convertToBitsString(const std::string& string) const {
    std::stringstream sstream;

    for (size_t i = 0; i != string.size(); i++) {
        sstream << static_cast<int>(string[i]);
    }

    return sstream.str();
}

std::string MD5::compute(const std::string& string) const {
    std::stringstream sstream;

    //Pre-processing: adding a single 1 bit
    // Notice: the input bytes are considered as bits strings, where the first bit is the most significant bit of the byte.
    sstream << convertToBitsString(string);

    sstream << "1";

    sstream.seekg(0, std::ios::end);
    int originalSizeOfBitsString = sstream.tellg();

//    int missingZeros = 0;
//    if (originalSizeOfBitsString <= 448) {
//        missingZeros = 448 - (originalSizeOfBitsString % 512);
//    } else {
//        missingZeros = 512 + 448 - originalSizeOfBitsString;
//    }
//
//    for (int i = 0; i != missingZeros; i++) {
//        sstream << "0";
//    }

//    sstream.seekg(0, std::ios::end);
//    int stringLength = sstream.tellg();
//    int missingZeros = 448 - (stringLength % 512);
//    for (int i = 0; i != missingZeros; i++) {
//        sstream << "0";
//    }

    /* Append length
     *
     * A 64-bit representation of b (the length of the message before the
     * padding bits were added) is appended to the result of the previous
     * step. In the unlikely event that b is greater than 2^64, then only
     * the low-order 64 bits of b are used. (These bits are appended as two
     * 32-bit words and appended low-order word first in accordance with the
     * previous conventions.)
     *
     * At this point the resulting message (after padding with bits and with b)
     * has a length that is an exact multiple of 512 bits. Equivalently,
     * this message has a length that is an exact multiple of 16 (32-bit)
     * words. Let M[0 ... N-1] denote the words of the resulting message,
     * where N is a multiple of 16.
     */

    //sstream << (originalSizeOfBitsString % static_cast<int>((pow(2, 64))));

    return sstream.str();
}

std::string MD5::convertIntToBitRepresentation(int x) const {
    std::vector<int> result;

    while (x) {
        if (x & 1) {
            result.push_back(1);
        } else {
            result.push_back(0);
        }
        x >>= 1;
    }
    std::reverse(result.begin(), result.end());

    std::stringstream sstream;
    for (int i = 0; i != result.size(); i++) {
        sstream << result[i];
    }

    return sstream.str();
}
