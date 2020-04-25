#ifndef LINKEDLIST_H
#define LINKEDLIST_H
// create_node, append_value, lookup, remove_head, remove_tail, remove_node
#include "definitions.h"

extern node *create_node(int val);
extern void insert_node(node *previous_node, node *new_node);
extern void append_value(node **tail, int val);
extern void insert_value(node *previous_node, int val);
extern node *insert_head(node **head, node *new_node);
extern void remove_node(node *old_node);
extern node *get_head(node *any_node);
extern node *get_tail(node *any_node);
extern void remove_head(node **head);
extern void remove_tail(node **tail);
extern node *lookup(node *head, int val);
extern void print_list(node *head);
extern void print_reverse(node *tail);
extern void print_node(node *any_node);
extern void move_ahead(node **head, node *any_node);
extern void shift_left(node **head, node **tail);
extern void shift_right(node **head, node **tail);
extern void swap_nodes(node **node1, node **node2);
#endif