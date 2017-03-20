//����һ���Ʒ�ʵ�ֵ�coroutine c�汾��ע����һ�¡���by kevinlin)
//==============================================

#ifndef C_COROUTINE_H
#define C_COROUTINE_H

#define COROUTINE_DEAD 0
#define COROUTINE_READY 1
#define COROUTINE_RUNNING 2
#define COROUTINE_SUSPEND 3

struct schedule;

typedef void (*coroutine_func)(struct schedule *, void *ud);

struct schedule * coroutine_open(void);
void coroutine_close(struct schedule *);

int coroutine_new(struct schedule *, coroutine_func, void *ud);
void coroutine_resume(struct schedule *, int id);
int coroutine_status(struct schedule *, int id);
int coroutine_running(struct schedule *);
void coroutine_yield(struct schedule *);

#endif

//==============================================
#include "coroutine.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _ON_MAC_
#include <sys/ucontext.h>
#else
#include <ucontext.h>
#endif

//FIXME ��macosx���ܱ��������������ʱ��co[1]->status��֪��ʱ���޸�,����191��assert����ʧ��

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#define STACK_SIZE (1024*1024)
#define DEFAULT_COROUTINE 16

struct coroutine;

struct schedule {
	char stack[STACK_SIZE]; //1M��ջ�ռ�, �������浱ǰ�������е�coroutine��stack��
                            //��������ջ�����棬������swapcontext������ȥ
	ucontext_t main; //main��������, ��coroutine�л���������Ҫ��
	int nco; //�Ѿ�ʹ�õ�co
	int cap; //�ܹ�co ������,��������malloc(cap*sizeof(coroutine*)))��С�ڴ�
	int running; // schedule״̬, ����ִ���ĸ�coroutine.. = id
	struct coroutine **co; //�������е�coroutine��Ϣ
};

struct coroutine {
	coroutine_func func; //Э�������ú���  
                    //typedef void (*coroutine_func)(struct schedule *, void *ud);
	void *ud;   //user data
	ucontext_t ctx; //��Э��������
	struct schedule * sch; //��coroutine������schedule
	ptrdiff_t cap; //Ϊcoroutine˽��ջ����Ŀռ� cap>=size
	ptrdiff_t size; //coroutine˽��ջ��ʵ�ʴ�С
	int status; //��coroutine��״̬�����������У����ǹ���
                //#define COROUTINE_DEAD 0
                //#define COROUTINE_READY 1
                //#define COROUTINE_RUNNING 2
                //#define COROUTINE_SUSPEND 3


	char *stack;
};

//�����ͳ�ʼ��һ��coroutine,���ڲ�ʹ��
struct coroutine * 
_co_new(struct schedule *S , coroutine_func func, void *ud) {
	struct coroutine * co = malloc(sizeof(*co));
    printf("%s:%s co:%p\n", __FILE__, __FUNCTION__, co);
	co->func = func;
	co->ud = ud;
	co->sch = S;
	co->cap = 0;
	co->size = 0;
	co->status = COROUTINE_READY; //���������READY״̬����resume()
	co->stack = NULL; //����Ҫ�л� ��ȥ��Ҫ�����coroutine��ջʱ��ʹ�ã���̬�����ڴ�
	return co;
}

//ɾ��һ��coroutine
void
_co_delete(struct coroutine *co) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
	free(co->stack); //�򴴽�ʱ�Ѿ��ÿգ��ʿ�ֱ��free
    // co->ud ����ɾ��������������紫�룬����������.�����㶼��֪������������ô����ģ���ʾ���е�args
	free(co);
}

//����һ��schedule, �о�������schedule_new��������ʣ����벻��Ҳ�ɣ���������̫��ĸ����������
struct schedule * 
coroutine_open(void) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
	struct schedule *S = malloc(sizeof(*S));
	S->nco = 0;
	S->cap = DEFAULT_COROUTINE;
	S->running = -1; //-1��Ϊ�����κ�coroutine������
	S->co = malloc(sizeof(struct coroutine *) * S->cap); //����ռ��ָ��
	memset(S->co, 0, sizeof(struct coroutine *) * S->cap);
	return S;
}

