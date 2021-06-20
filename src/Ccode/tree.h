#ifndef TREE_H
#define TREE_H

typedef struct Tree Tree; /* interface only has _name_ of type */

/* and functions manipulating values of that type */

Tree *tree_add(Tree *node,const char *word);
void tree_print(int depth,const Tree *node);
void tree_delete(Tree *node);
void tree_print_tikz(int depth, const Tree *node);

#endif
