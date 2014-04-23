#ifndef gnat_util_scoped_ptr_H
#define gnat_util_scoped_ptr_H

#include "gnat.h"

namespace gnat
{
	/** A reference-counted scoped pointer implementation. */
	template <typename T> class scoped_ptr
	{
	public:

		scoped_ptr()
			:ptr_(0),ref_count_(0)
		{}
		//---------------------------------------------------------------------------

		explicit scoped_ptr(T* p)
			:ptr_(0),ref_count_(0) {
			bind(p);
		}
		//---------------------------------------------------------------------------

		~scoped_ptr() {
			release();
		}
		//---------------------------------------------------------------------------

		/** copy constructor (templated so polymorphism works) */
		scoped_ptr(const scoped_ptr<T>& rhs)
			: ptr_(0), ref_count_(0) {
			ptr_ = rhs.get();
			ref_count_ = rhs.GetRefCountPtr();
			if(ref_count_)
				++(*ref_count_);
		}
		//---------------------------------------------------------------------------

		/** copy constructor (templated so polymorphism works) */
		template <typename Y> scoped_ptr(const scoped_ptr<Y>& rhs)
			:ptr_(0),ref_count_(0) {
			ptr_ = rhs.get();
			ref_count_ = rhs.GetRefCountPtr();
			if(ref_count_)
				++(*ref_count_);
		}
		//---------------------------------------------------------------------------

		/** Plain ol' assignment op */
		scoped_ptr& operator=(const scoped_ptr& rhs) {
			if(ptr_ == rhs.ptr_)
				return *this;
			scoped_ptr<T> temp(rhs);
			swap(temp);
			return *this;
		}
		//---------------------------------------------------------------------------

		/** Templated assignment op, so it can store derived types */
		template <typename Y> scoped_ptr& operator=(const scoped_ptr<Y>& rhs) {
			if(ptr_ == rhs.get())
				return *this;
			scoped_ptr<T> temp(rhs);
			swap(temp);
			return *this;
		}
		//---------------------------------------------------------------------------

		void bind(T* p) {
			ptr_ = p;
			ref_count_ = new unsigned int(1);
		}
		//---------------------------------------------------------------------------

		void release() {
			if(!ptr_)
				return;
			--(*ref_count_);
			if((*ref_count_) == 0)
			{
				delete ptr_;
				delete ref_count_;
				ptr_ = 0;
				ref_count_ = 0;
			}
		}
		//---------------------------------------------------------------------------

		inline T* get() const {
			return ptr_;
		}
		//---------------------------------------------------------------------------

		inline unsigned int* GetRefCountPtr() const {
			return ref_count_;
		}
		//---------------------------------------------------------------------------

		inline unsigned int GetRefCount() const {
			return *ref_count_;
		}
		//---------------------------------------------------------------------------
		
		inline bool IsNull() {
			return !ptr_;
		}
		//---------------------------------------------------------------------------

		inline void SetNull() {
			release();
			ptr_ = 0;
			ref_count_ = 0;
		}
		//---------------------------------------------------------------------------

		inline T& operator*() const { return *ptr_; }
		inline T* operator->() const { return ptr_; }

	protected:

		void swap(scoped_ptr<T>& other)
		{
			std::swap(ptr_, other.ptr_);
			std::swap(ref_count_, other.ref_count_);
		}
		//---------------------------------------------------------------------------

		unsigned int* ref_count_;
		T* ptr_;
		
	};
}

#endif  // gnat_util_scoped_ptr_H
