#include <std/str.h>

unsigned int str_strlen(const char* str) {
    unsigned int len = 0;
    while (*str) {
        ++len;
        ++str;
    }

    return len;
}

typedef struct {
    unsigned int  size;
    unsigned long hash;
} string_info_t;

/*
Calculate the basic string information.
Params:
    - `s` - The input string.
    - `len` - The string's size.
    - `info` - Output information.

Returns 1 if succeeds.
*/
static int _get_string_info(const char* s, int len, string_info_t* info) {
    char* h = (char*)s;
    info->size = 0;
    info->hash = 0xFFFF;
    while (h && *h && (len == -1 || len-- > 0)) {
        info->hash ^= *h * 0xFDDDF123;
        info->size++;
        h++;
    }

    return 1;
}

static int _string_cat(str_self self, string_t* dst) {
    char* nbody = (char*)mm_realloc(self->body, self->size + dst->size + 1);
    if (!nbody) return 0;

    str_memcpy(nbody + self->size, dst->head, dst->size + 1);

    string_info_t info;
    _get_string_info(nbody, -1, &info);

    self->body = nbody;
    self->hash = info.hash;
    self->size += dst->size;
    return 1;
}

static long long _string_to_llong(str_self self) {
    int neg         = 1;
    long long num   = 0;
    unsigned long i = 0;

    char* h = self->head;
    while (str_isspace(*h)) h++;
    if (*h == '-' || *h == '+') {
        neg = *h == '-' ? -1 : 1;
        h++;
    }

	while (*h >= '0' && *h <= '9' && *h) {
		num = num * 10 + (h[i] - '0');
        if (neg && num > LONG_MAX)        return LONG_MAX;
        if (neg == -1 && -num < LONG_MIN) return LONG_MIN;
		h++;
	}
    
	return (num * neg);
}

static unsigned long long _string_to_ullong(str_self self, int base) {
    char* h = self->head;
    unsigned long long result = 0;
    for (int i = 0; *h; ++i, ++h) {
        char c = *h;
        int digit = -1;

        if (c >= '0' && c <= '9')      digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
        else break;

        if (digit >= base) break;
        result = result * base + digit;
    }

    return result;
}

static string_t* _string_from_number(str_self self, int* is_float) {
    unsigned long long val = 0;
    for (unsigned int i = 0; i < self->size; i++) {
        if (self->head[i] == '.') {
            *is_float = 1;
            val = str_dob2bits(self->to_double(self));
            goto _force_creation;
        }
    }

    if (self->head[0] == '0' && (self->head[1] == 'x' || self->head[1] == 'X')) {
        self->hmove(self, 2);
        val = self->to_ullong(self, 16);
    }
    else if (self->head[0] == '0' && (self->head[1] == 'b' || self->head[1] == 'B')) {
        self->hmove(self, 2);
        val = self->to_ullong(self, 2);
    }
    else if (self->head[0] == '0' && self->head[1] && self->head[1] != '.') {
        self->hmove(self, 1);
        val = self->to_ullong(self, 8);
    }
    else {
        if (self->head[0] == '-') self->hmove(self, 1);
        val = self->to_ullong(self, 10);
    }
    
_force_creation: {}

    self->rhead(self);

    char buffer[128] = { 0 };
    snprintf(buffer, sizeof(buffer), "%s%llu", self->head[0] == '-' ? "-" : "", val);
    return create_string(buffer);
}

static double _string_to_double(str_self self) {
    char* s = self->head;
    double result = 0.0;
    int sign      = 1;
    int exp_sign  = 1;
    int exponent  = 0;
    int i         = 0;

    while (s[i] && (s[i] == ' ' || s[i] == '\t')) i++;
    if (s[i] && s[i] == '-') { sign = -1; i++; }
    else if (s[i] && s[i] == '+') { i++; }

    while (s[i] && s[i] >= '0' && s[i] <= '9') {
        result = result * 10.0 + (s[i] - '0');
        i++;
    }

    if (s[i] && s[i] == '.') {
        i++;
        double frac = 0.0;
        double base = 0.1;
        while (s[i] && s[i] >= '0' && s[i] <= '9') {
            frac += (s[i] - '0') * base;
            base *= 0.1;
            i++;
        }

        result += frac;
    }

    if (s[i] && (s[i] == 'e' || s[i] == 'E')) {
        i++;
        if (s[i] && s[i] == '+') i++;
        else if (s[i] && s[i] == '-') {
            exp_sign = -1; 
            i++;
        }

        while (s[i] && s[i] >= '0' && s[i] <= '9') {
            exponent = exponent * 10 + (s[i] - '0');
            i++;
        }
    }

    result *= mth_pow(10.0, exp_sign * exponent);
    return sign * result;
}

static string_t* _string_fchar(str_self self, char chr) {
    char* h = self->head;
    while (*h) {
        if (*h == chr) {
            return create_string(h);
        }

        h++;
    }

    return NULL;
}

static unsigned int _string_length(str_self self) {
    return self->size;
}

