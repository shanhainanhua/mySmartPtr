#pragma once
//引用计数块

class Counter {
private:
	int weakPtrCount;
	int sharedPtrCount;
public:
	explicit Counter() :weakPtrCount(0), sharedPtrCount(0) {};
	virtual ~Counter(){};
	//管理计数
		//获得弱引用计数
	int getWeakCount() {
		return weakPtrCount;
	}
		//增加弱指针引用计数
	void addWeakCount() {
		++weakPtrCount;
	}
		//释放弱指针引用计数
	void releaseWeakCount() {
		--weakPtrCount;
	}
		//获得共享指针计数
	int getSharedCount() {
		return sharedPtrCount;
	}
		//增加共享指针计数
	void addSharedCount() {
		sharedPtrCount++;
	}
		//释放共享指针计数
	void releaseSharedCount() {
		--sharedPtrCount;
	}
};

template<typename T>
class myWeakPtr; // 前向声明

template<typename T>
class mySharedPtr {
private:
	T* originPtr;
	Counter* countPtr;
public:
	friend class myWeakPtr<T>;
	//构造函数
	mySharedPtr() :originPtr(nullptr) {
		countPtr = new Counter();
	};
	explicit mySharedPtr(T* ptr) :originPtr(ptr) {
		countPtr = new Counter();
		countPtr->addSharedCount();
	};
	//析构函数
	virtual ~mySharedPtr() {
		release();
	};
	//拷贝构造
	template<typename U>
	mySharedPtr(mySharedPtr<U>& ptr) :originPtr(ptr.originPtr), countPtr(ptr.countPtr){
		countPtr->addSharedCount();
	};
	//移动构造
	template<typename U>
	mySharedPtr(mySharedPtr<U>&& ptr) :originPtr(ptr.originPtr), countPtr(ptr.countPtr) {};
	//赋值
	template<typename U>
	mySharedPtr<T>& operator=(mySharedPtr<U>& ptr) {
		if (this->originPtr != ptr.originPtr) {
			release();
			originPtr = ptr.originPtr;
			countPtr = ptr.countPtr;
			countPtr->addSharedCount();
		}
	}
	//操作符
	T* operator->() const {
		return originPtr;
	}
	T& operator*() const {
		return *originPtr;
	}
	//返回原生指针
	T* get() const {
		return originPtr;
	}
	//控制另一个对象 销毁之前保存的对象
	void reset(T* ptr = nullptr) {
		if (ptr != originPtr) {
			release();
			originPtr = ptr;
			countPtr = new Counter();
			countPtr->addSharedCount();
		}
	}
	//返回引用计数
	int use_count() {
		return countPtr->getSharedCount();
	}
	bool unique() {
		return countPtr->getSharedCount() == 1;
	}
	void release() {
		countPtr->releaseSharedCount();
		if (countPtr->getSharedCount() < 1) {
			delete originPtr;
			if (countPtr->getWeakCount() < 1) {
				delete countPtr;
				countPtr = nullptr;
			}
		}
	}
};

