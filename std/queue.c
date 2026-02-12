#include <std/queue.h>

int queue_init(queue_t* q) {
    if (!q) return 0;

    q->size = QUEUE_INITIAL_CAPACITY;
    q->body = (queue_entry_t*)mm_malloc(QUEUE_INITIAL_CAPACITY * sizeof(queue_entry_t));
    if (!q->body) return 0;

    q->meta.head  = 0;
    q->meta.tail  = 0;
    q->meta.count = 0;

    return 1;
}

static int _queue_resize(queue_t* q) {
    int old_capacity = q->size;
    int new_capacity = old_capacity * 2;
    
    int head  = q->meta.head;
    int tail  = q->meta.tail;
    int count = q->meta.count;

    queue_entry_t* new_body = (queue_entry_t*)mm_malloc(new_capacity * sizeof(queue_entry_t));
    if (!new_body) return 0;
    if (head < tail) {
        for (int i = 0; i < count; i++) {
            new_body[i].data = q->body[head + i].data;
        }
    } 
    else if (count > 0) {
        int part1 = old_capacity - head;
        for (int i = 0; i < part1; i++) {
            new_body[i].data = q->body[head + i].data;
        }

        for (int i = 0; i < tail; i++) {
            new_body[part1 + i].data = q->body[i].data;
        }
    }

    mm_free((char*)q->body);

    q->size       = new_capacity;
    q->body       = new_body;
    q->meta.head  = 0;
    q->meta.tail  = count;
    q->meta.count = count;
    return 1;
}

int queue_push(queue_t* q, void* d) {
    if (!q || !q->body) return 0;

    if (q->meta.count == q->size) {
        if (!_queue_resize(q)) {
            return 0;
        }
    }

    q->body[q->meta.tail].data = d;
    q->meta.tail = (q->meta.tail + 1) % q->size;
    q->meta.count++;
    return 1;
}

int queue_pop(queue_t* q, void** d) {
    if (!q || !q->body || !d) return 0;
    if (q->meta.count == 0) return 0;

    *d = q->body[q->meta.head].data;
    q->meta.head = (q->meta.head + 1) % q->size;
    q->meta.count--;

    return 1;
}

int queue_free(queue_t* q) {
    if (q->body) {
        mm_free(q->body);
        q->body = NULL;
    }

    q->body = 0;
    return 1;
}

int queue_free_force(queue_t* q) {
    if (q && q->body) {
        for (int i = 0; i < q->meta.count; i++) {
            int index = (q->meta.head + i) % q->size;
            void* elem = q->body[index].data;
            if (elem) {
                mm_free(elem);
                q->body[index].data = NULL;
            }
        }

        mm_free(q->body);
        q->body = NULL;
        q->meta.head = q->meta.tail = q->meta.count = 0;
        q->size = 0;
        return 0;
    }
    
    return -1;
}
