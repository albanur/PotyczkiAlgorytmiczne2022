#include <cstdio>
#include <cinttypes>
#include <algorithm>
#include <vector>

using namespace std;

struct Node
{
    int64_t sumOfCuriosity[11];
    int value;
    bool marked;

    Node()
    {
        for(int i = 0; i < 11; i++)
        {
            sumOfCuriosity[i] = 0;
        }
        value = 0;
        marked = false;
    }
};

class SegmentTree
{
private:
    Node* node;
    int size;

public:
    SegmentTree(int size)
    {
        this->size = size;
        node = new Node[4*size];
    }

    ~SegmentTree()
    {
        delete[] node;
    }

    // int sum(int v, int tl, int tr, int l, int r) {
    //     if (l > r) 
    //         return 0;
    //     if (l == tl && r == tr) {
    //         return t[v];
    //     }
    //     int tm = (tl + tr) / 2;
    //     return sum(v*2, tl, tm, l, min(r, tm))
    //         + sum(v*2+1, tm+1, tr, max(l, tm+1), r);
    // }

    int64_t SumOfK(int k)
    {
        return SumOfK(1, 1, size, 1, size, k);
    }

    int64_t SumOfK(int from, int to, int k)
    {
        return SumOfK(1, 1, size, from, to, k);
    }

    void UpdateValue(int index, int value)
    {
        UpdateValue(1, 1, size, index, value);
    }

    void UpdateRange(int from, int to, int delta)
    {
        UpdateRange(1, 1, size, from, to, delta);
    }

private:
    void Push(int current)
    {
        if (node[current].marked)
        {
            Node& left = node[2*current];
            Node& right = node[2*current+1];

            left.value += node[current].value;
            right.value += node[current].value;
            node[current].value = 0;

            left.marked = right.marked = true;
            node[current].marked = false;
        }
    }

    void UpdateSum(int current)
    {
        Node& parent = node[current];
        Node& left = node[2*current];
        Node& right = node[2*current+1];

        for(int k = 0; k < 11; k++)
        {
            int corrected = k - left.value;
            parent.sumOfCuriosity[k] = 
                (0 < corrected && corrected <= 10) ? left.sumOfCuriosity[corrected] : 0LL;
        }

        for(int k = 0; k < 11; k++)
        {
            int corrected = k - right.value;
            parent.sumOfCuriosity[k] += 
                (0 < corrected && corrected <= 10) ? right.sumOfCuriosity[corrected] : 0LL;
        }
    }

    int64_t SumOfK(int current, int low, int high, int from, int to, int k)
    {
        if (from > to)
        {
            return 0LL;
        }
        if (from == low && to == high)
        {
            int corrected = k - node[current].value;
            return (0 < corrected && corrected <= 10) ? node[current].sumOfCuriosity[corrected] : 0LL;
        }

        Push(current);
        int mid = (low + high) / 2;
        return SumOfK(2*current, low, mid, from, min(to, mid), k)
            + SumOfK(2*current+1, mid+1, high, max(from, mid+1), to, k);

    }

    void UpdateValue(int current, int low, int high, int index, int value)
    {
        if (low == high)
        {
            Node& item = node[current];
            if(item.value <= 10)
            {
                item.sumOfCuriosity[item.value]--;
            }
            //item.value = value;
            if(value <= 10)
            {
                item.sumOfCuriosity[value]++;
            }
        }
        else
        {
            Push(current);
            int mid = (low + high) / 2;
            if (index <= mid)
            {
                UpdateValue(2*current, low, mid, index, value);
            }
            else
            {
                UpdateValue(2*current+1, mid+1, high, index, value);
            }
            UpdateSum(current);
        }
    }

    void UpdateRange(int current, int low, int high, int from, int to, int delta)
    {
        if (from > to)
        {
            return;
        }
        if (from == low && to == high)
        {
            node[current].value += delta;
            node[current].marked = true;
        }
        else
        {
            Push(current);
            int mid = (low + high) / 2;
            UpdateRange(2*current, low, mid, from, min(to, mid), delta);
            UpdateRange(2*current+1, mid+1, high, max(from, mid+1), to, delta);
            UpdateSum(current);
        }
    }
};

