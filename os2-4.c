#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_reverse(pos, head, member)			\
	for (pos = list_entry((head)->prev, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.prev, typeof(*pos), member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

/**
 * list_for_each_entry_safe_continue -	iterate over list of given type
 *			continuing after existing point safe against removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe_continue(pos, n, head, member) 		\
	for (pos = list_entry(pos->member.next, typeof(*pos), member), 		\
		n = list_entry(pos->member.next, typeof(*pos), member);		\
	     &pos->member != (head);						\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

/**
 * list_for_each_entry_safe_reverse - iterate backwards over list of given type safe against
 *				      removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = list_entry((head)->prev, typeof(*pos), member),	\
		n = list_entry(pos->member.prev, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.prev, typeof(*n), member))

/**
 * list_prepare_entry - prepare a pos entry for use as a start point in
 *			list_for_each_entry_continue
 * @pos:	the type * to use as a start point
 * @head:	the head of the list
 * @member:	the name of the list_struct within the struct.
 */

static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next) {
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head) {
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head) {
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next) {
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry) {
	__list_del(entry->prev, entry->next);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}

static inline void list_del_init(struct list_head *entry) {
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

static inline void list_move(struct list_head *list, struct list_head *head) {
        __list_del(list->prev, list->next);
        list_add(list, head);
}

static inline void list_move_tail(struct list_head *list,
				  struct list_head *head) {
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

static inline int list_empty(const struct list_head *head) {
	return head->next == head;
}

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_for_each(pos, head) \
  for (pos = (head)->next; pos != (head);	\
       pos = pos->next)

#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; prefetch(pos->prev), pos != (head); \
        	pos = pos->prev)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_reverse(pos, head, member)			\
	for (pos = list_entry((head)->prev, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.prev, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

#define list_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = list_entry((head)->prev, typeof(*pos), member),	\
		n = list_entry(pos->member.prev, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.prev, typeof(*n), member))


#if 0    //DEBUG
#define debug(fmt, args...) fprintf(stderr, fmt, ##args)
#else
#define debug(fmt, args...)
#endif

 
typedef struct {    //codetuple 구조체 생성
    unsigned char op; //작업의 종류
    unsigned char length;   //수행시간
	//struct list_head list;
} code;

typedef struct {    // process 구조체 생성
    int pid;    //프로세스 ID
    int arrival_time;   //프로세스가 실행을 시작한시간
    int code_bytes; //프로그램 코드의 길이(바이트 단위,짝수)
	int pc;	//프로그램카운터
	int wait_time;	//process의 wait time을 나타냄
    int response_time;	//process의 response time을 나타냄
    int time_slice;	//process에 부여된 time slice를 나타내는 멤버변수
	code *operations;	//code 구조체를 가르키기 위한 변수
	struct list_head temp, job, ready, wait, real_time, idle;	//list_head 구조체 선언 
} process;

	process *cur, *next;	//연결리스트를 돌며 특정 list를 지정하기 위한 변수
	process *cpu_ptr;	//ready_q에서 작업중인 process를 지정하기 위한 변수
	process *temp_ptr;	//wait_q에서 진행중이거나 끝난 io작업을 지정하기 위한 변수 
	process *check_TS_ptr;	//Round Robin 스케쥴링 기법에서 ROUND END를 확인하기 위한 변수
	process *idle;	//idle상태를 나타내기 위한 변수
	process *max_len;	//SRJF스케쥴링 기법에서 가장 짧은 operations length를 찾기위한 변수

	LIST_HEAD(job_q);	//list_head job_q 선언 및 초기화
	LIST_HEAD(ready_q);	//nomal class가 관리 될 ready_q 선언 및 초기화
	LIST_HEAD(wait_q);	//list_head wait_q 선언 및 초기화
	LIST_HEAD(real_time_q);	//우선순위가 가장 높은 realtime process가 관리될 real_time_q
	LIST_HEAD(idle_q);//idle process가 관리되어질 idle_q
	LIST_HEAD(temp_q);

	int prev_pid = 0;	//전에 종료되었던 process id를 저장하기 위한 변수
	int Clock = 0;	//총 Clocks를 나타내는 변수
	int idle_Clock = 0;	//총 IDLE를 나타내는 변수
	int is_switched = 0;	//context switching이 일어났는지 카운트 하는 변수
	int max_arrival = 0;	//프로세스의 arrival_time중 최댓값을 저장하는 변수
    int total_wait = 0;	//총 wait time을 계산하기 위한 변수
    int total_response = 0;	//총 response time을 계산하기 위한 변수
	int process_num = 0;	//process의 수를 체크하기위한 변수
	int preemption = 0;	//preemption이 일어났는지를 확인하는 변수.


void make_idle(){	//idle process를 초기화
	idle = malloc(sizeof(process));	//idle프로세스에 동적 메모리 할당후 값 대입
	idle->operations = malloc(sizeof(code));	//idle operations의 크기만큼 동적 메모리 할당
	idle->pid = 100;	//idle 정보 대입
	idle->arrival_time = 0;
	idle->code_bytes = 2;
	idle->pc = 0;
	idle->operations[0].op = 0xff;
	idle->operations[0].length = 0;
	INIT_LIST_HEAD(&idle->job);	//job_q 초기화
	list_add_tail(&idle->job, &job_q);	//job_q에 idle 값을 넣어줌
}
void make_max_len(){//가장 짧은 길이의 프로세스를 찾기위해 비교를 위한 프로세스를 초괴화하기 위한 함수
	max_len = malloc(sizeof(process));	//max_len프로세스에 동적 메모리 할당후 값 대입
	max_len->operations = malloc(sizeof(code));	//max_len operations의 크기만큼 동적 메모리 할당
	max_len->pid = 0xff;	//max_len 정보 대입
	max_len->arrival_time = 0;
	max_len->code_bytes = 2;
	max_len->pc = 0;
	max_len->operations[0].op = 0;
	max_len->operations[0].length = 0xff;
}
void IO_task(){	//binary형태의 파일을 읽어들여 job_q에 저장
	while(fread(cur = malloc(sizeof(process)), sizeof(int), 3, stdin) == 3){    //pocess구조체의 크기를 동적할당 받아 int형 변수 3개에 값을 차래대로 대입 
		if(max_arrival < cur->arrival_time){
			max_arrival = cur->arrival_time;
		}
		process_num++;
		cur->pc = 0;
		cur->response_time = -1;	//resepose_time 초기화 - wait_time과 구별하기위해 -1로 함
		INIT_LIST_HEAD(&cur->job);
		list_add_tail(&cur->job, &job_q);
		cur->operations = malloc(sizeof(code) * (cur->code_bytes/2));	//code_bytes에 저장된 정보를 operaations크기만큼 동적 할당받아 저장.
		for(int i = 0; i < (cur->code_bytes/2); i++){ 
			fread(&cur->operations[i], sizeof(code), 1, stdin);	
        }
    }
}
void make_temp_q(){
	list_for_each_entry_safe(cur, next, &job_q, job){
		temp_ptr = malloc(sizeof(process));
		temp_ptr->pid = cur->pid;
		temp_ptr->arrival_time = cur->arrival_time;
		temp_ptr->code_bytes = cur->code_bytes;
		temp_ptr->pc = cur->pc;
		temp_ptr->response_time = cur->response_time;
		temp_ptr->time_slice = cur->time_slice;
		
		INIT_LIST_HEAD(&temp_ptr->temp);
		list_add_tail(&temp_ptr->temp, &temp_q);

		temp_ptr->operations = malloc(sizeof(code) * (cur->code_bytes/2));
		for(int i = 0; i < (cur->code_bytes/2); i++){ 
			temp_ptr->operations[i].op = cur->operations[i].op;
			temp_ptr->operations[i].length = cur->operations[i].length;
        }
	}
}
void Reload_job_q(){
	Clock = 0;
	idle_Clock = 0;
	total_wait = 0;
	total_response = 0;

	list_for_each_entry_safe(cur, next, &job_q, job){
		if(cur->pid != idle->pid){
			cur->pc = 0;
			cur->wait_time = 0;
			cur->response_time = -1;
		}
		list_for_each_entry(temp_ptr,&temp_q, temp){
			if(cur->pid == temp_ptr->pid){
				for(int i = 0; i < (cur->code_bytes/2); i++){ 
					cur->operations[i].length = temp_ptr->operations[i].length;
				}
			}
		}
	}
}
void IO_task_print(){	//job_q에 저장된 파일을 출력
		list_for_each_entry_safe(cur, next, &job_q, job){
		printf("PID: %03d\tARRIVAL: %03d\tCODESIZE: %03d\n",cur->pid, cur->arrival_time, cur->code_bytes);
		for(int i = 0; i < cur->code_bytes/2; i++){
			printf("%d %d\n",cur->operations[i].op,cur->operations[i].length);
		}
	}
}
void up_switch_count(){	//context switching이 일어났을 때 시간을 처리
	is_switched++;
	Clock +=5;
	idle_Clock +=5;
}
void ready_process_load(int Clock1, int Clock2){	//arrival_time이 clock과 일치하면 job_q에서 ready_q로 값을 복사	//arrival_time이 clock과 일치하면 job_q에서 ready_q로 값을 복사
	list_for_each_entry_safe(cur, next, &job_q, job){	//ready_q에 job_q를 순서대로 삽입.
		if(Clock1 == cur->arrival_time){
			if(cur->pid < 80){	//nomal process load
				cur->time_slice = 50;	//nomal process의 load시 50 time slice를 부여받음.
				INIT_LIST_HEAD(&cur->ready);
				list_add_tail(&cur->ready, &ready_q);
			} else if(cur->pid <= 99){	//realtime process load
				if(!(80 <= cpu_ptr->pid && cpu_ptr->pid <= 99))	//nomal process 실행중 realtime process load
					preemption = 1;	//preemption 된 상황
					
				cur->time_slice = cur->operations[cur->pc].length;	//real_time processs의 load시 프로세스의 operation length의 time slice를 부여받음.
				INIT_LIST_HEAD(&cur->real_time);
				list_add_tail(&cur->real_time, &real_time_q);
			} else if(cur == idle){ //idle process load
				INIT_LIST_HEAD(&cur->idle);
				list_add_tail(&cur->idle, &idle_q);
			}
			
			//printf("%04d CPU: Loaded PID: %03d\tArrival: %03d\tCodesize: %03d\tPC: %03d\n", Clock2, cur->pid, cur->arrival_time, cur->code_bytes, cur->pc);
		}				 
	}
}
void up_wait_time(){	//load되었지만 cpu를 사용하지 못하는 process들의 wait time을 늘려주는 함수
	list_for_each_entry_safe(cur, next, &real_time_q, real_time){
            cur->wait_time++;
	}
	list_for_each_entry_safe(cur, next, &ready_q, ready){
            cur->wait_time++;
	}
}

void search_FIFO(){	//우선순위에 의해 먼저 선택되고, 같은 우선순위내에서는 FIFO방식 스케쥴링
	list_for_each_entry_safe(cur, next, &real_time_q, real_time){	//search realtime process
		if(80 <= cur->pid && cur->pid <= 99){
			if(Clock != 0)	//프로세스가 전환되면 스위칭시간동안 wait_time이 올라가는것을 구현.
				cur->wait_time += 5;
			if(cur->response_time == -1){	//처음 프로세스가 실행될 때 미리 -1로 초기화해논 response값에 wiat_time의 값을 대입.
				cur->response_time = cur->wait_time;
			}
			prev_pid = cpu_ptr->pid;
			cpu_ptr = cur;	//cpu_ptr에 idle이 아닌 새로운 작업을 대입   
			//printf("%04d PID : %03d START\n", Clock, cur->pid);

			list_del(&cur->real_time);
			if(Clock != 0)
				up_switch_count();	//프로세스 전환으로 인한 process switching 발생
			return;	//idle이 아닌작업을 찾으면 종료
		}
	}
	list_for_each_entry_safe(cur, next, &ready_q, ready){//	search nomal process
		if(cur->pid < 80){
			if(Clock != 0)//프로세스가 전환되면 스위칭시간동안 wait_time이 올라가는것을 구현.
				cur->wait_time += 5;
			if(cur->response_time == -1){
				cur->response_time = cur->wait_time;
			}
			prev_pid = cpu_ptr->pid;
			cpu_ptr = cur;	//cpu_ptr에 idle이 아닌 새로운 작업을 대입   
			//printf("%04d PID : %03d START\n", Clock, cur->pid);

			list_del(&cur->ready);
			if(Clock != 0)
				up_switch_count();	//프로세스 전환으로 인한 process switching 발생
			return;
		}
	}
	// real_time_q, ready_q empty 
	// idle process execution
	if(cpu_ptr != idle){
		prev_pid = cpu_ptr->pid;
		cpu_ptr = idle;
		if(max_arrival >= Clock)
			up_switch_count();
		return;			
	}
}
void search_SRJF(){	//SRJF방식 스케쥴링
	list_for_each_entry_safe(cur, next, &real_time_q, real_time){	//search realtime process
		if(80 <= cur->pid && cur->pid <= 99){
			if(Clock != 0){
				cur->wait_time += 5;
			}
			
			if(cur->response_time == -1){
				cur->response_time = cur->wait_time;
			}
			prev_pid = cpu_ptr->pid;
			cpu_ptr = cur;	//cpu_ptr에 idle이 아닌 새로운 작업을 대입   
			//printf("%04d PID : %03d START\n", Clock, cur->pid);

			list_del(&cur->real_time);
			if(Clock != 0)
				up_switch_count();	//프로세스 전환으로 인한 process switching 발생
			return;	//idle이 아닌작업을 찾으면 종료
		}
	}
	list_for_each_entry_safe(cur, next, &ready_q, ready){//	search nomal process
			if(cur->operations[cur->pc].length < temp_ptr->operations[temp_ptr->pc].length)
				temp_ptr = cur;
	}

	if(temp_ptr != max_len){//SRJF
		if(Clock != 0)
			temp_ptr->wait_time += 5;
		if(temp_ptr->response_time == -1){
			temp_ptr->response_time = temp_ptr->wait_time;
		}
			
		prev_pid = cpu_ptr->pid;
		cpu_ptr = temp_ptr;	//cpu_ptr에 idle이 아닌 새로운 작업을 대입
		//printf("%04d PID : %03d START\n", Clock, cpu_ptr->pid);
		list_del(&temp_ptr->ready);
		if(Clock != 0){	//프로세스 전환으로 인한 process switching 발생
			up_switch_count();
		}	
		temp_ptr = max_len;
		return;
	}
	if(cpu_ptr != idle){
		if(max_arrival >= Clock)
			up_switch_count();
		prev_pid = cpu_ptr->pid;
		cpu_ptr = idle;
		return;
	}			
}
void search_RR(){
	list_for_each_entry_safe(cur, next, &real_time_q, real_time){	//search realtime process
		if(80 <= cur->pid && cur->pid <= 99){
			if(Clock != 0)
				cur->wait_time += 5;
			if(cur->response_time == -1){
				cur->response_time = cur->wait_time;
			}
			prev_pid = cpu_ptr->pid;
			cpu_ptr = cur;	//real_time process에게 cpu를 넘겨줌.  
			list_del(&cur->real_time);
			if(Clock != 0)
				up_switch_count();	//프로세스 전환으로 인한 process switching 발생
			return;	//real_time process를 찾을시 Return
		}
	}
	list_for_each_entry_safe(cur, next, &ready_q, ready){//	search nomal process
		if(temp_ptr == idle && cur->time_slice != 0)//timeslice가 남은 nomal process를 선택
			temp_ptr = cur;
		else if(check_TS_ptr == idle)
			check_TS_ptr = cur;
	}

	if(temp_ptr != idle){	//time_slice가 남은 nomal process가 남아있을때
		if(Clock != 0)
			temp_ptr->wait_time += 5;
		if(temp_ptr->response_time == -1){
			temp_ptr->response_time = temp_ptr->wait_time;
		}
		prev_pid = cpu_ptr->pid;
		cpu_ptr = temp_ptr;	//time slice가 남아있는 nomal process에게 cpu를 줌.
		//printf("%04d PID : %03d START\n", Clock, cpu_ptr->pid);

		list_del(&temp_ptr->ready);
		if(Clock != 0)
			up_switch_count();	//프로세스 전환으로 인한 process switching 발생
		temp_ptr = idle;
		check_TS_ptr = idle;
		return;
	} else if(check_TS_ptr != idle){	//모든 nomal process가 time_slice를 모두사용하여 라운드가 종료되었을때.(ROUND END)
		//printf("%04d CPU: ROUND ENDS. Recharge the Timeslices\n", Clock);
		list_for_each_entry_safe(cur, next, &ready_q, ready){//time slice 보충
			cur->time_slice = 50;
		}
		prev_pid = cpu_ptr->pid;
		cpu_ptr = check_TS_ptr;
		list_del(&check_TS_ptr->ready);
		if(prev_pid == cpu_ptr->pid){
			//printf("%04d CPU: Not Switched\tPID: %03d\n", Clock, cpu_ptr->pid);
		}else{
			if(Clock != 0)
				cpu_ptr->wait_time += 5;
			if(cpu_ptr->response_time == -1){
					cpu_ptr->response_time = cpu_ptr->wait_time;
			}
			up_switch_count();
		}
		check_TS_ptr = idle;
		return;
	}
	if(cpu_ptr != idle){//idle 상태
	prev_pid = cpu_ptr->pid;
	cpu_ptr = idle;
	if(max_arrival >= Clock)
		up_switch_count();
	return;
	}			
}
void check_end_process_RR(){	//마지막 operations까지 수행한 process를 지우고 다음 process을 찾아줌
	if(cpu_ptr->pc == cpu_ptr->code_bytes/2 || (cpu_ptr->time_slice == 0 && cpu_ptr != idle)) {	//만약 process의 pc와 code_bytes를 비교하여 같다면,
		if(cpu_ptr->pc == cpu_ptr->code_bytes/2){
			//printf("%04d CPU: Process is terminated\tPID:%03d\tPC:%03d\n", Clock, cpu_ptr->pid, cpu_ptr->pc);
		} else if(cpu_ptr->time_slice == 0 && cpu_ptr != idle){
			INIT_LIST_HEAD(&cpu_ptr->ready);
			list_add_tail(&cpu_ptr->ready, &ready_q);
		}
		
		search_RR();
	}		
}
void check_end_process_FIFO(){	//마지막 operations까지 수행한 process를 지우고 다음 process을 찾아줌
	if(cpu_ptr->pc == cpu_ptr->code_bytes/2 || (cpu_ptr->time_slice == 0 && cpu_ptr != idle)) {	//만약 process의 pc와 code_bytes를 비교하여 같다면,
		if(cpu_ptr->pc == cpu_ptr->code_bytes/2){
			//printf("%04d CPU: Process is terminated\tPID:%03d\tPC:%03d\n", Clock, cpu_ptr->pid, cpu_ptr->pc);
		search_FIFO();
		}		
	}
}
void check_end_process_SRJF(){	//마지막 operations까지 수행한 process를 지우고 다음 process을 찾아줌
	if(cpu_ptr->pc == cpu_ptr->code_bytes/2 || (cpu_ptr->time_slice == 0 && cpu_ptr != idle)) {	//만약 process의 pc와 code_bytes를 비교하여 같다면,
		if(cpu_ptr->pc == cpu_ptr->code_bytes/2){
			//printf("%04d CPU: Process is terminated\tPID:%03d\tPC:%03d\n", Clock, cpu_ptr->pid, cpu_ptr->pc);
			search_SRJF();
		}
	}		
}

void search_process_RR(){	//작업 할 process를 선택
	if(cpu_ptr == idle){//현재 실행중인 프로세스가 idle 상태면 서칭
		search_RR();	
	} else if(preemption == 1){//nomal process가 실행중에 realtime process가 들어온경우
		list_for_each_entry_safe(cur, next, &real_time_q, real_time){
			if(80 <= cur->pid && cur->pid <= 99){
				cur->wait_time += 5;
				if(cur->response_time == -1){
					cur->response_time = cur->wait_time;
				}
				prev_pid = cpu_ptr->pid;
				INIT_LIST_HEAD(&cpu_ptr->ready);
				list_add(&cpu_ptr->ready, &ready_q);
				cpu_ptr = cur;
				list_del(&cur->real_time);
				up_switch_count();
				preemption = 0;
				return;
			}
		}
	}
}
void search_process_FIFO(){	//작업 할 process를 선택
	if(cpu_ptr == idle){//현재 실행중인 프로세스가 idle 상태면 서칭
		search_FIFO();	
	} else if(preemption == 1){//nomal process가 실행중에 realtime process가 들어온경우
		list_for_each_entry_safe(cur, next, &real_time_q, real_time){
			if(80 <= cur->pid && cur->pid <= 99){
				cur->wait_time += 5;
				if(cur->response_time == -1){
					cur->response_time = cur->wait_time;
				}
				prev_pid = cpu_ptr->pid;
				INIT_LIST_HEAD(&cpu_ptr->ready);
				list_add(&cpu_ptr->ready, &ready_q);
				cpu_ptr = cur;
				list_del(&cur->real_time);
				up_switch_count();
				preemption = 0;
				return;
			}
		}
	}
}
void search_process_SRJF(){	//작업 할 process를 선택
	if(cpu_ptr == idle){//현재 실행중인 프로세스가 idle 상태면 서칭
		search_SRJF();	
	} else if(preemption == 1){//nomal process가 실행중에 realtime process가 들어온경우
		list_for_each_entry_safe(cur, next, &real_time_q, real_time){
			if(80 <= cur->pid && cur->pid <= 99){
				cur->wait_time += 5;
				if(cur->response_time == -1){
					cur->response_time = cur->wait_time;
				}
				prev_pid = cpu_ptr->pid;
				INIT_LIST_HEAD(&cpu_ptr->ready);
				list_add(&cpu_ptr->ready, &ready_q);
				cpu_ptr = cur;
				list_del(&cur->real_time);
				up_switch_count();
				preemption = 0;
				return;
			}
		}
	}
}
void work_operations(){	//cpu작업의 시작과 io작업의 시작, cpu작업의 종료를 처리
	if(cpu_ptr->operations[cpu_ptr->pc].op == 0){	//cpu 작업
		if(cpu_ptr->operations[cpu_ptr->pc].length == 1){	
			cpu_ptr->operations[cpu_ptr->pc].length--;
			cpu_ptr->time_slice--;
			cpu_ptr->pc++;	
		} else
        	cpu_ptr->operations[cpu_ptr->pc].length--;
			cpu_ptr->time_slice--;
	} 		
}

void SWITCHING_RR(){	//context switch가 일어났다면 처리해주는 함수
	while(is_switched != 0){	//switch문이 한번 돌때마다 is_switched 가 1씩 감소 0이될때까지 반복
		//printf("%04d CPU: Switched\tfrom: %03d\tto: %03d\n", Clock, prev_pid, cpu_ptr->pid);
		int prev_Clock;	//contexg switch로 인하여 skip된 clock를 처리해주기 위한 변수
		int check_load_pid = prev_pid;
		for(int i = 4; i >= 0 ; i--){	//skip 된 clock을 prev_clock이 대신하며 그 clock때 발생했어야할 이벤트를 처리해줌
			prev_Clock = Clock - i;
			ready_process_load(prev_Clock, Clock);		
			up_wait_time();
		}
		search_process_RR();
		//swiching 도중 cpu를 차지한 process에게 불핑요한 wait_time이 1추가 되었기에 빼주는 작업.
		if(prev_pid != check_load_pid){
			cpu_ptr->wait_time--;
			cpu_ptr->response_time--;
			check_load_pid = prev_pid;
		}
		is_switched--;
	}	
}
void SWITCHING_FIFO(){	//context switch가 일어났다면 처리해주는 함수
	while(is_switched != 0){	//switch문이 한번 돌때마다 is_switched 가 1씩 감소 0이될때까지 반복
		//printf("%04d CPU: Switched\tfrom: %03d\tto: %03d\n", Clock, prev_pid, cpu_ptr->pid);
		int prev_Clock;	//contexg switch로 인하여 skip된 clock를 처리해주기 위한 변수
		int check_load_pid = prev_pid;
		for(int i = 4; i >= 0 ; i--){	//skip 된 clock을 prev_clock이 대신하며 그 clock때 발생했어야할 이벤트를 처리해줌
			prev_Clock = Clock - i;
			ready_process_load(prev_Clock, Clock);		
			up_wait_time();
		}
		search_process_FIFO();
		//swiching 도중 cpu를 차지한 process에게 불핑요한 wait_time이 1추가 되었기에 빼주는 작업.
		if(prev_pid != check_load_pid){
			cpu_ptr->wait_time--;
			cpu_ptr->response_time--;
			check_load_pid = prev_pid;
		}
		is_switched--;
	}	
}
void SWITCHING_SRJF(){	//context switch가 일어났다면 처리해주는 함수
	while(is_switched != 0){	//switch문이 한번 돌때마다 is_switched 가 1씩 감소 0이될때까지 반복
		//printf("%04d CPU: Switched\tfrom: %03d\tto: %03d\n", Clock, prev_pid, cpu_ptr->pid);
		int prev_Clock;	//contexg switch로 인하여 skip된 clock를 처리해주기 위한 변수
		int check_load_pid = prev_pid;
		for(int i = 4; i >= 0 ; i--){	//skip 된 clock을 prev_clock이 대신하며 그 clock때 발생했어야할 이벤트를 처리해줌
			prev_Clock = Clock - i;
			ready_process_load(prev_Clock, Clock);		
			up_wait_time();
		}
		search_process_SRJF();
		//swiching 도중 cpu를 차지한 process에게 불핑요한 wait_time이 1추가 되었기에 빼주는 작업.
		if(prev_pid != check_load_pid){
			cpu_ptr->wait_time--;
			cpu_ptr->response_time--;
			check_load_pid = prev_pid;
		}
		is_switched--;
	}	
}
void sum_time(){
	list_for_each_entry_safe(cur, next, &job_q, job){	//job_q를 돌며 할당 받았던 메몰들을 반환한다
		total_wait += cur->wait_time;
		total_response += cur->response_time;
	}
}
void simulator_RR(){	//시뮬레이터

	//make_idle();	//	idle process 초기화
	make_max_len();
	cpu_ptr = idle;	//cpu_ptr를 idle로 초기화
	temp_ptr = idle;	//wait_ptr를 idle로 초기화
	check_TS_ptr = idle;	//check_TS_ptr를 ide로 초기화.

	while(1){

		check_end_process_RR();	//프로세스의 종료를 확인하고 다음 작업을 실행
		
		if(is_switched == 0){	//만약 context switch가 일어났다면 이부분을 스킵하고 switching함수 내에서 실행시켜줌.
			ready_process_load(Clock, Clock);	//준비가된 process를 ready_q로 이동
		}

		search_process_RR();	//	작업이 종료되거나 simulator가 처음 시작되었을때 실행시킬 process를 ready_q에서 찾아주는 함수
		up_wait_time();
		
		SWITCHING_RR();	//context switch가 일어났을때 skip된 작업들을 처리

		if(cpu_ptr == idle){	//cpu가 idle 상태일 때
			if(Clock >= max_arrival && list_empty(&ready_q) == 1 && list_empty(&real_time_q) == 1){//simulator 종료	
				sum_time();
				list_for_each_entry_safe_reverse(cur, next, &job_q, job){
					//printf("PID: %03d\tARRIVAL: %03d\tCODESIZE: %03d\tWAITING: %03d\tRESPONSE: %03d\n", cur->pid, cur->arrival_time, cur->code_bytes, cur->wait_time, cur->response_time);
				}
				printf("[Round-Robin] Scheduling Result\n");
				printf( "*** TOTAL CLOCKS: %04d IDLE: %04d UTIL: %2.2f%% WAIT: %2.2f RESPONSE: %2.2f\n", Clock, idle_Clock, (double)(Clock-idle_Clock) * 100/Clock, (double)(total_wait) / process_num, (double)total_response / process_num);
				break;
			}	
			idle_Clock++;// 종료조건을 만족하지 않았으면, idle time 증가
		}

		work_operations();	//operations의 작업을 처리
		Clock++;
		//printf("%04d DO PROCESS PID : %03d\n", Clock, cpu_ptr->pid);
	}
}void simulator_FIFO(){	//시뮬레이터

	make_idle();	//	idle process 초기화
	make_max_len();
	cpu_ptr = idle;	//cpu_ptr를 idle로 초기화
	temp_ptr = idle;	//wait_ptr를 idle로 초기화
	check_TS_ptr = idle;	//check_TS_ptr를 ide로 초기화.

	while(1){

		check_end_process_FIFO();	//프로세스의 종료를 확인하고 다음 작업을 실행
		
		if(is_switched == 0){	//만약 context switch가 일어났다면 이부분을 스킵하고 switching함수 내에서 실행시켜줌.
			ready_process_load(Clock, Clock);	//준비가된 process를 ready_q로 이동
		}

		search_process_FIFO();	//	작업이 종료되거나 simulator가 처음 시작되었을때 실행시킬 process를 ready_q에서 찾아주는 함수
		up_wait_time();
		
		SWITCHING_FIFO();	//context switch가 일어났을때 skip된 작업들을 처리

		if(cpu_ptr == idle){	//cpu가 idle 상태일 때
			if(Clock >= max_arrival && list_empty(&ready_q) == 1 && list_empty(&real_time_q) == 1){//simulator 종료	
				sum_time();
				list_for_each_entry_safe_reverse(cur, next, &job_q, job){
					//printf("PID: %03d\tARRIVAL: %03d\tCODESIZE: %03d\tWAITING: %03d\tRESPONSE: %03d\n", cur->pid, cur->arrival_time, cur->code_bytes, cur->wait_time, cur->response_time);
				}
				printf("[FIFO] Scheduling Result\n");
				printf( "*** TOTAL CLOCKS: %04d IDLE: %04d UTIL: %2.2f%% WAIT: %2.2f RESPONSE: %2.2f\n", Clock, idle_Clock, (double)(Clock-idle_Clock) * 100/Clock, (double)(total_wait) / process_num, (double)total_response / process_num);
				break;
			}	
			idle_Clock++;// 종료조건을 만족하지 않았으면, idle time 증가
		}

		work_operations();	//operations의 작업을 처리
		Clock++;
		//printf("%04d DO PROCESS PID : %03d\n", Clock, cpu_ptr->pid);
	}
}void simulator_SRJF(){	//시뮬레이터

	//make_idle();	//	idle process 초기화
	make_max_len();
	cpu_ptr = idle;	//cpu_ptr를 idle로 초기화
	temp_ptr = max_len;	//wait_ptr를 idle로 초기화
	check_TS_ptr = idle;	//check_TS_ptr를 ide로 초기화.

	while(1){

		check_end_process_SRJF();	//프로세스의 종료를 확인하고 다음 작업을 실행
		
		if(is_switched == 0){	//만약 context switch가 일어났다면 이부분을 스킵하고 switching함수 내에서 실행시켜줌.
			ready_process_load(Clock, Clock);	//준비가된 process를 ready_q로 이동
		}

		search_process_SRJF();	//	작업이 종료되거나 simulator가 처음 시작되었을때 실행시킬 process를 ready_q에서 찾아주는 함수
		up_wait_time();

		SWITCHING_SRJF();	//context switch가 일어났을때 skip된 작업들을 처리

		if(cpu_ptr == idle){	//cpu가 idle 상태일 때
			if(Clock >= max_arrival && list_empty(&ready_q) == 1 && list_empty(&real_time_q) == 1){//simulator 종료	
				sum_time();
				list_for_each_entry_safe_reverse(cur, next, &job_q, job){
					//printf("PID: %03d\tARRIVAL: %03d\tCODESIZE: %03d\tWAITING: %03d\tRESPONSE: %03d\n", cur->pid, cur->arrival_time, cur->code_bytes, cur->wait_time, cur->response_time);
				}
				printf("[SRJF] Scheduling Result\n");
				printf( "*** TOTAL CLOCKS: %04d IDLE: %04d UTIL: %2.2f%% WAIT: %2.2f RESPONSE: %2.2f\n", Clock, idle_Clock, (double)(Clock-idle_Clock) * 100/Clock, (double)(total_wait) / process_num, (double)total_response / process_num);
				break;
			}	
			idle_Clock++;// 종료조건을 만족하지 않았으면, idle time 증가
		}

		work_operations();	//operations의 작업을 처리
		Clock++;
		//printf("%04d DO PROCESS PID : %03d\n", Clock, cpu_ptr->pid);
	}
}
void free_malloc(){	//할당 받은 동적 메모리공간을 free
	list_for_each_entry_safe_reverse(cur, next, &job_q, job){	//job_q를 돌며 할당 받았던 메몰들을 반환한다
		free(cur->operations);
		free(cur);
	}
	list_for_each_entry_safe_reverse(cur, next, &temp_q, temp){	//job_q를 돌며 할당 받았던 메몰들을 반환한다
		free(cur->operations);
		free(cur);
	}
}


int main(){
	
	IO_task();	//이진파일을 읽어들여 job_q로 저장
	make_temp_q();
	IO_task_print();//job_q에 저장된 process정보 print
	simulator_FIFO();	//simulator 동작

	Reload_job_q();
	simulator_SRJF();

	Reload_job_q();
	simulator_RR();

	free_malloc();	//할당받은 메모리 해제
	return 0;
}