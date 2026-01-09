#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 30
#define MAX_FLIGHT 10
#define MAX_GATES 6

//==============================================================
//                         DATA STRUCTURES
//==============================================================

typedef struct {
    int id;
    char name[MAX_NAME];
    char flightNo[MAX_FLIGHT];
    int age;
    int classType; // 0 = Economy, 1 = Business, 2 = VIP
} Passenger;

// ---- FLIGHT (Linked List) ----
typedef struct Flight {
    char flightNo[MAX_FLIGHT];
    char destination[30];
    int capacity;
    int bookedSeats;
    int gateIndex;
    struct Flight *next;
} Flight;

// ---- Generic Passenger Node ----
typedef struct PNode {
    Passenger data;
    int priority;
    struct PNode *next;
} PNode;

// GLOBALS
Flight *flightHead = NULL;

PNode *checkInFront = NULL, *checkInRear = NULL;
PNode *priorityHead = NULL;
PNode *boardedTop = NULL;

//==============================================================
//                         GATE GRAPH
//==============================================================

const char *gateNames[MAX_GATES] = {
    "Entrance",
    "GateA1",
    "GateA2",
    "GateB1",
    "GateB2",
    "GateC1"
};

int gateGraph[MAX_GATES][MAX_GATES] = {
    {0,1,1,0,0,0},
    {1,0,0,1,0,0},
    {1,0,0,0,1,0},
    {0,1,0,0,1,1},
    {0,0,1,1,0,1},
    {0,0,0,1,1,0}
};

void showGateList() {
    printf("\n--- Available Gates ---\n");
    for (int i = 0; i < MAX_GATES; i++)
        printf("%d. %s\n", i, gateNames[i]);
}

