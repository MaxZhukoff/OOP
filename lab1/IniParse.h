#ifndef LAB_1_INIPARSE_H
#define LAB_1_INIPARSE_H

#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <map>
#include <regex>

enum file_processing {
    GOOD,
    WRONG_FORMAT,
    NOT_OPEN,
    BAD_GRAMMAR
};

class IniParse
{
public:
    file_processing read_file(const std::string &fileName, const std::string &format);
    std::optional<std::string> getString(const std::string &section, const std::string &param) const;
    std::optional<int> getInt(const std::string &section, const std::string &param) const;
    std::optional<double> getFloat(const std::string &section, const std::string &param) const;

private:
    std::string find(const std::string &section, const std::string &param) const;

    std::map<std::string, std::map<std::string, std::string>> data;
};

#endif
