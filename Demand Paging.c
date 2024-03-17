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

#define PAGESIZE (32)
#define PAS_FRAMES (256) //fit for unsigned char frame in PTE
#define PAS_SIZE (PAGESIZE*PAS_FRAMES) //32*256 = 8192 B
#define VAS_PAGES (64)
#define VAS_SIZE (PAGESIZE*VAS_PAGES) //32*64 = 2048 B
#define PTE_SIZE (4) //sizeof(pte)
#define PAGETABLE_FRAMES (VAS_PAGES*PTE_SIZE/PAGESIZE) //64*4/32 = 8 consecutive frames
#define PAGE_INVALID (0)
#define PAGE_VALID (1)
#define MAX_REFERENCES (256)

typedef struct{
    unsigned char frame; //allocated frame
    unsigned char vflag; //valid-invalid bit
    unsigned char ref; //reference bit
    unsigned char pad; //padding
} pte; // Page Table Entry (total 4 Bytes, always)

 
typedef struct{
    int pid;
    int ref_len; //Less than 255
    unsigned char *references;
	int process_page_faults; //프로세스 별로 page faults 횟수를 저장
	int process_references; //프로세스별로 reference 횟수를 저장
	int alloc_frame; //할당 된 
    struct list_head job, ready;
	
} process_raw;

typedef struct {
    unsigned char b[PAGESIZE];
} frame;

    process_raw *cur, *next; //연결리스트를 돌며 특정 list를 지정하기 위한 변수
	pte* pte_ptr; //pte 찾기 위한 포인터
	frame *pas; //physical adderess space

    LIST_HEAD(job_q);	//list_head job_q 선언 및 초기화
	LIST_HEAD(ready_q);	//list_head ready_q 선언 및 초기화

    int process_count = 0; //이진 파일로부터 읽어들인 process수를 세기 위한 변수
	int base_res = 0; // base register를 저장하기 위한 변수
	int ref_count = 0; //simulator 구현시 각 프로세스별로 순차적으로 reference를 한번씩 돌아가며 실행해주게 구현하였음.이 때 현재 실행해야할 reference를 찾기위한 변수. 과제 1, 2의 clock역할.
	int frame_count = -1; //frame_count - 할당 해야할 프레임 위치를 찾기 위한 변수
	int total_page_faults = 0; //전체 page faults를 저장하기 위한 변수. total_ 변수는 total값을 구할 때 따로 다시 queue를 돌며 합산하여 시간이 걸리지 않도록 하기위하여 따로 변수를 만들어놈.
	int total_references = 0; //전체 references를 저장하기 위한 변수.
	int num = 0; //pas 상세 frame를 찾기위한 변수

void IO_task(){	//binary형태의 파일을 읽어들여 job_q에 저장
	while(fread(cur = malloc(sizeof(process_raw)), sizeof(int), 2, stdin) == 2){    //pocess구조체의 크기를 동적할당 받아 int형 변수 3개에 값을 차래대로 대입
		cur->process_page_faults = 0;
		cur->process_references = 0;
		INIT_LIST_HEAD(&cur->job);
		list_add_tail(&cur->job, &job_q);
		cur->references = malloc(sizeof(unsigned char) * cur->ref_len);	//code_bytes에 저장된 정보를 operaations크기만큼 동적 할당받아 저장.
		for(int i = 0; i < cur->ref_len; i++){ 
			fread(&cur->references[i], sizeof(unsigned char), 1, stdin);
        }
    }
}
void IO_task_print(){	//job_q에 저장된 파일을 출력
		list_for_each_entry_safe(cur, next, &job_q, job){
		printf("%d %d\n",cur->pid, cur->ref_len);
		for(int i = 0; i < cur->ref_len; i++){
			printf("%d ",cur->references[i]);
		}
        printf("\n");
	}
}
void load_process(){ //ready_q에 job_q를 순서대로 삽입.
	list_for_each_entry_safe(cur, next, &job_q, job){	
		INIT_LIST_HEAD(&cur->ready);
		list_add_tail(&cur->ready, &ready_q);
		cur->alloc_frame = 8; //process별로 8개의 프레임 할당
		process_count++;
		frame_count += 8;
	}
}

