#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;

const static int Divisor = 1000000007;

struct Item
{
    int nextId;
    int countP;

    Item() : nextId(0), countP(0) { }
    Item(int nextId, int countP) : nextId(nextId), countP(countP) { }
};

struct SearchItem
{
    int length;
    int startId;
    int balance;

    SearchItem(int length, int startId, int balance)
        : length(length), startId(startId), balance(balance) { }
};

bool operator < (const SearchItem& si1, const SearchItem& si2)
{
    return (si1.length < si2.length)
        || (si1.length == si2.length && si1.startId < si2.startId)
        || (si1.length == si2.length && si1.startId == si2.startId && si1.balance < si2.balance);
}

class DribbleCounter
{
private:
    string chain;
    vector<Item> positions;
    map<SearchItem, int> buffer;

public:
    DribbleCounter(const string& chain1, const string& chain2) : chain(chain1+chain2)
    {
        InitializePositions();
    }

    int64_t Calculate()
    {
        int64_t result = 0LL;

        for(int i = 2; i <= chain.size(); i+=2)
        {
            // if(i % 100 == 0)
            // {
            //     printf("->%i (%i)\n", i, buffer.size());
            //     fflush(stdout);
            // }
            int64_t value = CalculateN(SearchItem(i, 0, 0));
            if(value != 0LL)
            {
                result = (result + value) % Divisor;
            }
            else
            {
                break;
            }
        }

        buffer.clear();

        return result;
    }

private:
    void InitializePositions()
    {
        positions.resize(chain.size());

        positions[chain.size()-1].nextId = -1;
        positions[chain.size()-1].countP = chain.back() == 'P' ? 1 : 0;
        for(int i = chain.size()-2; i >=0; i--)
        {
            if(chain[i] == chain[i+1])
            {
                positions[i].nextId = positions[i+1].nextId;
            }
            else
            {
                positions[i].nextId = i+1;
            }
            positions[i].countP = positions[i+1].countP + (chain[i] == 'P' ? 1 : 0);
        }
    }

    int64_t CalculateN(SearchItem item)
    {
        if (HasCalculatedValue(item))
        {
            return GetCalculatedValue(item);
        }

        stack<SearchItem> stack;
        stack.push(item);

        while(!stack.empty())
        {
            SearchItem& si = stack.top();

            SearchItem lx = GetLX(si);
            SearchItem px = GetPX(si);

            bool calculatedLX = HasCalculatedValue(lx);
            bool calculatedPX = HasCalculatedValue(px);

            if(calculatedLX && calculatedPX)
            {
                int64_t value = (GetCalculatedValue(lx) + GetCalculatedValue(px)) % Divisor;
                buffer[si] = value;
                stack.pop();
            }
            else
            {
                if (!calculatedLX)
                {
                    stack.push(lx);
                }
                if (!calculatedPX)
                {
                    stack.push(px);
                }
            }
        }

        return GetCalculatedValue(item);
    }

    bool HasCalculatedValue(SearchItem& item)
    {
        if (!Validate(item) || item.length == 0)
        {
            return true;
        }

        return buffer.count(item) > 0;
    }

    int64_t GetCalculatedValue(SearchItem& item)
    {
        if (item.startId < 0)
        {
            return 0LL;
        }

        if (item.length == 0 && item.balance == 0)
        {
            return 1LL;
        }

        if (item.length + item.balance == 0)
        {
            return positions[item.startId].countP >= item.length ? 1LL : 0LL;
        }

        if (!Validate(item))
        {
            return 0LL;
        }

        return buffer[item];
    }

    SearchItem GetLX(SearchItem& item)
    {
        int idL = (chain[item.startId] == 'L') ? item.startId : positions[item.startId].nextId;
        if (idL == -1)
        {
            return SearchItem(item.length-1, -1, item.balance-1);
        }

        return SearchItem(item.length-1, idL+1, item.balance-1);
    }

    SearchItem GetPX(SearchItem& item)
    {
        int idP = (chain[item.startId] == 'P') ? item.startId : positions[item.startId].nextId;
        if (idP == -1)
        {
            return SearchItem(item.length-1, -1, item.balance+1);
        }

        return SearchItem(item.length-1, idP+1, item.balance+1);
    }

    bool Validate(SearchItem& item)
    {
        int countP = positions[item.startId].countP;
        int countL = chain.size() - item.startId - countP;
        int count = 2*min(countP + item.balance, countL);

        return item.length > 0
            && item.startId >= 0
            && item.startId + item.length <= chain.size()
            && item.balance <= 0
            && item.length + item.balance > 0
            && positions[item.startId].countP + item.balance >= 0
            && item.length + item.balance <= count;
    }
};

const char* ReadChain()
{
    static char buffer[1024];
    fgets(buffer, 1024, stdin);

    int len = strlen(buffer);

    if (buffer[len-1] == '\n')
    {
        buffer[len-1] = '\0';
    }

    return buffer;
}

int main()
{
    int n;
    vector<string> chains;

    scanf("%i\n", &n);
    for(int i = 0; i < n; i++)
    {
        chains.push_back(string(ReadChain()));
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            DribbleCounter counter(chains[i], chains[j]);
            int64_t result = counter.Calculate();
            if(j != 0) printf(" ");
            printf("%lld", result);
            fflush(stdout);
        }
        printf("\n");
    }
    DribbleCounter counter(chains[0], chains[0]);
    int64_t result = counter.Calculate();

    return 0;
}