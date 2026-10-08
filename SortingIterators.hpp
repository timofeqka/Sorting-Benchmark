#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>

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

	mergeSortImpl(begin, mid, left_size, buffer, comp);					
	mergeSortImpl(mid, end, right_size, buffer, comp);

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


//*****************************************************************
// QUICK
//*****************************************************************
// template 
// 	<
// 		typename Iterator,
// 		typename Compare
// 	>
// Iterator medianOfThree(
// 	Iterator first,
// 	Iterator mid,
// 	Iterator last,
// 	Compare comp
// ) {
// 	if (
// 		(!comp(*mid, *first) && !comp(*last, *mid))
// 		||
// 		(!comp(*mid, *last) && !comp(*first, *mid))
// 	) return mid;

// 	if (
// 		(!comp(*first, *mid) && !comp(*last, *first))
// 		||
// 		(!comp(*first, *last) && !comp(*mid, *first))
// 	) return first;

// 	return last;
// }

class DefaultRandom {
private:
	std::mt19937 generator;

public:
  	DefaultRandom() : generator(std::random_device{}()) {}

	template <typename Int>
	Int operator()(Int min, Int max) {
		std::uniform_int_distribution<Int> dist(min, max);
		return dist(generator);
	}
};

template 
	<
	typename Iterator,
	typename Random
	>
Iterator randomPivot(
	Iterator begin,
	std::iter_difference_t<Iterator> size,
	Random& rnd
) {
	std::iter_difference_t<Iterator> dist = rnd(0, size - 1);

	Iterator random = begin;
	std::advance(random, dist);
	return random;
}

template 
  <
    typename Iterator, 
    typename Compare
  >
void quickSortImpl(
  Iterator begin, 
  Iterator end,
  Compare comp = {}
) {
  using DiffType  = std::iter_difference_t<Iterator>;
  const DiffType size = std::distance(begin, end);

  if (size <= 1) return;

  Iterator mid = begin;
  std::advance(mid, size/2);

  Iterator pivot = mid;

    Iterator left = begin;
    Iterator right = end - 1;
  
  
  while (left <= right) {
    while (comp(*left, *pivot)) {
      ++left;
    }
    while (comp(*pivot, *right)) {
      --right;
    }

    if (left <= right) {
      std::iter_swap(left, right);
      ++left;
      --right;
    }
  }

  quickSortImpl(begin, right + 1, comp);
    quickSortImpl(left, end, comp);
}

template 
  <
    typename Iterator, 
    typename Compare = std::less<>
  >
void quickSort(
  Iterator begin, 
  Iterator end, 
  Compare comp = {}
) {
  quickSortImpl(begin, end, comp);
}