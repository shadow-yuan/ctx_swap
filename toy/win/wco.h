#ifndef __WIN_COROUTINE_H__
#define __WIN_COROUTINE_H__

#ifdef __cplusplus
extern "C" {
#endif

// coroutine status
enum {
    COROUTINE_DEAD = 0,
    COROUTINE_READY,
    COROUTINE_RUNNING,
    COROUTINE_SUSPEND
};

typedef struct __schedule schedule;
typedef void (*coroutine_proc)(schedule* s, void* userdata);

schedule* create_schedule();
void close_schedule(schedule* s);

// On success, a id for the new coroutine is returned.
// On error, -1 is returned.
int coroutine_create(schedule* s, coroutine_proc proc, void* userdata);

void coroutine_resume(schedule* s, int id);
void coroutine_yield(schedule* s);
int  get_coroutine_status(schedule* s, int id);
int  get_current_coroutine_id(schedule* s);

#ifdef __cplusplus
}
#endif

#endif // __WIN_COROUTINE_H__
