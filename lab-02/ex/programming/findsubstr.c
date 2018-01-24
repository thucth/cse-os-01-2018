#include "findsubstr.h"

int find_sub_string(const char *str, const char *sub) 
{
    int n = strlen(str);
    int m = strlen(sub);
    if (n < m) return -1;
    for (int l = 0; l < n - m + 1; l++) {
        int i = 0;
        for (; i < m; i++) {
            int  r = l + i;
            if (str[r] != sub[i]) break;
        }
        if (i == m) return l;
    }
    return -1;
}