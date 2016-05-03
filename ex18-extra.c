#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** die function from ex17 */
void die(const char *message)
{
    if(errno){
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

// case for a function pointers
typedef int (*compare_cb)(int a, int b);
typedef int *(*sort)(int *numbers, int count, compare_cb cmp);


// Bubble sort function that uses compare_cb to do the sorting
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(count * sizeof(int));

    if(!target) die("Memory error.");

    // This copies the input numbers to target from numbers pointer, until count bytes is reached.
    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count; i++){
        for(j = 0; j < count - 1; j++){
            if(cmp(target[j], target[j+1]) > 0) {
                temp = target[j+1];
                target[j+1] = target[j];
                target[j] = temp;
            }
        }
    }

    return target;
}

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if(a == 0 || b == 0) {
        return 0;
    } else {
        return a % b;
    }
}

int mod_3_first(int a, int b)
{
    if(b % 3 == 0){
        return 1;
    } else {
        return 0;
    }
}

// Used to test that we are sorting things correctly
// By doring the sort and printing it out.
void test_sorting(int *numbers, int count, compare_cb cmp, sort method)
{
    int i = 0;
    int *sorted = method(numbers, count, cmp);

    if(!sorted) die("Failed to sort as requested.");

    for(i = 0; i < count; i++){
        printf("%d", sorted[i]);
    }
    printf("\n");

    free(sorted);
}

int main(int argc, char *argv[])
{
    if(argc < 2) die("USAGE: ex18 4 3 1 5 6");

    int count = argc - 1;
    int i = 0;
    char **inputs = argv + 1;  // argv is a pointer, so incrementing by 1 
                               // means that inputs points to the second 
                               // 

    int *numbers = malloc(count * sizeof(int));
    if(!numbers) die("Memory error.");

    for(i = 0; i < count; i ++){
        numbers[i] = atoi(inputs[i]);
    }

    test_sorting(numbers, count, sorted_order, bubble_sort);
    test_sorting(numbers, count, reverse_order, bubble_sort);
    test_sorting(numbers, count, strange_order, bubble_sort);
    test_sorting(numbers, count, mod_3_first, bubble_sort);

    free(numbers);

    return 0;
}