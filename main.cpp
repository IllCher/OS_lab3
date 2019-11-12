#include <iostream>
#include <pthread.h>
#include <string>
using namespace std;
int Length;
int Cores;
pthread_mutex_t mutex;
struct arguments {
    string* orig;
    int left;
    int right;
    string* mod;
};
int char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return (c-'0');
    } else {
        if (c-'W' > 0)
            return (c-'W');
        else {
            return 0;
        }
    }
}

bool a_lower_or_eq_b(string a, string b) {
    int v1 = 0;
    int v2 = 0;
    int i = a.size();
    int k = 1;
    while (i > 0) {
        i--;
        v1 += char_to_int(a[i])*k;
        k*=10;
    }
    i = b.size();
    k = 1;
    while (i > 0) {
        i--;
        v2 += char_to_int(b[i])*k;
        k*=10;
    }
    if (v1 <= v2) {
        return  true;
    }
    return false;
}

void merge(string *orig, int left, int mid, int right, string *modif) {
    int l = left;
    int r = mid;
    for (int i = left; i < right; i++)
        if (l < mid && (r >= right || a_lower_or_eq_b(orig[l], orig[r])))
            modif[i] = orig[l++];
        else
            modif[i] = orig[r++];
    for (int i = left; i < right; i++)
        orig[i] = modif[i];
}
void* split(void* param) {
    struct arguments* temp_args = new arguments;
    temp_args = (arguments*)param;
    if (temp_args->right - temp_args->left < 2) return NULL;
    int tmp_right = temp_args->right;
    int tmp_left = temp_args->left;
    temp_args->right = (temp_args->left + temp_args->right) / 2;
    split((void*)temp_args);
    temp_args->right = tmp_right;
    temp_args->left = (temp_args->left + temp_args->right) / 2;
    split((void*)temp_args);
    temp_args->left = tmp_left;
    merge(temp_args->orig, temp_args->left, (temp_args->left + temp_args->right) / 2, temp_args->right, temp_args->mod);
}
void merge_sort(string *in) {
    struct arguments* a = new arguments;
    string temp[Length];
    pthread_t threads[Cores];
    for (int i = 0; i < Cores; i++) {
        pthread_mutex_init(&mutex, NULL);
        int new_left = i * Length / Cores;
        int new_right = (i + 1) * Length / Cores;
        a->mod = temp;
        a->orig = in;
        a->left = new_left;
        a->right = new_right;
        pthread_create(&threads[i], NULL, split, (void*)a);
        pthread_mutex_destroy(&mutex);
    }
    for (int i = 0; i < Cores; i++)
        pthread_join(threads[i], NULL);
    for (int i = Cores/2; i > 0; i = i >> 1) //divide by 2
        for (int j = 0; j < i; j++){
            int left = (j)*Length / i;
            int right = (j + 1)*Length / i;
            merge(in, left, (left + right) / 2, right, temp);
        }
}
int main(int argc, char *argv[]) {

    cin >> Length;
    string array[Length];
    for (int i = 0; i < Length; i++)
        cin >> array[i];
    Cores = atoi(argv[1]);
    if (Cores > Length) {
        Cores = Length / 2;
    }
    merge_sort(array);
    cout << "Sorted array:\n";
    for (int i = 0; i < Length; i++)
        cout << array[i] << "\n";
    return 0;
}