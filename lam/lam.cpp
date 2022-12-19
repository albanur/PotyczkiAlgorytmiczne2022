#include <cstdio>
#include <cstring>
#include <deque>
#include <vector>

using namespace std;

inline bool IsVertical(char c)
{
    return c == 'G' || c == 'D';
}

inline bool IsHorizontal(char c)
{
    return c == 'L' || c == 'P';
}

inline bool SameDirection(char c1, char c2)
{
    return (IsVertical(c1) && IsVertical(c2)) || (IsHorizontal(c1) && IsHorizontal(c2));
}

void CompressCommands(vector<char>& commands)
{
    vector<char> compressed;

    compressed.push_back('O');
    compressed.push_back('O');

    for(int i = 0; i < commands.size(); i++)
    {
        if (SameDirection(compressed[compressed.size()-1], commands[i]))
        {
            compressed.pop_back();
            //compressed.push_back(commands[i]);
        }
        if (compressed[compressed.size()-2] != commands[i])
        {
            compressed.push_back(commands[i]);
        }
    }

    commands.clear();

    for(int i = 2; i < compressed.size(); i++)
    {
        commands.push_back(compressed[i]);
    }
}

struct Item
{
    char type;
    bool visited;
    int startX;
    int startY;
    //int cycleX;
    //int cycleY;

    Item() : type('\0'), visited(false), startX(-1), startY(-1)/*, cycleX(-1), cycleY(-1)*/ { }
    Item(char type) : type(type), visited(false), startX(-1), startY(-1)/*, cycleX(-1), cycleY(-1)*/ { }
};

inline bool IsTile(char t)
{
    return t == 'B' || t == 'C';
}

void ExecuteUp(vector<vector<Item>>& board, int n, int m)
{
    for(int i = 0; i < m; i++)
    {
        int k = 0;
        for(int j = 0; j < n; j++)
        {
            if (IsTile(board[i][j].type))
            {
                Item q = board[i][j];
                board[i][j] = board[i][k];
                board[i][k] = q;
                k++;
            }
        }
    }
}

void ExecuteDown(vector<vector<Item>>& board, int n, int m)
{
    for(int i = 0; i < m; i++)
    {
        int k = n-1;
        for(int j = n-1; j >= 0; j--)
        {
            if (IsTile(board[i][j].type))
            {
                Item q = board[i][j];
                board[i][j] = board[i][k];
                board[i][k] = q;
                k--;
            }
        }
    }
}

void ExecuteLeft(vector<vector<Item>>& board, int n, int m)
{
    for(int j = 0; j < n; j++)
    {
        int k = 0;
        for(int i = 0; i < m; i++)
        {
            if (IsTile(board[i][j].type))
            {
                Item q = board[i][j];
                board[i][j] = board[k][j];
                board[k][j] = q;
                k++;
            }
        }
    }
}

void ExecuteRight(vector<vector<Item>>& board, int n, int m)
{
    for(int j = 0; j < n; j++)
    {
        int k = m-1;
        for(int i = m-1; i >= 0; i--)
        {
            if (IsTile(board[i][j].type))
            {
                Item q = board[i][j];
                board[i][j] = board[k][j];
                board[k][j] = q;
                k--;
            }
        }
    }
}

void ExecuteCommand(char c, vector<vector<Item>>& board, int n, int m)
{
    switch(c)
    {
        case 'G': ExecuteUp(board, n, m); break;
        case 'D': ExecuteDown(board, n, m); break;
        case 'L': ExecuteLeft(board, n, m); break;
        case 'P': ExecuteRight(board, n, m); break;
    }
}

void PrintBoard(vector<vector<Item>>& board, int n, int m)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            printf("%c", board[j][i].type);
        }
        printf("\n");
    }
}

int main()
{
    int n, m, k, cmdId;
    vector<vector<Item>> board;
    char buffer[512];
    vector<char> commands;

    scanf("%i %i\n", &n, &m);

    board.resize(m);
    for(int i = 0; i < m; i++)
    {
        board[i].resize(n);
    }

    for(int i = 0; i < n; i++)
    {
        fgets(buffer, 512, stdin);
        for(int j = 0; j < m; j++)
        {
            board[j][i] = Item(buffer[j]);
        }
    }

    scanf("%i\n", &k);
    for(int i = 0; i < k; i++)
    {
        char c = getchar();
        commands.push_back(c);
    }

    CompressCommands(commands);

    if (commands.size() < 10)
    {
        for(char c : commands)
        {
            ExecuteCommand(c, board, n, m);
        }
        PrintBoard(board, n, m);

        return 0;
    }

    cmdId = 0;
    ExecuteCommand(commands[cmdId++], board, n, m);
    ExecuteCommand(commands[cmdId++], board, n, m);

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            if(IsTile(board[j][i].type))
            {
                board[j][i].startX = j;
                board[j][i].startY = i;
            }
        }
    }

    ExecuteCommand(commands[cmdId++], board, n, m);
    ExecuteCommand(commands[cmdId++], board, n, m);
    ExecuteCommand(commands[cmdId++], board, n, m);
    ExecuteCommand(commands[cmdId++], board, n, m);

    vector<deque<Item>> cycles;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            if(IsTile(board[j][i].type) && !board[j][i].visited)
            {
                cycles.push_back(deque<Item>());
                deque<Item>& cycle = cycles.back();

                int x = j;
                int y = i;
                do
                {
                    Item& item = board[x][y];
                    if(!IsTile(item.type) || item.visited)
                    {
                        printf("FAILED\n");
                        return 0;
                    }
                    item.visited = true;
                    cycle.push_front(item);
                    x = item.startX;
                    y = item.startY;
                } while (x != j || y != i);
            }
        }
    }

    int rounds = (commands.size() - 2) / 4;
    int finisher = (commands.size() - 2) % 4;

    for(auto& cycle : cycles)
    {
        int moveBy = rounds % cycle.size();
        for(int i = 0; i < cycle.size(); i++)
        {
            int nextId = (i + moveBy) % cycle.size();
            int endX = cycle[nextId].startX;
            int endY = cycle[nextId].startY;

            board[endX][endY] = cycle[i].type;
        }
    }

    cmdId = commands.size() - finisher;
    while(cmdId < commands.size())
    {
        ExecuteCommand(commands[cmdId++], board, n, m);
    }
    
    PrintBoard(board, n, m);

    return 0;
}