#include <cstdio>
#include <cinttypes>
#include <vector>

using namespace std;

const static int Divisor = 1000000007;
const static int Inverse3 = 333333336;

struct TermionCounter
{
    int cCounter;
    int nCounter;
    int zCounter;

    TermionCounter()
    {
        cCounter = nCounter = zCounter = 0;
    }

    void Add(char t)
    {
        switch(t)
        {
            case 'C':
                cCounter++;
                break;
            case 'N':
                nCounter++;
                break;
            case 'Z':
                zCounter++;
                break;
        }
    }

    void Remove(char t)
    {
        switch(t)
        {
            case 'C':
                cCounter--;
                break;
            case 'N':
                nCounter--;
                break;
            case 'Z':
                zCounter--;
                break;
        }
    }

    void Update(char oldT, char newT)
    {
        Remove(oldT);
        Add(newT);
    }
};

struct ParityState
{
    bool cOdd;
    bool cEven;
    bool zOdd;
    bool zEven;

    ParityState() : cOdd (false), cEven(false), zOdd(false), zEven(false) { }

    ParityState(char termion, int index)
    {
        cOdd = (termion == 'C' && (index % 2) == 1);
        cEven = (termion == 'C' && (index % 2) == 0);
        zOdd = (termion == 'Z' && (index % 2) == 1);
        zEven = (termion == 'Z' && (index % 2) == 0);
    }

    ParityState(bool cOdd, bool cEven, bool zOdd, bool zEven)
        : cOdd (cOdd), cEven(cEven), zOdd(zOdd), zEven(zEven) { }

    static ParityState Combine(ParityState s1, ParityState s2)
    {
        return ParityState(s1.cOdd || s2.cOdd, s1.cEven || s2.cEven, s1.zOdd || s2.zOdd, s1.zEven || s2.zEven);
    }

    int CalculateCorrection()
    {
        if ((cOdd || cEven || zOdd || zEven) == false)
        {
            return -2;
        }
        else if ((cOdd && cEven) || (zOdd && zEven) || (cOdd && zOdd) || (cEven && zEven))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
};

struct ParityTree
{
    vector<ParityState> tree;
    int size;

    ParityTree(vector<char>& chain)
    {
        size = chain.size();
        tree.resize(2 * size - 1);

        for (int i = 0; i < size; i++)
        {
            tree[TreeIndex(i)] = ParityState(chain[i], i);
        }

        for (int i = TreeIndex(0) - 1; i >= 0; i--)
        {
            UpdateState(i);
        }
    }

    int CalculateCorrection()
    {
        if ((size % 2) == 0 || size == 1)
        {
            return 0;
        }

        return Top().CalculateCorrection();
    }

    ParityState& Top()
    {
        return tree[0];
    }

    void UpdateTermion(char termion, int index)
    {
        int i = TreeIndex(index);
        tree[i] = ParityState(termion, index);

        while(i > 0)
        {
            i = ParentIndex(i);
            UpdateState(i);
        }
        
    }

    inline void UpdateState(int parent)
    {
        tree[parent] = ParityState::Combine(LeftChild(parent), RightChild(parent));
    }

    inline int TreeIndex(int index) { return index + size - 1; }

    inline int ParentIndex(int child) { return (child - 1) / 2; }

    inline int LeftChildIndex(int parent) { return 2 * parent + 1; }

    inline int RightChildIndex(int parent) { return 2 * parent + 2; }

    inline ParityState& Parent(int child) { return tree[ParentIndex(child)]; }

    inline ParityState& LeftChild(int parent) { return tree[LeftChildIndex(parent)]; }

    inline ParityState& RightChild(int parent) { return tree[RightChildIndex(parent)]; }
};

int64_t CalculatePowerOf2(int n)
{
    int64_t value = 1;
    int64_t power2 = 2;

    while(n > 0)
    {
        if ((n % 2) == 1)
        {
            value = (value * power2) % Divisor;
        }

        n /= 2;
        power2 = (power2 * power2) % Divisor;
    }

    return value;
}

int64_t CalculateBinomialFactor(int n, int diff)
{
    int64_t power2 = CalculatePowerOf2(n);
    int64_t power2_3;
    int64_t result = 0;

    if((n % 2) == 0)
    {
        power2_3 = ((power2 + 2) * Inverse3) % Divisor;

        if((diff + 0) % 3 != 0)
        {
            result += power2_3;
        }
        if((diff + 2) % 3 != 0)
        {
            result += power2_3 - 1;
        }
        if((diff + 4) % 3 != 0)
        {
            result += power2_3 - 1;
        }
    }
    else
    {
        power2_3 = ((power2 + 1) * Inverse3) % Divisor;

        if((diff + 1) % 3 != 0)
        {
            result += power2_3;
        }
        if((diff + 3) % 3 != 0)
        {
            result += power2_3 - 1;
        }
        if((diff + 5) % 3 != 0)
        {
            result += power2_3;
        }
    }

    return result % Divisor;
}

int main()
{
    int n, q;
    vector<char> chain;
    int ki;
    char ti;
    TermionCounter termionCounter;

    scanf("%i %i\n", &n, &q);

    for(int i = 0; i < n; i++)
    {
        scanf("%c", &ti);
        chain.push_back(ti);
        termionCounter.Add(ti);
    }

    ParityTree parityTree(chain);
    int diff = termionCounter.cCounter - termionCounter.zCounter;
    int64_t result = CalculateBinomialFactor(termionCounter.nCounter, diff);
    result = (result + parityTree.CalculateCorrection()) % Divisor;
    printf("%lld\n", result);

    for(int i = 0; i < q; i++)
    {
        scanf("%i %c\n", &ki, &ti);
        ki--;

        char oldT = chain[ki];
        chain[ki] = ti;
        termionCounter.Update(oldT, ti);
        parityTree.UpdateTermion(ti, ki);

        int diff = termionCounter.cCounter - termionCounter.zCounter;
        int64_t result = CalculateBinomialFactor(termionCounter.nCounter, diff);
        result = (result + parityTree.CalculateCorrection()) % Divisor;
        printf("%lld\n", result);
    }

    return 0;
}