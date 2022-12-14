#include <cstdio>
#include <vector>

using namespace std;

inline int CountBits(int v)
{
    int result = 0;
    while(v > 0)
    {
        result += (v & 1);
        v >>= 1;
    }
    return result;
}

int main()
{
    int n, i;
    vector<int> chain;
    int sum = 0;
    int removed = 0;
    
    scanf("%i", &n);
    
    i = 0;
    while(sum < n)
    {
        i++;
        sum += CountBits(i);
        chain.push_back(i);
    }
    
    while(sum > n)
    {
        i--;
        int bits = CountBits(i);
        if(sum - bits >= n)
        {
            removed++;
            sum -= bits;
            chain[i-1] = -1;
        }
    }
    
    printf("%i\n", (int)chain.size() - removed);
    
    bool first = true;
    for(i = chain.size() -1; i >= 0; i--)
    {
        if (chain[i] != -1)
        {
            if(first)
            {
                printf("%i", chain[i]);
                first = false;
            }
            else
            {
            printf(" %i", chain[i]);
            }
        }
    }

    printf("\n");
    
    return 0;
}