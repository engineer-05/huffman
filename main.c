#include "huffman.h"

int freq[256] = {0}; 

int main()
{
    // 文件名自行修改
    char source[] = "test.txt";     // 原始待压缩文本
    char zip[] = "data.huf";        // 压缩后的二进制文件
    char dest[] = "out.txt";        // 解压生成文件

    // 1.读取文件统计频率
    int n = ReadFileAndCount(source);
    int total = 2 * n - 1;

    // 2.初始化哈夫曼树
    HuffNode ht[MAX * 2] = {0};
    HuffCode hc[MAX] = {0};
    int pos = 1;
    for(int i = 0; i < 128; i++)
    {
        if(freq[i] > 0)
            ht[pos++].weight = freq[i];
    }

    // 3.建树 + 生成编码
    CreateHuffTree(ht, n);
    CreateHuffCode(ht, hc, n);

    // 4.执行文件压缩
    CompressFile(source, zip, hc, n);

    // 5.执行文件解压
    UnZipFile(zip, dest, ht, n);

    printf("\n操作结束\n");
    printf("原文件：test.txt\n");
    printf("压缩文件：data.huf(二进制)\n");
    printf("解压文件：out.txt\n");

    return 0;
}