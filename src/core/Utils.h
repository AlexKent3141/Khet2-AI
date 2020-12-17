#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <string>

namespace Utils
{
    // Split by character.
    std::vector<std::string> Split(
        const std::string&,
        char);

    // Split by characters.
    std::vector<std::string> Split(
        const std::string&,
        const std::string& delims);
}

#endif // __UTILS_H__
