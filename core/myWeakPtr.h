#pragma once
#include"mySharedPtr.h"
//weak_ptr 设计的目的是为配合 shared_ptr 而引入的一种智能指针来协助 shared_ptr 工作, 它只可以从一个 shared_ptr 或另一个 weak_ptr 对象构造, 它的构造和析构不会引起引用记数的增加或减少。 同时weak_ptr 没有重载* 和->但可以使用 lock 获得一个可用的 shared_ptr 对象。
//弱指针的使用；
//weak_ptr wpGirl_1; // 定义空的弱指针
//weak_ptr wpGirl_2(spGirl); // 使用共享指针构造
//wpGirl_1 = spGirl; // 允许共享指针赋值给弱指针
//弱指针也可以获得引用计数
//wpGirl_1.use_count()
//弱指针不支持* 和->对指针的访问；
//允许移动构造，也允许拷贝构造



template<typename T>
class myWeakPtr {
private:
	T* originPtr;
	Counter* countPtr;
public:
	friend class mySharedPtr<T>;
	//构造函数
	explicit myWeakPtr() :originPtr(nullptr),countPtr(nullptr) {};
	//析构函数
	virtual ~myWeakPtr() {
		release();
	};
	//拷贝构造
	template<typename U>
	myWeakPtr(myWeakPtr<U>& ptr) :originPtr(ptr.originPtr), countPtr(ptr.countPtr) {
		countPtr->addWeakCount();
	};

	template<typename U>
	myWeakPtr(mySharedPtr<U>& ptr) :originPtr(ptr.originPtr), countPtr(ptr.countPtr) {
		countPtr->addWeakCount();
	};
	//赋值
	template<typename U>
	myWeakPtr<T>& operator=(myWeakPtr<U>& ptr) {
		if (this->originPtr != ptr.originPtr) {
			release();
			originPtr = ptr.originPtr;
			countPtr = ptr.countPtr;
			countPtr->addWeakCount();
		}
		return *this;
	}

	T* get() const {
		return originPtr;
	}
	//控制另一个对象 销毁之前保存的对象
	void reset(T* ptr = nullptr) {
		if (ptr != originPtr) {
			delete originPtr;
			originPtr = ptr;
		}
	}
	mySharedPtr<T> lock() {
		return mySharedPtr<T>(*this);
	}

	//返回引用计数
	int use_count() {
		return countPtr->getSharedCount();
	}
	bool unique() {
		return use_count() == 1;
	}
	bool expired() {
		return use_count() == 0;
	}
	void release() {
		if (countPtr) {
			countPtr->releaseWeakCount();
			if (countPtr->getWeakCount() < 1 && countPtr->getSharedCount() < 1) {
				delete countPtr;
				countPtr = nullptr;
			}
		}

	}
	

};