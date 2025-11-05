// miniproject_functions.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iso.h"

// --------------- Constants ---------------
AuditTypePayment paymentTable[AUDIT_TYPE_COUNT] = {
    {"Standard", 10000, 1.0},
    {"Medium",   15000, 1.5},
    {"Fastest",  20000, 2.0}
};

// --------------- Stack ---------------
StackNode* push(StackNode* top, AuditRecord rec) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->record = rec;
    newNode->next = top;
    return newNode;
}

StackNode* pop(StackNode** top) {
    if (*top == NULL) return NULL;
    StackNode* temp = *top;
    *top = (*top)->next;
    return temp;
}

// --------------- Queue ---------------
void enqueue(QueueNode** front, QueueNode** rear, AuditRecord rec) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->record = rec;
    newNode->next = NULL;
    if (*rear == NULL) {
        *front = *rear = newNode;
        return;
    }
    (*rear)->next = newNode;
    *rear = newNode;
}

QueueNode* dequeue(QueueNode** front, QueueNode** rear) {
    if (*front == NULL) return NULL;
    QueueNode* temp = *front;
    *front = (*front)->next;
    if (*front == NULL) *rear = NULL;
    return temp;
}

// --------------- BST ---------------
int compareDate(char* d1, char* d2) {
    return strcmp(d1, d2);
}

BSTNode* insertBST(BSTNode* root, AuditRecord rec) {
    if (root == NULL) {
        BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
        node->record = rec;
        node->left = node->right = NULL;
        return node;
    }
    if (compareDate(rec.date, root->record.date) < 0)
        root->left = insertBST(root->left, rec);
    else
        root->right = insertBST(root->right, rec);
    return root;
}

void inorderBST(BSTNode* root) {
    if (root == NULL) return;
    inorderBST(root->left);
    printf("ID:%d | Date:%s | Dept:%s | CompanyType:%s | Category:%s | AuditType:%s | Payment:%.2lf\n",
           root->record.auditID, root->record.date, root->record.department,
           root->record.companyType, root->record.category,
           root->record.auditType, root->record.payment);
    inorderBST(root->right);
}

// --------------- Linked List ---------------
void addOngoingAudit(ListNode** head, AuditRecord rec) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->record = rec;
    newNode->next = *head;
    *head = newNode;
}

void removeOngoingAudit(ListNode** head, int auditID) {
    ListNode *temp = *head, *prev = NULL;
    while (temp && temp->record.auditID != auditID) {
        prev = temp;
        temp = temp->next;
    }
    if (!temp) return;
    if (!prev) *head = temp->next;
    else prev->next = temp->next;
    free(temp);
}

void printOngoingAudits(ListNode* head) {
    if (head == NULL) {
        printf("No ongoing audits.\n");
        return;
    }
    while (head) {
        printf("ID:%d | Dept:%s | Payment:%.2lf\n",
               head->record.auditID, head->record.department, head->record.payment);
        head = head->next;
    }
}

// --------------- Priority/Heap ---------------
PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->heapArray = (AuditRecord*)malloc(capacity * sizeof(AuditRecord));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swap(AuditRecord* a, AuditRecord* b) {
    AuditRecord temp = *a;
    *a = *b;
    *b = temp;
}

double computePriority(AuditRecord rec) {
    for (int i = 0; i < AUDIT_TYPE_COUNT; i++) {
        if (strcmp(rec.auditType, paymentTable[i].auditType) == 0) {
            return paymentTable[i].priorityValue;
        }
    }
    return 0;
}

void maxHeapify(PriorityQueue* pq, int i) {
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;
    if (left < pq->size && computePriority(pq->heapArray[left]) > computePriority(pq->heapArray[largest]))
        largest = left;
    if (right < pq->size && computePriority(pq->heapArray[right]) > computePriority(pq->heapArray[largest]))
        largest = right;
    if (largest != i) {
        swap(&pq->heapArray[i], &pq->heapArray[largest]);
        maxHeapify(pq, largest);
    }
}

void insertPQ(PriorityQueue* pq, AuditRecord rec) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->heapArray = (AuditRecord*)realloc(pq->heapArray, pq->capacity * sizeof(AuditRecord));
    }
    int i = pq->size++;
    pq->heapArray[i] = rec;
    while (i != 0 && computePriority(pq->heapArray[(i-1)/2]) < computePriority(pq->heapArray[i])) {
        swap(&pq->heapArray[i], &pq->heapArray[(i-1)/2]);
        i = (i-1)/2;
    }
}

