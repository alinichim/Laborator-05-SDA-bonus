#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE	256
#define MAX_STACK_SIZE 100

typedef struct queue_t queue_t;
struct queue_t
{
	/* Dimensiunea maxima a cozii */
	unsigned int max_size;
	/* Dimensiunea cozii */
	unsigned int size;
	/* Dimensiunea in octeti a tipului de date stocat in coada */
	unsigned int data_size;
	/* Indexul de la care se vor efectua operatiile de front si dequeue */
	unsigned int read_idx;
	/* Indexul de la care se vor efectua operatiile de enqueue */
	unsigned int write_idx;
	/* Bufferul ce stocheaza elementele cozii */
	void **buff;
};

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	/* TODO */
	queue_t *new_queue = (queue_t *)malloc(sizeof(queue_t));
	if (!new_queue)
		return NULL;

	new_queue->buff = malloc(max_size * sizeof(void *));
	if (!new_queue->buff) {
		free(new_queue);
		return NULL;
	}
	void **buff = new_queue->buff;
	for (unsigned int i = 0; i < max_size; ++i) {
		buff[i] = malloc(data_size);
		if (!buff[i]) {
			for (int j = i - 1; j >= 0; --j)
				free(buff[j]);
			free(buff);
			free(new_queue);
			return NULL;
		}
	}

	new_queue->data_size = data_size;
	new_queue->size = 0;
	new_queue->max_size = max_size;
	new_queue->read_idx = 0;
	new_queue->write_idx = 0;

	return new_queue;
}

/*
 * Functia intoarce numarul de elemente din coada al carei pointer este trimis
 * ca parametru.
 */
unsigned int
q_get_size(queue_t *q)
{
	/* TODO */
	if (!q)
		return 0;

	return q->size;
}

/*
 * Functia intoarce 1 daca coada este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q)
{
	/* TODO */
	if (!q)
		return 0;

	return (q->size) ? 0 : 1;
}

/* 
 * Functia intoarce primul element din coada, fara sa il elimine.
 */
void *
q_front(queue_t *q)
{
	/* TODO */
	if (!q)
		return NULL;

	return (q->buff)[q->read_idx];
}

/*
 * Functia scoate un element din coada. Se va intoarce 1 daca operatia s-a
 * efectuat cu succes (exista cel putin un element pentru a fi eliminat) si
 * 0 in caz contrar.
 */
int
q_dequeue(queue_t *q)
{
	/* TODO */
	if (!q || !q->size)
		return 0;

	q->read_idx = (q->read_idx + 1) % q->max_size;
	q->size--;

	return 1;
}

/* 
 * Functia introduce un nou element in coada. Se va intoarce 1 daca
 * operatia s-a efectuat cu succes (nu s-a atins dimensiunea maxima) 
 * si 0 in caz contrar.
 */
int
q_enqueue(queue_t *q, void *new_data)
{
	/* TODO */
	if (!q || (q->size == q->max_size))
		return 0;

	void **buff = q->buff;
	memcpy(buff[q->write_idx], new_data, q->data_size);
	q->write_idx = (q->write_idx + 1) % q->max_size;
	q->size++;

	return 1;
}

/*
 * Functia elimina toate elementele din coada primita ca parametru.
 */
void
q_clear(queue_t *q)
{
	/* TODO */
	if (!q)
		return;

	while (q_dequeue(q)) {}
}

/*
 * Functia elibereaza toata memoria ocupata de coada.
 */
void
q_free(queue_t *q)
{
	/* TODO */
	if (!q)
		return;

	q_clear(q);
	for (unsigned int i = 0; i < q->max_size; ++i)
		free(q->buff[i]);
	free(q->buff);
	free(q);
}

typedef struct stack_t stack_t;
struct stack_t
{
	queue_t *q1, *q2;
	unsigned int max_size, size;
};

stack_t *
st_create(unsigned int data_size, unsigned int max_size)
{
	/* TODO */
	queue_t *q1 = q_create(data_size, max_size);
	if (!q1)
		return NULL;
	queue_t *q2 = q_create(data_size, max_size);
	if (!q2) {
		q_free(q1);
		return NULL;
	}
	
	stack_t *new_stack = malloc(sizeof(stack_t));
	if (!new_stack) {
		q_free(q1);
		q_free(q2);
		return NULL;
	}

	new_stack->q1 = q1;
	new_stack->q2 = q2;
	new_stack->max_size = max_size;
	new_stack->size = 0;

    return new_stack;
}

