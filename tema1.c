#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  char data;
  struct node *next;
  struct node *prev;
} node;

typedef struct DoubleLinkedList {
  node *sentinel;
  node *mecanic;
  int lenght;
} DoubleLinkedList;

typedef struct node_queue {
  char *data;
  struct node_queue *next, *prev;
} node_queue;

typedef struct node_stack {
  char *data;
  struct node_stack *next;
} node_stack;

typedef struct Queue {
  node_queue *head, *tail;
  int len;
} Queue;

typedef struct Stack {
  node_stack *top;
  int size;
} Stack;

// List initialization
void init_list(DoubleLinkedList **list) {
  *list = (DoubleLinkedList *)malloc(sizeof(DoubleLinkedList));
  (*list)->sentinel = (node *)malloc(sizeof(node));

  if ((*list)->sentinel == NULL) {
    printf("Memory can't be allocated for this list.");
    exit(-1);
  } else {
    (*list)->sentinel->next = (*list)->sentinel;
    (*list)->sentinel->prev = (*list)->sentinel;
    (*list)->mecanic = NULL;
    (*list)->lenght = 0;
  }
}

// Queue initialization
void init_queue(Queue **q) {
  (*q) = (Queue *)malloc(sizeof(Queue));

  if ((*q) == NULL) {
    printf("Memory can't be allocated for this queue.");
    exit(-1);
  }

  (*q)->head = NULL;
  (*q)->tail = NULL;
  (*q)->len = 0;
}

// operation UPDATE
// INSERT_LEFT
// If the mecanic is not in the first node, I go through the list until
// I reach the current position of the mecanic and insert a node to the left
DoubleLinkedList *insert_left(DoubleLinkedList *list, int *position,
                              char index[], FILE *outfile) {
  int i;
  node *current, *new_node;
  if ((*position) < 0 || (*position) > list->lenght) {
    exit(-1);
  }

  new_node = (node *)malloc(sizeof(node));
  if (new_node == NULL) {
    printf("Memory can't be allocated for new node.");
    exit(-1);
  }

  new_node->data = index[0];

  // first position, node can't be inserted on the left
  if ((*position) - 1 == 0) {
    fprintf(outfile, "ERROR\n");
    return list;
  } else {
    current = list->sentinel;
    for (i = 0; i < (*position); i++) current = current->next;
    new_node->prev = current->prev;
    new_node->next = current;
    current->prev->next = new_node;
    current->prev = new_node;
    list->mecanic = current->prev;
  }

  list->lenght++;
  return list;
}

// INSERT RIGHT
// Insert the first carriage # or insert a node to the right
void insert_right(DoubleLinkedList *list, int *position, char index[]) {
  int i;
  node *current, *new_node;
  if ((*position) < 0 || (*position) > list->lenght) {
    exit(-1);
  }

  new_node = (node *)malloc(sizeof(node));
  if (new_node == NULL) {
    printf("Memory can't be allocated for new node.");
    exit(-1);
  }

  new_node->data = index[0];

  if ((*position) == 0) {
    new_node->prev = list->sentinel;
    new_node->next = list->sentinel->next;
    list->sentinel->next->prev = new_node;
    list->sentinel->next = new_node;
    list->mecanic = new_node;
    (*position)++;
  } else {
    current = list->sentinel;
    for (i = 0; i < (*position); i++) current = current->next;
    new_node->next = current->next;
    new_node->prev = current;
    current->next->prev = new_node;
    current->next = new_node;
    list->mecanic = new_node;
    (*position)++;
  }
  list->lenght++;
}

// MOVE LEFT
// Going through the list, I move the mecanic to the left or to the last
// position
char move_left(DoubleLinkedList *list, int *position) {
  int i;
  node *left_node;
  if ((*position) < 0 || (*position) > list->lenght) {
    exit(-1);
  }

  left_node = (node *)malloc(sizeof(node));
  if (left_node == NULL) {
    printf("Memory can't be allocated for left_node node.");
    exit(-1);
  }
  left_node = list->sentinel;

  if ((*position) == 1) {
    for (i = 0; i < list->lenght + 1; i++) left_node = left_node->next;
    list->mecanic = left_node;
    (*position) = list->lenght;
  } else {
    for (i = 0; i < (*position); i++) left_node = left_node->next;
    left_node = left_node->prev;
    list->mecanic = left_node;
    (*position)--;
  }

  return left_node->data;
}

