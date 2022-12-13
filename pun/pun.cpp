#include <cstdio>

using namespace std;

int main()
{
    int numberOfTests;
    char buffer[128];
    int score = 0;
    int groupSize;
    int i, j;
    
    scanf("%i\n", &numberOfTests);
    fgets(buffer, 128, stdin);
    
    groupSize = numberOfTests / 10;
    
    for(i = 0; i < 10; i++)
    {
        bool result = true;
        for(j = 0; j < groupSize; j++)
        {
            result &= buffer[i*groupSize+j] == 'T';
        }
        score += result ? 1 : 0;
    }

    printf("%i\n", score);
    return 0;
}