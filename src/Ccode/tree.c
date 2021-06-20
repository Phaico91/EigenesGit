#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

struct Tree /* only define a struct, typedef in tree.h */
{
  char *word;      /* points to copy of the word */
  unsigned long count;   /* number of its occurrences */
  struct Tree *left,   /* left child */
              *right;  /* right child */
};

static Tree *tree_node_new(const char *word)
{
  Tree *n = malloc(sizeof *n); /* create new node */
  assert(n != NULL);
  n->word = strdup(word); /* duplicate word */
  assert(n->word != NULL);
  n->count = 1;       /* seen for the first time */
  n->left = n->right = NULL; /* it is a new leaf */
  return n;
}

Tree *tree_add(Tree *n,const char *word)
{
  if (n == NULL)
  {
    n = tree_node_new(word);
  } else
  {
    int cond = strcmp(word,n->word); /* compare words */
    if (cond == 0) /* word and n->word are identical */
    {
      n->count++; /* already found => increment count */
    } else
    {
      if (cond < 0) /* word lex. smaller than n->word */
        n->left = tree_add(n->left,word);
      else /* word lex. larger than n->word */
        n->right = tree_add(n->right,word);
    }
  }
  return n;
}

void tree_print(int d,const Tree *node)
{ /* d = depth of recursion, used only for indentation of output */
  if (node != NULL)
  {
    tree_print(d+1,node->right);
    printf("%*s%s %lu\n",d*3,"",node->word,node->count);
    tree_print(d+1,node->left);
  }
}

void tree_print_tikz(int depth, const Tree *node)
{
  if (node != NULL) {
    printf("%*.*snode {%s %lu}", depth*2, depth*2, "", node->word, node->count);
    if (node->left) {
      printf ("\n%*.*schild {\n", depth*2, depth*2, "");
      tree_print_tikz(depth+1, node->left);
      printf ("}");
    }
    if (node->right) {
      printf ("\n%*.*schild {\n", depth*2, depth*2, "");
      tree_print_tikz(depth+1, node->right);
      printf ("}");
    }
  }
  if (depth == 0)
    printf("\n");
}

#ifdef TSEARCH_COMPACT
Tree *tree_add(Tree *n,
                const char *word)
{
  if (n == NULL)
    n = tree_node_new(word);
  else
    if (strcmp(word,n->word) == 0)
      n->count++;
    else
      if (strcmp(word,n->word) < 0)
        n->left = tree_add(n->left,word);
      else
        n->right = tree_add(n->right,word);
  return n;
}
#endif

void tree_delete(Tree *node)
{
  if (node == NULL)
    return;
  if (node->left != NULL)
    tree_delete(node->left);
  if (node->right != NULL)
    tree_delete(node->right);
  free(node->word);
  free(node);
}