// MOVE RIGHT
// Same as MOVE LEFT, but I make sure to insert # where needed
char move_right(DoubleLinkedList *list, int *position) {
  int i;
  char ch[] = {'#'};
  node *right_node;
  if ((*position) < 0 || (*position) > list->lenght) {
    exit(-1);
  }

  right_node = (node *)malloc(sizeof(node));
  if (right_node == NULL) {
    printf("Memory can't be allocated for right_node node.");
    exit(-1);
  }
  right_node = list->sentinel->next;

  if ((*position) == list->lenght) {
    insert_right(list, position, ch);
  } else {
    for (i = 0; i < (*position); i++) right_node = right_node->next;
    list->mecanic = right_node;
    (*position)++;
  }

  return right_node->data;
}

// WRITE <C>
// I go to the current position by going through the list from the sentinel,
// where I make sure the mecanic is, then I replace the old symbol with
// the new one
void write(DoubleLinkedList *list, int position, char index[]) {
  int i;
  node *write_ch;
  if (position < 0 || position > list->lenght) return;

  write_ch = (node *)malloc(sizeof(sizeof(node)));
  if (write_ch == NULL) {
    printf("Memory can't be allocated for write_ch node.");
    exit(-1);
  }
  write_ch = list->sentinel;

  for (i = 0; i < position; i++) write_ch = write_ch->next;
  list->mecanic = write_ch;

  write_ch->data = index[0];
  list->mecanic->data = write_ch->data;
}

// CLEAR CELL
// If I have only one node, the list will return to its original form. If it is
// the last node, I make sure that I move the mecanic to the left and redo the
// links, otherwise I delete the node, doing roughly the same steps as in the
// case of the last node
void clear_cell(DoubleLinkedList *list, int *position) {
  int i;
  node *delete_node, *tmp;
  char first_carrige_delete[] = {'#'};

  delete_node = list->sentinel->next;

  if (list == NULL) {
    printf("List is empty");
    exit(-1);
  }

  if ((*position) == 1 && list->lenght == 1) {
    list->sentinel->next = list->sentinel;
    list->sentinel->prev = list->sentinel;
    list->mecanic = NULL;
    list->lenght = 0;
    *position = 0;
    insert_right(list, position, first_carrige_delete);
  } else if ((*position) == list->lenght) {
    delete_node = list->sentinel->prev;
    delete_node->prev->next = list->sentinel;
    list->sentinel->prev = delete_node->prev;

    list->mecanic = list->sentinel->prev;
    (*position)--;
    list->lenght--;
    move_left(list, position);
  } else {
    for (i = 0; i < (*position - 1); i++) delete_node = delete_node->next;
    tmp = delete_node->prev;

    delete_node->prev->next = delete_node->next;
    delete_node->next->prev = delete_node->prev;

    list->mecanic = tmp;
    (*position)--;
    list->lenght--;
    move_left(list, position);
  }

  free(delete_node);
}

// CLEAR ALL
// I delete each node, one by one, freeing the memory, restore the links,
// basically reinitialize the list, and finally return the list to its initial
// form
void clear_all(DoubleLinkedList *list, int *position) {
  node *delete_node = (node *)malloc(sizeof(node)),
       *delete_tmp = (node *)malloc(sizeof(node));
  char first_carrige_delete[] = {'#'};

  delete_node = list->sentinel->next;

  if (list == NULL) {
    printf("List is empty");
    exit(-1);
  }

  while (delete_node != list->sentinel) {
    delete_tmp = delete_node->next;
    free(delete_node);
    delete_node = delete_tmp;
  }

  list->sentinel->next = list->sentinel;
  list->sentinel->prev = list->sentinel;
  list->mecanic = NULL;
  list->lenght = 0;
  *position = 0;

  insert_right(list, position, first_carrige_delete);
}

