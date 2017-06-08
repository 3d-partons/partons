#ifndef M_D_ARRAY_H
#define M_D_ARRAY_H

/**
 * @file MDArray.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 *  @author \<contributor\> Adrien KIELB (ModuloPI)
 * @date July 07, 2014
 * @version 1.0
 */

#include <ElementaryUtils/string_utils/Formatter.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdexcept>
#include <string>
#include <vector>

//TODO modifier pour qu'il ne s'utilise pas avant 2 dimensions

/**
 * @class MDArray
 *
 * @brief Multidimensional optimized array.
 *
 * This class acts as a STL-like container to store multidimensional data in a single vector only (here, std::vector).
 * The usage of this class is illustrated by the following example:
 \code{.cpp}
 //initialize MDArray object
 //in this example it will store 4x4x4 array
 //initial value for all elements will be 1.
 std::vector<size_t> dims;
 dims.push_back(4);
 dims.push_back(4);
 dims.push_back(4);

 MDArray<double> myArray(dims, 1.);

 //set value at (1, 2, 3) coordinate
 myArray(1, 2, 3) = 5.2;

 //access values at (1, 2, 3) coordinate
 double value = myArray(1, 2, 3);

 //print
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Value at (1, 2, 3) is: " << value);
 \endcode
 which gives via Logger:
 \code
 07-06-2017 08:37:42 [INFO] (example::main) Value at (1, 2, 3) is: 5.2
 \endcode
 */
template<typename T>
class MDArray {

public:

    //TODO voir comment supprimer le constructeur par défaut pour s'approcher de la classe std::vector.
    /**
     * Default constructor.
     */
    MDArray() {
        dims.push_back(0);
    }

    /**
     * Constructor.
     * @param _dims Vector containing array dimension sizes.
     * @param initValue Initialization element to be used to fill this array.
     */
    MDArray(std::vector<size_t> _dims, const T & initValue = T()) :
            dims(_dims) {
        size_t tempDataSize = 1;
        for (unsigned int i = 0; i != dims.size(); i++) {
            tempDataSize *= dims[i];
        }

        data = std::vector<double>(tempDataSize);
    }

    /**
     * Destructor.
     */
    ~MDArray() {
        dims.clear();
        data.clear();
    }

    /**
     * Push back one element.
     * To be used only with one dimensional arrays.
     * @param value Element to be pushed back.
     */
    void push_back(T value) {
        if (dims.size() == 1) {
            data.push_back(value);
            dims[0] = data.size();
        } else {
            throw std::logic_error(
                    "[MDArray] Unable to perform push_back() action : cause you handle a multi dimentional array ! - Action non permitted -");
        }
    }

    /**
     * Get size of the data array.
     */
    const size_t getSize() {
        return data.size();
    }

    /**
     * () accessor for setting value at target index.
     */
    T & operator()(size_t coordValue, ...) {
        va_list ap;
        va_start(ap, coordValue);
        return data[indexOf(coordValue, ap)];
    }

    /**
     * () accessor for getting value at target index.
     */
    T const & operator()(size_t coordValue, ...) const {
        va_list ap;
        va_start(ap, coordValue);
        return data[indexOf(coordValue, ap)];
    }

    /**
     * Get string representing the array data.
     * @return
     */
    std::string toString() const {
        ElemUtils::Formatter formatter;

        for (unsigned int i = 0; i != data.size(); i++) {
            formatter << data[i] << " ";
        }

        return formatter.str();
    }

private:

    /**
     * Vector containing array dimension sizes.
     */
    std::vector<size_t> dims;

    /**
     * Array data.
     */
    std::vector<T> data;

    /**
     * Return index of specifics coordinates. Transform x,y,z,... coordinates to index x.
     */
    size_t indexOf(size_t coordValue, va_list ap) const {
        size_t index = 0;

        //TODO check index algorithm
        index += computeIntermediateIndex(coordValue, 1);

        for (unsigned int numDim = 2; numDim <= dims.size(); numDim++) {
            size_t a = va_arg(ap, size_t);
            index += computeIntermediateIndex(a, numDim);
        }
        va_end(ap);

        return index;
    }

    /**
     * Compute intermediate index value.
     */
    size_t computeIntermediateIndex(size_t coordValue,
            unsigned int numDim) const {
        size_t tempIndex = coordValue;
        for (unsigned int i = numDim; i < dims.size(); i++) {
            tempIndex *= dims[i];
        }

        return tempIndex;
    }
};

#endif /* M_D_ARRAY_H */
