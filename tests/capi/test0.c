#include <api/carbon.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int main()
{
    /* Create the necessary carbon object */
    struct carbon carb = init_carbon("test", "x86");

    /* Create a function object, used for managing a function and its operations */
    struct cfunc func = c_create_func(carb, "main", CTYPE_I32, 0, 0);
    
    /* We want our function to be global so we add that attribute */
    c_add_func_attribute(func, "global:true");

    /* Pushing operations to the function */
    c_push_op(func, INTLIT, 1, -1, c_reg(func, 0, CTYPE_I32), CTYPE_I32);
    c_push_op(func, RETURN, c_reg(func, 0, CTYPE_I32), -1, -1, CTYPE_I32);
    
    /* When we actually want to generate the function we call c_gen_func() */
    c_gen_func(func);

    /* The function object is not necessary anymore */
    c_destroy_func(func);

    /**
     * When we are done generating functions etc we write the generated assembly to file
     * and link and assemble 
     **/
    c_writeassembly(carb, "test.s");
    c_assemble(carb, "test.s", "test.o", NULL);
    c_link(carb, "test.o", "test", NULL);

    /* Destroy the carbon object */
    end_carbon(carb);
    return 0;
}