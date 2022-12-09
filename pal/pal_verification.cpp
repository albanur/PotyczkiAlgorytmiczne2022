#include <map>
#include <string>

#include <chrono>

#include "pal.cpp"

using namespace std;

void addCharacter(map<string, int>& test, string& current, int toAdd)
{
    if(toAdd == 0)
    {
        test[current] = -1;
        return;
    }
    
    current.push_back('a');
    addCharacter(test, current, toAdd-1);
    current.pop_back();

    current.push_back('b');
    addCharacter(test, current, toAdd-1);
    current.pop_back();
}

bool isPalindrom(const string& text)
{
    int i = 0;
    int j = text.size() -1;

    while(i < j)
    {
        if(text[i] != text[j])
        return false;
        i++;
        j--;
    }

    return true;
}

bool canSwap(string& input, int pos)
{
    return input[pos] != input[pos-1];
}

string swap(string& input, int pos)
{
    string result = input;
    result[pos] = input[pos-1];
    result[pos-1] = input[pos];
    return result;
}

void measure_time()
{
    auto start = chrono::high_resolution_clock::now();

    //operation

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    printf("time: %lldms\n", duration.count());
}

int main()
{
    int testSize = 12;
    string testString;
    map<string, int> test;
    deque<pair<string,int>> toProcess;
    addCharacter(test, testString, testSize);
    
    for(const auto& kv : test)
    {
        if (isPalindrom(kv.first))
        {
            toProcess.push_back(pair<string,int>(kv.first, 0));
        }
    }

    for(int i = 0; i < toProcess.size(); i++)
    {
        test[toProcess[i].first] = toProcess[i].second;
    }

    while(toProcess.size() > 0)
    {
        pair<string,int> current = toProcess.front();
        toProcess.pop_front();

        for(int i = 1; i<current.first.size(); i++)
        {
            if(canSwap(current.first, i))
            {
                string next = swap(current.first, i);
                if(test[next] == -1)
                {
                    test[next] = current.second + 1;
                    toProcess.push_back(pair<string,int>(next, current.second + 1));
                }
            }
        }
    }

    //auto fixer = PalindromFixer("aaaaab");
    //int result = fixer.Score();

    for(const auto& kv : test)
    {
        auto fixer = PalindromFixer(kv.first.c_str());
        int result = fixer.Score();
        //int result = FixPalindrom(kv.first.c_str());

        if(result == kv.second)
        {
            printf("%s: %i (PASSED)\n",kv.first.c_str(), result);
        }
        else
        {
            printf("%s: %i <-> %i (FAILED)\n",kv.first.c_str(), kv.second, result);
        }
    }
    
    return 0;
}