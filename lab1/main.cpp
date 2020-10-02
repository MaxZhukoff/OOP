#include "IniParse.h"
using namespace std;

int main() {
    IniParse ini;
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
