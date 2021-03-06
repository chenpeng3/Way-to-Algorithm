// MIT License
// Copyright 2017 zhaochenyou16@gmail.com

#pragma once

#include <algorithm>
using namespace std;
#ifndef MAX
#define MAX 64
#endif


//
// interface
//

struct LeftistTree;

struct Node {
  //节点下标
  int index;
  int distance;
  Node *left;
  Node *right;
  LeftistTree *tree;
};

struct LeftistTree {
  Node *root;
  int size;
  int (*cmp)(Node*, Node*);
};

LeftistTree *LeftistTreeNew(int (*Compare)(Node *a, Node *b));
void LeftistTreeFree(LeftistTree *t);
LeftistTree *LeftistTreeMerge(LeftistTree *a, LeftistTree *b);
int LeftistTreeTop(LeftistTree *t);
int LeftistTreePush(LeftistTree *t, int index);
int LeftistTreePop(LeftistTree *t);


//
// implement
//

namespace detail {

  void NodeFree(Node *e);
  Node *NodeMerge(Node *a, Node *b, LeftistTree *t);

}

LeftistTree *LeftistTreeNew(int (*Compare)(Node *a, Node *b)) {
  LeftistTree *t = new LeftistTree();
  if (!t) return nullptr;
  t->cmp = Compare;
  t->root = nullptr;
  t->size = 0;
  return t;
}

void LeftistTreeFree(LeftistTree *t) {
  detail::NodeFree(t->root);
  delete t;
}

LeftistTree *LeftistTreeMerge(LeftistTree *a, LeftistTree *b) {
  LeftistTree *t = new LeftistTree();
  if (!t)
    return nullptr;
  t->cmp = a->cmp;
  t->size = a->size + b->size;
  t->root = detail::NodeMerge(a->root, b->root, a);
  return t;
}

int LeftistTreeTop(LeftistTree *t) {
  return t->root ? t->root->index : -1;
}

int LeftistTreePush(LeftistTree *t, int index) {
  Node *e = new Node();
  if (!e)
    return -1;
  e->distance = 0;
  e->index = index;
  e->left = nullptr;
  e->right = nullptr;
  e->tree = nullptr;

  t->root = detail::NodeMerge(t->root, e, t);
  t->size += 1;

  return 0;
}

int LeftistTreePop(LeftistTree *t) {
  if (t->size <= 0)
    return -1;

  Node *old = t->root;
  t->root = detail::NodeMerge(t->root->left, t->root->right, t);
  t->size -= 1;
  delete old;

  return 0;
}

namespace detail {

  void NodeFree(Node *e) {
    if (!e) return;
    NodeFree(e->left);
    NodeFree(e->right);
    delete e;
  }

  Node *NodeMerge(Node *a, Node *b, LeftistTree *t) {
    if (!a and !b)
      return nullptr;
    if (!a) {
      b->tree = t;
      return b;
    }
    if (!b) {
      a->tree = t;
      return a;
    }

    if (t->cmp(a, b) > 0) {
      return NodeMerge(b, a, t);
    }

    a->right = NodeMerge(a->right, b, t);
    a->right->tree = t;

    if (!a->left) {
      swap(a->left, a->right);
    } else {
      if (a->left->distance < a->right->distance)
        swap(a->left, a->right);
      a->distance = a->right->distance + 1;
    }

    return a;
  }

}