// SHOW CURRENT
// Prints the node where the mecanic is
void show_current(DoubleLinkedList *list, FILE *outfile) {
  fprintf(outfile, "%c", list->mecanic->data);
  fprintf(outfile, "\n");
}

// SHOW
// Prints the whole list, highlighting the mecanic's position
void show(DoubleLinkedList *list, FILE *outfile) {
  node *current = (node *)malloc(sizeof(node));
  current = list->sentinel->next;

  while (current != list->sentinel) {
    if (current == list->mecanic)
      fprintf(outfile, "|%c|", current->data);
    else
      fprintf(outfile, "%c", current->data);
    current = current->next;
  }

  fprintf(outfile, "\n");
}

// Adds commands in the queue, I make sure the links for head and tail are made
// corectly
void create_queue(Queue *q, char s[]) {
  node_queue *aux;
  aux = (node_queue *)malloc(sizeof(node_queue));
  if (aux == NULL) {
    printf("Memory can't be allocated for aux node.");
    exit(-1);
  }

  aux->data = (char *)malloc((strlen(s) + 1) * sizeof(char));
  if (aux->data == NULL) {
    printf("Memory can't be allocated for aux data node.");
    exit(-1);
  }

  strcpy(aux->data, s);
  aux->next = NULL;

  if (q->tail == NULL) {
    aux->prev = NULL;
    q->tail = aux;
    q->head = aux;
  } else {
    aux->prev = q->tail;
    q->tail->next = aux;
    q->tail = aux;
  }
  q->len++;
}

// Deletes first command from queue
void pop_queue(Queue *q) {
  node_queue *p;
  if (q->head == NULL) {
    printf("Queue is empty.");
    exit(-1);
  }

  if (q->head == q->tail) {
    free(q->head->data);
    free(q->head);
    q->head = NULL;
    q->tail = NULL;
  } else {
    p = q->head;
    q->head = q->head->next;
    q->head->prev = NULL;
    free(p->data);
    free(p);
  }
}

// Deletes last command from stack
void pop_stack(Stack *stack) {
  node_stack *new_stack_node;
  if (stack->top == NULL) {
    printf("Stack is empty.");
    exit(-1);
  }

  new_stack_node = stack->top;
  stack->top = stack->top->next;
  stack->size--;
  free(new_stack_node->data);
  free(new_stack_node);
}

// Initialization of stack
void init_stack(Stack **s) {
  *s = (Stack *)malloc(sizeof(Stack));
  if ((*s) == NULL) {
    printf("Memory can't be allocated for stack.");
    exit(-1);
  }
  (*s)->top = NULL;
  (*s)->size = 0;
}

// Adds commands in the stack
char *add_stack(Stack *stack, char s[]) {
  node_stack *stack_tmp;
  stack_tmp = (node_stack *)malloc(sizeof(node_stack));
  if (stack_tmp == NULL) {
    printf("Memory can't be allocated for stack_tmp node.");
    exit(-1);
  }

  stack_tmp->data = (char *)malloc((strlen(s) + 1) * sizeof(char));
  if (stack_tmp->data == NULL) {
    printf("Memory can't be allocated for stack_tmp data node.");
    exit(-1);
  }

  strcpy(stack_tmp->data, s);
  stack_tmp->next = stack->top;
  stack->top = stack_tmp;
  stack->size++;
  return stack_tmp->data;
}

// I check if the queue or stack is empty
int is_empty_stack(Stack *stack) { return stack->top == NULL; }
int is_empty_queue(Queue *q) { return q->tail == NULL; }

// SWITCH
// To reverse the queue, I used a stack
void switch_queue(Queue *q, Stack *stack) {
  while (!is_empty_queue(q)) {
    add_stack(stack, q->head->data);
    pop_queue(q);
  }

  while (!is_empty_stack(stack)) {
    create_queue(q, stack->top->data);
    pop_stack(stack);
  }
}

