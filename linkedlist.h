#ifndef LINKEDLIST_HEADER
#define LINKEDLIST_HEADER

typedef struct node {
    int data;
    struct node* next;
} Node;

typedef struct list {
    Node * head; 
} List;

List* makelist();
void add(int data, List * list);
void delete(int data, List * list);
void display(List * list);
void destroy(List * list);
int find(int data, List * list);

#endif
