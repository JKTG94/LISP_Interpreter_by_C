#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "lexer.h"
#include "util.h"
#include "OP~NTH.h"

#include "CONS~SUBST.h"
//머야 씨펄
/*
필요한 함수 목록: car , cdr 
*/

//T_OBJ에 car과 cdr이 있다고 가정
//car은 리스트의 첫번째, cdr은 그나머지

//CONS
//EX) (cons 'a '(b c)) => (a b c)를 리턴한다.
// A 와 'A와 '(A B C)를 구분해야함

T_OBJ fn_cons(){
    if(cur_node->value.type == LEFT_PAREN){
        left_paren_Count++;
        cur_node = cur_node->next;
        cur_node = cur_node->next;
    }
    else{
        printf("ERROR");
        return return_false();
    }
    
	int cnt = 0;
	c_LIST* tmp_list = initialize_list();
    while(cnt < 2){
        	//리스트를 생성하기 위한 임시 리스트
        while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
            T_OBJ tmp;
            if (cur_node->value.type == SQUOTE) {
                cur_node = cur_node->next;
                if (cur_node->value.type == LEFT_PAREN) {	//괄호가 올 경우 처리함
                    LIST_NODE* tmp_node = cur_node->next;
                    if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
                        //괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
                        tmp = fn_make_list();
                    }
                    else {
                        //괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
                        tmp = call_fn();
                    }
                }
                else {	//괄호가 없을 경우 현재 노드의 value를 tmp에 할당
                    tmp = cur_node->value;
                    cur_node = cur_node->next;
                }
            }
            else if (cur_node->value.type == IDENT) {
                tmp = get_dict_obj(dict, cur_node->value.t_string);
                cur_node = cur_node->next;
            }
            else {
                printf("ERROR : TYPE ERROR FOR LIST\n");
                free_list(tmp_list);
                return return_false();
            }
            if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
                tmp.type = STRING;
                insert_list_node(tmp_list, &tmp);
            }
            else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
                insert_list_node(tmp_list, &tmp);
            }
            else {
                printf("ERROR : TYPE ERROR FOR LIST\n");
                free_list(tmp_list);
                return return_false();
            cnt++;
        	}
		}
    }

    T_OBJ head;
    T_OBJ* pre_obj = &head;
    head.type = T_LIST;
    head.t_int = cnt;
    head.next = head.t_list_value = NULL;
    

    LIST_NODE* tmp_node = tmp_list->head;
    while(tmp_node != NULL){
        if(head.t_list_value == NULL){
            head.t_list_value = &(tmp_node->value);
        }
        else{
            T_OBJ* tmp = malloc(sizeof(T_OBJ));
            tmp->type = T_LIST;
            tmp->t_int = cnt;
            tmp->t_list_value = &(tmp_node->value);
            pre_obj->next = tmp;
            pre_obj = tmp;
        }
        tmp_node = tmp_node->next;
        if(cur_node->value.type == RIGHT_PAREN){
            right_paren_Count++;
            cur_node = cur_node->next;
            free(tmp_list);
            return head;
        }
        else{
            printf("ERROR");
            free_list(tmp_list);
            return return_false();
        }
    }
}

