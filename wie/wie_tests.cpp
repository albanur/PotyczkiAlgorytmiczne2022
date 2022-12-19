#include <chrono>
#include <cstdio>
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

int main()
{
    string path = "./in";
    for (const auto & entry : fs::directory_iterator(path))
    {
        string command = "wie.exe < ";
        command += entry.path().string();
        command += " 2>&1";
        
        string result;
        char buffer[128];

        auto start = chrono::high_resolution_clock::now();

        FILE* pipe = popen(command.c_str(), "r");
        while (!feof(pipe))
        {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);

        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        string output = "./out/";
        output += entry.path().filename().replace_extension(".out").string();

        string expected;
        FILE* file = fopen(output.c_str(), "r");
        while (!feof(file))
        {
            if (fgets(buffer, 128, file) != NULL)
                expected += buffer;
        }
        fclose(file);

        if(expected.compare(result) == 0)
        {
            printf("%s: %lldms - PASSED\n", entry.path().filename().string().c_str(), duration.count());
        }
        else
        {
            printf("%s: %lldms - FAILED\n", entry.path().filename().string().c_str(), duration.count());
        }
    }

    return 0;
}