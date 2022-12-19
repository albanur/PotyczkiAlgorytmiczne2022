#include <cstdio>
#include <vector>
#include <algorithm>
#include <deque>

using namespace std;

struct ValPos
{
    int value;
    int position;

    ValPos(int value, int position) : value(value), position(position) { }
};

bool operator < (const ValPos& vp1, const ValPos& vp2)
{
    return vp1.value < vp2.value;
}

int main()
{
    int n, h;
    vector<int> chain;
    vector<deque<int>> rounds;

    scanf("%i", &n);
    for(int i = 0; i < n; i++)
    {
        scanf("%i", &h);
        chain.push_back(h);
    }

    while(true)
    {
        vector<ValPos> sorted;
        vector<bool> verified;
        deque<int> swaps;
        deque<int> cycle;
        int maxCycle;

        for(int i = 0; i < n; i++)
        {
            sorted.push_back(ValPos(chain[i], i));
            verified.push_back(false);
        }
        sort(sorted.begin(), sorted.end());

        for(int i = 0; i < n; i++)
        {
            if(verified[i])
            {
                continue;
            }

            cycle.clear();
            int j = i;
            do
            {
                verified[j] = true;
                cycle.push_back(j);
                j = sorted[j].position;
            } while (i != j);

            while(cycle.size() > 1)
            {
                swaps.push_back(cycle.back());
                cycle.pop_back();
                swaps.push_front(cycle.front());
                cycle.pop_front();
            }
        }

        if(swaps.empty())
        {
            break;
        }

        rounds.push_back(swaps);

        for (int i = 0, j = swaps.size() - 1; i < j; i++, j--)
        {
            swap(chain[swaps[i]], chain[swaps[j]]);
        }
    }

    printf("%i\n", rounds.size());
    for(int i = 0; i < rounds.size(); i++)
    {
        printf("%i\n", rounds[i].size());
        bool first = true;
        for(int j = 0; j < rounds[i].size(); j++)
        {
            if(first)
            {
                printf("%i", rounds[i][j]+1);
                first = false;
            }
            else
            {
                printf(" %i", rounds[i][j]+1);
            }
        }
        printf("\n");
    }

    return 0;
}