// SEARCH OPERATIONS
// SEARCH
// I search circularly, going through the list and moving the mecanic when
// needed. I update the position if it is the case, I test letter by letter
// if the strings are identical.
void search(DoubleLinkedList *list, char index[], int *position,
            FILE *outfile) {
  int i, new_position = 0, ok = 0, tmp_position = *position,
         dim = strlen(index);
  node *search_node = (node *)malloc(sizeof(node));
  node *aux = (node *)malloc(sizeof(node));

  search_node = list->sentinel;
  for (i = 0; i < *position; i++) search_node = search_node->next;

  i = 0;
  while (ok <= list->lenght && i < dim) {
    if (search_node == list->sentinel) {
      search_node = search_node->next;
      tmp_position = 1;
      list->mecanic = list->sentinel->next;
    }

    if (search_node->data == index[i]) {
      i++;
      if (i == 1) aux = search_node;
      search_node = search_node->next;
      new_position = tmp_position;
    } else {
      i = 0;
      search_node = search_node->next;
      tmp_position++;
    }
    ok++;
  }

  if (ok == (list->lenght + 1)) {
    fprintf(outfile, "ERROR\n");
    for (i = 0; i <= *position; i++) search_node = search_node->next;

    list->mecanic = search_node->prev;
  } else {
    list->mecanic = aux;
    if (i == dim)
      *position = new_position;
    else
      *position = tmp_position;
  }
}

// SEARCH LEFT
// I go through the list, compare the strings, if they are identical, I move
// the mecanic to the left
void search_left(DoubleLinkedList *list, char index[100], int *position,
                 FILE *outfile) {
  int i, r, new_pos = *position, dim = strlen(index);
  node *current;

  current = list->sentinel;
  for (i = 0; i < (*position); i++) current = current->next;

  i = 0;
  while (current != list->sentinel && i < dim) {
    if (current->data == index[i]) {
      i++;
      current = current->prev;
      new_pos--;
    } else {
      current = current->prev;
      i = 0;
      new_pos--;
    }
  }

  if (current == list->sentinel && i == 0) {
    fprintf(outfile, "ERROR\n");
    for (r = 0; r < (*position); r++) current = current->next;

    list->mecanic = current;
  } else {
    list->mecanic = current->next;
    (*position) = new_pos + 1;
  }
}

// SEARCH RIGHT
// I go through the list and check if the strings match, I move the mecanic to
// the right if it is the case
void search_right(DoubleLinkedList *list, char index[100], int *position,
                  FILE *outfile) {
  int i, r, new_pos = *position, dim = strlen(index);
  node *current;

  current = list->sentinel;
  for (i = 0; i < (*position); i++) current = current->next;

  i = 0;
  while (current != list->sentinel && i < dim) {
    if (current->data == index[i]) {
      i++;
      current = current->next;
      new_pos++;
    } else {
      current = current->next;
      i = 0;
      new_pos++;
    }
  }

  if (current == list->sentinel && i == 0) {
    fprintf(outfile, "ERROR\n");
    for (r = 0; r < (*position); r++) current = current->next;

    list->mecanic = current;
  } else {
    list->mecanic = current->prev;
    (*position) = new_pos - 1;
  }
}

// free list
void free_list(DoubleLinkedList **list) {
  node *tmp = (*list)->sentinel->next, *aux;

  while (tmp != (*list)->sentinel) {
    aux = tmp;
    tmp = tmp->next;
    free(aux);
  }

  free((*list)->sentinel);
  (*list)->sentinel = NULL;
  (*list)->lenght = 0;
  free(*list);
  *list = NULL;
}

// free queue
void free_queue(Queue **q) {
  node_queue *tmp;

  while ((*q)->head != NULL) {
    tmp = (*q)->head;
    (*q)->head = (*q)->head->next;
    free(tmp->data);
    free(tmp);
  }

  free(*q);
  *q = NULL;
}

// free stack
void free_stack(Stack **stack) {
  node_stack *t;

  while ((*stack)->top != NULL) {
    t = (*stack)->top;
    (*stack)->top = (*stack)->top->next;
    free(t->data);
    free(t);
  }

  free(*stack);
  *stack = NULL;
}

