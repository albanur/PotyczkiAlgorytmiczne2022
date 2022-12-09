#include <cstdio>
#include <cctype>
#include <deque>

using namespace std;

int main()
{
    const int BUFFER_SIZE = 5002;
    char buffer[BUFFER_SIZE];
    deque<char> a;
    deque<char> b;
    deque<char> c;
    int i;
    char k, ai, bi, ci;
    
    fgets(buffer, BUFFER_SIZE, stdin);
    for (i = 0; isdigit(buffer[i]); i++)
    {
        a.push_front(buffer[i] - '0');
    }
    
    fgets(buffer, BUFFER_SIZE, stdin);
    for (i = 0; isdigit(buffer[i]); i++)
    {
        b.push_front(buffer[i] - '0');
    }
    
    k = 0;
    for (i = 0; i < a.size() || i < b.size(); i++)
    {
        ai = i < a.size() ? a[i] : 0;
        bi = i < b.size() ? b[i] : 0;
        ci = ai + bi + k;
        k = ci / 10;
        ci = ci % 10;
        c.push_back(ci);
    }
    
    if (k > 0)
    {
        c.push_back(k);
    }
    
    for(i = c.size()-1; i >= 0; i--)
    {
        printf("%c", c[i] + '0');
    }

    return 0;
}
