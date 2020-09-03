const int INPUT_BUF = 1 << 12;
const int OUTPUT_BUF = 1 << 16;
char input[INPUT_BUF];
char output[OUTPUT_BUF];
int input_ptr = 0;
int input_len = 0;
int output_ptr = 0;

inline
bool check_eof() {
    if (input_ptr == input_len) {
        input_ptr = 0;
        input_len = fread(input, 1, INPUT_BUF, stdin);
    }
    return input_ptr == input_len;
}
inline
char get_char() {
    assert(!check_eof());
    return input[input_ptr++];
}
inline
char read_char() {
    char c = get_char();
    while (c <= ' ') {
        c = get_char();
    }
    return c;
}
inline
int read_int() {
    int sgn = +1;
    char c = read_char();
    if (c == '-') {
        sgn = -1;
        c = get_char();
    }
    int x = 0;
    while ('0' <= c && c <= '9') {
        x = 10 * x + c - '0';
        c = get_char();
    }
    return x * sgn;

}
inline
__attribute__((destructor))
void flush_output() {
    fwrite(output, 1, output_ptr, stdout);
    output_ptr = 0;
}
inline
void write(char c) {
    if (output_ptr == OUTPUT_BUF) {
        flush_output();
    }
    output[output_ptr++] = c;
}
inline
void write(int x) {
    static const int LEN = 20;
    static char tmp[LEN];
    int n = 0;
    if (x < 0) {
        write('-');
        x *= -1;
    }
    while (!n || x) {
        tmp[n++] = (char)(x % 10 + '0');
        x /= 10;
    }
    while (n--) {
        write(tmp[n]);
    }
}
inline
void write(long long x) {
    static const int LEN = 20;
    static char tmp[LEN];
    int n = 0;
    if (x < 0) {
        write('-');
        x *= -1;
    }
    while (!n || x) {
        tmp[n++] = (char)(x % 10 + '0');
        x /= 10;
    }
    while (n--) {
        write(tmp[n]);
    }
}
inline
void write(string s) {
    for (char c : s) {
        write(c);
    }
}
inline
void write(const char *s) {
    for (const char *ptr = s; *ptr; ++ptr) {
        write(*ptr);
    }
}
