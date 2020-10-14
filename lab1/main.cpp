#include "IniParse.h"
using namespace std;

int main() {
    IniParse ini;
    if (ini.read_file("file", "ini") == GOOD) {

        std::optional<int> result = ini.getInt("COMMON", "StatisterTimeMs");
        if (result)
            cout << result.value() << endl;

        std::optional<float> result1 = ini.getFloat("ADC_DEV", "BufferLenSeconds");
        if (result)
            cout << result1.value() << endl;

        std::optional<string> result2 = ini.getString("ADC_DEV", "Driver");
        if (result)
            cout << result2.value() << endl;

//        ini.getInt("COMMON", "LogNCMD")
//        ini.getInt("COMMON", "LogXML")
//        ini.getString("COMMON", "DiskCachePath")
//        ini.getInt("COMMON", "OpenMPThreadsCount")
//        ini.getFloat("ADC_DEV", "BufferLenSeconds")
//        ini.getFloat("ADC_DEV", "SampleRate")
//        ini.getString("ADC_DEV", "Driver")
//        ini.getInt("NCMD", "EnableChanellControl")
//        ini.getFloat("NCMD", "SampleRate")
//        ini.getInt("NCMD", "TidPacketVersionForTidControlCommand")
//        ini.getInt("LEGACY_XML", "ListenTcpPort")
//        ini.getInt("Debug", "PlentySockMaxQSize")
    }
    return 0;
}
