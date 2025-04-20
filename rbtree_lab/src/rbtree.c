#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed dddddd
  node_t *n = (node_t*)calloc(1,sizeof(node_t));
  n->color = RBTREE_BLACK;
  n->left = n;
  n->right = n;
  n->parent = n;
  p->nil = n;
  p->root = n;
  return p;
}

void delete_node(rbtree*  t, node_t *n){
  if(n == t->nil){
    return;
  }
  delete_node(t, n->left);
  delete_node(t, n->right);
  free(n);
}



void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory

  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

void left_rotation(rbtree *t, node_t* n){
  
  node_t *child = n->right;

  // 부모 오른쪽 자식을 자식의 왼쪽 노드로 (nil도 같이)
  n->right = child->left;

  // 자식의 왼쪽 자식가 nil이 아니라면 왼쪽 자식의 부모를 설정
  if(child->left != t->nil){
    child->left->parent = n;
  }  

  // 조부모를 자식의 부모로 연결
  child->parent = n->parent;

  // 조부모가 nil이면 root가 자식이 된다.
  if(n->parent == t->nil){
    t->root = child;
  }
  // 부모가 조부모의 왼쪽 자식이라면 자식을 조부모의 왼쪽 자식으로 연결
  else if(n == n->parent->left){
    n->parent->left = child;
  }
  // 부모가 조부모의 오른쪽 자식이라면 자식을 조부모의 오른쪽 자식으로 연결
  else{
    n->parent->right = child;
  }

  // 자식의 왼쪽은 부모로 
  child->left = n;

  // 부모의 부모를 자식으로 
  n->parent= child;

}

void right_rotation(rbtree *t, node_t* n){
  
  node_t *child = n->left;

  n->left = child->right;

  if (child->right != t->nil){
    n->left->parent = n;
  }

  child->parent = n->parent;
  if (n->parent == t->nil){
    t->root = child;
  }else if(n->parent->left ==n){
    n->parent->left = child;
  }else{
    n->parent->right = child;
  }

  n->parent = child;
  child->right = n;

}