struct Position
{
    int x;
    int y;
    Position() : x(-1), y(-1) { }
    Position(int x, int y) : x(x), y(y) { }
    int nl;
    int nr;
    int nv;
};

int main()
{
    int n, k, c;
    int color[100000][2];
    vector<Position> position;
    int64_t curiosity[11];

    for(int i = 0; i <= 10; i++)
    {
        curiosity[i] = 0;
    }

    scanf("%i%i", &n, &k);
    position.resize(2*n + 1);

    for(int j = 0; j < 2; j++)
    {
        for(int i = 0; i < n; i++)
        {
            scanf("%i", &c);
            color[i][j] = c;
            position[c] = Position(i,j);
        }
    }

    for(int j = 0; j < 2; j++)
    {
        for(int i = 0; i < n; i++)
        {
            c = color[i][j];
            position[c].nl = color[(i+n-1)%n][j];
            position[c].nr = color[(i+1)%n][j];
            position[c].nv = color[i][1-j];
        }
    }

    SegmentTree tree(2*n);
    tree.UpdateValue(2*n, 1);
    curiosity[1] = 1;

    for(int i = 2*n-1; i >= 1; i--)
    {
        Position& current = position[i];

        tree.UpdateValue(i, 1);

        int c1 = current.nl;
        int c2 = current.nr;
        int c3 = current.nv;

        if (c2 > c1) swap (c2, c1);
        if (c3 > c2) swap (c3, c2);
        if (c2 > c1) swap (c2, c1);

        if (i > c3) // lonely island
        {
            tree.UpdateRange(i+1, 2*n, 1);
        }
        else if (i > c2) // i only lower than c3
        {
            // nothing to update, i is attached to existing area
        }
        else if (i > c1) // i lower than c2 and c3
        {
            if (c2 == current.nv || c3 == current.nv)
            {
                int e;
                if (c2 == current.nl || c2 == current.nr)
                {
                    e = color[position[c2].x][1-position[c2].y];
                }
                if (c3 == current.nl || c3 == current.nr)
                {
                    e = color[position[c3].x][1-position[c3].y];
                }

                if(e < i)
                {
                    tree.UpdateRange(c3, 2*n, -1);
                }
                else if (e < c3)
                {
                    // nothing to update, i is attached to existing area
                }
                else
                {
                    tree.UpdateRange(c3, e-1, -1);
                }
            }
            else // i will merge 2 areas
            {
                tree.UpdateRange(c3, 2*n, -1);
            }
        }
        else // i lower than c1, c2 and c3
        {
            int e1 = color[(position[i].x+n-1)%n][1-position[i].y];
            int e2 = color[(position[i].x+1)%n][1-position[i].y];

            if (e1 < i && e2 < 1) // i will merge 2 or 3 areas
            {
                tree.UpdateRange(c2, c3-1, -1);
                tree.UpdateRange(c3, 2*n, -2);
            }
            if (e1 < i && e2 > c3)
            {
                tree.UpdateRange(c2, c3-1, -1);
                tree.UpdateRange(c3, e2-1, -2);
                tree.UpdateRange(e2, 2*n, -1);
            }
            if (e2 < i && e1 > c3)
            {
                tree.UpdateRange(c2, c3-1, -1);
                tree.UpdateRange(c3, e1-1, -2);
                tree.UpdateRange(e1, 2*n, -1);
            }
            if (e1 > c3 && e2 > c3)
            {
                if(e1 > e2) swap(e1, e2);
                tree.UpdateRange(c2, c3-1, -1);
                tree.UpdateRange(c3, e1-1, -2);
                tree.UpdateRange(e1, e2-1, -1);
            }
        }

        for(int i = 1; i <= 10; i++)
        {
            curiosity[i] += tree.SumOfK(i);
        }
    }

    for(int i = 1; i <= k; i++)
    {
        // if(i == 1) printf("%lld", curiosity[i]);
        // else printf(" %lld", curiosity[i]);
        printf("%lld ", curiosity[i]);
    }

    printf("\n");

    return 0;
}