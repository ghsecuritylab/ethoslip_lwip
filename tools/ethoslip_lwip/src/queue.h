#ifndef __QUEUE_H__
#define __QUEUE_H__

// ���е�˳��洢�ṹ(ѭ������)
#define MAX_QSIZE 100 // �����г���+1
typedef struct {
    int *base; // ��ʼ���Ķ�̬����洢�ռ�
    int front; // ͷָ�룬�����в��գ�ָ�����ͷԪ��
    int rear; // βָ�룬�����в��գ�ָ�����βԪ�ص���һ��λ��
} SqQueue;


void Q_Init(SqQueue *Q);
void Q_Destroy(SqQueue *Q);
int Q_Empty(SqQueue Q);
int Q_Length(SqQueue Q);
int Q_Put(SqQueue *Q, int e);
int Q_Poll(SqQueue *Q, int *e);

//QHAL
int queue_try_put(SqQueue *queue, void *p);
void *queue_try_get(SqQueue *queue);

#endif
