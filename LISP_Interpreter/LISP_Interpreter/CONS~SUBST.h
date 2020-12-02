#ifndef CONS_SUBST
#define CONS_SUBST

#include "type.h"
#include "lexer.h"
#include <stdbool.h>
#include <string.h>
#include "c_dictionary.h"

P_OBJ cons(P_OBJ *_car, P_OBJ *_cdr);
P_OBJ reverse(P_OBJ *p);
int length(P_OBJ *list);
P_OBJ member(void* target, P_OBJ* list);
P_OBJ assoc(void* target, P_OBJ* list);
P_OBJ remove(void* target, P_OBJ* list);
P_OBJ subst(void* a_target, void* b_target, P_OBJ list);

#endif