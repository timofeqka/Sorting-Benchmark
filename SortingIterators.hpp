#pragma once
#include <vector>
#include <algorithm>
#include <iterator>

//*****************************************************************
// BUBBLE
//*****************************************************************
template 
	<
		typename Iterator, 
		typename Compare = std::less<>
	>
void bubbleSort(
	Iterator begin, 
    Iterator end,
	Compare comp = {}
) {
	if (begin == end) {
        return;
    }

    for (Iterator end = end; end != begin; --end) {
        bool swapped = false;

        for (Iterator j = begin; std::next(j) != end; ++j) {
            if (comp(*(std::next(j)), *j)) {
                std::iter_swap(j, std::next(j));
                swapped = true;
            }
        }
    
		if (!swapped) {
			break;
		}
	}
}


//*****************************************************************
// INSERTION
//*****************************************************************
template 
	<
		typename Iterator, 
		typename Compare = std::less<>
	>
void insertionSort(
    Iterator begin, 
    Iterator end, 
    Compare comp = {}
) {
    if (begin == end) {
            return;
        }
    
	for (Iterator i = std::next(begin); i != end; ++i) {
		auto key = *i;
		Iterator j = i;

		while (j != begin && comp(key, std::prev(j))) {
			*j = *std::prev(j);
			--j;
		}
		*j = key;
	}
}


//*****************************************************************
// SELECTION
//*****************************************************************
template 
	<
		typename Iterator, 
		typename Compare = std::less<>
	>
void selectionSort(
    Iterator begin, 
    Iterator end, 
    Compare comp = {}
) {
	for (Iterator i = begin; i != end; ++i) {
		Iterator min = i;
		Iterator j   = std::next(i);

		while(j != end) {
			if (comp(*j, *min)) {
				min = j;
			}
			++j;
		}

		if (min != i) {
			std::iter_swap(i, min);
		}
	}
}


//*****************************************************************
// MERGE
//*****************************************************************
template 
	<
		typename Iterator, 
		typename Compare 
	>
void merge(
	Iterator begin,
    Iterator mid,
    Iterator end,
	std::vector<std::iter_value_t<Iterator>>& buffer,
	Compare comp = {}
) {
	Iterator left  = begin;
	Iterator right = mid;

	//TODO: 
	//iterator
    size_t index = 0;

	while (left != mid && right != end) {
		if (comp(*left, *right)) {
			buffer[index] = *left;
            ++left;
            ++index;
		}
		else {
			buffer[index] = *right;
            ++right;
            ++index;
		}
	}

	while (left != mid) {
		buffer[index] = *left;
        ++left;
        ++index;
	}

	while (right != end) {
		buffer[index] = *right;
        ++right;
        ++index;
	}

	//TODO:
	//iterator output = begin
	//output++
	for (auto& value : buffer) {
        *begin = std::move(value);
        ++begin;
    }
}
//requires iterator_category
template 
	<
		typename Iterator, 
		typename Compare 
	>
void mergeSortImpl(
	Iterator begin,
    Iterator end,
    std::iter_difference_t<Iterator> size,
	std::vector<std::iter_value_t<Iterator>>& buffer,
	Compare comp = {}
) {
    if (size <= 1) return; 

	const auto left_size  = size / 2;
	const auto right_size = size - left_size;  
	Iterator mid = std::next(begin, left_size);

	mergeSortImpl(begin, mid, comp);					
	mergeSortImpl(mid, end, comp);

	merge(begin, mid, end, buffer, comp);
}

template 
	<
		typename Iterator, 
		typename Compare = std::less<>
	>
void mergeSort(
    Iterator begin,
    Iterator end, 
    Compare comp = {}
) {
	using ValueType = std::iter_value_t<Iterator>;
	using DiffType  = std::iter_difference_t<Iterator>;

	DiffType size = std::distance(begin, end);

	if (size <= 1) return;

    std::vector<ValueType> buffer(static_cast<size_t>(size));

	mergeSortImpl(begin, end, size, &buffer, comp);
}
