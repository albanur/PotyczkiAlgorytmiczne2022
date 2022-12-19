#include <cstdio>
#include <cinttypes>
#include <vector>

using namespace std;

#include <cstdio>
#include <cinttypes>
#include <vector>

using namespace std;

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

    long double CalculateProbability()
    {
        return (long double)CalculatePossibleMoves() / (long double)CalculateAllPossibleMoves();
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

    int CalculatePossibleMoves()
    {
        int count = 0;

        for (Pawn pawn : pawns)
        {
            count += CountPawnMoves(&pawn);
        }

        return count;
    }

    int64_t CalculateAllPossibleMoves()
    {
        int64_t result = 0LL;;
        int n = width * height;
        int k = pawns.size();

        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < height; y++)
            {
                int moves = 0;
                if ((x-1) >= 0) moves++;
                if ((x+1) < width) moves++;
                if ((y-1) >= 0) moves++;
                if ((y+1) < height) moves++;

                if((x+y)%2 != 0)
                {
                    continue;
                }

                if (moves == 1)
                {
                    result += CalculateBinomialTheorem(n-2, k-1);
                }
                if (moves == 2)
                {
                    result += 2*CalculateBinomialTheorem(n-3, k-1);
                    result += 2*CalculateBinomialTheorem(n-3, k-2);
                }
                if (moves == 3)
                {
                    result += 3*CalculateBinomialTheorem(n-4, k-1);
                    result += 2*3*CalculateBinomialTheorem(n-4, k-2);
                    result += 3*CalculateBinomialTheorem(n-4, k-3);
                }
                if (moves == 4)
                {
                    result += 4*CalculateBinomialTheorem(n-5, k-1);
                    result += 2*6*CalculateBinomialTheorem(n-5, k-2);
                    result += 3*4*CalculateBinomialTheorem(n-5, k-3);
                    result += 4*CalculateBinomialTheorem(n-5, k-4);
                }
            }
        }

        return result;
    }

    int64_t CalculateMovesForSlots(vector<int>& slots, int k)
    {
        int64_t result = 0;

        for (int i1 = 0; i1 <= k && i1 <= slots[1]; i1++)
        {
            for (int i2 = 0; i1+i2 <= k && i2 <= slots[2]; i2++)
            {
                for (int i3 = 0; i1+i2+i3 <= k && i3 <= slots[3]; i3++)
                {
                    int i4 = k - i1-i2-i3;
                    if(i4 <= slots[4])
                    {
                        result += (1*i1 + 2*i2 + 3*i3 + 4*i4)
                            * CalculateBinomialTheorem(slots[1], i1)
                            * CalculateBinomialTheorem(slots[2], i2)
                            * CalculateBinomialTheorem(slots[3], i3)
                            * CalculateBinomialTheorem(slots[4], i4);
                    }
                }
            }
        }

        return result;
    }

    int64_t CalculateBinomialTheorem(int n, int k)
    {
        if (k < 0 || n < k)
        {
            return 0LL;
        }

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

    long double result = endGame.CalculateProbability();
    printf("%.15Lf\n", result);

    return 0;
}