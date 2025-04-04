#include "linked_list.h"

#include <stdlib.h>

// creates a new node with the given value
struct list_node *new_node(size_t value) 
{ 
  struct list_node *node = malloc(sizeof(struct list_node));

  if(node == NULL){return NULL;}

  node->value = value;

  node->next= NULL;

  return node;
}

// puts a new value at the head of the list
void insert_at_head(struct linked_list *list, size_t value) 
{
  struct list_node *node = new_node(value);

  if(node == NULL){return;}

  node->next = list->head;
  list->head = node;
}

// puts a new value at the tail of the list
void insert_at_tail(struct linked_list *list, size_t value)
{
  struct list_node *node = new_node(value);

  if(node == NULL){return;}

  // if the list is empty, we'll set head to the new node
  if (list->head == NULL) 
  {
    list->head = node;
    return;
  }
  
  struct list_node *now_node = list->head;

  while (now_node->next != NULL) 
  {now_node = now_node->next;}

  // appends the new node at the tail
  now_node->next = node;
}

// removes from the head of the list and return the removed value
size_t remove_from_head(struct linked_list *list) 
{ 
  size_t removed_value;
  struct list_node *temp_hold;

  if (list->head == NULL) {return 0;} // there are no head values to be removed

  temp_hold = list->head; // holds the current head to store
  removed_value = temp_hold->value; // this is where we want to remove
  list->head = list->head->next; // moves head to the next node so that the current head is skipped and removed

  free(temp_hold); // frees the memory of the removed node since we don't need it anymore 
  return removed_value;
}

// Remove from the tail of the list and return the removed value
size_t remove_from_tail(struct linked_list *list)
{ 
  size_t removed_value;

  if (list->head == NULL) {return 0;} // once again if there's nothing then we just return the current value of null

  struct list_node *current = list->head; // this stores the current head
  struct list_node *prev = NULL; // this store the previous value

  while (current->next != NULL) // we're iterating through the list until we get to the tail, meaning the next is null
  {
    prev = current;
    current = current->next;
  }

  removed_value = current->value; // then once we ensure that we're at the tail 

  //this is for if there's only one node in the list
  if (prev == NULL) {list->head = NULL;} 
  // else we make sure that the next is null the last node
  else {prev->next = NULL; }

  free(current); // once again we free the node we removed and return it
  return removed_value;
}

// Free the entire linked list
void free_list(struct linked_list list) 
{
  struct list_node *current = list.head;
  struct list_node *next_node;

  while (current != NULL) 
  {
    next_node = current->next;
    free(current);
    current = next_node;
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