void showPathToGate(int gateIndex) {
    if (gateIndex < 0 || gateIndex >= MAX_GATES) {
        printf("Invalid Gate.\n");
        return;
    }

    int visited[MAX_GATES] = {0};
    int parent[MAX_GATES];
    int queue[MAX_GATES];
    int front = 0, rear = 0;

    for (int i = 0; i < MAX_GATES; i++)
        parent[i] = -1;

    visited[0] = 1;
    queue[rear++] = 0;

    while (front < rear) {
        int u = queue[front++];
        if (u == gateIndex) break;
        for (int v = 0; v < MAX_GATES; v++) {
            if (gateGraph[u][v] && !visited[v]) {
                visited[v] = 1;
                parent[v] = u;
                queue[rear++] = v;
            }
        }
    }

    if (!visited[gateIndex]) {
        printf("No path available.\n");
        return;
    }

    int path[MAX_GATES], len = 0;
    int current = gateIndex;

    while (current != -1) {
        path[len++] = current;
        current = parent[current];
    }

    printf("Shortest path to %s: ", gateNames[gateIndex]);
    for (int i = len - 1; i >= 0; i--) {
        printf("%s", gateNames[path[i]]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

//==============================================================
//                         FLIGHT LIST
//==============================================================

Flight* findFlight(char fn[]) {
    Flight *t = flightHead;
    while (t) {
        if (!strcmp(t->flightNo, fn)) return t;
        t = t->next;
    }
    return NULL;
}

void addFlight() {
    Flight *f = malloc(sizeof(Flight));

    printf("\nEnter Flight No: ");
    scanf("%s", f->flightNo);
    printf("Enter Destination: ");
    scanf("%s", f->destination);
    printf("Enter Seat Capacity: ");
    scanf("%d", &f->capacity);

    showGateList();
    printf("Enter Gate Index: ");
    scanf("%d", &f->gateIndex);
    if (f->gateIndex < 0 || f->gateIndex >= MAX_GATES)
        f->gateIndex = 0;

    f->bookedSeats = 0;
    f->next = NULL;

    if (!flightHead) flightHead = f;
    else {
        Flight *t = flightHead;
        while (t->next) t = t->next;
        t->next = f;
    }
    printf("Flight Added.\n");
}

void showFlights() {
    if (!flightHead) {
        printf("\nNo flights found.\n");
        return;
    }

    Flight *t = flightHead;
    printf("\n--- Flight List ---\n");
    while (t) {
        printf("Flight: %s | To: %s | Seats: %d/%d | Gate: %s\n",
            t->flightNo, t->destination,
            t->bookedSeats, t->capacity,
            gateNames[t->gateIndex]);
        t = t->next;
    }
}

//==============================================================
//                         CHECK-IN QUEUE
//==============================================================

int isCheckEmpty() { return checkInFront == NULL; }

void enqueueCheck(Passenger p) {
    PNode *n = malloc(sizeof(PNode));
    n->data = p; n->next = NULL;

    if (!checkInFront) checkInFront = checkInRear = n;
    else checkInRear->next = n, checkInRear = n;

    printf("Added to Check-in Queue.\n");
}

Passenger dequeueCheck() {
    Passenger d = {-1,"","",0,0};
    if (isCheckEmpty()) return d;

    PNode *t = checkInFront;
    d = t->data;
    checkInFront = checkInFront->next;
    if (!checkInFront) checkInRear = NULL;
    free(t);
    return d;
}

void showCheck() {
    if (isCheckEmpty()) {
        printf("Check-in Queue Empty.\n");
        return;
    }
    PNode *t = checkInFront;
    printf("\n--- Check-In Queue ---\n");
    while (t) {
        printf("%d | %s | %s | Age:%d | Class:%d\n",
            t->data.id, t->data.name, t->data.flightNo,
            t->data.age, t->data.classType);
        t = t->next;
    }
}

//==============================================================
//                         PRIORITY QUEUE
//==============================================================

int getPriority(Passenger *p) {
    int pr = 0;
    if (p->classType == 2) pr += 5;
    if (p->classType == 1) pr += 3;
    if (p->age >= 60) pr += 2;
    return pr;
}

void insertPriority(Passenger p) {
    PNode *n = malloc(sizeof(PNode));
    n->data = p;
    n->priority = getPriority(&p);
    n->next = NULL;

    if (!priorityHead || n->priority > priorityHead->priority) {
        n->next = priorityHead;
        priorityHead = n;
    } else {
        PNode *t = priorityHead;
        while (t->next && t->next->priority >= n->priority)
            t = t->next;
        n->next = t->next;
        t->next = n;
    }
    printf("Added to PRIORITY queue.\n");
}

int isPriorityEmpty() { return priorityHead == NULL; }

Passenger removePriority() {
    Passenger d = {-1,"","",0,0};
    if (isPriorityEmpty()) return d;
    PNode *t = priorityHead;
    d = t->data;
    priorityHead = priorityHead->next;
    free(t);
    return d;
}

void showPriority() {
    if (isPriorityEmpty()) {
        printf("Priority Queue Empty.\n");
        return;
    }
    printf("\n--- Priority Boarding Queue ---\n");
    PNode *t = priorityHead;
    while (t) {
        printf("%d | %s | %s | P:%d\n",
            t->data.id, t->data.name,
            t->data.flightNo, t->priority);
        t = t->next;
    }
}

//==============================================================
//                         STACK (Boarded)
//==============================================================

int isStackEmpty() { return boardedTop == NULL; }

void pushBoard(Passenger p) {
    PNode *n = malloc(sizeof(PNode));
    n->data = p;
    n->next = boardedTop;
    boardedTop = n;
}

Passenger popBoard() {
    Passenger d = {-1,"","",0,0};
    if (isStackEmpty()) return d;
    PNode *t = boardedTop;
    d = t->data;
    boardedTop = boardedTop->next;
    free(t);
    return d;
}

void showBoarded() {
    if (isStackEmpty()) {
        printf("No boarded passengers.\n");
        return;
    }
    printf("\n--- Recently Boarded ---\n");
    PNode *t = boardedTop;
    while (t) {
        printf("%d | %s | %s\n",
            t->data.id, t->data.name, t->data.flightNo);
        t = t->next;
    }
}

//==============================================================
//                         BOARDING LOGIC
//==============================================================

Passenger inputPassenger() {
    Passenger p;
    printf("Enter ID: "); scanf("%d", &p.id);
    printf("Name: "); scanf("%s", p.name);
    printf("Flight No: "); scanf("%s", p.flightNo);
    printf("Age: "); scanf("%d", &p.age);
    printf("Class (0=Eco,1=Bus,2=VIP): "); scanf("%d", &p.classType);
    return p;
}

void boardPassenger() {
    Passenger p;

    if (!isPriorityEmpty()) p = removePriority();
    else if (!isCheckEmpty()) p = dequeueCheck();
    else {
        printf("No passengers waiting.\n");
        return;
    }

    Flight *f = findFlight(p.flightNo);
    if (!f) {
        printf("Flight not found.\n");
        return;
    }

    if (f->bookedSeats >= f->capacity) {
        printf("Flight full.\n");
        return;
    }

    f->bookedSeats++;
    pushBoard(p);

    printf("\nPassenger %s boarded Flight %s\n", p.name, f->flightNo);
    printf("Gate: %s\n", gateNames[f->gateIndex]);
    showPathToGate(f->gateIndex);
}

void undoBoarding() {
    Passenger p = popBoard();
    if (p.id == -1) return;

    Flight *f = findFlight(p.flightNo);
    if (f && f->bookedSeats > 0)
        f->bookedSeats--;

    printf("Undo Successful: Removed %s from boarded list.\n", p.name);
}

//==============================================================
//                         PRELOAD FLIGHTS
//==============================================================

void preload() {
    Flight *a = malloc(sizeof(Flight));
    strcpy(a->flightNo, "AI101");
    strcpy(a->destination, "NYC");
    a->capacity = 3; a->bookedSeats = 0; a->gateIndex = 1; a->next = NULL;
    flightHead = a;

    Flight *b = malloc(sizeof(Flight));
    strcpy(b->flightNo, "BA202");
    strcpy(b->destination, "LON");
    b->capacity = 2; b->bookedSeats = 0; b->gateIndex = 3; b->next = NULL;
    a->next = b;
}

//==============================================================
//                         MAIN MENU
//==============================================================

int main() {
    preload();

    int ch;
    while (1) {
        printf("\n===== AIRPORT SYSTEM =====\n");
        printf("1. Add Flight\n");
        printf("2. Show Flights\n");
        printf("3. Check-In Passenger\n");
        printf("4. Show Check-In Queue\n");
        printf("5. Add to Priority Queue\n");
        printf("6. Show Priority Queue\n");
        printf("7. Board Next Passenger\n");
        printf("8. Show Boarded Stack\n");
        printf("9. Undo Last Boarding\n");
        printf("10. Show Gate List\n");
        printf("11. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        Passenger p;

        switch (ch) {
            case 1: addFlight(); break;
            case 2: showFlights(); break;
            case 3: p = inputPassenger(); enqueueCheck(p); break;
            case 4: showCheck(); break;
            case 5: p = inputPassenger(); insertPriority(p); break;
            case 6: showPriority(); break;
            case 7: boardPassenger(); break;
            case 8: showBoarded(); break;
            case 9: undoBoarding(); break;
            case 10: showGateList(); break;
            case 11: exit(0);
            default: printf("Invalid.\n");
        }
    }
    return 0;
}