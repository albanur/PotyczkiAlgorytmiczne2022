#include <cstdint>
#include <cstdio>
#include <cmath>
#include <deque>

using namespace std;

class LetterRange
{
private:
    char letter;
    int count;

public:
    LetterRange(char letter, int startPosition) : letter(letter), count(1) { }

    inline char GetLetter() { return letter; }
    inline int GetCount() { return count; }

    inline void Push(int size = 1) { count+=size; }
    inline void Pop(int size = 1) { count-=size; }
};

class PalindromFixer
{
private:
    deque<LetterRange> inscription;

public:
    PalindromFixer(const char* input)
    {
        Initialize(input);
    }

    int64_t Score()
    {
        return CanBeFixed() ? Fix() : -1;
    }

    bool CanBeFixed()
    {
        int count_a = 0;
        int count_b = 0;

        for(auto& r : inscription)
        {
            if (r.GetLetter() == 'a')
            {
                count_a += r.GetCount();
            }
            else if(r.GetLetter() == 'b')
            {
                count_b += r.GetCount();
            }
        }

        return ((count_a % 2) != 1) || ((count_b % 2) != 1);
    }

    int64_t Fix()
    {
        int64_t steps = 0;

        while (inscription.size() > 1)
        {
            if (GetFirstLetter() == GetLastLetter())
            {
                int toRemove = min(GetFirstRange().GetCount(), GetLastRange().GetCount());
                PopFront(toRemove);
                PopBack(toRemove);
            }
            else
            {
                int step_front = GetFirstRange().GetCount();
                int step_back = GetLastRange().GetCount();

                steps += min(step_front, step_back);

                if(step_front <= step_back)
                {
                    SwapAtFront();
                }
                else
                {
                    SwapAtBack();
                }
            }
        }

        return steps;
    }

private:
    void Initialize(const char* input)
    {
        inscription.clear();

        for (int i = 0; input[i] == 'a' || input[i] == 'b'; i++)
        {
            if (GetLastLetter() == input[i])
            {
                inscription.back().Push();
            }
            else
            {
                inscription.push_back(LetterRange(input[i], i));
            }
        }
    }

    inline char GetFirstLetter() { return inscription.size() > 0 ? inscription.front().GetLetter() : '\0'; }
    inline char GetLastLetter() { return inscription.size() > 0 ? inscription.back().GetLetter() : '\0'; }
    inline LetterRange& GetFirstRange() { return inscription.front(); }
    inline LetterRange& GetLastRange() { return inscription.back(); }

    inline void PopFront(int size)
    {
        if (GetFirstRange().GetCount() < size)
        {
            throw exception();
        }

        if (GetFirstRange().GetCount() == size)
        {
            inscription.pop_front();
        }
        else
        {
            GetFirstRange().Pop(size);
        }
    }

    inline void PopBack(int size)
    {
        if (GetLastRange().GetCount() < size)
        {
            throw exception();
        }

        if (GetLastRange().GetCount() == size)
        {
            inscription.pop_back();
        }
        else
        {
            GetLastRange().Pop(size);
        }
    }

    inline void SwapAtFront()
    {
        LetterRange& front = inscription[0];
        LetterRange& next = inscription[1];

        if (next.GetCount() == 1)
        {
            if (inscription.size() > 2)
            {
                inscription[2].Push(front.GetCount());
                inscription.pop_front();
            }
            else
            {
                inscription[0] = next;
                inscription[1] = front;
            }
        }
        else
        {
            next.Pop();
            inscription.push_front(LetterRange(next.GetLetter(), 1));
        }
    }

    inline void SwapAtBack()
    {
        LetterRange& back = inscription[inscription.size() - 1];
        LetterRange& prev = inscription[inscription.size() - 2];

        if (prev.GetCount() == 1)
        {
            if (inscription.size() > 2)
            {
                inscription[inscription.size() - 3].Push(back.GetCount());
                inscription.pop_back();
            }
            else
            {
                inscription[inscription.size() - 1] = prev;
                inscription[inscription.size() - 2] = back;
            }
        }
        else
        {
            prev.Pop();
            inscription.push_back(LetterRange(prev.GetLetter(), 1));
        }
    }
};

int main()
{
    const int BufferSize = 200001;
    char buffer[BufferSize];

    fgets(buffer, BufferSize, stdin);

    auto fixer = PalindromFixer(buffer);
    int64_t result = fixer.Score();

    printf("%lld\n", result);
    
    return 0;
}