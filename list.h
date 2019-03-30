/*
 * 리스트 노드를 데이터가 포함하는 방식으로 구현한
 * 간단한 리스트
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

struct list_node {
  struct list_node *prev;
  struct list_node *next;
};

static inline void list_init(struct list_node *head)
{
        head->prev = head;
        head->next = head;
}

static inline void list_insert(struct list_node *new, struct list_node *prev)
{
        struct list_node *next = prev->next;
        prev->next = new;
        new->prev = prev;
        new->next = next;
        next->prev = new;
}

static inline void list_delete(struct list_node *target)
{
        target->prev->next = target->next;
        target->next->prev = target->prev;
}

static inline int list_empty(struct list_node *head)
{
        return (head->next == head);
}

static void inline list_concat(struct list_node *left_merged,
                               struct list_node *right)
{
        if (!list_empty(right)) {
                left_merged->prev->next = right->next;
                right->next->prev = left_merged->prev;
                left_merged->prev = right->prev;
                right->prev->next = left_merged;
        }
}

#define container_of(ptr, type, member)\
        (type *)((char *)ptr - offsetof(type, member))

#define for_each_list_node(head)\
        for (struct list_node *node = (head).next;\
             node != &(head); node = node->next)

#endif