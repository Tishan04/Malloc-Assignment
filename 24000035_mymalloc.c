#include "24000035_mymalloc.h"
#include <stddef.h>

#define HEAP_CAPACITY 25000

static unsigned char heapArea[HEAP_CAPACITY];

typedef struct MemNode
{
    size_t blockSize;
    int isFree;
    struct MemNode *nextNode;
} MemNode;

static MemNode *heapStart = NULL;

static void initHeap()
{
    heapStart = (MemNode *)heapArea;
    heapStart->blockSize = HEAP_CAPACITY - sizeof(MemNode);
    heapStart->isFree = 1;
    heapStart->nextNode = NULL;
}

static void divideBlock(MemNode *currentBlock, size_t requestSize)
{
    MemNode *splitNode = (MemNode *)((unsigned char *)currentBlock + sizeof(MemNode) + requestSize);

    splitNode->blockSize = currentBlock->blockSize - requestSize - sizeof(MemNode);
    splitNode->isFree = 1;
    splitNode->nextNode = currentBlock->nextNode;

    if (splitNode->blockSize >= 8)
    {
        
    }

    currentBlock->blockSize = requestSize;
    currentBlock->isFree = 0;
    currentBlock->nextNode = splitNode;
}

void *MyMalloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    if (!heapStart)
    {
        initHeap();
    }
    MemNode *walker = heapStart;

    while (walker)
    {
        if (walker->isFree && walker->blockSize >= size)
        {
            if (walker->blockSize > size + sizeof(MemNode))
            {
                divideBlock(walker, size);
            }
            else
            {
                walker->isFree = 0;
            }
            return (unsigned char *)walker + sizeof(MemNode);
        }
        walker = walker->nextNode;
    }
    return NULL;
}

void MyFree(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    MemNode *target = (MemNode *)((unsigned char *)ptr - sizeof(MemNode));
    target->isFree = 1;
    MemNode *current = heapStart;

    while (current && current->nextNode)
    {
        if (current->isFree && current->nextNode->isFree)
        {
            current->blockSize += sizeof(MemNode) + current->nextNode->blockSize;
            current->nextNode = current->nextNode->nextNode;
        }
        else
        {
            current = current->nextNode;
        }
    }
}