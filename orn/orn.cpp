#include <cstdio>
#include <vector>

using namespace std;

int main()
{
    int size;
    vector<int> tones;
    int i, k;
    int countA=0;
    int countB=0;
    
    scanf("%i", &size);
    for(i = 0; i < size; i++)
    {
        scanf("%i", &k);
        tones.push_back(k);
    }
    
    for(i = 1; i < size; i++)
    {
        if((i % 2) == 0)
        {
            if(tones[i-1] <= tones[i])
            {
                countA++;
                i++;
            }
        }
        else
        {
            if(tones[i-1] >= tones[i])
            {
                countA++;
                i++;
            }
        }
    }
    
    for(i = 1; i < size; i++)
    {
        if((i % 2) == 0)
        {
            if(tones[i-1] >= tones[i])
            {
                countB++;
                i++;
            }
        }
        else
        {
            if(tones[i-1] <= tones[i])
            {
                countB++;
                i++;
            }
        }
    }

    printf("%i\n", countA < countB ? countA : countB);

    return 0;
}