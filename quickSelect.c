#include <stdio.h>

// Swap two elements in an array
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

//Partition function
int partition(int arr[], int n) {
	int L = 0;
	int R = n - 1;
	while (L < R) {
		if (arr[L + 1] <= arr[L]) {
			swap(&arr[L], &arr[L + 1]);
			L++;
		} else if (arr[R] > arr[L]) {
			R--;
		} else {
			swap(&arr[R], &arr[L + 1]);
		}
	}
	return L;
}

//Quickselect function
int qs(int arr[], int n, int k) {
	int p = partition(arr, n);
	if (k == p) {
		return arr[p];
	} else if (k < p) {
		return qs(arr, p, k);
	} else {
		return qs(arr + p + 1, n - (p + 1), k - (p + 1));
	}
}

//Test the Quickselect function
int main(){
	int arr1[] = {7, 9, 1, 2, 3, 6, 8};
	int n1 = sizeof(arr1) / sizeof(arr1[0]);
	int k1 = 4;
	int k2 = 5;
	int result1 = qs(arr1, n1, k1);
	int result2 = qs(arr1, n1, k2);
	printf("The forth smallest element in array1 is %d\n", result1);
	printf("The fifth smallest element in array2 is %d\n", result2);
	int arr2[] = {13, 25, 1, 66, 120, 47};
	int k3 = 3;
	int n2 = sizeof(arr2) / sizeof(arr2[0]);
	int result3 = qs(arr2, n2, k3);
	printf("The third smallest element in array3 is %d\n", result3);
	return 0;
}
