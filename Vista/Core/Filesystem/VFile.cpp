#include "VFile.hpp"

VFile::VFile(const std::optional<std::string>& filename, const std::optional<std::string>& mode)
    : filename_(filename), mode_(mode) {
    cout << "VFile created with filename: " << (filename_ ? *filename_ : "default")
              << " and mode: " << (mode_ ? *mode_ : "default") << std::endl;
}

VFile::~VFile() {
    cout << "VFile destroyed" << std::endl;
}

void VFile::LoadFile(const string& filePath, const char* mode)
{
    FILE* file;
}