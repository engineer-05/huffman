#include "huffman.h"

// 翻转字符串
char *strrev(char *str)
{
    if (str == NULL)
        return NULL;
    char *start = str;
    char *end = str + strlen(str) - 1;
    char temp;

    while (start < end)
    {
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    return str;
}

// 生成哈夫曼编码
void CreateHuffCode(HuffNode ht[], HuffCode hc[], int n)
{
    char tmp[MAX];
    int i, cur, p, len;
    for (i = 1; i <= n; i++)
    {
        len = 0;
        cur = i;
        p = ht[cur].parent;
        while (p != 0)
        {
            if (ht[p].lch == cur)
                tmp[len++] = '0';
            else
                tmp[len++] = '1';
            cur = p;
            p = ht[cur].parent;
        }
        tmp[len] = '\0';
        strrev(tmp);
        strcpy(hc[i].code, tmp);
    }
}