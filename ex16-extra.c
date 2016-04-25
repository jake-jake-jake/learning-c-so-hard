#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight)
{
    struct Person *who = malloc(sizeof(struct Person));
    assert(who != NULL);

    who->name = strdup(name);
    who->age = age;
    who->height = height;
    who->weight = weight;

    return who;
}

struct Person Person_create_stack(char *name, int age, int height, int weight)
{
    struct Person who;
    who.name = strdup(name);
    who.age = age;
    who.height = height;
    who.weight = weight;
    return who;
}

void Person_destroy(struct Person *who)
{
    assert(who != NULL);

    free(who->name);
    free(who);
}

// void Person_destroy_stack(struct Person who)
// {
//     assert(who.name != NULL);

//     free(who);
//     // free(who.age);
//     // free(who.height);
//     // free(who.weight);
// }

void Person_print(struct Person *who)
{
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

void Person_print_stack(struct Person who)
{
    printf("Name: %s\n", who.name);
    printf("\tAge: %d\n", who.age);
    printf("\tHeight: %d\n", who.height);
    printf("\tWeight: %d\n", who.weight);
}

int main(int argc, char *argv[])
{
    // make two people structures
    struct Person *joe = Person_create(
            "Joe Alex", 32, 64, 140);
    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);
    printf("---\n");

    struct Person *frank = Person_create(
            "Frank Blank", 20, 72, 180);
    printf("Frank is at memory location %p:\n", frank);
    Person_print(frank);
    printf("---\n");


    struct Person stacked_person = Person_create_stack(
            "On the stack", 24, 70, 165);
    printf("stacked_person is at memory location: %p\n", &stacked_person);
    Person_print_stack(stacked_person);

    int some_int = 100;
    printf("some_int is at memory location: %p\n", &some_int);



    // make everyone age 2- years and print them again
    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    Person_print(joe);
    printf("---\n");

    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);
    printf("---\n");

    // destroy them both so we clean up
    Person_destroy(joe);
    Person_destroy(frank);

    return 0;
}