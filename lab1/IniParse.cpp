#include "IniParse.h"

void IniParse::read_file(const std::string &fileName, const std::string &format)
{
    std::ifstream file;
    if (format != "ini")
    {
        std::cerr << "Неверный формат файла" << std::endl;
        exit(-1);
    }
    file.open(fileName + '.' + format);
    if (!file.is_open())
    {
        std::cerr << "Не удалось открыть файл" << std::endl;
        exit(-1);
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
                    exit(-1);
                }
                section += buffer[i];
            }
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
                    exit(-1);
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
            std::pair<std::string, std::string> param(name, value);
            std::pair<std::string, std::pair<std::string, std::string>> data_now(section, param);
            data.push_back(data_now);
        }
    }
    flag = true;
    file.close();
}

std::string IniParse::get_value(const std::string &section, const std::string &param, const std::string &type) const
{
    if (!flag)
    {
        std::cerr << "Нет данных для анализа";
        exit(-1);
    }
    if (type == "string")
        return check_string(section, param);
    if (type == "int")
        return check_int(section, param);
    if (type == "float")
        return check_float(section, param);
    std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
    exit(-1);
}


std::string IniParse::find(const std::string &section, const std::string &param) const
{
    for (auto it = data.begin (); it != data.end (); it++)
    {
        if (it->first == section && it->second.first == param) {
            return it->second.second;
        }
    }
    return "#NO_RESULT#";
}


std::string IniParse::check_string(const std::string &section, const std::string &param) const {
    std::string result = this->find(section, param);
    if (result == "#NO_RESULT#") {
        std::cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << std::endl;
        exit(-1);
    } else {
        for (int i = 0; i < result.length(); i++) {
            if (!(result[i] >= 65 && result[i] <= 90) && !(result[i] >= 97 && result[i] <= 122) &&
                (result[i] != '_') && (result[i] != '.') && result[i] != '/')
            {
                std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
                exit(-1);
            }
        }
        return result;
    }
}

std::string IniParse::check_int(const std::string &section, const std::string &param) const {
    std::string result = this->find(section, param);
    if (result == "#NO_RESULT#") {
        std::cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << std::endl;
        exit(-1);
    } else {
        for (int i = 0; i < result.length(); i++) {
            if (!(result[i] >= 48 && result[i] <= 57)) {
                std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
                exit(-1);
            }
        }
        return result;
    }
}

std::string IniParse::check_float(const std::string &section, const std::string &param) const {
    std::string result = this->find(section, param);
    if (result == "#NO_RESULT#")
    {
        std::cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << std::endl;
        exit(-1);
    } else {
        int check_float = 0;
        for (int i = 0; i < result.length(); i++) {
            if (result[i] == '.')
                check_float++;
            if (!(result[i] >= 48 && result[i] <= 57) && result[i] != '.')
            {
                std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
                exit(-1);
            }
        }
        if (check_float != 1)
        {
            std::cerr << "Неверный тип данных " << section << " " << param << std::endl;
            exit(-1);
        }
        else
            return result;
    }
}