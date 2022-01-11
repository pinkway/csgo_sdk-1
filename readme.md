# csgo_sdk

a csgo hack base written in c++20, compatible w msvc & clang-cl

## features
- easy to use n understand project structure & code
- the base is split into 2 "projects", one of which is a [vuln-sdk](https://github.com/vulnerable-development/sdk/) that can be used elsewhere
- compatibility w old csgo ver ( 8 march 2018 )
- byte_seq_t, cfg_var_t n other things present in vuln-sdk

## definitions
- ```SDK_CFG_ID_OBJECT``` - specifies the cfgs id object n folder name
- ```CSGO2018``` - specifies that you're building for the 2018 ver of csgo

## requirements
- [dx sdk](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
- [c++ redists ( optional )](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)

## thirdparty
- [imgui](https://github.com/ocornut/imgui)
- [json](https://github.com/nlohmann/json/)
- [minhook](https://github.com/TsudaKageyu/minhook)
- [vuln-sdk](https://github.com/vulnerable-development/sdk/)