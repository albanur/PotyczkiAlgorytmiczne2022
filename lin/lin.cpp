#include <cstdio>
#include <cinttypes>
#include <vector>
#include <map>
#include <queue>

using namespace std;

struct Node
{
    int idx;
    int length;
    Node(int i, int l) : idx(i), length(l) { }
};

bool operator < (const Node& n1, const Node& n2)
{
    return n1.length > n2.length;
}

int main()
{
    int n, k;
    vector<int> line;
    vector<vector<int>> edges;
    vector<map<int, int>> shortestPaths;

    scanf("%i", &n);
    line.resize(n+1);
    edges.resize(n+1);
    shortestPaths.resize(n+1);

    for(int i = 1; i <= n; i++)
    {
        scanf("%i", &k);
        line[i] = k;
    }

    for(int i = 1; i <= n; i++)
    {
        for(int j = i+1; j <= n; j++)
        {
            if(line[i]>line[j])
            {
                edges[i].push_back(j);
                edges[j].push_back(i);
            }
        }
    }

    for(int i = 1; i <= n; i++)
    {
        priority_queue<Node> queue;
        vector<bool> visited(n+1, false);

        queue.push(Node(i, 0));

        while(!queue.empty())
        {
            Node node = queue.top();
            queue.pop();

            if(visited[node.idx])
            {
                continue;
            }
            visited[node.idx] = true;

            shortestPaths[i][node.idx] = node.length;

            for(int nextNode : edges[node.idx])
            {
                if(!visited[nextNode])
                {
                    queue.push(Node(nextNode, node.length+1));
                }
            }
        }
    }

    for(int i = 1; i <= n; i++)
    {
        int64_t sum = 0;

        for(auto& item : shortestPaths[i])
        {
            sum+= item.second;
        }

        if (i == 1)
        {
            printf("%lld", sum);
        }
        else
        {
            printf(" %lld", sum);
        }
    }

    printf("\n");
}