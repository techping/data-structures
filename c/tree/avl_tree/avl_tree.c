/*
 * file: avl_tree.h
 * author: Ziping Chen
 * github: techping
 */
#include "avl_tree.h"
#include <malloc.h>

static inline int max(int n1, int n2)
{
	return n1 > n2 ? n1 : n2;
}

static int height(position p)
{
	if (!p)
		return -1;
	return p->height;
}

/*   O     <- k2
 *     O   <- k1
 *       O
 */
static position single_rotate_left(position k2)
{
	position k1;
	k1 = k2->right;
	k2->right = k1->left;
	k1->left = k2;
	k2->height = max(height(k2->left), height(k2->right)) + 1;
	k1->height = max(height(k1->right), k2->height) + 1;
	return k1;
}

/*       O <- k2
 *     O   <- k1
 *   O
 */
static position single_rotate_right(position k2)
{
	position k1;
	k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = max(height(k2->left), height(k2->right)) + 1;
	k1->height = max(height(k1->left), k2->height) + 1;
	return k1;
}

/*     O <- k3
 *   O   <- k2
 *     O <- k1
 */
static position double_rotate_left_right(position k3)
{
	k3->left = single_rotate_left(k3->left);
	return single_rotate_right(k3);
}

/*   O   <- k3
 *     O <- k2
 *   O   <- k1
 */
static position double_rotate_right_left(position k3)
{
	k3->right = single_rotate_right(k3->right);
	return single_rotate_left(k3);
}

avl_tree create_tree(element_type x)
{
	return insert(x, NULL);
}

void delete_tree(avl_tree t)
{
	if (!t)
		return;
	delete_tree(t->left);
	delete_tree(t->right);
	free(t);
}

position find(element_type x, avl_tree t)
{
	if (!t)
		return NULL;
	else if (x < t->data)
		return find(x, t->left);
	else if (x > t->data)
		return find(x, t->right);
	else
		return t;
}

position find_min(avl_tree t)
{
	while (t->left)
		t = t->left;
	return t;
}

position find_max(avl_tree t)
{
	while (t->right)	
		t = t->right;
	return t;
}

avl_tree insert(element_type x, avl_tree t)
{
	if (!t) {
		t = malloc(sizeof(struct node));
		t->left = t->right = NULL;
		t->data = x;
		t->height = 0;
	} else if (x < t->data) {
		t->left = insert(x, t->left);
		if (height(t->left) - height(t->right) == 2)
			if (x < t->left->data)
				t = single_rotate_right(t);
			else
				t = double_rotate_left_right(t);
	} else if (x > t->data) {
		t->right = insert(x, t->right);
		if (height(t->right) - height(t->left) == 2)
			if (x > t->right->data)
				t = single_rotate_left(t);
			else
				t = double_rotate_right_left(t);
	}
	/* else x is in the tree already, we'll do nothing */
	t->height = max(height(t->left), height(t->right)) + 1;
	return t;
}

avl_tree delete(element_type x, avl_tree t)
{
	position tmp;
	if (!t)
		return NULL;
	if (x < t->data) {
		t->left = delete(x, t->left);
		if (height(t->left) - height(t->right) == 2)
			if (height(t->left->left) >= height(t->left->right))
				t = single_rotate_right(t);
			else
				t = double_rotate_left_right(t);
		else if (height(t->left) - height(t->right) == -2)
			if (height(t->right->right) >= height(t->right->left))
				t = single_rotate_left(t);
			else
				t = double_rotate_right_left(t);
	} else if (x > t->data) {
		t->right = delete(x, t->right);
		if (height(t->left) - height(t->right) == 2)
			if (height(t->left->left) >= height(t->left->right))
				t = single_rotate_right(t);
			else
				t = double_rotate_left_right(t);
		else if (height(t->left) - height(t->right) == -2)
			if (height(t->right->right) >= height(t->right->left))
				t = single_rotate_left(t);
			else
				t = double_rotate_right_left(t);
	} else if (t->left && t->right) {
		/* put the minimum of right-child or the maximum of left-child on are ok */
		tmp = find_min(t->right);
		t->data = tmp->data;
		t->right = delete(t->data, t->right);
		if (height(t->left) - height(t->right) == 2)
			if (height(t->left->left) >= height(t->left->right))
				t = single_rotate_right(t);
			else
				t = double_rotate_left_right(t);
		else if (height(t->left) - height(t->right) == -2)
			if (height(t->right->right) >= height(t->right->left))
				t = single_rotate_left(t);
			else
				t = double_rotate_right_left(t);
	} else if (t->left) {
		tmp = t;
		t = t->left;
		free(tmp);
	} else if (t->right) {
		tmp = t;
		t = t->right;
		free(tmp);
	} else {//zero children
		free(t);
		t = NULL;
	}
	if (t)
		t->height = max(height(t->left), height(t->right)) + 1;
	return t;
}
