#include <stdint.h>
#include <stdlib.h>
#include "queue.h"


// ����һ���ն���Q
void Q_Init(SqQueue* Q)
{
    Q->base = (int *)malloc(MAX_QSIZE * sizeof(int));
    // �洢����ʧ��
    if (!Q->base){
        return;
    }
    Q->front = Q->rear = 0;
    return;
}

// ���ٶ���Q��Q���ٴ���
void Q_Destroy(SqQueue *Q)
{
    if (Q->base)
        free(Q->base);
    Q->base = NULL;
    Q->front = Q->rear = 0;
}

// ��Q��Ϊ�ն���
void Q_Clear(SqQueue *Q)
{
    Q->front = Q->rear = 0;
}

// ������QΪ�ն��У��򷵻�1�����򷵻�-1
int Q_Empty(SqQueue Q)
{
    if (Q.front == Q.rear) // ���пյı�־
        return 1;
    else
        return -1;
}

// ����Q��Ԫ�ظ����������еĳ���
int Q_Length(SqQueue Q)
{
    return (Q.rear - Q.front + MAX_QSIZE) % MAX_QSIZE;
}

// �����в��գ�����e����Q�Ķ�ͷԪ�أ�������OK�����򷵻�ERROR
int Q_GetHead(SqQueue Q, int *e) {
    if (Q.front == Q.rear) // ���п�
        return -1;
    *e = Q.base[Q.front];
    return 1;
}

//// ��ӡ�����е�����
//void Q_Print(SqQueue Q) {
//    int p = Q.front;
//    while (Q.rear != p) {
//        printf("%d\n", Q.base[p]);
//        p += 1;
//    }
//}

// ����Ԫ��eΪQ���µĶ�βԪ��
int Q_Put(SqQueue *Q, int e)
{
    if ((Q->rear + 1) % MAX_QSIZE == Q->front) // ������
        return -1;
    Q->base[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAX_QSIZE;
    return 1;
}

// �����в��գ���ɾ��Q�Ķ�ͷԪ�أ���e������ֵ��������1�����򷵻�-1
int Q_Poll(SqQueue *Q, int *e)
{
    if (Q->front == Q->rear) // ���п�
        return -1;
    *e = Q->base[Q->front];
    Q->front = (Q->front + 1) % MAX_QSIZE;
    return 1;
}


//QHAL

int queue_try_put(SqQueue *queue, void *p)
{
	if(Q_Put(queue, (int)p) == 1) {
		return 1;
	} else {
		return 0;
	}
}


void *queue_try_get(SqQueue *queue)
{
	int e;
	if(Q_Poll(queue, &e) == 1) {
		return (void *)e;
	} else {
		return NULL;
	}
}

