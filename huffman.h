#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 256

typedef struct
{
    int weight;
    int parent, lch, rch;
} HuffNode;

typedef struct
{
    char code[MAX];
} HuffCode;

extern int freq[256];

// 哈夫曼树
void SelectMin(HuffNode ht[], int len, int *s1, int *s2);
void CreateHuffTree(HuffNode ht[], int n);

// 哈夫曼编码
char *strrev(char *str);
void CreateHuffCode(HuffNode ht[], HuffCode hc[], int n);

// 读文件
int ReadFileAndCount(char *filename);

// 哈夫曼压缩文件
void CompressFile(char *srcFile, char *zipFile,HuffCode hc[], int n);

// 解压缩文件
void UnZipFile(char *zipFile, char *outFile,HuffNode ht[], int n);