//REVERSE
//EX) (reverse (a b c d)) => (d b c a)를 리턴한다.
T_OBJ fn_reverse(){
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR LIST\n");
		return return_false();
	}

	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//리스트를 생성하기 위한 임시 리스트
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
		T_OBJ tmp;
		if (cur_node->value.type == SQUOTE) {
			cur_node = cur_node->next;
			if (cur_node->value.type == LEFT_PAREN) {	//괄호가 올 경우 처리함
				LIST_NODE* tmp_node = cur_node->next;
				if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
					//괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
					tmp = fn_make_list();
				}
				else {
					//괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
					tmp = call_fn();
				}
			}
			else {	//괄호가 없을 경우 현재 노드의 value를 tmp에 할당
				tmp = cur_node->value;
				cur_node = cur_node->next;
			}
		}
		else if (cur_node->value.type == IDENT) {
			tmp = get_dict_obj(dict, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		cnt++;
	}
	printf("list size : %d , %d\n", tmp_list->list_size, cnt);	//디버깅용
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	if (cnt == 0) {	//인자가 0개면 길이가 0인 리스트를 반환한다.
		return head;
	}
	//아니라면 임시로 만든 리스트의 값을 이용해서 리스트를 생성한다.
	LIST_NODE* tmp_node = tmp_list->head;
    //tmp_node의 값을 reverse해준다. 다음은 reverse을 위한 임시 노드
    LIST_NODE* a = NULL;
	LIST_NODE* b = 1234567;

    while(tmp_node != NULL){
        b = tmp_node->next;
        tmp_node->next = a;
        a = tmp_node;
        tmp_node = b;
    }

	while (b != NULL) {
		if (head.t_list_value == NULL) {
			head.t_list_value = &(b->value);
		}
		else { 
			T_OBJ* tmp = malloc(sizeof(T_OBJ));
			tmp->type = T_LIST;
			tmp->t_int = cnt;
			tmp->t_list_value = &(b->value);
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		b = b->next;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		free(tmp_list);
        return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		free_list(tmp_list);
		return return_false();
	}
}
//APPEND
//EX) (append '(a) '(b c)) => (a b c)를 리턴한다.
//조건을 어떻게 해야할지 고민중.
T_OBJ fn_append(){
    int tmp_count = 0;
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
        tmp_count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR");
		return return_false();
	}
    int cnt = 0;
    c_LIST* tmp_list = initialize_list();

    while(cnt < 2){
        while(cur_node->value.type == RIGHT_PAREN){
            T_OBJ tmp;
            if(cur_node->value.type == SQUOTE){
                cur_node = cur_node->next;
                if(cur_node->value.type == LEFT_PAREN){
                    LIST_NODE* tmp_node = cur_node->next;
                    if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
                        //괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
                        tmp = fn_make_list();
                    }
                    else {
                        //괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
                        tmp = call_fn();
                    }
                }
                else{
                    tmp = cur_node->value;
                    cur_node = cur_node->next;
                }
            }
            else if (cur_node->value.type == IDENT) {
                tmp = get_dict_obj(dict, cur_node->value.t_string);
                cur_node = cur_node->next;
            }
            else {
                printf("ERROR : TYPE ERROR FOR LIST\n");
                free_list(tmp_list);
                return return_false();
            }

            if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
                tmp.type = STRING;
                insert_list_node(tmp_list, &tmp);
            }
            else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
                insert_list_node(tmp_list, &tmp);
            }
            else {
                printf("ERROR : TYPE ERROR FOR LIST\n");
                free_list(tmp_list);
                return return_false();
            }
            cnt++;
            
        }
    }


    	printf("list size : %d , %d\n", tmp_list->list_size, cnt);	//디버깅용
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	if (cnt == 0) {	//인자가 0개면 길이가 0인 리스트를 반환한다.
		return head;
	}
	//아니라면 임시로 만든 리스트의 값을 이용해서 리스트를 생성한다.
	LIST_NODE* tmp_node = tmp_list->head;
	while (tmp_node != NULL) {
		if (head.t_list_value == NULL) {
			head.t_list_value = &(tmp_node->value);
		}
		else {
			T_OBJ* tmp = malloc(sizeof(T_OBJ));
			tmp->type = T_LIST;
			tmp->t_int = cnt;
			tmp->t_list_value = &(tmp_node->value);
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		free(tmp_list);
		return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		free_list(tmp_list);
		return return_false();
	}
}

//////////////////////////////////////////////////////////

//LENGTH
//EX) (length 'a 'b) => 2
// APPEND 하고나서 길이를 계산해준다
int fn_length(){
    int tmp_count = 0;
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
        tmp_count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR");
		return;
	}
    int cnt = 0;
    c_LIST* tmp_list = initialize_list();

        while(cur_node->value.type == RIGHT_PAREN){
            T_OBJ tmp;
            if(cur_node->value.type == SQUOTE){
                cur_node = cur_node->next;
                if(cur_node->value.type == LEFT_PAREN){
                    LIST_NODE* tmp_node = cur_node->next;
                    if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
                        //괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
                        tmp = fn_make_list();
                    }
                    else {
                        //괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
                        tmp = call_fn();
                    }
                }
                else{
                    tmp = cur_node->value;
                    cur_node = cur_node->next;
                }
            }
            else if (cur_node->value.type == IDENT) {
                tmp = get_dict_obj(dict, cur_node->value.t_string);
                cur_node = cur_node->next;
            }
            else {
                printf("ERROR : TYPE ERROR FOR LIST\n");
                free_list(tmp_list);
                return;
            }

            if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
                tmp.type = STRING;
                insert_list_node(tmp_list, &tmp);
            }
            else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
                insert_list_node(tmp_list, &tmp);
            }
            else {
                printf("ERROR : TYPE ERROR FOR LIST\n");
                free_list(tmp_list);
                return;
            }
            cnt++;
    	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
        return cnt;
		free(tmp_list);
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		free_list(tmp_list);
		return;
	}
}


