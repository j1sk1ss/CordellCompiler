#include <std/str.h>

unsigned int str_strlen(const char* str) {
    unsigned int len = 0;
    while (*str) {
        ++len;
        ++str;
    }

    return len;
}

static int string_cat(str_self self, string_t* dst) {
    char* nbody = (char*)mm_realloc(self->body, self->size + dst->size + 1);
    if (!nbody) return 0;

    str_memcpy(nbody + self->size, dst->body, dst->size + 1);

    char* h = nbody;
    unsigned long hash = 0xFFFF;
    while (*h) {
        hash ^= *h * 0xFDDDF123;
        h++;
    }

    self->body = nbody;
    self->hash = hash;
    self->size += dst->size;
    return 1;
}

static long long string_to_llong(str_self self) {
    int neg         = 1;
    long long num   = 0;
    unsigned long i = 0;

    char* h = self->body;
    while (str_isspace(*h)) h++;
    if (*h == '-' || *h == '+') {
        neg = *h == '-' ? 0 : 1;
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

static unsigned long long string_to_ullong(str_self self, int base) {
    char* h = self->body;
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

static string_t* string_from_number(str_self self) {
    int isfloat = 0;
    unsigned long long val = 0;
    for (unsigned int i = 0; i < self->size; i++) {
        if (self->body[i] == '.') {
            isfloat = 1;
            val = str_dob2bits(self->to_double(self));
            break;
        }
    }

    if (!isfloat) {
        if (self->body[0] == '0' && (self->body[1] == 'x' || self->body[1] == 'X')) {
            self->hmove(self, 2);
            val = self->to_ullong(self, 16);
        }
        else if (self->body[0] == '0' && (self->body[1] == 'b' || self->body[1] == 'B')) {
            self->hmove(self, 2);
            val = self->to_ullong(self, 2);
        }
        else if (self->body[0] == '0' && self->body[1] && self->body[1] != '.') {
            self->hmove(self, 1);
            val = self->to_ullong(self, 8);
        }
        else {
            if (self->body[0] == '-') self->hmove(self, 1);
            val = self->to_ullong(self, 10);
        }
    }

    self->rhead(self);

    char buffer[128] = { 0 };
    snprintf(buffer, sizeof(buffer), "%s%llu", self->body[0] == '-' ? "-" : "", val);
    return create_string(buffer);
}

static double string_to_double(str_self self) {
    char* s = self->body;
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

static string_t* string_fchar(str_self self, char chr) {
    char* h = self->body;
    while (*h) {
        if (*h == chr) {
            return create_string(h);
        }

        h++;
    }

    return NULL;
}

static unsigned int string_length(str_self self) {
    return self->size;
}

static string_t* string_copy(str_self src) {
    string_t* str = (string_t*)mm_malloc(sizeof(string_t));
    if (!str) return NULL;
    
    str_memcpy(str, src, sizeof(string_t));
    str->body = (char*)mm_malloc(str->size + 1);
    if (!str->body) {
        mm_free(str);
        return NULL;
    }

    str_memcpy(str->body, src->body, str->size + 1);
    return str;
}

static int string_equals(str_self self, string_t* s) {
    if (!self || !s) return 0;
    if (self->hash != s->hash) return 0;
    char *fh = self->body, *sh = s->body;
    while (*fh && *sh) {
        if (*fh != *sh) return 0;
        fh++;
        sh++;
    }

    return *fh == *sh;
}

static int string_equals_raw(str_self self, const char* s) {
    if (!self || !s) return 0;
    char *fh = self->body, *sh = s;
    while (*fh && *sh) {
        if (*fh != *sh) return 0;
        fh++;
        sh++;
    }

    return *fh == *sh;
}

static unsigned int string_index_of(str_self self, char c) {
    return 1;
}

static int string_replace(str_self self, const char* src, const char* dst) {
    return 1;
}

static int string_move_head(str_self self, unsigned int offset) {
    self->head += offset;
    return 1;
}

static int string_reset_head(str_self self) {
    self->head = self->body;
    return 1;
}

static string_t* _create_base_string(const char* s, unsigned int off, int len) {
    string_t* str = (string_t*)mm_malloc(sizeof(string_t));
    if (!str) return NULL;

    char* h = (char*)s + off;
    unsigned int size = 0;
    unsigned long hash = 0xFFFF;
    while (
        h && *h && (len == -1 || len-- > 0)
    ) {
        hash ^= *h * 0xFDDDF123;
        size++;
        h++;
    }

    str->size = size;
    str->hash = hash;
    str->body = (char*)mm_malloc(size + 1);
    if (!str->body) {
        mm_free(str);
        return NULL;
    }

    str->copy        = string_copy;
    str->equals      = string_equals;
    str->requals     = string_equals_raw;
    str->fchar       = string_fchar;
    str->index_of    = string_index_of;
    str->len         = string_length;
    str->cat         = string_cat;
    str->to_llong    = string_to_llong;
    str->to_ullong   = string_to_ullong;
    str->to_double   = string_to_double;
    str->from_number = string_from_number;
    str->hmove       = string_move_head;
    str->rhead       = string_reset_head;
    str->replace     = string_replace;

    str->head = str->body;
    if (s) str_memcpy(str->body, s, size + 1);
    return str;
}

string_t* create_string(const char* s) {
    return _create_base_string(s, 0, -1);
}

string_t* create_string_from_part(const char* s, unsigned int off, int len) {
    return _create_base_string(s, off, len);
}

string_t* create_string_from_char(char c) {
    char buffer[2] = { c, 0 };
    return create_string(buffer);
}

int destroy_string(string_t* s) {
    if (!s) return 0;
    mm_free(s->body);
    return mm_free(s);
}
