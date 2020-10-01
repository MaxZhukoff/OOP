#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class INI {
public:
    void read_file(const string &fileName, const string &format)
    {
        ifstream file;
        if (format != "ini")
        {
            cerr << "Неверный формат файла" << endl;
            exit(-1);
        }
        file.open(fileName + '.' + format);
        if (!file.is_open())
        {
            cerr << "Не удалось открыть файл" << endl;
            exit(-1);
        }
        string name, value;
        string section;
        while (!file.eof())
        {
            string buffer;
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
                        cerr << "Недопустимый символ в названии секции" << endl;
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
                        cerr << "Недопустимый символ в названии параметра" << endl;
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
                pair<string, string> param(name, value);
                pair<string, pair<string, string>> a(section, param);
                data.push_back(a);
            }
        }
        flag = true;
        file.close();
    }

    string get_value(const string &section, const string &param, const string &type) const
    {
        if (!flag)
        {
            cerr << "Нет данных для анализа";
            exit(-1);
        }
        if (type == "string")
        {
            return check_string(section, param);
        }
        if (type == "int")
        {
            return check_int(section, param);
        }
        if (type == "float")
        {
            return check_float(section, param);
        }
        cerr << "Неверный тип данных " << section << " " << param << endl;
        exit(-1);
    }

private:
    string find(const string &section, const string &param) const
    {
        for (auto it = data.begin (); it != data.end (); it++)
        {
            if (it->first == section && it->second.first == param) {
                return it->second.second;
            }
        }
        return "#NO_RESULT#";
    }


    string check_string(const string &section, const string &param) const {
        string result = this->find(section, param);
        if (result == "#NO_RESULT#") {
            cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << endl;
            exit(-1);
        } else {
            for (int i = 0; i < result.length(); i++) {
                if (!(result[i] >= 65 && result[i] <= 90) && !(result[i] >= 97 && result[i] <= 122) &&
                (result[i] != '_') && (result[i] != '.') && result[i] != '/')
                {
                    cerr << "Неверный тип данных " << section << " " << param << endl;
                    exit(-1);
                }
            }
            return result;
        }
    }

    string check_int(const string &section, const string &param) const {
        string result = this->find(section, param);
        if (result == "#NO_RESULT#") {
            cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << endl;
            exit(-1);
        } else {
            for (int i = 0; i < result.length(); i++) {
                if (!(result[i] >= 48 && result[i] <= 57)) {
                    cerr << "Неверный тип данных " << section << " " << param << endl;
                    exit(-1);
                }
            }
            return result;
        }
    }

    string check_float(const string &section, const string &param) const {
        string result = this->find(section, param);
        if (result == "#NO_RESULT#") {
            cerr << "Заданной пары секция " << section << " параметр " << param << " нет в конфигурационном файле" << endl;
            exit(-1);
        } else {
            int check_float = 0;
            for (int i = 0; i < result.length(); i++) {
                if (result[i] == '.')
                    check_float++;
                if (!(result[i] >= 48 && result[i] <= 57) && result[i] != '.') {
                    cerr << "Неверный тип данных " << section << " " << param << endl;
                    exit(-1);
                }
            }
            if (check_float != 1)
            {
                cerr << "Неверный тип данных " << section << " " << param << endl;
                exit(-1);
            }
            else
                return result;
        }
    }

    bool flag;
    vector<pair<string, pair<string, string>>> data;
};


int main() {
    INI ini;
    /*cout << "Введите название и формат файла" << endl;
    string fileName, format;
    cin >> fileName >> format;
    ini.read_file(fileName, format);
    cout << "Введите название секции, название параметра и требуемый тип данных(int, float, string)" << endl;
    string section, param, type;
    cin >> section >> param >> type;
    cout << ini.get_value(section, param, type) << endl;*/
    ini.read_file("file", "ini");
    cout << ini.get_value("COMMON", "StatisterTimeMs", "int") << endl;
    cout << ini.get_value("COMMON", "LogNCMD", "int") << endl;
    cout << ini.get_value("COMMON", "LogXML", "int") << endl;
    cout << ini.get_value("COMMON", "DiskCachePath", "string") << endl;
    cout << ini.get_value("COMMON", "OpenMPThreadsCount", "int") << endl;
    cout << ini.get_value("ADC_DEV", "BufferLenSeconds", "float") << endl;
    cout << ini.get_value("ADC_DEV", "SampleRate", "float") << endl;
    cout << ini.get_value("ADC_DEV", "Driver", "string") << endl;
    cout << ini.get_value("NCMD", "EnableChanellControl", "int") << endl;
    cout << ini.get_value("NCMD", "SampleRate", "float") << endl;
    cout << ini.get_value("NCMD", "TidPacketVersionForTidControlCommand", "int") << endl;
    cout << ini.get_value("LEGACY_XML", "ListenTcpPort", "int") << endl;
    cout << ini.get_value("Debug", "PlentySockMaxQSize", "int") << endl;
    return 0;
}
