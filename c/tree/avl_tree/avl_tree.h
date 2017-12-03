/*
 * file: avl_tree.h
 * author: Ziping Chen
 * github: techping
 */
#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#define NULL 0

typedef int element_type;
typedef struct node *position;
typedef struct node *avl_tree;

avl_tree create_tree(element_type x);
void delete_tree(avl_tree t);
position find(element_type x, avl_tree t);
position find_min(avl_tree t);
position find_max(avl_tree t);
avl_tree insert(element_type x, avl_tree t);
avl_tree delete(element_type x, avl_tree t);

struct node
{
	element_type data;
	position left;
	position right;
	int height;
};

#endif /* _AVL_TREE_H */
