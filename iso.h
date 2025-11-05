// miniproject.h

#ifndef MINIPROJECT_H
#define MINIPROJECT_H

// ------------ Structs ------------ 
typedef struct AuditRecord {
    int auditID;
    char department[50];
    char date[11];
    char companyType[10]; // MNC or Non-MNC
    char category[20];    // Product / Service / Location / N/A
    char auditType[10];   // Standard, Medium, Fastest
    double payment;
} AuditRecord;

typedef struct StackNode {
    AuditRecord record;
    struct StackNode* next;
} StackNode;

typedef struct QueueNode {
    AuditRecord record;
    struct QueueNode* next;
} QueueNode;

typedef struct BSTNode {
    AuditRecord record;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct ListNode {
    AuditRecord record;
    struct ListNode* next;
} ListNode;

typedef struct PriorityQueue {
    AuditRecord* heapArray;
    int size;
    int capacity;
} PriorityQueue;

typedef struct {
    char auditType[10];
    double payment;
    double priorityValue;
} AuditTypePayment;

// ------------ Constants ------------
#define AUDIT_TYPE_COUNT 3

extern AuditTypePayment paymentTable[AUDIT_TYPE_COUNT];

// ------------ Function Prototypes ------------
StackNode* push(StackNode* top, AuditRecord rec);
StackNode* pop(StackNode** top);

void enqueue(QueueNode** front, QueueNode** rear, AuditRecord rec);
QueueNode* dequeue(QueueNode** front, QueueNode** rear);

BSTNode* insertBST(BSTNode* root, AuditRecord rec);
void inorderBST(BSTNode* root);

void addOngoingAudit(ListNode** head, AuditRecord rec);
void removeOngoingAudit(ListNode** head, int auditID);
void printOngoingAudits(ListNode* head);

PriorityQueue* createPriorityQueue(int capacity);
void insertPQ(PriorityQueue* pq, AuditRecord rec);
AuditRecord extractMax(PriorityQueue* pq);
void printPriorityQueue(PriorityQueue* pq);

AuditRecord inputAuditRecord();

double computePriority(AuditRecord rec);

#endif
