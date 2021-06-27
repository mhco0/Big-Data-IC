#ifndef QSBD_HEAP_TRACKER
#define QSBD_HEAP_TRACKER 
#include "../qsbd_debug/qsbd_debug.h"
#include <new>
#include <cstdlib>
#include <iostream>
#include <vector>

#if __cplusplus >= 201703L
#  define ATTRIBUTE_NODISCARD [[nodiscard]]
#else
#  define ATTRIBUTE_NODISCARD
#endif

std::size_t bytes_allocated;

ATTRIBUTE_NODISCARD void* allocate(std::size_t n){
  if (n == 0) n = 1;
  bytes_allocated += n;
  if (void* result = ::malloc(n)) return result;
  bytes_allocated -= n;
  throw std::bad_alloc();
}

void deallocate(void* ptr, std::size_t = 0) { 
	::free(ptr); 
}

ATTRIBUTE_NODISCARD void* operator new   (std::size_t n) { return allocate(n); }
ATTRIBUTE_NODISCARD void* operator new[] (std::size_t n) { return allocate(n); }
void  operator delete  (void* ptr)                { deallocate(ptr);    }
void  operator delete[](void* ptr)                { deallocate(ptr);    }
void  operator delete  (void* ptr, std::size_t n) { deallocate(ptr, n); }
void  operator delete[](void* ptr, std::size_t n) { deallocate(ptr, n); }

template<typename T>
struct MyAllocator{
	typedef typename std::allocator<T>::value_type value_type;
	typedef typename std::allocator<T>::pointer pointer;
	typedef typename std::allocator<T>::const_pointer const_pointer;
	typedef typename std::allocator<T>::reference reference;
	typedef typename std::allocator<T>::const_reference const_reference;
	typedef typename std::allocator<T>::size_type size_type;

	pointer allocate (size_type n, typename std::allocator<void>::const_pointer hint = 0){
		std::cout << typeid(T).name() << std::endl;

		bytes_allocated += n * sizeof(T);
		return std::allocator<T>{}.allocate(n, hint);
	}

	void deallocate (pointer p, size_type n){
		std::cout << typeid(T).name() << std::endl;

		bytes_allocated -= n  * sizeof(T);
		return std::allocator<T>{}.deallocate(p, n);
	}
	template <class Type> struct rebind{
		typedef MyAllocator<Type> other;
	};

	MyAllocator(){
	}

	MyAllocator(const MyAllocator<T>& other ){
	}

	template<class U>
	MyAllocator( const MyAllocator<U>& other ){
	}
};

namespace std{
    template<typename T, typename Alloc = MyAllocator<T>>
    using tracked_vector = vector<T, Alloc>;
}

#define vector tracked_vector

#endif