//�ر�schedule���ͷ����е�coroutine
void 
coroutine_close(struct schedule *S) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
	int i;
	for (i=0;i<S->cap;i++) {
		struct coroutine * co = S->co[i];
		if (co) {
			_co_delete(co);
		}
	}
	free(S->co);
	S->co = NULL;
	free(S);
}

//����һ��coroutine,��������id
int 
coroutine_new(struct schedule *S, coroutine_func func, void *ud) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
    //�����ڲ���������coroutine
	struct coroutine *co = _co_new(S, func , ud);

    //��������coroutine���뵽schedule�й���
	if (S->nco >= S->cap) {
		int id = S->cap;
		S->co = realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));
		memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);
		S->co[S->cap] = co;
		S->cap *= 2;
		++S->nco;
        printf("%s:%s:%d gen coroutine id:%d\n", __FILE__, __FUNCTION__,__LINE__, id);
		return id;
	} else {
		int i;
        //�����ǰ�����ҿ��е�λ�ò�ʹ��,������id��ܿ챻���õ�
		for (i=0;i<S->cap;i++) {
			int id = (i+S->nco) % S->cap;
			if (S->co[id] == NULL) {
				S->co[id] = co;
				++S->nco;
                printf("%s:%s:%d gen coroutine id:%d co:%p\n", __FILE__, __FUNCTION__,__LINE__, id, co);
				return id;
			}
		}
	}
	assert(0);
	return -1;
}

//���������coroutine��callback,��callback()���غ󣬴�coroutine������
static void
mainfunc(uint32_t low32, uint32_t hi32) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
    //Ϊɶ���˸������Ķ�������ptr -- schedule*
	uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);
	struct schedule *S = (struct schedule *)ptr;
	int id = S->running;//����-1��, Ŀǰ�ǵ��ô˺���ǰ�������˴˴�croutine id
	struct coroutine *C = S->co[id];
	C->func(S,C->ud); //����coroutine��callback(?)

    //��func()���ý����󣬴�co������������
	_co_delete(C);
	S->co[id] = NULL;
	--S->nco;
	S->running = -1; //-1����ǰû��coroutine��ִ��, �����״̬�£�ִ����(��)�������߳�(main)
}

//����coroutine, resume�ǽл��ѵ���˼�����»ص�����
void 
coroutine_resume(struct schedule * S, int id) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
    printf("coroutine:[%d]\n", id);
	assert(S->running == -1);
	assert(id >=0 && id < S->cap);
	struct coroutine *C = S->co[id];
    printf("coroutine:[%d] co:%p\n", id, C);
	if (C == NULL)
		return;
	int status = C->status;
	switch(status) {
	case COROUTINE_READY:
        printf("coroutine:[%d] COROUTINE_READY\n", id);
		getcontext(&C->ctx);//������� ������ʹctx�ڵĶ����͵�ǰ�������Ĺ����ϣ�ss,sp??)
		C->ctx.uc_stack.ss_sp = S->stack; //Ҫ�㣺 ����context,  stack��Ҫ�Լ��������,������S->stack����ȥ���൱�������˶�ջ��ָ�룬���ʹ�����ĵ��ù����е�ջ��Ϣ�ŵ���������Ҫ�����λ�� . ��S->stack�൱�ڵ��º������õ�ջ��
		C->ctx.uc_stack.ss_size = STACK_SIZE; //�ṩջ��С�����������
		C->ctx.uc_link = &S->main; //����ǵ����context����ʱ, resume��Ŀ��
		S->running = id;//��������croutine id, ��˿�������Ƿ��̰߳�ȫ?
		C->status = COROUTINE_RUNNING;
		uintptr_t ptr = (uintptr_t)S;
        //����Ϊ��Ҫ��uintptr��Ϊ����uint32_t���ݣ� --> man makecontext
		makecontext(&C->ctx, (void (*)(void)) mainfunc, 2, (uint32_t)ptr, (uint32_t)(ptr>>32));
        //����ǰ�������ı��浽S->main�У����ҿ���Ȩ����C->ctx. ��ȥִ��ctx�����õĺ����ȵ�
		swapcontext(&S->main, &C->ctx);
        printf("coroutine:[%d] COROUTINE_READY swapcontext return\n", id);
		break;
	case COROUTINE_SUSPEND://֮ǰ��yield���ˣ����Ѿ������Լ���stack����
        printf("coroutine:[%d] COROUTINE_SUSPEND\n", id);
        //��Ҫ���ѵ�croutine��stack������S->stackĩβȥ�����Ǻ��⣿Ϊʲô��ĩβ
        //�����棺��Ϊstack�Ǵ������ϣ��Ӹߵ�ַ���ͣ�)�ŵģ�������ջһ��int,�������S->stack+STACK_SIZEλ�ã�Ȼ��ջ��������sizeof(int), �������ǿ���ʱ��Ҫ��coroutine��˽��ջ������S->stack + STACK_SIZE - C->size��λ��
		memcpy(S->stack + STACK_SIZE - C->size, C->stack, C->size);
		S->running = id;
		C->status = COROUTINE_RUNNING;
		swapcontext(&S->main, &C->ctx);
        printf("coroutine:[%d] COROUTINE_SUSPEND swapcontext\n", id);
		break;
	default:
        printf("ERROR[coroutine status]:%d", status);
		assert(0);
	}
}

