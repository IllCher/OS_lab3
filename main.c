#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
int opt_a;
int opt_t;
int opt_r;

// number of elements in array
//#define MAX 15
//#define MAX 16
int MAX;

// number of threads
//#define THREAD_MAX 4
int THREAD_MAX;

//using namespace std;

// array of size MAX
char** array;

// thread control parameters
typedef struct tsk tsk;
struct tsk {
    int tsk_no;
    int tsk_low;
    int tsk_high;
};
int char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return (c-'0');
    } else {
        return (c-'W');
    }
}

bool a_lower_or_eq_b(char* a, char* b) {
    int v1 = 0;
    int v2 = 0;
    int i = 0;
    int k = 1;
    while (a[i] != '\0') {
        v1 += char_to_int(a[i])*k;
        k*=10;
        i++;
    }
    i = 0;
    k = 0;
    while (b[i] != '\0') {
        v2 += char_to_int(b[i])*k;
        k*=10;
        i++;
    }
    if (v1 <= v2) {
        return  true;
    }
    return false;
}
// merge function for merging two parts
void merge(int low, int mid, int high) {

    // n1 is size of left part and n2 is size of right part
    int n1 = mid - low + 1;
    int n2 = high - mid;

    char** left = (char**)malloc(n1 * sizeof(char*));
    char** right = (char**)malloc(n2 * sizeof(char*));
    for (int i = 0; i < MAX; i++) {
        left[i] = (char*)malloc(sizeof(char)*30);
    }    for (int i = 0; i < MAX; i++) {
        right[i] = (char*)malloc(sizeof(char)*30);
    }
    // storing values in left part
    for (int i = 0; i < n1; i++)
        strcpy(left[i], array[i+low]);
    // storing values in right part
    for (int i = 0; i < n2; i++)
        strcpy(right[i], array[i + mid + 1]);
    int k = low;
    int i = 0;
    int j = 0;
    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (a_lower_or_eq_b(left[i], right[j]))
            strcpy(array[k++], left[i++]);
        else
            strcpy(array[k++], right[j++]);
    }

    // insert remaining values from left
    while (i < n1)
        strcpy(array[k++], left[i++]);

    // insert remaining values from right
    while (j < n2)
        strcpy(array[k++], right[j++]);

    //free(left);
    //free(right);
}

// merge sort function
void merge_sort(int low, int high) {

    // calculating mid point of array
    int mid = low + (high - low) / 2;

    if (low < high) {
        // calling first half
        merge_sort(low, mid);

        // calling second half
        merge_sort(mid + 1, high);

        // merging the two halves
        merge(low, mid, high);
    }
}

// thread function for multi-threading
void* merge_sort123(void *arg) {
    struct tsk *tsk = arg;
    int low;
    int high;

    // calculating low and high
    low = tsk->tsk_low;
    high = tsk->tsk_high;

    // evaluating mid point
    int mid = low + (high - low) / 2;

    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }

    return 0;
}
// Driver Code
int main(int argc, char **argv) {
    char *cp;
    tsk* tsk;

    //--argc;
    //++argv;
    scanf("%d", &MAX);
    THREAD_MAX = 4;
    array = (char**)malloc(sizeof(char*) * MAX);

    for (int i = 0; i < MAX; i++) {
        array[i] = (char*)malloc(sizeof(char)*30);
        scanf("%s", array[i]);
    }
    // generating random values in array
    pthread_t threads[THREAD_MAX];
    struct tsk tsklist[THREAD_MAX];

    int len = MAX / THREAD_MAX;

    int low = 0;

    for (int i = 0; i < THREAD_MAX; i++, low += len) {
        tsk = &tsklist[i];
        tsk->tsk_no = i;

        if (opt_a) {
            tsk->tsk_low = low;
            tsk->tsk_high = low + len - 1;
            if (i == (THREAD_MAX - 1))
                tsk->tsk_high = MAX - 1;
        }

        else {
            tsk->tsk_low = i * (MAX / THREAD_MAX);
            tsk->tsk_high = (i + 1) * (MAX / THREAD_MAX) - 1;
        }
    }

    // creating 4 threads
   for (int i = 0; i < THREAD_MAX; i++) {
        tsk = &tsklist[i];
        pthread_create(&threads[i], NULL, merge_sort123, tsk);
    }

    // joining all 4 threads
    for (int i = 0; i < THREAD_MAX; i++)
        pthread_join(threads[i], NULL);

    // show the array values for each thread


    // merging the final 4 parts
    if (opt_a) {
        struct tsk *tskm = &tsklist[0];
        for (int i = 1; i < THREAD_MAX; i++) {
            struct tsk *tsk = &tsklist[i];
            merge(tskm->tsk_low, tsk->tsk_low - 1, tsk->tsk_high);
        }
    }
    else {
        merge(0, (MAX / 2 - 1) / 2, MAX / 2 - 1);
        merge(MAX / 2, MAX / 2 + (MAX - 1 - MAX / 2) / 2, MAX - 1);
        merge(0, (MAX - 1) / 2, MAX - 1);
    }

    // displaying sorted array
    printf("\n\nSorted array:");
    for (int i = 0; i < MAX; i++) {
        printf("%s\n", array[i]);
    }
    for (int i = 0; i < MAX; i++) {
        free(array[i]);
    }
    free(array);
    return 0;
}