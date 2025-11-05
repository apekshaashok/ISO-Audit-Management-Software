// miniproject_main.c

#include <stdio.h>
#include <stdlib.h>
#include "iso.h"


int main() {
    StackNode* undoStack = NULL;
    QueueNode* front = NULL;
    QueueNode* rear = NULL;
    BSTNode* bstRoot = NULL;
    ListNode* ongoingHead = NULL;
    PriorityQueue* pq = createPriorityQueue(10);
    int choice;
    AuditRecord rec;

    while(1) {
        printf("\n===== ISO Audit Management =====\n");
        printf("1. Add Audit Record\n");
        printf("2. Schedule Audit\n");
        printf("3. Process Audit Request\n");
        printf("4. Undo Last Change\n");
        printf("5. View Audit Records by Date\n");
        printf("6. Add Ongoing Audit\n");
        printf("7. View Ongoing Audits\n");
        printf("8. View All Scheduled Audits by Priority\n");
        printf("9. Remove Ongoing Audit by ID\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                rec = inputAuditRecord();
                undoStack = push(undoStack, rec);
                bstRoot = insertBST(bstRoot, rec);
                printf("Added Audit Record and Undo Stack updated.\n");
                break;
            case 2:
                rec = inputAuditRecord();
                enqueue(&front, &rear, rec);
                insertPQ(pq, rec);
                printf("Audit Scheduled (FIFO and Priority Queue).\n");
                break;
            case 3: {
                QueueNode* qnode = dequeue(&front, &rear);
                if(qnode) {
                    printf("Processing Audit ID: %d Dept: %s\n", qnode->record.auditID, qnode->record.department);
                    // Optionally, add to ongoing audits here
                    free(qnode);
                } else {
                    printf("Queue is empty.\n");
                }
                break;
            }
            case 4: {
                StackNode* undone = pop(&undoStack);
                if(undone) {
                    printf("Undo Audit ID: %d Dept: %s\n", undone->record.auditID, undone->record.department);
                    free(undone);
                } else {
                    printf("Nothing to undo.\n");
                }
                break;
            }
            case 5:
                printf("\nAudit Records Sorted by Date:\n");
                inorderBST(bstRoot);
                break;
            case 6:
                rec = inputAuditRecord();
                addOngoingAudit(&ongoingHead, rec);
                printf("Ongoing Audit Added.\n");
                break;
            case 7:
                printf("\nOngoing Audits:\n");
                printOngoingAudits(ongoingHead);
                break;
            case 8:
                printPriorityQueue(pq);
                break;
            case 9: {
                int id;
                printf("Enter Audit ID to remove from ongoing audits: ");
                scanf("%d", &id);
                removeOngoingAudit(&ongoingHead, id);
                printf("Removed if existed.\n");
                break;
            }
            case 0:
                printf("Exiting Program.\n");
                return 0;
            default:
                printf("Invalid Choice.\n");
        }
    }
    return 0;
}
