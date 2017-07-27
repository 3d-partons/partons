/*
 * MapUtils.h
 *
 *  Created on: Jun 30, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef MAP_UTILS_H
#define MAP_UTILS_H

#include <map>
#include <vector>

#include "VectorUtils.h"

namespace PARTONS {

//TODO tester les fonctions de la classe. Je ne sais pas si elles sont correctes.
class MapUtils {
public:
    template<typename Key, typename Value>
    static std::vector<Key> intersectionOfKey(const std::map<Key, Value> & lhs,
            const std::map<Key, Value> & rhs) {

        std::vector<Key> lhsKey = MapUtils::mapToVectorOfKey(lhs);
        std::vector<Key> rhsKey = MapUtils::mapToVectorOfKey(rhs);

        return VectorUtils::intersection(lhsKey, rhsKey);
    }

    template<typename Key, typename Value>
    static std::vector<Key> mapToVectorOfKey(const std::map<Key, Value> & map) {
        std::vector<Key> keys;

        typename std::map<Key, Value>::const_iterator it;

        for (it = map.begin(); it != map.end(); it++) {
            keys.push_back(it->first);
        }

        return keys;
    }
};

} /* namespace PARTONS */

#endif /* MAP_UTILS_H */