void Init_PTE(){ //page table를 초기화 하는 변수
	pas = (frame*) malloc(PAS_SIZE); //(unsigned char형 32칸 배열) * 256개
	for(int i = 0; i < process_count * 8; i++){
		pte_ptr = (pte*) &pas[i];
		for(int j = 0; j < 8; j++){
			pte_ptr[j].vflag = PAGE_INVALID;
		}
	}
}
void end_simulator(){ //simulator가 종료 조건인지 검사하여서 종료후 출력하는 함수.
	list_for_each_entry_safe(cur, next, &job_q, job){
		printf("** Process %03d: Allocated Frames=%03d PageFaults/References=%03d/%03d\n", cur->pid, cur->alloc_frame, cur->process_page_faults, cur->process_references);
		base_res = cur->pid * 8;
		for(int i = base_res; i < (base_res + 8); i++){
			pte_ptr = (pte*) &pas[i];
			for(int j = 0; j < 8; j++){
				if(pte_ptr[j].vflag == PAGE_VALID){	//1번이라도 reference된 경우 출력
					printf("%03d -> %03d REF=%03d\n", ((i - base_res) * 8 + j), pte_ptr[j].frame, pte_ptr[j].ref);
				}
			}
		}
	}
	printf("Total: Allocated Frames=%03d Page Faults/References=%03d/%03d\n", frame_count + 1, total_page_faults, total_references);
}
void up_count_pf(){//page fault시 각 변수들의 count를 올려주는 함수
	cur->process_page_faults++;
	cur->alloc_frame++;
	total_page_faults++;
}
void up_count_rf(){//refence시 각변수들의 count를 올려주는 함수
	cur->process_references++;
	total_references++;
}
void make_valid(){	//invaild 상황시 vaild로 변경해주고 할당 된 frame의 정보를 넣어주는 함수
	pte_ptr[num].frame = frame_count;
    pte_ptr[num].vflag = PAGE_VALID;
}
void simulator(){

	load_process();
	Init_PTE();
	int ref;	//수행 해야 할 reference
	while(1){
		list_for_each_entry_safe(cur, next, &ready_q, ready){ //ready_q를 돌며 references를 하나씩 실행
			base_res = cur->pid * 8; //각 process별로 처음 생성시 8개의 프레임을 할당받으므로 pid * 8을 해줘서 base register를 찾음.
			ref = cur->references[ref_count]; //접근할 페이지 정보를 ref변수에 담아둠
			pte_ptr = (pte*) &pas[base_res + (ref / 8)]; //접근할 페이지의 frame을 pte단위의 포인터로 가리킴
			num = ref % 8;//위에서 가리킨 포인터의 몇번째 index에 reference값이 있는지 찾기 위한 값을 num에 대입.
			if(pte_ptr[num].vflag == PAGE_INVALID){ //해당 page의 물리프레임이 할당되어 있지 않은 경우. page fault 상황, PAS 할당
				frame_count++;
				if(frame_count > 255){
					break;
				}
				make_valid(); //할당후 flag를 valid로 바꾸어 할당됨을 표시.
				pte_ptr[num].ref = 1;
				up_count_pf(); //page fault 횟수 증가
				up_count_rf(); //reference 횟수 증가
			} else if(pte_ptr[num].vflag == PAGE_VALID){ //해당 page의 물리프레임이 이미 할당 되어 있는 경우.
				pte_ptr[num].ref++; //참조 횟수 증가
				up_count_rf();	
			}
			if(cur->ref_len - 1 == ref_count){ //모든 reference에 대해서 pas할당이 끝난 process는 ready_q에서 제거
				list_del(&cur->ready);
			}
		}
		if(frame_count > 255){ //free frame을 모두 사용한 경우
			frame_count--;
			printf("Out of memory!!\n");
			end_simulator(); //Out of memory 출력 후 종료 함수 실행.
			break;
		}
		if(list_empty(&ready_q) == 1){ //ready_q에 들어있는 모든 process가 참조하려는 page에 frame을 모두 할당한 상황.
			end_simulator();
			break;
		}
		ref_count++;
	}
}
void free_malloc(){ //할당한 동적메모리를 반환
	free(pas);
	list_for_each_entry_safe_reverse(cur, next, &job_q, job){ //job_q를 돌며 할당 받았던 메모리들을 반환한다
		free(cur->references);
		free(cur);	
	}
}
int main(){

    IO_task();
    //IO_task_print();
	simulator();
	free_malloc();
	
    return 0;
} 