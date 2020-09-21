#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
struct carbon
{
    const char *infile;
    struct Parser *__parser;
};

struct cfunc
{
    const char *name;
    int findx;
    struct OpList *__statements;
    struct Parser *__parser;
};

struct ctype
{
    int byteSize;
    int ptr;
    int isFloat;
};

extern struct ctype __i32_type;
#define CTYPE_I32 __i32_type


/* Initialisation and end of carbon */  
struct carbon init_carbon(const char *infile, const char *arch);
void end_carbon(struct carbon carb);

/* Generation functions */
void c_gen_glob(const char *name, struct ctype t, int init_count, void *initlist);
void c_gen_func(struct cfunc func);
void c_push_op(struct cfunc func, int op, int ar1, int ar2, int ret, struct ctype t);

int c_reg(struct cfunc f, int r, struct ctype t);

/* Create a function object */
struct cfunc c_create_func(struct carbon carb, const char *name, struct ctype rettype,
                           int argc, struct ctype *arglist);
void c_destroy_func(struct cfunc func);

/* Add attribute functions */
void c_add_func_attribute(struct cfunc f, const char *attrib);

/* Write carbon's output to file */
int c_writeassembly(struct carbon carb, const char *file);
int c_assemble(struct carbon carb, const char *infile, const char *outfile, const char *assembler);
int c_link(struct carbon carb, const char *infile, const char *outfile, const char *linker);

#ifdef __cplusplus
}
#endif