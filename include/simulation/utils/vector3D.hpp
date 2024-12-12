#pragma once

#include <vector>


template <typename T>
class Vector3D {
public:
    constexpr Vector3D(const size_t sizeX, const size_t sizeY, const size_t sizeZ):
        sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ), data(sizeX*sizeY*sizeZ) {}

    constexpr T& At(const size_t x, const size_t y, const size_t z)
        { return data.at(sizeX*sizeY*z + sizeX*y + x); }

    constexpr const T& At(const size_t x, const size_t y, const size_t z) const
        { return data.at(sizeX*sizeY*z + sizeX*y + x); }

    [[nodiscard]]
    constexpr size_t SizeX() const
        { return sizeX; }

    [[nodiscard]]
    constexpr size_t SizeY() const
        { return sizeY; }

    [[nodiscard]]
    constexpr size_t SizeZ() const
        { return sizeZ; }

private:
    size_t sizeX;
    size_t sizeY;
    size_t sizeZ;
    std::vector<T> data;
};