//����croutine��stack����
static void
_save_stack(struct coroutine *C, char *top) {
    printf("%s:%s top:%p\n", __FILE__, __FUNCTION__, top);
    //��ջ�ϴ���һ���ֲ�����, ������ջ���ˣ����µ���һ�ζ��Ǻ������õĶ�ջ
	char dummy = 0;
	assert(top - &dummy <= STACK_SIZE);
	if (C->cap < top - &dummy) {
		free(C->stack);
		C->cap = top-&dummy; //�����˴�ʱcoroutine��˽��ջ��С
		C->stack = malloc(C->cap); //C->stack��������coroutine�����ջ����
	}
	C->size = top - &dummy; //�������Ҫ�����ջ����
    printf("%s:%s C->stack:%p dummy:%p C->size:%ld\n", 
            __FILE__, __FUNCTION__, C->stack, &dummy, C->size);
	memcpy(C->stack, &dummy, C->size);
}


void
coroutine_yield(struct schedule * S) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
	int id = S->running;
    //���assert(id>=0) ����ʾ�˴˺���ֻ����coroutine�е��á�
	assert(id >= 0);
    //ȡ���������е�croutine
	struct coroutine * C = S->co[id];
    //���assert()�Ǻ��⣿ �����S->stackΪ����ջ�ϵĵ�ַ�����ǴӶ��Ϸ������
    //��������ʽ����Ϊswapcontext()ʱ��ָ����ss_sp��S->stack,������contextִ��ʱ������ջ�Ƿ���S->stack�ϵġ�
    //����C, S->stack ���ǵĵ�ַ�����������
    printf("&C:%p S->stack:%p\n", &C, S->stack);
	assert((char *)&C > S->stack);
    //�������coroutine��stack
	_save_stack(C,S->stack + STACK_SIZE);
    //ʹ����SUSPEND״̬
	C->status = COROUTINE_SUSPEND; 
	S->running = -1;
    //�л���S->main, ���ǵ�����coroutine_resume()�й���������
	swapcontext(&C->ctx , &S->main);
}

//��ȡĳ��coroutine��״̬
int 
coroutine_status(struct schedule * S, int id) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
	assert(id>=0 && id < S->cap);
	if (S->co[id] == NULL) {
		return COROUTINE_DEAD;
	}
	return S->co[id]->status;
}

//����������е�croutine id
int 
coroutine_running(struct schedule * S) {
    printf("%s:%s\n", __FILE__, __FUNCTION__);
	return S->running;
}
