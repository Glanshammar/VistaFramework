#pragma once

#include <VObject>
#include <cstdio>

class VFile : public VObject {
private:
    FILE* file{};
    std::optional<std::string> filename_;
    std::optional<std::string> mode_;
public:
    explicit VFile(const std::optional<std::string>& filename = std::nullopt, const std::optional<std::string>& mode = "r");
    ~VFile();
    static void LoadFile(const string& filePath, const char* mode);
};