/*
 * Functia intoarce numarul de elemente din stiva al carei pointer este trimis
 * ca parametru.
 */
unsigned int
st_get_size(stack_t *st)
{
	/* TODO */
	if (!st)
		return 0;

	return st->size;
}

/*
 * Functia intoarce 1 daca stiva al carei pointer este trimis
 * ca parametru este goala si 0 in caz contrar.
 */
unsigned int
st_is_empty(stack_t *st)
{
	/* TODO */
	if (!st)
		return 0;

	return (st->size) ? 0 : 1;
}

/*
 * Functia intoarce elementul din vârful stivei al carei pointer este trimis
 * ca parametru fără a efectua nicio modificare asupra acesteia.
 */
void *
st_peek(stack_t *st)
{
    /* TODO */
	if (!st || !st->size)
		return NULL;
	
	return q_front(st->q1);
}

/*
 * Functia elimina elementul din vârful stivei al carei pointer este trimis
 * ca parametru. Atentie! Este responsabilitatea acestei functii de a elibera
 * memoria pentru elementul eliminat.
 */
void
st_pop(stack_t *st)
{
	/* TODO */
	if (!st)
		return;
	q_dequeue(st->q1);
	st->size--;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se adauga un nou element in
 * vârful stivei al carei pointer este trimis ca parametru.
 */
void
st_push(stack_t *st, void *new_data)
{
	/* TODO */
	if (!st || st->size == st->max_size)
		return;

	while (!q_is_empty(st->q1)) {
		q_enqueue(st->q2, q_front(st->q1));
		q_dequeue(st->q1);
	}

	q_enqueue(st->q1, new_data);

	while (!q_is_empty(st->q2)) {
		q_enqueue(st->q1, q_front(st->q2));
		q_dequeue(st->q2);
	}

	st->size++;
}

/*
 * Functia elimina toate elementele stivei al carei pointer este trimis
 * ca parametru.
 */
void
st_clear(stack_t *st)
{
	/* TODO */
	if (!st)
		return;
	
	q_clear(st->q1);
	st->size = 0;
}

/*
 * Functia elibereaza memoria folosita de toate elementele, iar la
 * sfarsit elibereaza memoria folosita de structura stivei.
 */
void
st_free(stack_t *st)
{
    /* TODO */
	if (!st)
		return;
	q_free(st->q1);
	q_free(st->q2);
	free(st);
}

int main()
{
    
    stack_t *stack;
    int is_int = 0;
    int is_string = 0;

    while (1) {
        char command[MAX_STRING_SIZE], added_elem[MAX_STRING_SIZE];
        long nr;

        scanf("%s", command);

        if (strncmp(command, "create_stack_str", 16) == 0) {
            stack = st_create(MAX_STRING_SIZE, MAX_STACK_SIZE);
            is_string = 1;
        }

        if (strncmp(command, "create_stack_int", 16) == 0) {
            stack = st_create(sizeof(int), MAX_STACK_SIZE);
            is_int = 1;
        }

        if (strncmp(command, "push", 4) == 0) {
          
            if(is_int) {
                scanf("%ld", &nr);
                st_push(stack, &nr);
            } else if(is_string) {
                scanf("%s", added_elem);
                st_push(stack, added_elem);
            } else {
                printf("Create a stack first!\n");
                exit(0);
            }
        }

        if (strncmp(command, "pop", 3) == 0) {
            if(!is_int && !is_string) {
                printf("Create a stack first!\n");
                exit(0);
            }

            st_pop(stack);
        }

        if (strncmp(command, "peek", 4) == 0) {
            if(!is_int && !is_string) {
                printf("Create a stack first!\n");
                exit(0);
            }

            if (is_int == 1) {
                int val = *(int *) st_peek(stack);
                printf("%d\n", val);
            }

            if (is_string == 1) {
                char *val_str = (char *) st_peek(stack);
                printf("%s\n", val_str);
            }
        }

        if (strncmp(command, "empty", 5) == 0) {
            if(!is_int && !is_string) {
                printf("Create a stack first!\n");
                exit(0);
            }

            if (st_is_empty(stack)) {
                printf("empty\n");
            } else {
                printf("not empty\n");
            }
        }

        if (strncmp(command, "free", 4) == 0) {
            if(!is_int && !is_string) {
                printf("Create a stack first!\n");
                exit(0);
            }
            st_clear(stack);
            st_free(stack);
            break;
        }
    }

    return 0;
}
