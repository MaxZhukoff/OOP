#include "IniParse.h"

file_processing IniParse::read_file(const std::string &fileName, const std::string &format)
{
    std::map<std::string, std::string> data_now;
    std::ifstream file;
    if (format != "ini")
    {
        std::cerr << "Неверный формат файла" << std::endl;
        return WRONG_FORMAT;
    }
    file.open(fileName + '.' + format);
    if (!file.is_open())
    {
        std::cerr << "Не удалось открыть файл" << std::endl;
        return NOT_OPEN;
    }
    std::string name, value;
    std::string section;
    while (!file.eof())
    {
        std::string buffer;
        getline(file, buffer);
        if (buffer[0] == ';')
            continue;
        if (buffer[0] == '[')
        {
            section = "";
            for (int i = 1; i < buffer.length() - 1; i++) {
                if (!(buffer[i] >= 48 && buffer[i] <= 57) && !(buffer[i] >= 65 && buffer[i] <= 90) &&
                    !(buffer[i] >= 97 && buffer[i] <= 122) && (buffer[i] != '_'))
                {
                    std::cerr << "Недопустимый символ в названии секции" << std::endl;
                    file.close();
                    return BAD_GRAMMAR;
                }
                section += buffer[i];
            }
            data_now.clear();
            continue;
        }
        else if (buffer[0] != '\0')
        {
            name = value = "";
            int i = 0;
            while (buffer[i] != ' ' && buffer[i] != '=' && buffer[i] != ';')
            {
                if (!(buffer[i] >= 48 && buffer[i] <= 57) && !(buffer[i] >= 65 && buffer[i] <= 90) &&
                    !(buffer[i] >= 97 && buffer[i] <= 122) && (buffer[i] != '_'))
                {
                    std::cerr << "Недопустимый символ в названии параметра" << std::endl;
                    file.close();
                    return BAD_GRAMMAR;
                }
                name += buffer[i++];
            }
            while (buffer[i] == ' ' || buffer[i] == '=')
                i++;
            for (; i < buffer.length(); i++) {
                if (buffer[i] == ';' || buffer[i] == ' ')
                    break;
                value += buffer[i];
            }
            if (!section.empty() || !name.empty() || !value.empty()) {
                auto it = data_now.emplace(name, value);
                if (it.second)
                    data[section][name] = value;
                else
                    std::cerr << "Есть повторяющиеся имена параметров в одной секции!";
            }
        }
    }
    file.close();
    return GOOD;
}

std::string IniParse::find(const std::string &section, const std::string &param) const
{
    auto it_section = data.find(section);
    if (it_section == data.end())
        return "";
    auto it_param = it_section->second.find(param);
    if (it_param == it_section->second.end())
        return "";
    return it_param->second;
}


std::optional<std::string> IniParse::getString(const std::string &section, const std::string &param) const {
    std::string result = this->find(section, param);
    if (result.empty()) {
        std::cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << std::endl;
        return { };
    } else {
        for (int i = 0; i < result.length(); i++) {
            if (!(result[i] >= 65 && result[i] <= 90) && !(result[i] >= 97 && result[i] <= 122) &&
                (result[i] != '_') && (result[i] != '.') && result[i] != '/')
            {
                std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
                return { };
            }
        }
        return result;
    }
}

std::optional<int> IniParse::getInt(const std::string &section, const std::string &param) const {
    std::string result = this->find(section, param);
    if (result.empty()) {
        std::cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << std::endl;
        return { };
    } else {
        for (int i = 0; i < result.length(); i++) {
            if (!(result[i] >= 48 && result[i] <= 57)) {
                std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
                return { };
            }
        }
        return {std::stoi(result)};
    }
}

std::optional<double> IniParse::getFloat(const std::string &section, const std::string &param) const {
    std::string result = this->find(section, param);
    if (result.empty())
    {
        std::cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << std::endl;
        return { };
    } else {
        int check_float = 0;
        for (int i = 0; i < result.length(); i++) {
            if (result[i] == '.')
                check_float++;
            if (!(result[i] >= 48 && result[i] <= 57) && result[i] != '.')
            {
                std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
                return { };
            }
        }
        if (check_float != 1)
        {
            std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
            return { };
        }
        else
            return std::stod(result);
    }
}
