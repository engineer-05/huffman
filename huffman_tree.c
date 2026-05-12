#include "huffman.h"

// 挑选两个权值最小节点
void SelectMin(HuffNode ht[], int len, int *s1, int *s2)
{
    int i, min1 = 0x7fffffff, min2 = 0x7fffffff;
    *s1 = *s2 = 0;
    for (i = 1; i <= len; i++)
    {
        if (ht[i].parent == 0)
        {
            if (ht[i].weight < min1)
            {
                min2 = min1;
                *s2 = *s1;
                min1 = ht[i].weight;
                *s1 = i;
            }
            else if (ht[i].weight < min2)
            {
                min2 = ht[i].weight;
                *s2 = i;
            }
        }
    }
}

// 创建哈夫曼树
void CreateHuffTree(HuffNode ht[], int n)
{
    int i, s1, s2;
    int total = 2 * n - 1;
    for (i = n + 1; i <= total; i++)
    {
        SelectMin(ht, i - 1, &s1, &s2);
        ht[s1].parent = i;
        ht[s2].parent = i;
        ht[i].lch = s1;
        ht[i].rch = s2;
        ht[i].weight = ht[s1].weight + ht[s2].weight;
    }
}