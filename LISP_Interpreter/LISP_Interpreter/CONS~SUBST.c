#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <CONS~SUBST.h>

/*
필요한 함수 목록: car , cdr 
*/

//CONS
//EX) (cons 'a '(b c)) => (a b c)를 리턴한다.
P_OBJ cons(P_OBJ *_car, P_OBJ *_cdr){
    P_OBJ *cell;
    cell = malloc(2*sizeof(P_OBJ));
    cell->t_list_value = _car;
    cell->next = _cdr;
    
    return cell;
}

//REVERSE
//EX) (reverse (a (b c) d)) => (d (b c) a)를 리턴한다.
P_OBJ reverse(P_OBJ *p){
    P_OBJ *ret = NIL;
    while(p != NIL){
        P_OBJ* head = p;
        p = p->cdr; 
        head->cdr = ret;
        ret = head;
    }

    return ret;
}
//APPEND
//EX) (append '(a) '(b c)) => (a b c)를 리턴한다.



//LENGTH
//EX) (length 'a 'b) => 2
int length(P_OBJ *list){
    int len = 0;
    for (;;list = list ->cdr)
        len++;
    return len;
}


//MEMBER
/* EX)
> (SETQ CLUB '(TOM HARRY JOHN DANIEL))
(TOM HARRY JOHN DANIEL)
> (MEMBER 'HARRY CLUB)
(HARRY JOHN DANIEL) ;  CLUB내의 원소 HARRY부터 그 리스트의 끝까지 반환됨
*/
P_OBJ member(void* target, P_OBJ* list){
    //원소 자료형에 따라 해줘야하는데..
    P_OBJ* temp = list -> next;

    while(temp != NIL){
        if(temp->t_list_value == target)
            return temp;

        temp = temp->next;
    }    
}
//ASSOC
/* 
EX)
> (ASSOC 'TWO '((ONE 1)(TWO 2)(THREE 3)))
(TWO 2) ; 첫번째 원소가 TWO인 리스트를 찾아내서 리턴함
*/
P_OBJ assoc(void* target, P_OBJ* list){
    P_OBJ *temp = list->next;

    while(temp != NIL){
        if(temp->t_list_value == target)
            return temp;

        temp = temp -> next;
    }
}

//REMOVE
/*
Ex)
> (SETQ MYLIST '(A B C D E F))
(A B C D E F)
> (REMOVE 'D MYLIST)
(A B C E F) ;  D가 삭제되었다.
> (SETQ MYLIST '(A D B C D E D F))
> (REMOVE D MYLIST)
(A B C E F) ;  D가 중복되어 여러개가 있어도 모두 삭제한다.
*/
P_OBJ remove(void *target, P_OBJ* list){
    P_OBJ *temp = list;

    if (head == NIL) return;
    
    
    if(temp->data == target)
}
//SUBST
/*
EX)
> (SUBST 'GOOD 'BAD '(I AM BAD))
  (I AM GOOD) ;  BAD가 GOOD으로 대치되었다.
*/
