/*
 * 리스트 노드를 데이터가 포함하는 방식으로 구현한 간단한 리스트
 *
 * head를 중심으로 원형 리스트를 형성합니다.
 * 이때, head는 리스트 자체를 의미하며 어떠한 데이터에도 속하지 않습니다.
 * 기본적인 list 연산들은 단순하므로 추가적인 코멘트를 달지 않겠습니다.
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

/* 현 과제에서는 사용되지 않습니다. */
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

/* container_of:
 * 해당 멤버변수가 속한 구조체의 포인터를 리턴합니다.
 */
#define container_of(ptr, type, member)\
        (type *)((char *)ptr - offsetof(type, member))

/* for_each_list_node:
 * list를 next 방향으로 순차적으로 순회합니다.
 */
#define for_each_list_node(head)\
        for (struct list_node *node = (head).next;\
             node != &(head); node = node->next)

#endif