// I call the functions according to the commands, then delete the command from
// the queue, I also used strtok to extract the strings that I need for
// operations like search, write etc.
void execute_commands(Queue *q, DoubleLinkedList *list, int *position,
                      FILE *outfile) {
  char s[100], *string, index[100];

  if (!is_empty_queue(q)) {
    if (strncmp(q->head->data, "INSERT_LEFT", 11) == 0) {
      strcpy(s, q->head->data);
      string = strtok(s, " ");
      if (string != NULL) {
        string = strtok(NULL, " ");
        strcpy(index, string);
      }

      insert_left(list, position, index, outfile);

    } else if (strncmp(q->head->data, "INSERT_RIGHT", 12) == 0) {
      strcpy(s, q->head->data);
      string = strtok(s, " ");
      if (string != NULL) {
        string = strtok(NULL, " ");
        strcpy(index, string);
      }

      insert_right(list, position, index);
    } else if (strcmp(q->head->data, "MOVE_LEFT") == 0) {
      move_left(list, position);
    } else if (strcmp(q->head->data, "MOVE_RIGHT") == 0) {
      move_right(list, position);
    } else if (strncmp(q->head->data, "WRITE", 5) == 0) {
      strcpy(s, q->head->data);
      string = strtok(s, " ");
      if (string != NULL) {
        string = strtok(NULL, " ");
        strcpy(index, string);
      }

      write(list, *position, index);
    } else if (strcmp(q->head->data, "CLEAR_ALL") == 0) {
      clear_all(list, position);
    } else if (strcmp(q->head->data, "CLEAR_CELL") == 0) {
      clear_cell(list, position);
    } else if (strncmp(q->head->data, "SEARCH_LEFT", 11) == 0) {
      strcpy(s, q->head->data);
      string = strtok(s, " ");
      if (string != NULL) {
        string = strtok(NULL, " ");
        strcpy(index, string);
      }

      search_left(list, index, position, outfile);
    } else if (strncmp(q->head->data, "SEARCH_RIGHT", 12) == 0) {
      strcpy(s, q->head->data);
      string = strtok(s, " ");
      if (string != NULL) {
        string = strtok(NULL, " ");
        strcpy(index, string);
      }

      search_right(list, index, position, outfile);
    } else if (strncmp(q->head->data, "SEARCH ", 7) == 0) {
      strcpy(s, q->head->data);
      string = strtok(s, " ");
      if (string != NULL) {
        string = strtok(NULL, " ");
        strcpy(index, string);
      }

      search(list, index, position, outfile);
    }
    pop_queue(q);
  }
}

// I initialize the list, the queue, the stack, read the commands, call the
// ones that should not be put in the queue. Also, I read and display in the
// file.
int main(void) {
  FILE *infile, *outfile;
  int n, i, position = 0;
  DoubleLinkedList *list;
  Queue *q;
  Stack *stack;
  char s[100];
  char first_carrige[] = {'#'};

  infile = fopen("tema1.in", "r");
  if (infile == NULL) {
    printf("infile can't be opened.");
    return 1;
  }

  outfile = fopen("tema1.out", "w");
  if (outfile == NULL) {
    printf("outfile can't be opened.");
    return 1;
  }

  init_list(&list);
  insert_right(list, &position, first_carrige);
  init_queue(&q);
  init_stack(&stack);

  fscanf(infile, "%d", &n);
  fgetc(infile);

  for (i = 0; i < n; i++) {
    fgets(s, sizeof(s), infile);
    s[strlen(s) - 1] = '\0';

    if (strcmp(s, "SHOW_CURRENT") == 0) {
      show_current(list, outfile);
    } else if (strcmp(s, "SHOW") == 0) {
      show(list, outfile);
    } else if (strcmp(s, "SWITCH") == 0) {
      switch_queue(q, stack);
    } else if (strcmp(s, "EXECUTE") == 0) {
      execute_commands(q, list, &position, outfile);
    } else {
      create_queue(q, s);
    }
  }

  free_list(&list);
  free_queue(&q);
  free_stack(&stack);

  fclose(infile);
  fclose(outfile);
  return 0;
}