void insert_fixup(rbtree* t, node_t *n) {
  while (n->parent->color == RBTREE_RED) {
    node_t *uncle;
    //LL or LR
    if (n->parent == n->parent->parent->left) {
      uncle = n->parent->parent->right;
      // 삼촌이 RED일 때
      if (uncle->color == RBTREE_RED) {
        // Case 1
        n->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      } else {
        if (n == n->parent->right) {
          // Case 2
          n = n->parent;
          left_rotation(t, n);
        }
        // Case 3
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        right_rotation(t, n->parent->parent);
      }
    } 
    //RR or RL
    else {
      uncle = n->parent->parent->left;
      // 삼촌이 RED일 때
      if (uncle->color == RBTREE_RED) {
        // Case 1 
        n->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      } 
       // 삼촌이 BLACK일 때
      else {
        if (n == n->parent->left) {
          // Case 2 
          n = n->parent;
          right_rotation(t, n);
        }
        // Case 3
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        left_rotation(t, n->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *cur = t->root;
  
  // 트리가 비었을 경우, 루트에 새로운 노드를 삽입
  if (cur == t->nil) {
    node_t *new = (node_t *)calloc(1, sizeof(node_t));
    new->color = RBTREE_BLACK;  
    new->key = key;
    new->left = t->nil;
    new->right = t->nil;
    new->parent = t->nil;
    t->root = new;  
    return new;
  }

  // 트리가 비지 않았을 경우
  while (cur != t->nil) {
    //key보다 크면 왼쪽
    if (key < cur->key) {
      //위치 찾음
      if (cur->left == t->nil) {
        node_t *new = (node_t *)calloc(1, sizeof(node_t));
        new->color = RBTREE_RED;  
        new->key = key;
        new->parent = cur;
        new->left = t->nil;
        new->right = t->nil;
        cur->left = new;
        insert_fixup(t, new);
        return new;
      }
      cur = cur->left;  // 왼쪽 자식으로 내려가기
    } 
    //key보다 같거나 작으면 오른쪽
    else {
      //위치 찾음
      if (cur->right == t->nil) {
        node_t *new = (node_t *)calloc(1, sizeof(node_t));
        new->color = RBTREE_RED;  
        new->key = key;
        new->parent = cur;
        new->left = t->nil;
        new->right = t->nil;
        cur->right = new;
        insert_fixup(t, new);
        return new;
      }
      cur = cur->right;  // 오른쪽 자식으로 내려가기
    }
  }
  return cur;
}



node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur  = t->root;
  while (cur != t->nil)
  {
    if(cur->key == key){
      return cur;
    }
  
    if(cur->key<key){
      cur = cur->right;
    }
    else {
      cur = cur->left;
    }
  }
  return NULL;
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* cur = t->root;
  if(cur == t->nil){
    return NULL;
  }
  while(cur->left != t->nil){
    cur = cur->left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  
  node_t* cur = t->root;
  if(cur == t->nil){
    return NULL;
  }
  while(cur->right != t->nil){
    cur = cur->right;
  }
  return cur;
}

void erase_fixup(rbtree *t, node_t *n){
  node_t *cur = n;
  while (cur != t->root && cur->color == RBTREE_BLACK)
  {
    //printf("cur key: %d, cur color: %s\n", cur->key, cur->color == RBTREE_RED ? "RED" : "BLACK");
    if (cur == t->nil) break;
    node_t *bro;
    int lr = 0;
    if(cur->parent->left == cur){
      bro = cur->parent->right;
      lr = 1;
    }
    else{
      bro = cur->parent->left;
    }
    // Case 1
    if(bro->color == RBTREE_RED){
      bro->color = RBTREE_BLACK;
      bro->parent->color = RBTREE_RED;
      if(lr==1){
        left_rotation(t,bro->parent);
      }
      else{
        right_rotation(t,bro->parent);
      } 
    }
    else{
      // Case 2
      if((bro != t->nil) && ((bro->left->color == RBTREE_BLACK) && (bro->right->color == RBTREE_BLACK))){
        bro->color = RBTREE_RED;
        if (cur->parent->color == RBTREE_RED) {
          cur->parent->color = RBTREE_BLACK;
          break;  
        }
        cur = cur->parent;
      }
      else{
        // Case 3,4
        if(lr == 1){
          if((bro != t->nil) && (bro->right->color == RBTREE_RED)){
            bro->color = bro->parent->color;
            bro->parent->color = RBTREE_BLACK;
            bro->right->color = RBTREE_BLACK;
            left_rotation(t,bro->parent);
            break;;
          }
          else if((bro != t->nil) && (bro->left->color == RBTREE_RED) && (bro->right->color == RBTREE_BLACK)){
            bro->left->color = RBTREE_BLACK;
            bro->color = RBTREE_RED;
            left_rotation(t,bro);
          }
        }
        else{
          if((bro != t->nil) && (bro->left->color == RBTREE_RED)){
            bro->color = bro->parent->color;
            bro->parent->color = RBTREE_BLACK;
            bro->left->color = RBTREE_BLACK;
            right_rotation(t,bro->parent);
            break;;
          }
          else if((bro != t->nil) && (bro->right->color == RBTREE_RED) && (bro->left->color == RBTREE_BLACK)){
            bro->left->color = RBTREE_BLACK;
            bro->color = RBTREE_RED;
            right_rotation(t,bro);
          }
          
        }
      }
      
      
    }
  }
  cur->color = RBTREE_BLACK;
}


int rbtree_erase(rbtree *t, node_t *p) {
  //TODO: implement erase
  node_t *erase = rbtree_find(t,p->key);
  node_t *child;
  node_t *cur = erase;
  if(cur == NULL){
    return 0;
  }
 
  
  // 자식이 두개 일때 후임자를 찾아 값 바꾸고 후임자 삭제 
  if((cur->left != t->nil) && (cur->right != t->nil)){
    cur = cur -> right;
    while (cur->left != t->nil)
    {
      cur = cur->left;
    }
    erase->key = cur->key;
  }
  // 삭제할 노드의 하나 있는 자식 찾기
  child = (cur->left != t->nil) ? cur->left : cur->right;

  // cur이 root면
  if (cur->parent == t->nil) {
    t->root = child;
  } 
  else if (cur->parent->left == cur) {
    cur->parent->left = child;
  } else {
    cur->parent->right = child;
  }
  // 삭제할 노드의 자식이 없는 경우
  if (child != t->nil) {
    child->parent = cur->parent;
  }

  if(cur->color==RBTREE_BLACK){
    if(child->color==RBTREE_RED){
      child->color =RBTREE_BLACK;
    }
    else{
      erase_fixup(t, child);
    }
  }
  free(cur);
  return 1;
 
}



int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  child_rbtree_to_array(t, arr, n,n ,t->root); 
  return 0;
}


int child_rbtree_to_array(const rbtree *t, key_t *arr, const size_t n ,size_t l, node_t *cur) {
  // TODO: implement to_array
  if (l<=n || cur == t->nil){
    return 0;
  }
  arr[l] = cur->key;
  child_rbtree_to_array(t, arr, n, l-1 ,cur->left); 
  child_rbtree_to_array(t, arr, n,l-2 ,cur->right);

  return 0;
}
