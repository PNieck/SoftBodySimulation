#pragma once

#include <string>
#include <optional>
#include <vector>


class MeshLoader {
public:
    static std::tuple<std::vector<float>, std::vector<uint32_t>> LoadWithNormals(const std::string& filepath);
};
