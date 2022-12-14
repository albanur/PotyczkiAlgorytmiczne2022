#include <cstdio>
#include <cinttypes>
#include <string>
#include <vector>

using namespace std;

void DrawTriangle(string& instructions, int64_t size);
void DrawParallelogram(string& instructions, int64_t sizeX, int64_t sizeY);
void DrawRightZigZag(string& instructions, int64_t size);
void DrawRight(string& instructions, int64_t size);
void DrawDownLeft(string& instructions, int64_t size);
void DrawLeft(string& instructions, int64_t size);
string RepeatPattern(string pattern, int64_t times);

void DrawTriangle1(string& instructions)
{
    instructions += "AE";
}

void DrawTriangle2(string& instructions)
{
    instructions += "AEACAEE";
}

void DrawTriangle(string& instructions, int64_t size)
{
    if (size == 0)
    {
        return;
    }
    if (size == 1)
    {
        DrawTriangle1(instructions);
        return;
    }
    if (size == 2)
    {
        DrawTriangle2(instructions);
        return;
    }

    int64_t sizeX, sizeY;
    sizeX = size / 2;
    sizeY = size - sizeX;
    if ((sizeY) % 2 == 1)
    {
        sizeY++;
        sizeX--;
    }

    DrawParallelogram(instructions, sizeX, sizeY);
    DrawRight(instructions, sizeX);
    DrawDownLeft(instructions, sizeY);
    DrawTriangle(instructions, sizeY);
    DrawLeft(instructions, sizeX);
    DrawTriangle(instructions, sizeX-1);
}

void DrawParallelogram(string& instructions, int64_t sizeX, int64_t sizeY)
{
    string pattern;
    DrawRightZigZag(pattern, sizeX);
    DrawLeft(pattern, sizeX);

    instructions += RepeatPattern(pattern, sizeY/2);
}

void DrawRightZigZag(string& instructions, int64_t size)
{
    instructions += RepeatPattern("AEAC", size-1);
    instructions += "AEA";
}

void DrawRight(string& instructions, int64_t size)
{
    instructions += RepeatPattern("A", size);
}

void DrawDownLeft(string& instructions, int64_t size)
{
    instructions += RepeatPattern("C", size);
}

void DrawLeft(string& instructions, int64_t size)
{
    instructions += RepeatPattern("EC", size-1);
    instructions += "E";
}

string SimpleRepeatPattern(string pattern, int times)
{
    string result;
    
    if(times == 0)
    {
        return result;
    }
    if(times == 1)
    {
        return pattern;
    }
    
    result += ('0' + times);
    if(pattern.size() == 1)
    {
        result += pattern;
    }
    else
    {
        result += '[' + pattern + ']';
    }

    return result;
}

string RepeatPattern(string pattern, int64_t times)
{
    string result;
    vector<int> polynomial;

    if (times == 0)
    {
        return result;
    }

    while(times > 0)
    {
        polynomial.push_back(times % 9);
        times /= 9;
    }

    if (polynomial.size() == 1)
    {
        return SimpleRepeatPattern(pattern, polynomial.back());
    }

    for(int i = 2; i < polynomial.size(); i++)
    {
        result += "9[";
    }

    if (polynomial.back() != 1)
    {
        result += "9[" + SimpleRepeatPattern(pattern, polynomial.back()) + "]";
    }
    else
    {
        result += SimpleRepeatPattern(pattern, 9);
    }

    for(int i = polynomial.size() - 2; i >= 0; i--)
    {
        result += (i == polynomial.size() - 2) ? "" : "]";
        result += SimpleRepeatPattern(pattern, polynomial[i]);
    }

    return result;
}

int main()
{
    int64_t size;
    string instructions;

    scanf("%lld", &size);
    
    DrawTriangle(instructions, size);
    DrawDownLeft(instructions, size);

    printf("%s\n", instructions.c_str());

    return 0;
}