#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {

  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  // nil 노드 생성 및 초기화
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;
  
  return p;
}

void postorder(rbtree *t, node_t *delete_node){    // 삭제를 위한 후위순회
    if(delete_node == NULL || delete_node == t->nil){
        return;
    }
    postorder(t,delete_node->right);
    postorder(t,delete_node->left);
    if(delete_node != NULL){
        free(delete_node);
    }
}

void delete_rbtree(rbtree *t) {
  postorder(t, t->nil);  // 
  free(t->nil);
  free(t);
}

void rotate_left(rbtree *t,node_t *x){
    node_t *y = x->right; // 현재노드의 오른쪽 노드 포인터
    x->right = y->left; // 왼쪽으로 돌리는것. 돌리는 노드 자식 왼쪽 노드가 기준 노드 오른쪽에 붙음
    
    if (y->left != t->nil) { // 원래 자손노드가 닐노드가 아니면 
        y->left->parent = x; // 위에서 y를 x에 넣어 줬으니 조상 갱신 
    }
    y->parent = x->parent; // 조상 노드 갱신
    if (x->parent == t->nil){ // 만약 최상위 노드라면 y를 조상으로 바꿈
        t->root = y;
    }
    else if(x == x->parent->left) { // 완벽하게 이어주기
        x->parent->left = y;
    }
    else {
        x->parent->right = y; // parent 
    }
    y->left = x; // 위치 변환
    x->parent = y;
}

void rotate_right(rbtree *t,node_t *y){
    node_t *x = y->left;
    y->left = x->right;
    // 왼쪽 
    if (x->right != t->nil) {
        x->right->parent = y;
    }
    // 
    x->parent = y->parent;
    if (y->parent == t->nil) {
        t->root = x;
    }
    else if(y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void insert_fixup(rbtree* t,node_t *z){
    while (z->parent->color == RBTREE_RED){
        if (z->parent == z->parent->parent->left) {
            node_t *y = z->parent->parent->right;
            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->right){
                    z = z->parent;
                    rotate_left(t,z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                rotate_right(t,z->parent->parent);
            }
        }
        else{
            node_t *y = z->parent->parent->left;
            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->left){
                    z = z->parent;
                    rotate_right(t,z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                rotate_left(t,z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert kjz
  node_t *z = (node_t*)calloc(1,sizeof(node_t)); 
  z->key = key;                                   

  node_t *y = t->nil;                           
  node_t *x = t->root;                          
  while (x != t->nil)                          
  {
    y = x;
    if(z->key < x->key) {
      x = x->left;                            
    } 
    else x = x->right;
  }

  z->parent = y;                                
  if (y == t->nil) t -> root = z;               
  else if (z -> key < y->key) y->left = z;       
  else y->right = z;
  
  z->left = t -> nil;                          
  z->right = t -> nil;
  z->color = RBTREE_RED;

  insert_fixup(t, z); 
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *ptr = t->root;
  while(ptr != t->nil && key != ptr->key){
    if (key < ptr->key){
        ptr = ptr->left;
    }
    else {
        ptr = ptr->right;
    }
  }
  if (ptr == t->nil) {
    return NULL;
  }
  return ptr;

}

node_t *tree_minimum(node_t *x, node_t *nil) {
  while (x->left != nil){
        x = x->left;
    }
    return x;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *real_min;
  real_min = tree_minimum(t->root, t->nil);
  return real_min;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *real_max = t->root;

  while(real_max->right != t->nil) {
    real_max = real_max -> right;
  }
  return real_max;
}

void rb_transplant(rbtree *t,node_t *n1,node_t *n2) {
    if(n1->parent == t->nil){
        t->root = n2;
    }
    else if(n1 == n1->parent->left){
        n1->parent->left = n2;
    }
    else {
        n1->parent->right = n2;
    }
    n2->parent = n1->parent;
}

void rb_delete_fixup(rbtree *t,node_t *x){
    while (x != t->root && x->color == RBTREE_BLACK){
        if(x == x->parent->left){
            node_t *w = x->parent->right;
            if(w->color == RBTREE_RED){
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotate_left(t,x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else{ 
                if(w->right->color == RBTREE_BLACK){
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotate_right(t, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                rotate_left(t, x->parent);
                x = t->root;
            }
        }
        else {
            node_t *w = x->parent->left;
            if(w->color == RBTREE_RED){
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotate_right(t, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else{ 
                if(w->left->color == RBTREE_BLACK){
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotate_left(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                rotate_right(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *x;
  node_t *y = p;
  int orginal_color = y->color;
  if(p->left == t->nil){
    x = p->right;
    rb_transplant(t, p, p->right);
  }
  else if(p->right == t->nil){
    x = p->left;
    rb_transplant(t, p, p->left);
  }
  else{
    y = tree_minimum(p->right,t->nil);
    orginal_color = y->color;
    x = y->right;
    if(y->parent == p){
        x->parent = y;
    }
    else{
        rb_transplant(t,y,y->right);
        y->right = p->right;
        y->right->parent = y;
    }
    rb_transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (orginal_color == RBTREE_BLACK){
    rb_delete_fixup(t, x);
  }
  return 0;
}


void inorder(const rbtree *t,node_t *search, key_t *arr,int* index){
    if (search == t->nil) return;
    inorder(t, search->left, arr, index);
    arr[(*index)++] = search->key;
    inorder(t, search->right, arr, index);
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *search_node = t->root;
  int *index = calloc(1,sizeof(int));
  inorder(t, search_node, arr, index);
  free(index);
  return 0;
}

