#include <stdio.h>

int as_array(int count, char **names, int *ages)
{
    int i = 1;
    // attempt to make a function iterate over 
    for(i = 1; i < count; i++){
        printf("%s is %d.\n", names[i], ages[i-1]);
    }

    return 0;
}


int main(int argc, char *argv[])
{
    // create two arrays we care about
    int ages[] = {23, 43, 12, 89};
    char **names = argv;
    int name_address = names;
    int argv_address = argv;
    printf("Address of **names: %p\n", (void *)name_address);
    printf("Address of *argv: %p\n", (void *)argv_address);

    int count = sizeof(ages) / sizeof(int);

    // safely get the size of ages
    int i = 0;

    // first way using indexing
    for(i = 1; i < count; i++) {
        printf("%s has had %d years alive.\n", 
                names[i], ages[i]);
    }

    printf("---\n");

    as_array(argc, names, ages);

    printf("---\n");

    int *ptr;
    int val = 1;
    ptr = &val;
 
    // print out dereferenced values
    printf("dereference *ptr = %p\n", (void *)ptr);
    printf("dereference address of val *(&val) = %d\n", *(&val));

    printf("---\n");
    int *uninit; // leave the int pointer uninitialized
    int *nullptr = 0; // initialized to 0, could also be NULL
    void *vptr; // declare as a void pointer type
    int valu = 1;
    int *iptr;
    int *backptr;
     
    // void type can hold any pointer type or reference
    iptr = &valu;
    vptr = iptr;
    printf("iptr=%p, vptr=%p\n", (void *)iptr, (void *)vptr);
     
    // assign void pointer back to an int pointer and dereference
    backptr = vptr;
    printf("*backptr=%d\n", *backptr);
     
    // print null and uninitialized pointers
    printf("uninit=%p, nullptr=%p\n", (void *)uninit, (void *)nullptr);
    // don't know what you will get back, random garbage?
    // printf("*nullptr=%d\n", nullptr);
    // will cause a segmentation fault
    // printf("*nullptr=%d\n", nullptr);



    // setup the pointers to the start of the arrays
    // int *cur_age = ages;
    // char **cur_name = names;

    // // second way using pointers
    // for(i = 0; i < count; i++){
    //     printf("%s is %d years old.\n",
    //             *(cur_name + i), *(cur_age + i));
    // }

    // printf("---\n");

    // // third way, pointers are just arrays
    // for(i = 0; i < count; i++){
    //     printf("%s is %d years old again.\n",
    //             cur_name[i], cur_age[i]);
    // }

    // printf("---\n");

    // // fourth way with pointers in a stupid complex way
    // for(cur_name = names, cur_age = ages;
    //         (cur_age - ages) < count;
    //         cur_name++, cur_age++)
    // {
    //     printf("%s lived %d years so far.\n",
    //             *cur_name, *cur_age);
    // }

    return 0;
}