#pragma once
#include <vector>
#include <iostream>
#include <algorithm>


//void printVector(const std::vector<int>& v) {
//	for (int i = 0; i < v.size(); ++i) {
//		std::cout << v[i] << " ";
//	}
//	std::cout << "\n";
//}


//*****************************************************************
// BUBBLE
//*****************************************************************
template <typename T, typename Compare = std::less<>>
void bubbleSort(
	std::vector<T>& v, 
	Compare comp = {}
) {
	for (int i = 0; i < static_cast<int>(v.size()); ++i) {
		int c = 0;

		for (int j = 0; j < static_cast<int>(v.size()) - 1 - i; ++j) {
			if (comp(v[j + 1], v[j])) {
				T a = v[j];
				v[j] = v[j + 1];
				v[j + 1] = a;
				c++;
			}
		}

		if (c == 0) {
			break;
		}
	}
}


//*****************************************************************
// INSERTION
//*****************************************************************
template <typename T, typename Compare>
void insertionSort(std::vector<T>& v, Compare comp) {
	for (int i = 1; i < static_cast<int>(v.size()); ++i) {
		T key = v[i];
		int j = i - 1;

		while (j >= 0 && comp(key, v[j])) {
			v[j + 1] = v[j];
			--j;
		}
		v[j + 1] = key;
	}
}


//*****************************************************************
// SELECTION
//*****************************************************************
template <typename T, typename Compare>
void selectionSort(std::vector<T>& v, Compare comp) {
	for (int i = 0; i < static_cast<int>(v.size()) - 1; ++i) {
		int min = i;

		for (int j = i + 1; j < static_cast<int>(v.size()); ++j) {
			if (comp(v[j], v[min])) {
				min = j;
			}
		}
		if (min != i) {
			std::swap(v[i], v[min]);
		}
	}
}


//*****************************************************************
// MERGE
//*****************************************************************
template <typename T, typename Compare>
void merge(
	std::vector<T>& src,
	std::vector<T>& buffer,
	size_t left,
	size_t mid,
	size_t right,
	Compare comp
) {
	size_t i = left;
	size_t j = mid + 1;
	size_t k = left;

	while (i <= mid && j <= right) {
		if (comp(src[i], src[j])) {
			buffer[k++] = src[i++];
		}
		else {
			buffer[k++] = src[j++];
		}
	}

	while (i <= mid) {
		buffer[k++] = src[i++];
	}

	while (j <= right) {
		buffer[k++] = src[j++];
	}

	for (size_t x = left; x <= right; ++x) {
		src[x] = buffer[x];
	}
}
template <typename T, typename Compare>
void mergeSortImpl(
	std::vector<T>& src, 
	std::vector<T>& buffer, 
	size_t left, 
	size_t right,
	Compare comp
) {
	if (left >= right) {
		return;
	}

	size_t mid = left + (right - left) / 2;

	mergeSortImpl(src, buffer, left, mid, comp);					
	mergeSortImpl(src, buffer, mid + 1, right, comp);

	merge(src, buffer, left, mid, right, comp);
}

template <typename T, typename Compare>
void mergeSort(std::vector<T>& v, Compare comp) {
	if (v.empty()) { return; }

	std::vector<T> buffer(v.size());

	// auto comp = [](int a, int b) {
	// 	return a < b;
	// };

	mergeSortImpl(v, buffer, 0, v.size() - 1, comp);
}

//*****************************************************************
// PARALLEL MERGE SORT
//*****************************************************************
#include <thread>
template <typename Compare>
void parallelMergeSortImpl(
	std::vector<int>& src,
	std::vector<int>& buffer,
	size_t left,
	size_t right,
	Compare comp,
	size_t &thread_count
){
	if (left >= right) {
		return;
	}
	
	size_t mid = left + (right - left) / 2;

	if (right - left > 100000) {

		std::thread t1(
			[&src, &buffer, left, mid, comp, &thread_count]() {
				parallelMergeSortImpl(src, buffer, left, mid, comp, thread_count);
			}
		);

		std::thread t2(
			[&src, &buffer, mid, right, comp, &thread_count]() {
				parallelMergeSortImpl(src, buffer, mid, right, comp, thread_count);
			}
		);

		thread_count += 2;

		t1.join();
		t2.join();
	}
	else {
		mergeSortImpl(src, buffer, left, mid, comp);
		mergeSortImpl(src, buffer, mid + 1, right, comp);
	}
	merge(src, buffer, left, mid, right, comp);
}

void parallelMergeSort(std::vector<int>& v) {
	if (v.empty()) { return; }

	std::vector<int> buffer(v.size());

	auto comp = [](int a, int b) {
		return a < b;
	};
	
	size_t thread_count = 0; 

	parallelMergeSortImpl(v, buffer, 0, v.size() - 1, comp, thread_count);
	std::cout << thread_count << "\n";
}


//*****************************************************************
// QUICK
//*****************************************************************
template <typename T, typename Compare>
void quickSortImpl(std::vector<T>& v, int left, int right, Compare comp) {
	if (left >= right) {
		return;
	}

	int i = left;
	int j = right;

	T pivot = v[left + (right - left) / 2];

	while (i <= j) {
		while (comp(v[i], pivot)) {
			++i;
		}
		while (comp(pivot, v[j])) {
			--j;
		}

		if (i <= j) {
			std::swap(v[i], v[j]);
			++i;
			--j;
		}
	}

	quickSortImpl(v, left, j, comp);
	quickSortImpl(v, i, right, comp);
}

template <typename T, typename Compare>
void quickSort(std::vector<T>& v, Compare comp) {
	if (v.empty()) {
		return;
	}
	quickSortImpl(v, 0, static_cast<int>(v.size()) - 1, comp);
}


//*****************************************************************
// HEAP SORT
//*****************************************************************
template <typename T, typename Compare>
class Heap {
	Compare comp;

	void heapify(std::vector<T>& v, int size, int i) {
		int selected = i;
		int l = 2 * i + 1;
		int r = 2 * i + 2;

		if (l < size && comp(v[selected], v[l])) {
			selected = l;
		}

		if (r < size && comp(v[selected], v[r])) {
			selected = r;
		}

		if (selected != i) {
			std::swap(v[i], v[selected]);
			heapify(v, size, selected);
		}
	}
	void build(std::vector<T>& v)
	{
		for (int i = static_cast<int>(v.size()) / 2 - 1; i >= 0; --i) {
			heapify(v, static_cast<int>(v.size()), i);
		}
	}

public:
	Heap(Compare comp) : comp(comp) {}

	void sort(std::vector<T>& v) {
		build(v);
		for (int end = static_cast<int>(v.size()) - 1; end > 0; --end) {
			std::swap(v[0], v[end]);
			heapify(v, end, 0);
		}
	}
};

template <typename T, typename Compare>
void heapSort(std::vector<T>& arr, Compare comp){
	Heap<T, Compare>(comp).sort(arr);
}


//*****************************************************************
// STD::SORT
//*****************************************************************
template <typename T, typename Compare>
void std_sort(std::vector<T>& v, Compare comp) {
	std::sort(v.begin(), v.end(), comp);
}


template <typename T, typename Compare>
bool isSorted(const std::vector<T>& v, Compare comp) {
	for (size_t i = 1; i < v.size(); ++i) {
		if (comp(v[i], v[i - 1])) {
			return false;
		}
	}
	return true;
}

 