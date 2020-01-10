#include <limits.h>
#include <stddef.h>
#include <windows.h>
#include <map>
#include <new>
#include "wco.h"

constexpr int INIT_STACK_SIZE = 1024 * 1024;
constexpr int INVALID_ID = -1;
constexpr size_t MAX_SIZE = INT_MAX - 1;

typedef struct _coroutine {
    schedule* s;
    coroutine_proc proc;
    void* userdata;
    int status;
    void* fiber;
    bool finish;
} coroutine;

struct __schedule {
    int cid;
    int index;
    void* fiber;
    std::map<int, coroutine*> objs;
};

static void release_coroutine(coroutine* c) {
    if (c) {
        if (c->fiber) {
            DeleteFiber(c->fiber);
        }
        delete c;
    }
}

static int add_coroutine_to_schedule(schedule* s, coroutine* c) {
    if (s->index >= static_cast<int>(MAX_SIZE)) {
        s->index = 0;
    }
    int key = ++ s->index;
    auto result = s->objs.insert({ key, c });
    if (!result.second) {
        return add_coroutine_to_schedule(s, c);
    }
    return key;
}

schedule* create_schedule() {
    schedule* s = new schedule;
    s->cid = INVALID_ID;
    s->index = 0;
    s->fiber = ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH);
    return s;
}

void close_schedule(schedule* s) {
    for (auto pair : s->objs) {
        release_coroutine(pair.second);
    }
    delete s;
}

void __stdcall fiber_start_routine(void* lpFiberParameter) {
    schedule* s = reinterpret_cast<schedule*>(lpFiberParameter);
    int id = s->cid;
    coroutine* obj = s->objs[id];
    
    (obj->proc)(s, obj->userdata);

    s->cid = INVALID_ID;
    obj->finish = true;

    SwitchToFiber(s->fiber);
}

int coroutine_create(schedule* s, coroutine_proc proc, void* userdata) {
    if (s->objs.size() >= MAX_SIZE) {
        return -1;
    }

    coroutine* co = new coroutine;
    co->s = s;
    co->proc = proc;
    co->userdata = userdata;
    co->status = COROUTINE_READY;
    co->fiber = CreateFiberEx(INIT_STACK_SIZE, 0, FIBER_FLAG_FLOAT_SWITCH, fiber_start_routine, s);
    co->finish = false;
    return add_coroutine_to_schedule(s, co);
}

void coroutine_resume(schedule* s, int id) {
    auto it = s->objs.find(id);
    if (it == s->objs.end()) {
        return;
    }

    coroutine* obj = it->second;

    if (obj->status == COROUTINE_READY || obj->status == COROUTINE_SUSPEND) {
        obj->status = COROUTINE_RUNNING;
        s->cid = id;
        SwitchToFiber(obj->fiber);
        if (obj->finish) {
            release_coroutine(obj);
            s->objs.erase(id);
        }
    }
}

void coroutine_yield(schedule* s) {
    auto it = s->objs.find(s->cid);
    if (it == s->objs.end()) {
        return;
    }

    coroutine* obj = it->second;
    obj->status = COROUTINE_SUSPEND;
    s->cid = INVALID_ID;
    SwitchToFiber(s->fiber);
}

int get_coroutine_status(schedule* s, int id) {
    auto it = s->objs.find(id);
    if (it == s->objs.end()) {
        return COROUTINE_DEAD;
    }

    return it->second->status;
}

int get_current_coroutine_id(schedule* s) {
    return s->cid;
}
