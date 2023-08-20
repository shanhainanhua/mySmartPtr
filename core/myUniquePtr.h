#pragma once
//unique_ptr特性
//基于排他所有权模式：两个指针不能指向同一个资源
//无法进行左值unique_ptr复制构造，也无法进行左值复制赋值操作，但允许临时右值赋值构造和赋值
//保存指向某个对象的指针，当它本身离开作用域时会自动释放它指向的对象。
//在容器中保存指针是安全的
template<typename T>
class myUniquePtr {
private:
	T* originPtr; //原生指针
public:
	//构造函数
	myUniquePtr():originPtr(nullptr) {};
	explicit myUniquePtr(T* oriPtr) :originPtr(oriPtr) {};
	//析构函数
	virtual ~myUniquePtr() {
		delete originPtr;
	}
	//拷贝构造
		//禁用左值拷贝构造
	template<typename U>
	myUniquePtr(myUniquePtr<U>&) = delete;
		//可以进行右值拷贝构造
	template<typename U>
	myUniquePtr(myUniquePtr<U>&& ptr):originPtr(ptr.release()){}

	//赋值
		//禁用左值赋值
	template<typename U>
	myUniquePtr<T>& operator=(myUniquePtr<U>& ptr) = delete;
		//可以进行右值赋值
	template<typename U>
	myUniquePtr<T>& operator=(myUniquePtr<U>&& ptr) {
		if (this->originPtr != ptr.originPtr) {
			reset(ptr.release());
		}
		return *this;
	};

	//重载运算符
		//->
	T* operator->() const {
		return originPtr;
	}
		//*
	T& operator*() const {
		return *originPtr;
	}
	//智能指针管理
		//返回原始指针
	T* get() const {
		return originPtr;
	}
		//放弃智能指针管理，不销毁对象
	T* release() {
		T* old_ptr = originPtr;
		originPtr = nullptr;
		return old_ptr;
	}
		//控制另一个对象,销毁之前保存的对象
	void reset(T* ptr = nullptr) {
		if (ptr != nullptr&&ptr!=originPtr) {
			delete originPtr;
			originPtr = ptr;
		}
	}

};