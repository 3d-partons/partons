/*
 * VectorUtils.h
 *
 *  Created on: Jun 30, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <algorithm>
#include <vector>

//TODO tester les fonctions de la classe. Je ne sais pas si elles sont correctes.
class VectorUtils {
public:
    //TODO attention ici on modifie les vecteurs car on les tri. Faut-il copier les vector ou bien ne pas trier ?
    // http://www.geeksforgeeks.org/find-union-and-intersection-of-two-unsorted-arrays/
    template<typename T>
    static std::vector<T> intersection(std::vector<T> & lhs,
            std::vector<T> & rhs) {
        std::vector<T> result;

        std::sort(lhs.begin(), lhs.end());
        std::sort(rhs.begin(), rhs.end());

        std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                std::back_inserter(result));

        return result;
    }
};

#endif /* VECTOR_UTILS_H */