//MEMBER
/* EX)
> (SETQ CLUB '(TOM HARRY JOHN DANIEL))
(TOM HARRY JOHN DANIEL)
> (MEMBER 'HARRY CLUB)
(HARRY JOHN DANIEL) ;  CLUB내의 원소 HARRY부터 그 리스트의 끝까지 반환됨

list를 만든 후 검색을 통해 
*/
T_OBJ fn_member(){
    	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR LIST\n");
		return return_false();
	}
    //찾아야하는 노드를 target으로 대입.
    char target = cur_node->value.t_string;
    bool isfind = false;
    while(cur_node->next == NULL){
        cur_node = cur_node->next;
        if(cur_node->value.t_string == target){
            break;
        }
    }
    if(cur_node->next == NULL){
        printf("ERROR");
        return return_false();
    }


	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//리스트를 생성하기 위한 임시 리스트
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
		T_OBJ tmp;
		if (cur_node->value.type == SQUOTE) {
			cur_node = cur_node->next;
			if (cur_node->value.type == LEFT_PAREN) {	//괄호가 올 경우 처리함
				LIST_NODE* tmp_node = cur_node->next;
				if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
					//괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
					tmp = fn_make_list();
				}
				else {
					//괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
					tmp = call_fn();
				}
			}
			else {	//괄호가 없을 경우 현재 노드의 value를 tmp에 할당
				tmp = cur_node->value;
				cur_node = cur_node->next;
			}
		}
		else if (cur_node->value.type == IDENT) {
			tmp = get_dict_obj(dict, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		cnt++;
	}
	printf("list size : %d , %d\n", tmp_list->list_size, cnt);	//디버깅용
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	if (cnt == 0) {	//인자가 0개면 길이가 0인 리스트를 반환한다.
		return head;
	}
	//아니라면 임시로 만든 리스트의 값을 이용해서 리스트를 생성한다.
	LIST_NODE* tmp_node = tmp_list->head;
	while (tmp_node != NULL) {
		if (head.t_list_value == NULL) {
			head.t_list_value = &(tmp_node->value);
		}
		else {
			T_OBJ* tmp = malloc(sizeof(T_OBJ));
			tmp->type = T_LIST;
			tmp->t_int = cnt;
			tmp->t_list_value = &(tmp_node->value);
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		free(tmp_list);
		return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		free_list(tmp_list);
		return return_false();
	}
}
//ASSOC
/* 
EX)
> (ASSOC 'TWO '((ONE 1)(TWO 2)(THREE 3)))
(TWO 2) ; 첫번째 원소가 TWO인 리스트를 찾아내서 리턴함
*/
T_OBJ fn_assoc(){
    	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
        
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR LIST\n");
		return return_false();
	}

    char target = cur_node->value.t_string;
    bool isfind = false;
    while(cur_node->next == NULL){
        cur_node = cur_node->next;
        if(cur_node->value.t_string == target){
            break;
        }
    }
    if(cur_node->next == NULL){
        printf("ERROR");
        return return_false();
    }


	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//리스트를 생성하기 위한 임시 리스트
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
		T_OBJ tmp;
		if (cur_node->value.type == SQUOTE) {
			cur_node = cur_node->next;
			if (cur_node->value.type == LEFT_PAREN) {	//괄호가 올 경우 처리함
				LIST_NODE* tmp_node = cur_node->next;
				if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
					//괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
					tmp = fn_make_list();
				}
				else {
					//괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
					tmp = call_fn();
				}
			}
			else {	//괄호가 없을 경우 현재 노드의 value를 tmp에 할당
				tmp = cur_node->value;
				cur_node = cur_node->next;
			}
		}
		else if (cur_node->value.type == IDENT) {
			tmp = get_dict_obj(dict, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		cnt++;
	}


	printf("list size : %d , %d\n", tmp_list->list_size, cnt);	//디버깅용
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	if (cnt == 0) {	//인자가 0개면 길이가 0인 리스트를 반환한다.
		return head;
	}
	//아니라면 임시로 만든 리스트의 값을 이용해서 리스트를 생성한다.
	LIST_NODE* tmp_node = tmp_list->head;
	while (tmp_node != NULL) {
		if (head.t_list_value == NULL) {
			head.t_list_value = &(tmp_node->value);
		}
		else {
			T_OBJ* tmp = malloc(sizeof(T_OBJ));
			tmp->type = T_LIST;
			tmp->t_int = cnt;
			tmp->t_list_value = &(tmp_node->value);
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		free(tmp_list);
        return head;

	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		free_list(tmp_list);
		return return_false();
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
T_OBJ fn_remove(){
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR LIST\n");
		return return_false();
	}
    char target = cur_node->value.t_string;
    cur_node = cur_node->next;

	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//리스트를 생성하기 위한 임시 리스트
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
		T_OBJ tmp;
		if (cur_node->value.type == SQUOTE) {
			cur_node = cur_node->next;
			if (cur_node->value.type == LEFT_PAREN) {	//괄호가 올 경우 처리함
				LIST_NODE* tmp_node = cur_node->next;
				if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
					//괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
					tmp = fn_make_list();
				}
				else {
					//괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
					tmp = call_fn();
				}
			}
			else {	//괄호가 없을 경우 현재 노드의 value를 tmp에 할당
				tmp = cur_node->value;
				cur_node = cur_node->next;
			}
		}
		else if (cur_node->value.type == IDENT) {
			tmp = get_dict_obj(dict, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		cnt++;
	}
    // list_node를 

	printf("list size : %d , %d\n", tmp_list->list_size, cnt);	//디버깅용
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	if (cnt == 0) {	//인자가 0개면 길이가 0인 리스트를 반환한다.
		return head;
	}
	//아니라면 임시로 만든 리스트의 값을 이용해서 리스트를 생성한다.
	LIST_NODE* tmp_node = tmp_list->head;
	while (tmp_node != NULL) {
		if (head.t_list_value == NULL) {
			head.t_list_value = &(tmp_node->value);
		}
		else {
			T_OBJ* tmp = malloc(sizeof(T_OBJ));
			tmp->type = T_LIST;
			tmp->t_int = cnt;
			tmp->t_list_value = &(tmp_node->value);
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		free(tmp_list);
        return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		free_list(tmp_list);
		return return_false();
	}
}
//SUBST
/*
EX)
> (SUBST 'GOOD 'BAD '(I AM BAD))
  (I AM GOOD) ;  BAD가 GOOD으로 대치되었다.
*/
T_OBJ fn_subst(){
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR LIST\n");
		return return_false();
	}

    if(cur_node->value.type == SQUOTE){
        cur_node = cur_node->next;
    }
    else{
        printf("ERROR");
        return return_false();
    }

    //대체할 단어
    char to_subst = cur_node->value.t_string;
    cur_node = cur_node->next;

    if(cur_node->value.type == SQUOTE){
        cur_node = cur_node->next;
    }
    else{
        printf("ERROR");
        return return_false();
    } 

    //대체될 단어
    char for_subst = cur_node->value.t_string;
    cur_node = cur_node->next;

	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//리스트를 생성하기 위한 임시 리스트
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
		T_OBJ tmp;
		if (cur_node->value.type == SQUOTE) {
			cur_node = cur_node->next;
			if (cur_node->value.type == LEFT_PAREN) {	//괄호가 올 경우 처리함
				LIST_NODE* tmp_node = cur_node->next;
				if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
					//괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
					tmp = fn_make_list();
				}
				else {
					//괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
					tmp = call_fn();
				}
			}
			else {	//괄호가 없을 경우 현재 노드의 value를 tmp에 할당
				tmp = cur_node->value;
				cur_node = cur_node->next;
			}
		}
		else if (cur_node->value.type == IDENT) {
			tmp = get_dict_obj(dict, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		cnt++;
	}

    

	printf("list size : %d , %d\n", tmp_list->list_size, cnt);	//디버깅용
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	if (cnt == 0) {	//인자가 0개면 길이가 0인 리스트를 반환한다.
		return head;
	}
	//아니라면 임시로 만든 리스트의 값을 이용해서 리스트를 생성한다.

	LIST_NODE* tmp_node = tmp_list->head;
	while (tmp_node != NULL) {
		if (head.t_list_value == NULL) {
			head.t_list_value = &(tmp_node->value);
		}
		else {
            if(strcmp(tmp_node->value.t_list_value,for_subst)){
                tmp_node->value.t_list_value = to_subst;
            }
			T_OBJ* tmp = malloc(sizeof(T_OBJ));
			tmp->type = T_LIST;
			tmp->t_int = cnt;
			tmp->t_list_value = &(tmp_node->value);
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		free(tmp_list);
        return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		free_list(tmp_list);
		return return_false();
	}
}