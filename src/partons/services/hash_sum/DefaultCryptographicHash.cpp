#include "../../../../include/partons/services/hash_sum/DefaultCryptographicHash.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

namespace PARTONS {

DefaultCryptographicHash::DefaultCryptographicHash() :
        CryptographicHashI("DefaultCryptographicHash") {
}

DefaultCryptographicHash::~DefaultCryptographicHash() {
}

std::string DefaultCryptographicHash::generateSHA1HashSum(
        const std::string& input) const {

    auto rol = [](uint32_t value, size_t bits) {
        return (value << bits) | (value >> (32 - bits));
    };

    // Initial hash values
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    // Pre-processing
    std::vector<uint8_t> msg(input.begin(), input.end());
    uint64_t bitLen = msg.size() * 8;

    // Append '1' bit
    msg.push_back(0x80);

    // Pad with zeros until length ≡ 448 (mod 512)
    while ((msg.size() % 64) != 56) {
        msg.push_back(0);
    }

    // Append length (big endian)
    for (int i = 7; i >= 0; --i) {
        msg.push_back(static_cast<uint8_t>(bitLen >> (i * 8)));
    }

    // Process 512-bit chunks
    for (size_t chunk = 0; chunk < msg.size(); chunk += 64) {
        uint32_t w[80];
        for (int i = 0; i < 16; i++) {
            w[i] = (msg[chunk + i*4] << 24) |
                   (msg[chunk + i*4 + 1] << 16) |
                   (msg[chunk + i*4 + 2] << 8) |
                   (msg[chunk + i*4 + 3]);
        }
        for (int i = 16; i < 80; i++) {
            w[i] = rol(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
        }

        uint32_t a = h0, b = h1, c = h2, d = h3, e = h4;

        for (int i = 0; i < 80; i++) {
            uint32_t f, k;
            if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
            else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
            else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
            else { f = b ^ c ^ d; k = 0xCA62C1D6; }

            uint32_t temp = rol(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = rol(b, 30);
            b = a;
            a = temp;
        }

        h0 += a; h1 += b; h2 += c; h3 += d; h4 += e;
    }

    std::ostringstream result;
    result.imbue(std::locale::classic());
    result << std::hex << std::setfill('0')
           << std::setw(8) << h0
           << std::setw(8) << h1
           << std::setw(8) << h2
           << std::setw(8) << h3
           << std::setw(8) << h4;

    return result.str();
}

} /* namespace PARTONS */
