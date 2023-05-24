#include <iostream>
#include <omp.h>
#include <cstdlib>
#define N 100000

using namespace std;

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
    }
}

void parallelBubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        #pragma omp parallel for
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
    }
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void parallelMergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        #pragma omp task
        parallelMergeSort(arr, m + 1, r);
        #pragma omp taskwait
        merge(arr, l, m, r);
    }
}

int main()
{
    int arr[N];
    for (int i = 0; i < N; i++)
        arr[i] = rand() % N + 1;

    double start, end;

    start = omp_get_wtime();
    bubbleSort(arr, N);
    end = omp_get_wtime();
    cout << "Sequential bubble sort time: " << end - start << endl;

    for (int i = 0; i < N; i++)
        arr[i] = rand() % N + 1;

    start = omp_get_wtime();
    parallelBubbleSort(arr, N);
    end = omp_get_wtime();
    cout << "Parallel bubble sort time: " << end - start << endl;

    for (int i = 0; i < N; i++)
        arr[i] = rand() % N + 1;

    start = omp_get_wtime();
    mergeSort(arr, 0, N - 1);
    end = omp_get_wtime();
    cout << "Sequential merge sort time: " << end - start << endl;

    for (int i = 0; i < N; i++)
        arr[i] = rand() % N + 1;

    start = omp_get_wtime();
    parallelMergeSort(arr, 0, N - 1);
    end = omp_get_wtime();
    cout << "Parallel merge sort time: " << end - start << endl;

    return 0;
}

