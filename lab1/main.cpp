#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


class INI {
public:
    INI()
    {
    }

    void Read_file()
    {
        string name, value;
        string section;
        file.open("file.ini");
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
                    section += buffer[i];
                }
            }
            else if (buffer.length() > 2)
            {
                name = value = "";
                int i = 0;
                while (buffer[i] != ' ' && buffer[i] != '=' && buffer[i] != ';')
                {
                    name += buffer[i++];
                }
                while (buffer[i] == ' ' || buffer[i] == '=')
                    i++;
                for (; i < buffer.length(); i++) {
                    if (buffer[i] == ';')
                        break;
                    value += buffer[i];
                }
                pair<string, string> param(name, value);
                pair<string, pair<string, string>> a(section, param);
                tripleData.push_back(a);
            }
        }
        file.close();
    }

    void Find(string section, string param)
    {
        for (auto it = tripleData.begin (); it != tripleData.end (); it++) {
            if (it->first == section) {
                if (it->second.first == param) {
                    cout << it->second.second;
                }
            }
        }
    }

private:
    ifstream file;
    vector<pair<string, pair<string, string>>> tripleData;
};


int main() {
    INI ini;
    ini.Read_file();
    ini.Find("COMMON", "LogNCMD");
    return 0;
}
