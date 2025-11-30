#include "Common.h"

void bubble_sort(uint16_t *a, int n)
{
    int i,j;
    uint16_t temp;
    for(i =0;i<n-1;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(a[j]>a[j+1])
            {
                temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        }
    }
}