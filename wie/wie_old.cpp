#include <cstdio>
#include <cinttypes>
#include <vector>

using namespace std;

class EvenOddCounter
{
private:
    vector<vector<int64_t>> even;
    vector<vector<int64_t>> odd;

public:
    int64_t ResultEven;
    int64_t ResultOdd;

    void Calculate(int n, int k)
    {
        ResultEven = ResultOdd = 0LL;
        even.resize(n+1);
        odd.resize(n+1);
        for(int i = 0; i <= n; i++)
        {
            even[i].resize(k+1);
            odd[i].resize(k+1);
            for(int j = 0; j <= k; j++)
            {
                even[i][j] = odd[i][j] = 0LL;
            }

            even[i][1] = (i+1)/2;
            odd[i][1] = (i)/2;
        }

        for(int j = 2; j <= k; j++)
        {
            even[j][j] = (j%4 == 0) || (j%4 == 1) ? 1 : 0;
            odd[j][j] = (j%4 == 2) || (j%4 == 3) ? 1 : 0;

            for(int i = j+1; i <= n; i++)
            {
                int start = 0;
                for(int q = i-1; q >= j-1; q--)
                {
                    start++;
                    if((start%2 == 0) || (start%2 == 1 && (j - 1)%2 == 1 && (q)%2 == 1))
                    {
                        even[i][j] += odd[q][j-1];
                        odd[i][j] += even[q][j-1];
                    }
                    else
                    {
                        even[i][j] += even[q][j-1];
                        odd[i][j] += odd[q][j-1];
                    }
                }
            }
        }

        ResultEven = even[n][k];
        ResultOdd = odd[n][k];
    }
};

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

struct Pawn
{
    int x;
    int y;

    Pawn() : x(0), y(0) { }
    Pawn(int x, int y) : x(x), y(y) { }
};

class Game
{
private:
    vector<Pawn> pawns;
    int width;
    int height;
    vector<vector<Pawn*>> board;
    bool calculatedEqualState;
    long double equalStateProbability;

public:
    Game(int w, int h) : width(w), height(h)
    {
        board.resize(w);
        for (int i = 0; i < w; i++)
        {
            board[i].resize(h);
            for(int j = 0; j < h; j++)
            {
                board[i][j] = NULL;
            }
        }

        calculatedEqualState = false;
        equalStateProbability = 0.;
    }

    void AddPawn(Pawn pawn)
    {
        pawns.push_back(pawn);
        board[pawn.x][pawn.y] = &(pawns.back());
    }

    int CalculateParity()
    {
        int result = 0;

        for (Pawn& pawn : pawns)
        {
            result = (result + pawn.x + pawn.y) % 2;
        }

        return result;
    }

    long double CalculateStateProbability(int depth)
    {
        calculatedEqualState = false;
        equalStateProbability = -1.;

        return InternalStateProbability(depth);
    }

    int GetPawnCount()
    {
        return pawns.size();
    }

private:
    bool CanMove(Pawn* pPawn, Direction dir)
    {
        int x = pPawn->x;
        int y = pPawn->y;

        switch(dir)
        {
            case Up: y--; break;
            case Down: y++; break;
            case Left: x--; break;
            case Right: x++; break;
        }

        if (0 <= x && x < width && 0 <= y && y < height)
        {
            return board[x][y] == NULL;
        }
        return false;
    }

    int CountPawnMoves(Pawn* pPawn)
    {
        int count = 0;

        if (CanMove(pPawn, Up)) count++;
        if (CanMove(pPawn, Down)) count++;
        if (CanMove(pPawn, Left)) count++;
        if (CanMove(pPawn, Right)) count++;

        return count;
    }

    bool Move(Pawn* pPawn, Direction dir)
    {
        int x = pPawn->x;
        int y = pPawn->y;

        switch(dir)
        {
            case Up: y--; break;
            case Down: y++; break;
            case Left: x--; break;
            case Right: x++; break;
        }

        if (0 <= x && x < width && 0 <= y && y < height && board[x][y] == NULL)
        {
            board[pPawn->x][pPawn->y] = NULL;
            board[x][y] = pPawn;
            pPawn->x = x;
            pPawn->y = y;

            return true;
        }
        return false;
    }

    int CalculatePossibleMoves()
    {
        int count = 0;

        for (Pawn pawn : pawns)
        {
            count += CountPawnMoves(&pawn);
        }

        return count;
    }

    long double InternalStateProbability(int depth)
    {
        if (depth == 0)
        {
            return CalculateEqualStateProbability();
        }

        long double probability = 0.;

        for (Pawn& pawn : pawns)
        {
            if (Move(&pawn, Up))
            {
                probability += InternalStateProbability(depth-1) / (long double)CalculatePossibleMoves();
                Move(&pawn, Down);
            }

            if (Move(&pawn, Down))
            {
                probability += InternalStateProbability(depth-1) / (long double)CalculatePossibleMoves();
                Move(&pawn, Up);
            }

            if (Move(&pawn, Left))
            {
                probability += InternalStateProbability(depth-1) / (long double)CalculatePossibleMoves();
                Move(&pawn, Right);
            }

            if (Move(&pawn, Right))
            {
                probability += InternalStateProbability(depth-1) / (long double)CalculatePossibleMoves();
                Move(&pawn, Left);
            }
        }

        return probability;
    }

    long double CalculateEqualStateProbability()
    {
        if (calculatedEqualState)
        {
            return equalStateProbability;
        }
        
        calculatedEqualState = true;

        EvenOddCounter stateCounter;
        stateCounter.Calculate(width*height, pawns.size());
        int64_t possibleStates = CalculateParity() == 0 ? stateCounter.ResultEven : stateCounter.ResultOdd;
        
        //equalStateProbability = (long double)2 / (long double)CalculateBinomialTheorem(width * height, pawns.size());
        equalStateProbability = (long double)1 / (long double)possibleStates;

        return equalStateProbability; 
    }

    int64_t CalculateBinomialTheorem(int n, int k)
    {
        int64_t result = 1LL;

        if ((n-k) < k)
        {
            k = n-k;
        }

        for (int i = 1; i <= k; i++)
        {
            result *= n;
            result /= i;
            n--;
        }

        return result;
    }
};

int main()
{
    int n, m;
    char buffer[16];

    scanf("%i %i\n", &n, &m);
    Game startGame(m, n);
    Game endGame(m, n);

    for (int j = 0; j < n; j++)
    {
        fgets(buffer, 16, stdin);
        for (int i = 0; i < m; i++)
        {
            if(buffer[i] == 'O')
            {
                startGame.AddPawn(Pawn(i, j));
            }
        }
    }

    fgets(buffer, 16, stdin);

    for (int j = 0; j < n; j++)
    {
        fgets(buffer, 16, stdin);
        for (int i = 0; i < m; i++)
        {
            if(buffer[i] == 'O')
            {
                endGame.AddPawn(Pawn(i, j));
            }
        }
    }

    if (startGame.CalculateParity() != endGame.CalculateParity())
    {
        printf("0\n");
        return 0;
    }

    int d = 0;
    int cases = 1;
    while(cases < 10000000)
    {
        d++;
        cases *= (4*endGame.GetPawnCount());
    }

    // for(int i = 0; i<=5 ; i++)
    // {
    //     long double result = endGame.CalculateStateProbability(i);
    //     printf("%i: %.15Lf\n", i, result);
    // }

    long double result = endGame.CalculateStateProbability(d-1);
    printf("%.15Lf\n", result);

    return 0;
}