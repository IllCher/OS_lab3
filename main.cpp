#include <iostream>
#include <thread>
#include <string>
#include <functional>
using namespace std;
#define LENGTH 4
#define CORES 4
int Length;
int char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return (c-'0');
    } else {
        return (c-'W');
    }
}

bool a_lower_or_eq_b(string a, string b) {
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
    k = 1;
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
void split(string *orig, int left, int right, string *modif) {
    if (right - left < 2)return;
    split(orig, left, (left + right) / 2,modif);
    split(orig, (left + right) / 2, right,modif);
    merge(orig, left, (left + right) / 2, right,modif);
}
void merge_sort(string *in, int length) {
    string temp[length];
    thread threads[CORES];
    for (int i = 0; i < CORES; i++) {
        int new_left = i * length / CORES;
        int new_right = (i + 1) * length / CORES;
        threads[i] = thread(&split, ref(in), ref(new_left), ref(new_right), ref(temp));
    }
    for (int i = 0; i < CORES; i++)
        threads[i].join();
    for (int i = CORES/2; i > 0; i = i >> 1)
        for (int j = 0; j < i; j++){
            int left = (j)*length / i;
            int right = (j + 1)*length / i;
            merge(in, left, (left + right) / 2, right, temp);
        }
}
int main(int argc, char *argv[]) {
    int length;
    cin >> length;
    string array[LENGTH];
    for (int i = 0; i < LENGTH; i++)
        cin >> array[i];
    merge_sort(array, length);
    for (int i = 0; i < LENGTH; i++)
        cout << array[i] << endl;
    return 0;
}