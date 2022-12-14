#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

void PrintPositiveResult(vector<int>& result)
{
    printf("TAK\n");

    bool first = true;
    for(int i = 0; i < result.size(); i++)
    {
        if(first)
        {
            printf("%i", result[i]);
            first = false;
        }
        else
        {
            printf(" %i", result[i]);
        }
    }
    printf("\n");
}

struct ValuePosition
{
    int value;
    int position;

    ValuePosition() : value(0), position(0) { }

    ValuePosition(int value, int position) : value(value), position(position) { }
};

bool operator < (const ValuePosition& vp1, const ValuePosition& vp2)
{
    return vp1.value < vp2.value;
}

void Case2(vector<int>& chain)
{
    priority_queue<ValuePosition> queue;
    int min, g;

    min = chain[0];
    for(int i = 1; i < chain.size(); i++)
    {
        queue.push(ValuePosition(chain[i], i+1));
    }

    g = 1;
    while(!queue.empty() && min < queue.top().value)
    {
        ValuePosition max = queue.top();
        queue.pop();

        while(g < max.position)
        {
            if(min > chain[g])
            {
                min = chain[g];
            }
            g++;
        }

        while(!queue.empty() && queue.top().position < g)
        {
            queue.pop();
        }
    }

    if (queue.empty() && chain.back() == min)
    {
        printf("TAK\n%i\n", chain.size()-1);
        //printf("TAK\n");
    }
    else if (queue.empty())
    {
        printf("NIE\n");
    }
    else
    {
        printf("TAK\n%i\n", g);
        //printf("TAK\n");
    }
}

void Case3(vector<int>& chain)
{
    int min, max;
    int minPos, maxPos;
    vector<int> result;

    min = __INT32_MAX__;
    max = -__INT32_MAX__;

    for (int i = 0; i < chain.size(); i++)
    {
        if(chain[i] <= min)
        {
            min = chain[i];
            minPos = i;
        }
        if(chain[i] > max)
        {
            max = chain[i];
            maxPos = i;
        }
    }

    minPos++;
    maxPos++;
    
    if(minPos == chain.size())
    {
        minPos--;
    }
    if(minPos > 1)
    {
        result.push_back(minPos - 1);
        result.push_back(minPos);
        PrintPositiveResult(result);
        return;
    }

    if(maxPos == 1)
    {
        maxPos++;
    }
    if(maxPos < chain.size())
    {
        result.push_back(maxPos - 1);
        result.push_back(maxPos);
        PrintPositiveResult(result);
        return;
    }

    printf("NIE\n");
}

void CaseK(vector<int>& chain, int k)
{
    int g = -1;
    vector<int> result;

    for (int i = 1; i < chain.size(); i++)
    {
        if (chain[i-1] >= chain[i])
        {
            g = i;
            break;
        }
    }

    if (g == -1)
    {
        printf("NIE\n");
        return;
    }
    
    if (g == chain.size() -1)
    {
        g--;
    }
    if (g > 1)
    {
        g--;
    }

    for(int i = 1; i < g && k > 4; i++)
    {
        result.push_back(i);
        k--;
    }
    result.push_back(g++);
    result.push_back(g++);
    result.push_back(g++);
    k -= 3;
    for(int i = g; k > 1; i++)
    {
        result.push_back(i);
        k--;
    }

    PrintPositiveResult(result);
}

int main()
{
    int k, n, a;
    vector<int> chain;

    scanf("%i%i", &n, &k);
    chain.reserve(n);

    for (int i = 0; i < n; i++)
    {
        scanf("%i", &a);
        chain.push_back(a);
    }

    if (k == 2)
    {
        Case2(chain);
    }
    else if (k == 3)
    {
        Case3(chain);
    }
    else
    {
        CaseK(chain, k);
    }

    return 0;
}
