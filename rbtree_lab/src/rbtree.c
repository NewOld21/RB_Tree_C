#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed dddddd
  node_t *n = (node_t*)calloc(1,sizeof(node_t));
  n->color = RBTREE_BLACK;
  p->nil = n;
  p->root = n;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
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


void insert_fixup(rbtree* t, node_t *n){
  node_t*par = n->parent;
  while(par->color == RBTREE_RED){
    node_t*uncle;
    if(par->parent->left != par){
      uncle = par->parent->left;
    }
    else{
      uncle = par->parent->right;
    }
    

    if (uncle->color == RBTREE_RED){
      uncle->color = RBTREE_BLACK;
      par->color = RBTREE_BLACK;
      par->parent->color = RBTREE_RED;
    }
    else{
      if(par->parent->right->left == n){
        right_rotation(t, par);
      }
      else if(par->parent->left->right == n){
        left_rotation(t,par);
      }


      if(par->left == n){
        par->color = RBTREE_BLACK;
        par->parent->color = RBTREE_RED;
        right_rotation(t,par->parent);
      }
      else if(par->right == n){
        par->color = RBTREE_BLACK;
        par->parent->color = RBTREE_RED;
        left_rotation(t, par->parent);
      }

      par = par->parent;
      if (par == t->root){
        par->color = RBTREE_BLACK;
        break;
      }
    }
  }
}



node_t *rbtree_insert(rbtree *t, const key_t key) {
  //TODO: implement insert
  node_t *cur  = t->root;
  if(cur==t->nil){
    node_t *new = (node_t *)calloc(1, sizeof(node_t));
    new->color = RBTREE_BLACK;
    new->key = key;
    new->left = t->nil;
    new->right = t->nil;
    new->parent = t->nil;
    t->root = new;
    return new;
  }

  if(cur->key<key){
    if (cur->right==t->nil){
      node_t *new = (node_t*)calloc(1,sizeof(node_t));;
      new->color = RBTREE_RED;
      new->key = key;
      new->parent = cur;
      new->left = t->nil;
      new->right = t->nil;
      cur->right = new;
      insert_fixup(t, new);
      return new;
    }
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    p->nil = t->nil;
    p->root = cur->right;
    cur = rbtree_insert(p, key);
    free(p);
  }
  else {
    if (cur->left==t->nil){
      node_t *new = (node_t*)calloc(1,sizeof(node_t));;
      new->color = RBTREE_RED;
      new->key = key;
      new->parent = cur;
      new->left = t->nil;
      new->right = t->nil;
      cur->left = new;
      insert_fixup(t, new);
      return new;
    }
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    p->nil = t->nil;
    p->root = cur->left;
    cur = rbtree_insert(p, key);
    free(p);
  }
  
  return cur;
}




node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur  = t->root;
  if(cur==t->nil){
    return NULL;
  }
  if(cur->key == key){
    return cur;
  }

  if(cur->key<key){
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    p->nil = t->nil;
    p->root = cur->right;
    cur = rbtree_find(p, key);
    free(p);
  }
  else {
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    p->nil = t->nil;
    p->root = cur->left;
    cur = rbtree_find(p, key);
    free(p);
  }
  
  return cur;
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

void erase_fixup(rbtree *t, node_t *n ){

}


int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // node_t *cur = t->root;
  
  // if(cur == t->nil || p==NULL){
  //   return 0;
  // }

  // // 삭제할 노드를 찾음
  // if(cur->key == p->key){
  //   //삭제할 노드의 자식이 없을 때
  //   if(cur->left == cur->right == t->nil){
  //     //삭제할 노드가 red일 때
  //     if(cur->color ==RBTREE_RED){
  //       if(cur->parent->left == cur){
  //         cur->parent->left = t->nil;
  //       }
  //       else{
  //         cur->parent->right = t->nil;
  //       }
  //       free(cur);
  //       return 1;
  //     }
  //     //삭제 노드가 black일때
  //     else{
  //       erase_fixup(t,cur);
  //       return 1;
  //     }
  //   }

  //   else if(cur->left == t->nil && cur->right != t->nil){
  //     //삭제할 노드가 red일 때
  //     if(cur->color ==RBTREE_RED){
  //       if(cur->parent->left == cur){
  //         cur->parent->left = cur->right;
  //         cur->right->parent = cur->parent;
  //       }
  //       else{
  //         cur->parent->right = cur->right;
  //         cur->right->parent = cur->parent;
  //       }
  //       free(cur);
  //       return 1;
  //     }
  //     //삭제할 노드가 black이고 자식이 red일 때
  //     else if(cur->right->color == RBTREE_RED){
  //       cur->right->color = RBTREE_BLACK;
  //       if(cur->parent->left == cur){
  //         cur->parent->left = cur->right;
  //         cur->right->parent = cur->parent;
  //       }
  //       else{
  //         cur->parent->right = cur->right;
  //         cur->right->parent = cur->parent;
  //       }
  //       free(cur);
  //       return 1;
  //     }
  //     //삭제 노드가 black일때
  //     else{

  //     }
  //   }
  //   else if(cur->left != t->nil && cur->right == t->nil){
  //     //삭제할 노드가 red일 때
  //     if(cur->color ==RBTREE_RED){
  //       if(cur->parent->left == cur){
  //         cur->parent->left = cur->left;
  //         cur->left->parent = cur->parent;
  //       }
  //       else{
  //         cur->parent->right = cur->left;
  //         cur->left->parent = cur->parent;
  //       }
  //       free(cur);
  //       return 1;
  //     }
  //     //삭제할 노드가 black이고 자식이 red일 때
  //     else if(cur->left->color == RBTREE_RED){
  //       cur->left->color = RBTREE_BLACK;
  //       if(cur->parent->left == cur){
  //         cur->parent->left = cur->left;
  //         cur->left->parent = cur->parent;
  //       }
  //       else{
  //         cur->parent->right = cur->left;
  //         cur->left->parent = cur->parent;
  //       }
  //       free(cur);
  //       return 1;
  //     }
  //     //삭제 노드가 black일때
  //     else{

  //     }
  //   }
  //   else{
  //     node_t*successor = cur->right;
  //     while (successor->left != t->nil)
  //     {
  //       successor = successor->left;
  //     }
  //     cur->key = successor->key;
  //     //삭제할 노드가 red일 때
  //     if(successor->color ==RBTREE_RED){
  //       if(successor->right != t->nil){
  //         successor->parent->left = successor->right;
  //         successor->right->parent = successor->parent;
  //       }
  //       else{
  //         successor->parent->left = t->nil;
  //       }
  //       free(successor);
  //       return 1;
  //     }
  //     //삭제 노드가 black일때
  //     else{

  //     }

  //   }
  // }
  // // 삭제할 노드의 값이 현재 노드의 값보다 클 때
  // else if(cur->key<p->key ){
  //   return rbtree_erase(t,cur->left);
  // }
  // // 삭제할 노드의 값이 현재 노드의 값보다 작을 때
  // else {
  //   return rbtree_erase(t,cur->left);
  // }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