static string_t* _string_copy(str_self src) {
    string_t* str = (string_t*)mm_malloc(sizeof(string_t));
    if (!str) return NULL;
    
    str_memcpy(str, src, sizeof(string_t));
    str->body = (char*)mm_malloc(str->size + 1);
    if (!str->body) {
        mm_free(str);
        return NULL;
    }

    str_memcpy(str->body, src->body, str->size + 1);
    str->head = str->body;
    return str;
}

static int _string_equals(str_self self, string_t* s) {
    if (!self || !s) return 0;
    if (self->hash != s->hash) return 0;
    char *fh = self->head, *sh = s->head;
    while (*fh && *sh) {
        if (*fh != *sh) return 0;
        fh++;
        sh++;
    }

    return *fh == *sh;
}

static int _string_equals_raw(str_self self, const char* s) {
    if (!self || !s) return 0;
    char *fh = self->head, *sh = (char*)s;
    while (*fh && *sh) {
        if (*fh != *sh) return 0;
        fh++;
        sh++;
    }

    return *fh == *sh;
}

static int _string_index_of(str_self self, char c) {
    for (unsigned int i = 0; i < self->size; i++) {
        if (self->head[i] == c) return i;
    }

    return -1;
}

static int _string_replace(str_self self, const char* src, const char* dst) {
    unsigned int src_len = str_strlen(src);
    unsigned int dst_len = str_strlen(dst);
    char* s = self->head;

    if (!src_len) return 0;
    unsigned int count = 0;
    unsigned int j = 0;
    for (unsigned int i = 0; i < self->size; i++) {
        if (s[i] != src[j]) j = 0;
        else {
            j++;
            if (j == src_len) {
                count++;
                j = 0;
            }
        }
    }

    if (!count) return 0;
    unsigned int new_size = self->size + count * (dst_len - src_len);
    char* result = (char*)mm_malloc(new_size + 1);
    if (!result) return -1;

    unsigned int ri = 0;
    j = 0;

    for (unsigned int i = 0; i < self->size; i++) {
        if (s[i] == src[j]) {
            j++;
            if (j == src_len) {
                str_memcpy(result + ri, dst, dst_len);
                ri += dst_len;
                j = 0;
            }
        }
        else {
            if (j > 0) {
                unsigned int k = i - j;
                for (unsigned int t = 0; t < j; t++)
                    result[ri++] = s[k + t];
                j = 0;
            }

            result[ri++] = s[i];
        }
    }

    if (j > 0) {
        unsigned int k = self->size - j;
        for (unsigned int t = 0; t < j; t++)
            result[ri++] = s[k + t];
    }

    result[ri] = 0;
    mm_free(self->body);
    self->body = result;

    string_info_t info;
    _get_string_info(result, -1, &info);

    self->hash = info.hash;
    self->size = info.size;
    return count;
}

static int _string_move_head(str_self self, unsigned int offset) {
    self->head += offset;
    return 1;
}

static int _string_reset_head(str_self self) {
    self->head = self->body;
    return 1;
}

static string_t* _create_base_string(const char* s, unsigned int off, int len) {
    string_t* str = (string_t*)mm_malloc(sizeof(string_t));
    if (!str) return NULL;

    string_info_t info;
    _get_string_info((char*)s + off, len, &info);

    str->size = info.size;
    str->hash = info.hash;
    str->body = (char*)mm_malloc(info.size + 1);
    if (!str->body) {
        mm_free(str);
        return NULL;
    }

    str->copy        = _string_copy;
    str->equals      = _string_equals;
    str->requals     = _string_equals_raw;
    str->fchar       = _string_fchar;
    str->index_of    = _string_index_of;
    str->len         = _string_length;
    str->cat         = _string_cat;
    str->to_llong    = _string_to_llong;
    str->to_ullong   = _string_to_ullong;
    str->to_double   = _string_to_double;
    str->from_number = _string_from_number;
    str->hmove       = _string_move_head;
    str->rhead       = _string_reset_head;
    str->replace     = _string_replace;

    str->head = str->body;
    if (s) str_memcpy(str->body, s, info.size + 1);
    return str;
}

string_t* create_string(const char* s) {
    return _create_base_string(s, 0, -1);
}

string_t* create_string_from_part(const char* s, unsigned int off, int len) {
    return _create_base_string(s, off, len);
}

string_t* create_string_from_int(int c) {
    int len = 0;
    char buffer[255] = { 0 };
    if (c == 0) buffer[len++] = '0';
    else {
        unsigned char temp = c;
        while (temp > 0) {
            buffer[len++] = (temp % 10) + '0';
            temp /= 10;
        }
    }

    for (int i = 0; i < len / 2; i++) {
        char t = buffer[i];
        buffer[i] = buffer[len - 1 - i];
        buffer[len - 1 - i] = t;
    }

    return create_string(buffer);
}

int destroy_string(string_t* s) {
    if (!s) return 0;
    mm_free(s->body);
    return mm_free(s);
}
