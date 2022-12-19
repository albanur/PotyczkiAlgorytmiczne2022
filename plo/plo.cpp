#include <cstdio>
#include <cinttypes>
#include <algorithm>
#include <vector>

using namespace std;

class SetUnion
{
private:
    int* parent;
    int* rank;
    int size;
    int sets;

public:
    SetUnion(int size)
    {
        this->size = size+1;
        
        parent = new int[this->size];
        rank = new int[this->size];

        MakeSet();
    }

    ~SetUnion()
    {
        delete[] parent;
        delete[] rank;
    }

    void MakeSet()
    {
        sets = 0;

        for(int i = 0; i < this->size; i++)
        {
            parent[i] = 0;
            rank[i] = 0;
        }
    }

    int GetSets()
    {
        return sets;
    }

    void AddSet(int i)
    {
        if (parent[i] == 0)
        {
            parent[i] = i;
            sets++;
        }
    }

    int FindSet(int i)
    {
        if (i == parent[i])
        {
            return i;
        }

        return parent[i] = FindSet(parent[i]);
    }

    void UnionSets(int i, int j)
    {
        if (parent[i] == 0 || parent[j] == 0)
        {
            return;
        }

        i = FindSet(i);
        j = FindSet(j);

        if (i != j)
        {
            if (rank[i] < rank[j])
            {
                swap(i,j);
            }

            parent[j] = i;
            sets--;

            if (rank[i] == rank[j])
            {
                rank[i]++;
            }
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

    SetUnion sets(2*n);

    for(int i = 1; i <= 2*n; i++)
    {
        sets.MakeSet();

        for(int j = i; j <= 2*n; j++)
        {
            sets.AddSet(j);
            sets.UnionSets(j, position[j].nl);
            sets.UnionSets(j, position[j].nr);
            sets.UnionSets(j, position[j].nv);

            if(sets.GetSets() <= k)
            {
                curiosity[sets.GetSets()]++;
            }
        }
    }

    for(int i = 1; i <= k; i++)
    {
        if(i == 1) printf("%lld", curiosity[i]);
        else printf(" %lld", curiosity[i]);
    }

    printf("\n");

    return 0;
}