#ifndef LAB_1_INIPARSE_H
#define LAB_1_INIPARSE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class IniParse
{
public:
    void read_file(const std::string &fileName, const std::string &format);
    std::string get_value(const std::string &section, const std::string &param, const std::string &type) const;

private:
    std::string find(const std::string &section, const std::string &param) const;
    std::string check_string(const std::string &section, const std::string &param) const;
    std::string check_int(const std::string &section, const std::string &param) const;
    std::string check_float(const std::string &section, const std::string &param) const;

    bool flag = false;
    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> data;
};

#endif //LAB_1_INIPARSE_H