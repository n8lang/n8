/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of N8.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

#include <N8.hpp>
#include <n8/util/VectorMath.hpp>

namespace N8Util {

bool isNumberArray(std::vector<DynamicObject> vec) {
    for(size_t i = 0; i < vec.size(); i++)
        if(!vec[i].isNumber())
            return false;

    return true;
}

DynamicObject vector2Object(const std::vector<double>& vec) {
    std::vector<DynamicObject> objects(vec.size());

    parsync(size_t i = 0; i < vec.size(); ++i)
        objects[i] = DynamicObject(vec[i]);

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(std::move(objects))
    );
}

std::vector<double> object2Vector(const DynamicObject object) {
    std::vector<DynamicObject> objects = *object.getArray();
    size_t objSize = objects.size();

    std::vector<double> values(objSize);
    parsync(long i = 0; i < (long) objSize; ++i)
        values[(size_t) i] = objects[(size_t) i].getNumber();

    return values;
}

std::vector<double> N8Util::VectorMath::add(
        std::vector<double> left,
        std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = left[i] + right[i];

    return result;
}

std::vector<double> N8Util::VectorMath::addSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = array[i] + value;

    return result;
}

std::vector<double> N8Util::VectorMath::sub(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = left[i] - right[i];

    return result;
}

std::vector<double> N8Util::VectorMath::subSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = array[i] - value;

    return result;
}

std::vector<double> N8Util::VectorMath::div(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = left[i] / right[i];

    return result;
}

std::vector<double> N8Util::VectorMath::divSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = array[i] / value;

    return result;
}

std::vector<double> N8Util::VectorMath::mul(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = left[i] * right[i];

    return result;
}

std::vector<double> N8Util::VectorMath::mulSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = array[i] * value;

    return result;
}

std::vector<double> N8Util::VectorMath::rem(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] % (long) right[i]);

    return result;
}

std::vector<double> N8Util::VectorMath::remSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = static_cast<double>(
            static_cast<long>(array[i]) %
                static_cast<long>(value)
        );

    return result;
}

std::vector<double> N8Util::VectorMath::bitwiseAnd(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] & (long) right[i]);

    return result;
}

std::vector<double> N8Util::VectorMath::bitwiseAndSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = static_cast<double>(
            static_cast<long>(array[i]) &
                static_cast<long>(value)
        );

    return result;
}

std::vector<double> N8Util::VectorMath::bitwiseOr(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] | (long) right[i]);

    return result;
}

std::vector<double> N8Util::VectorMath::bitwiseOrSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = static_cast<double>(
            static_cast<long>(array[i]) |
                static_cast<long>(value)
        );

    return result;
}

std::vector<double> N8Util::VectorMath::bitwiseXor(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] ^ (long) right[i]);

    return result;
}

std::vector<double> N8Util::VectorMath::bitwiseXorSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = static_cast<double>(
            static_cast<long>(array[i]) ^
                static_cast<long>(value)
        );

    return result;
}

std::vector<double> N8Util::VectorMath::shiftLeft(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] << (long) right[i]);

    return result;
}

std::vector<double> N8Util::VectorMath::shiftLeftSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = static_cast<double>(
            static_cast<long>(array[i]) <<
                static_cast<long>(value)
        );

    return result;
}

std::vector<double> N8Util::VectorMath::shiftRight(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);
    parsync(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] >> (long) right[i]);

    return result;
}

std::vector<double> N8Util::VectorMath::shiftRightSingle(
    double value,
    std::vector<double> array
) {
    size_t size = array.size();
    std::vector<double> result(size);

    parsync(size_t i = 0; i < size; ++i)
        result[i] = static_cast<double>(
            static_cast<long>(array[i]) >>
                static_cast<long>(value)
        );

    return result;
}

}
