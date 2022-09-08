#include <fstream>
#include <streambuf>
#include <string>
#include <vector>

#include <qstring.h>

inline std::string read_file(const char* file_name){
    auto file = std::ifstream(file_name);
    return std::string ((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
}


auto map(auto&& container,auto&& func) {
    std::vector<decltype(func(container[0]))> ret_vec;

    for(auto&& item : container){
        ret_vec.push_back(func(item));
    }

    return ret_vec;
}

inline std::string qstring_to_stdstring(const QString& qstr) {
    return std::string(qstr.toLocal8Bit().data());
}