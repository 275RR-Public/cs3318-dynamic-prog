/*
*   James Hofer
*   1000199225
*   Compile: /usr/bin/c99 -o app3 app3.c
*
*   Usage1: ./app3 (for typing or pasting input)
*   Usage2: ./app3 < input.dat (for input redirection)
*
*   This program solves the two room, weighted interval
*   scheduling problem by using dynamic programming.
*
*   https://ranger.uta.edu/~weems/NOTES3318/LAB/LAB3FALL23/lab3fall23.pdf
*
*   Code modified from Dr. Weems one room solution
*   https://ranger.uta.edu/~weems/NOTES3318/wis.bs.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Weighted interval scheduling.  See Kleinberg & Tardos, chapter 6
// Uses binary search for finding p[] values 09/28/07 BPW

#define TABSIZE (100)

int n,s[TABSIZE],f[TABSIZE],v[TABSIZE],p[TABSIZE],M1[TABSIZE], M2[TABSIZE];

int max(int x,int y)
{
    if (x>y)
        return x;
    return y;
}

int binSearchLast(int *a,int n,int key)
{
    // Input: int array a[] with n elements in ascending order.
    //        int key to find.
    // Output: Returns subscript of the last a element <= key.
    //         Returns -1 if key<a[0].
    // Processing: Binary search.
    int low,high,mid;
    low=0;
    high=n-1;
    // subscripts between low and high are in search range.
    // size of range halves in each iteration.
    // When low>high, low==high+1 and a[high]<=key and a[low]>key.
    while (low<=high)
    {
        mid=(low+high)/2;
        if (a[mid]<=key)
            low=mid+1;
        else
            high=mid-1;
    }
    return high;
}

int main()
{
    // check if redirected input
    if(isatty(0))
        printf("Type input by line or Paste entire input.\n");
    
    int i,j,sum=0;

    scanf("%d",&n);
    f[0]=(-999999); // For binarySearchLast
    
    for (i=1;i<=n;i++)
        scanf("%d %d %d",&s[i],&f[i],&v[i]);
    
    for (i=2;i<=n && f[i-1]<=f[i];i++)
        ;
    
    if (i<=n)
    {
        printf("Intervals not ordered by finish time %d\n",__LINE__);
        exit(0);
    }

    for (i=1;i<=n;i++)
        p[i]=binSearchLast(f,n+1,s[i]);

    M1[0]=0;
    M2[0]=0;
    for (i=1;i<=n;i++)
        //M[i]=max(v[i]+M[p[i]],M[i-1]);
        M1[i] = max(v[i] + M2[p[i]], M1[i-1]);
        M2[i] = max(v[i] + M1[p[i]], M2[i-1]);
    printf("  i   s   f   v   p   M1   M2\n");
    
    for (i=1;i<=n;i++)
        printf("%3d %3d %3d %3d %3d %3d %3d\n",i,s[i],f[i],v[i],p[i],M1[i],M2[i]);
    
    /* for (i=n;i>0; )
        if (v[i]+M[p[i]]>=M[i-1])
            {
                printf("Include interval %d\n",i);
                sum+=v[i];
                i=p[i];
            }
        else
            i--; */

    for (i = n; i > 0; ) {
        if (v[i] + M1[p[i]] >= M1[i-1]) {
            printf("Include interval %d in room 1\n", i);
            sum += v[i];
            i = p[i];
        } else if (v[i] + M2[p[i]] >= M2[i-1]) {
            printf("Include interval %d in room 2\n", i);
            sum += v[i];
            i = p[i];
        } else {
            i--;
        }
    }
    printf("sum is %d\n",sum);
}