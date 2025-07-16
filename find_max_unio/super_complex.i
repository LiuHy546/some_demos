# 0 "super_complex.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "super_complex.c"






typedef unsigned char u8;
typedef unsigned int u32;
typedef long long int64;

typedef struct {
    int id;
    char name[40];
    struct {
        u8 flags:3;
        u8 mode:5;
        unsigned int status:4;
        unsigned int type:12;
    } bitfields;
    union {
        int64 timestamp;
        struct {
            int year;
            int month;
            int day;
        } date;
    } timeinfo;
} InnerStruct;

typedef union {
    float f;
    int i;
    char label[8];
    struct {
        u8 a;
        u8 b;
        union {
            int x;
            double y;
            struct {
                int arr[5];
                struct {
                    int z;
                    char c;
                } deep_anon;
            } deep_struct;
        } deep_union;
    } nested_anon;
} SmallUnion;

typedef struct {
    InnerStruct inners[2];
    SmallUnion su;
    double values[40];
    struct {
        int x;
        int y;
        union {
            int u;
            char v[4];
        } point_union;
    } point;
    int *ptr;
    union {
        char tag;
        struct {
            int a;
            int b;
        } ab;
    } tag_or_ab;
} BigStruct;

typedef union {
    BigStruct bs;
    InnerStruct is;
    SmallUnion su;
    int arr[40];
    struct {
        u32 a;
        char b[40];
        union {
            int x;
            double y;
            struct {
                int arr2[5];
                struct {
                    int z2;
                    char c2;
                } deep_anon2;
            } deep_struct2;
        } deep_union2;
    } anon_struct;
    unsigned long long bigval;
    struct {
        struct {
            int m;
            union {
                int n;
                char o[3];
            } mn_union;
        } mn_struct;
        int p;
    } nested_struct;
} MegaUnion;
