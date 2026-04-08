#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define todo(msg)                                                              \
    ({                                                                         \
        _Pragma("GCC diagnostic ignored \"-Wunused\"");                        \
        _Pragma("GCC diagnostic ignored \"-Wunused-variable\"");               \
                                                                               \
        fprintf(stderr, "not yet implemented: \"%s\" at %s:%d\n", msg,         \
                __FILE__, __LINE__);                                           \
        exit(1);                                                               \
        0;                                                                     \
    })

typedef uint8_t u8;
typedef uint16_t u16;

struct Cpu {

    struct {
        u16 pc;
        u16 sp;
        u8 a;

        union {
            struct {
                u8 c;
                u8 b;
                u8 e;
                u8 d;
                u8 l;
                u8 h;
            };

            struct {
                u16 bc;
                u16 de;
                u16 hl;
            };
        };
    } Register;

    struct {
        u8 z;
        u8 h;
        u8 c;
        u8 n;
    } Flag;
};

int main(int argc, char *argv[]) {
    todo("Implement CPU");
    return 0;
}