AuditRecord extractMax(PriorityQueue* pq) {
    if (pq->size <=0) {
        AuditRecord empty = {0};
        return empty;
    }
    AuditRecord root = pq->heapArray[0];
    pq->heapArray[0] = pq->heapArray[--pq->size];
    maxHeapify(pq, 0);
    return root;
}

void printPriorityQueue(PriorityQueue* pq) {
    AuditRecord* tempArr = (AuditRecord*)malloc(pq->size * sizeof(AuditRecord));
    int tempSize = pq->size;
    for (int i = 0; i < tempSize; i++) tempArr[i] = pq->heapArray[i];

    // Sort by priority (descending)
    for (int i = 0; i < tempSize-1; i++) {
        int maxIdx = i;
        double maxPriority = computePriority(tempArr[i]);
        for (int j = i+1; j < tempSize; j++) {
            double curPriority = computePriority(tempArr[j]);
            if (curPriority > maxPriority) {
                maxIdx = j;
                maxPriority = curPriority;
            }
        }
        if (maxIdx != i) {
            AuditRecord tmp = tempArr[i];
            tempArr[i] = tempArr[maxIdx];
            tempArr[maxIdx] = tmp;
        }
    }

    printf("\n--- Audits by Priority (Highest to Lowest) ---\n");
    for(int i=0; i<tempSize; i++) {
        printf("ID:%d | Dept:%s | Type:%s | Payment:%.2lf | Priority Level:%.1lf\n",
            tempArr[i].auditID, tempArr[i].department, tempArr[i].auditType,
            tempArr[i].payment, computePriority(tempArr[i]));
    }
    free(tempArr);
}

// --------------- Input Validation ---------------
AuditRecord inputAuditRecord() {
    AuditRecord rec;
    int valid = 0, auditTypeIndex = -1;

    // Audit ID
    do {
        printf("Enter Audit ID (positive integer): ");
        if(scanf("%d", &rec.auditID)!=1 || rec.auditID <= 0) {
            printf("Invalid input. Enter positive integer.\n");
            while(getchar()!='\n');
            valid = 0;
        } else valid = 1;
    } while (!valid);

    // Department
    do {
        printf("Enter Department: ");
        scanf(" %49[^\n]", rec.department);
        valid = strlen(rec.department)>0;
        if (!valid) printf("Department cannot be empty.\n");
    } while (!valid);

    // Date
    do {
        printf("Enter Date (YYYY-MM-DD): ");
        scanf(" %10s", rec.date);
        valid = (strlen(rec.date)==10 && rec.date[4]=='-' && rec.date[7]=='-');
        if (!valid) printf("Invalid date format.\n");
    } while (!valid);

    // Company Type
    do {
        printf("Enter Company Type (MNC or Non-MNC): ");
        scanf(" %9s", rec.companyType);
        valid = (strcmp(rec.companyType,"MNC")==0 || strcmp(rec.companyType,"Non-MNC")==0);
        if (!valid) printf("Must be MNC or Non-MNC.\n");
    } while(!valid);

    // Category if MNC
    if(strcmp(rec.companyType, "MNC")==0) {
        do {
            printf("Enter Category (Product, Service, Location): ");
            scanf(" %19s", rec.category);
            valid = (strcmp(rec.category,"Product")==0 || strcmp(rec.category,"Service")==0 || strcmp(rec.category,"Location")==0);
            if(!valid) printf("Invalid category for MNC.\n");
        } while(!valid);
    } else {
        strcpy(rec.category,"N/A");
    }

    // Audit Type: must be one of Standard, Medium, Fastest
    do {
        printf("Enter Audit Type (Standard, Medium, Fastest): ");
        scanf(" %9s", rec.auditType);
        valid = 0;
        for(int i=0; i<AUDIT_TYPE_COUNT; i++) {
            if(strcmp(rec.auditType, paymentTable[i].auditType)==0) {
                auditTypeIndex = i;
                valid = 1;
                break;
            }
        }
        if(!valid) printf("Invalid audit type.\n");
    } while(!valid);

    // Payment - must match exactly the allowed value
    do {
        double allowed = paymentTable[auditTypeIndex].payment;
        printf("Payment for %s audit MUST be %.0lf: ", rec.auditType, allowed);
        if(scanf("%lf", &rec.payment)!=1 || rec.payment != allowed) {
            printf("Invalid. Must be exactly %.0lf.\n", allowed);
            while(getchar()!='\n');
            valid = 0;
        } else valid = 1;
    } while(!valid);

    return rec;
}
