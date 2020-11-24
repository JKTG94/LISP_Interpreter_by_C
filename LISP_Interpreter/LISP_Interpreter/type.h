#ifndef _TYPE_
#define _TYPE_

/* Token codes */

/* Ÿ���߷��� ���� ���� */
#define T_NIL 1001
#define T_CODE 1002
#define T_LIST 1003
#define T_STRING 1004
#define T_FLOAT 1005
#define T_INT 1006
#define T_SYMBOL 1007


/* 1. ��Ģ���� */
#define ADD_OP 102
#define SUB_OP 103
#define MULT_OP 104
#define DIV_OP 105

/* 2. �⺻�Լ�*/
#define SETQ 200
#define LIST 201
#define CAR 202
#define CDR 203
#define NTH 204
#define CONS 205
#define REVERSE 206
#define APPEND 207
#define LENGTH 208
#define MEMBER 209
#define ASSOC 210
#define REMOVE 211
#define SUBST 212

/* 3. Predicate �Լ� */
#define ATOM 301
#define L_NULL 302	//lisp�� NULL�� ���� ó�� NULL�� �̹� define�Ǿ�����
#define NUMBERP 303
#define ZEROP 304
#define MINUSP 305
#define EQUAL 306
#define RIGHT_INEQUAL_SIGN 307
#define RIGHT_INEQUAL_SAME_SIGN 308
#define LEFT_INEQUAL_SIGN 309
#define LEFT_INEQUAL_SAME_SIGN 310
#define STRINGP 311

/* 4. ���ǹ� */
#define IF 401
#define COND 402

/* 5.  */
#define INT 501
#define FLOAT 502
#define IDENT 503
#define SQUOTE 504
#define DQUOTE 505
#define LEFT_PAREN 506
#define RIGHT_PAREN 507
#define SEMI_COLON 508
#define SHARP 509

typedef struct T_OBJ* P_OBJ;
typedef struct {
	int type;
	int lexed_len;
	char *t_string;
	int t_int;
	float t_float;
	P_OBJ t_list;
}T_OBJ;

#endif