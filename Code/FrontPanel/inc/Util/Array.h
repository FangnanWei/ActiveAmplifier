#ifndef Array_H__
#define Array_H__

extern "C" {
#include "stm8s.h"
}
#include <stdlib.h>

template<class T>
class Array {
public:
	// maxSize==0意味着不限制最大长度
	Array(int initSize, int maxSize) {
		this->num_ = initSize;
		this->maxSize_ = maxSize;
		entries_ = NULL;
		entries_ = num_==0 ? NULL : (T *)malloc(num_*sizeof(T));
		cnt_ = 0;
	}

	inline void Clear() {
		cnt_ = 0;
	}

	~Array() {
		free(entries_);
	}

	inline T Back() const { return entries_[cnt_-1]; }
	inline void PopBack() { --cnt_; }

	inline bool Inited() const { return entries_!=NULL; }
	T *Find(const T &newEntry) {
		for (int i=0; i<cnt_; ++i) {
			T *tmp = &entries_[i];
			if (*tmp == newEntry) {
				return tmp;
			}
		}
		return NULL;
	}

	// 返回值表示是否添加了一项
	T *Add(const T &ptr, int pos=-1) {
		if (cnt_ >= num_) {
			if (maxSize_>0 && num_ >= maxSize_)
				return NULL;
			int entryNum1 = num_ + 2;
			int entryNum2 = num_ * 1.5;

			if (entryNum2 < entryNum1)
				entryNum2 = entryNum1;
			if (maxSize_>0 && entryNum2 > maxSize_)
				entryNum2 = maxSize_;

			T *entries1 = (T *)realloc(entries_, entryNum2*sizeof(T));

			if (entries1==NULL)
				return NULL;
			entries_ = entries1;
			num_ = entryNum2;
		}

		if (pos<0 || pos>=cnt_) {
			pos = cnt_;
		}
		else {
			for (int i=cnt_; i>pos; --i) {
				entries_[i] = entries_[i-1];
			}
		}
		entries_[pos] = ptr;
		T *ret = &entries_[pos];
		++cnt_;
		return ret;
	}

	void Del(T *newEntry) {
		int id = newEntry - entries_;
		if (id<0 || id>=cnt_)
			return;
		cnt_--;
		for (int i=id; i<cnt_; ++i) {
			entries_[i] = entries_[i+1];
		}
	}

	inline int GetNum() const { return num_; }
	inline int GetCount() const { return cnt_; }
	inline T *Get(int index) const { return &entries_[index]; }

private:
	int num_;
	int cnt_;
	int maxSize_;
	T *entries_;
};

#endif