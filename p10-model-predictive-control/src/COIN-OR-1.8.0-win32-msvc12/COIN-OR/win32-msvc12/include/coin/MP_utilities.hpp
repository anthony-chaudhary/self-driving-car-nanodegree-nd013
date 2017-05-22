// ******************** FlopCpp **********************************************
// File: MP_utilities.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************

#ifndef _MP_utilities_hpp_
#define _MP_utilities_hpp_

#include <string>
#include <vector>

namespace flopc {

    /** @file This file contains several different useful utilities which are
        used to simplify use of FlopC++.
        @ingroup PublicInterface
     */

    /** @brief Function object.  Often used
        @ingroup INTERNAL_USE
        @note is the base class for passing a fucntion object around.
    */
    class Functor {
    public:
	virtual void operator()() const = 0;
    protected:
	Functor() {}
	virtual ~Functor() {}
    private:
	Functor(const Functor&);
	Functor& operator=(const Functor&);
    };	

    /** This template makes a vector of appropriate size out of the
        variable number of arguments.
        @ingroup INTERNAL_USE
     */
    template<int nbr, class T>
    std::vector<T> makeVector(T i1, T i2=0, T i3=0, T i4=0, T i5=0) {
	std::vector<T> S(nbr);
	S[0] = i1; 
	if (nbr==1) return S;
	S[1] = i2;
	if (nbr==2) return S;
	S[2] = i3;
	if (nbr==3) return S;
	S[3] = i4;
	if (nbr==4) return S;
	S[4] = i5; 
	return S;
    }

    /// return the strictly positive modulus of two integers
    inline int mod(int a, int b) {
	int t = a % b;
	return  (t>=0) ? t : t+b;
    }

    /// Distinct return value on conditions where an index goes out of bounds.
    const int outOfBound = -2;    

    /** Utility class to flatten multidimensional information into single
        dimentional offset information.
        @ingroup INTERNAL_USE
    */
    class RowMajor {
    public:
	int size() const { return size_; }
    protected:
	RowMajor(int s1, int s2, int s3, int s4, int s5) :
	    size1(s1), size2(s2), size3(s3), size4(s4), size5(s5),
	    size_(s1*s2*s3*s4*s5) {}
	int f(int i1=0, int i2=0, int i3=0, int i4=0, int i5=0) const {
	    if ( i1==outOfBound || i2==outOfBound || i3==outOfBound ||
		 i4==outOfBound || i5==outOfBound ) {
		return outOfBound;
	    } else {
		int i = i1;
		i *= size2; i += i2;  i *= size3; i += i3;
		i *= size4; i += i4;  i *= size5; i += i5;
		return i;
	    } 
	}
	int size1,size2,size3,size4,size5,size_;
    };

    /** @brief Utility interface class for adding a string name onto a
        structure.
        @ingroup INTERNAL_USE
     */
    class Named {
    public:
	std::string getName() const { return name; }
	void setName(const std::string& n) { name = n; }
    private:
	std::string name;
    };

    /** @brief Utility for doing reference counted pointers.
        @ingroup INTERNAL_USE
     */
    template<class T> class Handle {
    public:
	const T &operator->() const {
	    return root;
	}
	Handle(const T &r) : root(r) {
	    increment();
	}
	Handle(const Handle& h) : root(h.root) {
	    increment();
	}
	const Handle& operator=(const Handle& h) {
	    if (root != h.root) {
		decrement();
		root = h.root;
		increment();
	    }
	    return *this;
	}
	~Handle() {
	    decrement();
	}
    protected:
	void increment() {
	    if(root != 0) {
		(root->count)++;
	    }
	}
	void decrement() {
	    if(root != 0) {
		if(root->count == 1) {
		    delete root;
		    root = 0;
		} else {
		    ///if(root->count != 0) {
		    --(root->count);
		    ///}
		}
	    }
	}
    private:
	Handle() : root(0) {}
	T root;
    };

} // End of namespace flopc
#endif
