// Copyright (C) 2004, 2011 International Business Machines and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id: IpSmartPtr.hpp 2005 2011-06-06 12:55:16Z stefan $
//
// Authors:  Carl Laird, Andreas Waechter     IBM    2004-08-13

// copied from IpReferenced.hpp so as to allow OS to be compiled
// stand-alone (without the Fortran implications inherent in Ipopt)

#ifndef OSSMARTPTR_HPP
#define OSSMARTPTR_HPP

#include "OSReferenced.hpp"

  /** Template class for Smart Pointers.
   * An OSSmartPtr behaves much like a raw pointer, but manages the lifetime 
   * of an object, deleting the object automatically. This class implements
   * a reference-counting, intrusive smart pointer design, where all
   * objects pointed to must inherit from ReferencedObject, which
   * stores the reference count. Although this is intrusive (native types
   * and externally authored classes require wrappers to be referenced
   * by smart pointers), it is a safer design. A more detailed discussion of
   * these issues follows after the usage information.
   * 
   * Usage Example:
   * Note: to use the OSSmartPtr, all objects to which you point MUST
   * inherit from ReferencedObject.
   * 
   * \verbatim
   * 
   * In MyClass.hpp...
   * 
   * #include "OSReferenced.hpp"

   *  class MyClass : public ReferencedObject // must derive from ReferencedObject
   *    {
   *      ...
   *    }
   * 
   * In my_usage.cpp...
   * 
   * #include "OSSmartPtr.hpp"
   * #include "MyClass.hpp"
   * 
   * void func(AnyObject& obj)
   *  {
   *    OSSmartPtr<MyClass> ptr_to_myclass = new MyClass(...);
   *    // ptr_to_myclass now points to a new MyClass,
   *    // and the reference count is 1
   *  
   *    ...
   * 
   *    obj.SetMyClass(ptr_to_myclass);
   *    // Here, let's assume that AnyObject uses a
   *    // OSSmartPtr<MyClass> internally here.
   *    // Now, both ptr_to_myclass and the internal
   *    // OSSmartPtr in obj point to the same MyClass object
   *    // and its reference count is 2.
   * 
   *    ...
   * 
   *    // No need to delete ptr_to_myclass, this
   *    // will be done automatically when the
   *    // reference count drops to zero.
   * 
   *  }  
   *  
   * \endverbatim
   *
   * It is not necessary to use OSSmartPtr's in all cases where an
   * object is used that has been allocated "into" a OSSmartPtr.  It is
   * possible to just pass objects by reference or regular pointers,
   * even if lower down in the stack an OSSmartPtr is to be held on to.
   * Everything should work fine as long as a pointer created by "new"
   * is immediately passed into an OSSmartPtr, and if OSSmartPtr's are used
   * to hold on to objects.
   *
   * Other Notes:
   *  The OSSmartPtr implements both dereference operators -> & *.
   *  The OSSmartPtr does NOT implement a conversion operator to
   *    the raw pointer. Use the GetRawPtr() method when this
   *    is necessary. Make sure that the raw pointer is NOT
   *    deleted. 
   *  The OSSmartPtr implements the comparison operators == & !=
   *    for a variety of types. Use these instead of
   *    \verbatim
   *    if (GetRawPtr(smrt_ptr) == ptr) // Don't use this
   *    \endverbatim
   * OSSmartPtr's, as currently implemented, do NOT handle circular references.
   *    For example: consider a higher level object using OSSmartPtrs to point to 
   *    A and B, but A and B also point to each other (i.e. A has an OSSmartPtr 
   *    to B and B has an OSSmartPtr to A). In this scenario, when the higher
   *    level object is finished with A and B, their reference counts will 
   *    never drop to zero (since they reference each other) and they
   *    will not be deleted. This can be detected by memory leak tools like
   *    valgrind. If the circular reference is necessary, the problem can be
   *    overcome by a number of techniques:
   *  
   *    1) A and B can have a method that "releases" each other, that is
   *        they set their internal OSSmartPtrs to NULL.
   *        \verbatim
   *        void AClass::ReleaseCircularReferences()
   *          {
   *          smart_ptr_to_B = NULL;
   *          }
   *        \endverbatim
   *        Then, the higher level class can call these methods before
   *        it is done using A & B.
   * 
   *    2) Raw pointers can be used in A and B to reference each other.
   *        Here, an implicit assumption is made that the lifetime is
   *        controlled by the higher level object and that A and B will
   *        both exist in a controlled manner. Although this seems 
   *        dangerous, in many situations, this type of referencing
   *        is very controlled and this is reasonably safe.
   * 
   *    3) This OSSmartPtr class could be redesigned with the Weak/Strong
   *        design concept. Here, the OSSmartPtr is identified as being
   *        Strong (controls lifetime of the object) or Weak (merely
   *        referencing the object). The Strong OSSmartPtr increments 
   *        (and decrements) the reference count in ReferencedObject
   *        but the Weak OSSmartPtr does not. In the example above,
   *        the higher level object would have Strong OSSmartPtrs to
   *        A and B, but A and B would have Weak OSSmartPtrs to each
   *        other. Then, when the higher level object was done with
   *        A and B, they would be deleted. The Weak OSSmartPtrs in A
   *        and B would not decrement the reference count and would,
   *        of course, not delete the object. This idea is very similar
   *        to item (2), where it is implied that the sequence of events 
   *        is controlled such that A and B will not call anything using
   *        their pointers following the higher level delete (i.e. in
   *        their destructors!). This is somehow safer, however, because
   *        code can be written (however expensive) to perform run-time 
   *        detection of this situation. For example, the ReferencedObject
   *        could store pointers to all Weak OSSmartPtrs that are referencing
   *        it and, in its destructor, tell these pointers that it is
   *        dying. They could then set themselves to NULL, or set an
   *        internal flag to detect usage past this point.
   * 
   * Comments on Non-Intrusive Design:
   * In a non-intrusive design, the reference count is stored somewhere other
   * than the object being referenced. This means, unless the reference
   * counting pointer is the first referencer, it must get a pointer to the 
   * referenced object from another smart pointer (so it has access to the 
   * reference count location). In this non-intrusive design, if we are 
   * pointing to an object with a smart pointer (or a number of smart
   * pointers), and we then give another smart pointer the address through
   * a RAW pointer, we will have two independent, AND INCORRECT, reference
   * counts. To avoid this pitfall, we use an intrusive reference counting
   * technique where the reference count is stored in the object being
   * referenced. 
   */
  template<class T>
  class OSSmartPtr : public OSReferencer
  {
  public:

    /**@name Constructors/Destructors */
    //@{
    /** Default constructor, initialized to NULL */
    OSSmartPtr();

    /** Copy constructor, initialized from copy */
    OSSmartPtr(const OSSmartPtr<T>& copy);

    /** Constructor, initialized from T* ptr */
    OSSmartPtr(T* ptr);

    /** Destructor, automatically decrements the
     * reference count, deletes the object if
     * necessary.*/
    ~OSSmartPtr();
    //@}

    /**@name Overloaded operators. */
    //@{
    /** Overloaded arrow operator, allows the user to call
     * methods using the contained pointer. */
    T* operator->() const;

    /** Overloaded dereference operator, allows the user
     * to dereference the contained pointer. */
    T& operator*() const;

    /** Overloaded equals operator, allows the user to
     * set the value of the OSSmartPtr from a raw pointer */
    OSSmartPtr<T>& operator=(T* rhs);

    /** Overloaded equals operator, allows the user to
     * set the value of the OSSmartPtr from another 
     * OSSmartPtr */
    OSSmartPtr<T>& operator=(const OSSmartPtr<T>& rhs);

    /** Overloaded equality comparison operator, allows the
     * user to compare the value of two OSSmartPtrs */
    template <class U1, class U2>
    friend
    bool operator==(const OSSmartPtr<U1>& lhs, const OSSmartPtr<U2>& rhs);

    /** Overloaded equality comparison operator, allows the
     * user to compare the value of an OSSmartPtr with a raw pointer. */
    template <class U1, class U2>
    friend
    bool operator==(const OSSmartPtr<U1>& lhs, U2* raw_rhs);

    /** Overloaded equality comparison operator, allows the
     * user to compare the value of a raw pointer with an OSSmartPtr. */
    template <class U1, class U2>
    friend
    bool operator==(U1* lhs, const OSSmartPtr<U2>& raw_rhs);

    /** Overloaded in-equality comparison operator, allows the
     * user to compare the value of two OSSmartPtrs */
    template <class U1, class U2>
    friend
    bool operator!=(const OSSmartPtr<U1>& lhs, const OSSmartPtr<U2>& rhs);

    /** Overloaded in-equality comparison operator, allows the
     * user to compare the value of an OSSmartPtr with a raw pointer. */
    template <class U1, class U2>
    friend
    bool operator!=(const OSSmartPtr<U1>& lhs, U2* raw_rhs);

    /** Overloaded in-equality comparison operator, allows the
     * user to compare the value of an OSSmartPtr with a raw pointer. */
    template <class U1, class U2>
    friend
    bool operator!=(U1* lhs, const OSSmartPtr<U2>& raw_rhs);
    //@}

    /**@name friend method declarations. */
    //@{
    /** Returns the raw pointer contained.
     * Use to get the value of
     * the raw ptr (i.e. to pass to other
     * methods/functions, etc.)
     * Note: This method does NOT copy, 
     * therefore, modifications using this
     * value modify the underlying object 
     * contained by the OSSmartPtr,
     * NEVER delete this returned value.
     */
    template <class U>
    friend
    U* GetRawPtr(const OSSmartPtr<U>& smart_ptr);

    /** Returns a const pointer */
    template <class U>
    friend
    OSSmartPtr<const U> ConstPtr(const OSSmartPtr<U>& smart_ptr);

    /** Returns true if the OSSmartPtr is NOT NULL.
     * Use this to check if the OSSmartPtr is not null
     * This is preferred to if(GetRawPtr(sp) != NULL)
     */
    template <class U>
    friend
    bool IsValid(const OSSmartPtr<U>& smart_ptr);

    /** Returns true if the OSSmartPtr is NULL.
     * Use this to check if the OSSmartPtr IsNull.
     * This is preferred to if(GetRawPtr(sp) == NULL)
     */
    template <class U>
    friend
    bool IsNull(const OSSmartPtr<U>& smart_ptr);
    //@}

  private:
    /**@name Private Data/Methods */
    //@{
    /** Actual raw pointer to the object. */
    T* ptr_;

    /** Set the value of the internal raw pointer
     * from another raw pointer, releasing the 
     * previously referenced object if necessary. */
    OSSmartPtr<T>& SetFromRawPtr_(T* rhs);

    /** Set the value of the internal raw pointer
     * from an OSSmartPtr, releasing the previously referenced
     * object if necessary. */
    OSSmartPtr<T>& SetFromSmartPtr_(const OSSmartPtr<T>& rhs);

    /** Release the currently referenced object. */
    void ReleasePointer_();
    //@}
  };

  /**@name OSSmartPtr friend function declarations.*/
  //@{
  template <class U>
  U* GetRawPtr(const OSSmartPtr<U>& smart_ptr);

  template <class U>
  OSSmartPtr<const U> ConstPtr(const OSSmartPtr<U>& smart_ptr);

  template <class U>
  bool IsNull(const OSSmartPtr<U>& smart_ptr);

  template <class U>
  bool IsValid(const OSSmartPtr<U>& smart_ptr);

  template <class U1, class U2>
  bool operator==(const OSSmartPtr<U1>& lhs, const OSSmartPtr<U2>& rhs);

  template <class U1, class U2>
  bool operator==(const OSSmartPtr<U1>& lhs, U2* raw_rhs);

  template <class U1, class U2>
  bool operator==(U1* lhs, const OSSmartPtr<U2>& raw_rhs);

  template <class U1, class U2>
  bool operator!=(const OSSmartPtr<U1>& lhs, const OSSmartPtr<U2>& rhs);

  template <class U1, class U2>
  bool operator!=(const OSSmartPtr<U1>& lhs, U2* raw_rhs);

  template <class U1, class U2>
  bool operator!=(U1* lhs, const OSSmartPtr<U2>& raw_rhs);

  //@}


  template <class T>
  OSSmartPtr<T>::OSSmartPtr()
      :
      ptr_(0)
  {

#ifdef CHECK_SMARTPTR

    const OSReferencedObject* trying_to_use_SmartPtr_with_an_object_that_does_not_inherit_from_ReferencedObject_
    = ptr_;
    trying_to_use_SmartPtr_with_an_object_that_does_not_inherit_from_ReferencedObject_ = 0;
#endif

  }


  template <class T>
  OSSmartPtr<T>::OSSmartPtr(const OSSmartPtr<T>& copy)
      :
      ptr_(0)
  {

#ifdef CHECK_SMARTPTR

    const ReferencedObject* trying_to_use_SmartPtr_with_an_object_that_does_not_inherit_from_ReferencedObject_
    = ptr_;
    trying_to_use_SmartPtr_with_an_object_that_does_not_inherit_from_ReferencedObject_ = 0;
#endif

    (void) SetFromSmartPtr_(copy);
  }


  template <class T>
  OSSmartPtr<T>::OSSmartPtr(T* ptr)
      :
      ptr_(0)
  {
#ifdef CHECK_SMARTPTR

    const ReferencedObject* trying_to_use_SmartPtr_with_an_object_that_does_not_inherit_from_ReferencedObject_
    = ptr_;
    trying_to_use_SmartPtr_with_an_object_that_does_not_inherit_from_ReferencedObject_ = 0;
#endif

    (void) SetFromRawPtr_(ptr);
  }

  template <class T>
  OSSmartPtr<T>::~OSSmartPtr()
  {
    ReleasePointer_();
  }

  template <class T>
  T* OSSmartPtr<T>::operator->() const
  {
    return ptr_;
  }


  template <class T>
  T& OSSmartPtr<T>::operator*() const
  {
    return *ptr_;
  }


  template <class T>
  OSSmartPtr<T>& OSSmartPtr<T>::operator=(T* rhs)
  {
    return SetFromRawPtr_(rhs);
  }


  template <class T>
  OSSmartPtr<T>& OSSmartPtr<T>::operator=(const OSSmartPtr<T>& rhs)
  {
    return SetFromSmartPtr_(rhs);
  }


  template <class T>
  OSSmartPtr<T>& OSSmartPtr<T>::SetFromRawPtr_(T* rhs)
  {
    // Release any old pointer
    ReleasePointer_();

    if (rhs != 0) {
      rhs->AddRef(this);
      ptr_ = rhs;
    }

    return *this;
  }

  template <class T>
  OSSmartPtr<T>& OSSmartPtr<T>::SetFromSmartPtr_(const OSSmartPtr<T>& rhs)
  {
    T* ptr = GetRawPtr(rhs);
    /* AW: I changed this so that NULL is correctly copied from the
       right hand side */
    //     if (ptr != NULL) {
    //       SetFromRawPtr_(ptr);
    //     }
    SetFromRawPtr_(ptr);

    return (*this);
  }


  template <class T>
  void OSSmartPtr<T>::ReleasePointer_()
  {
    if (ptr_) {
      ptr_->ReleaseRef(this);
      if (ptr_->ReferenceCount() == 0) {
        delete ptr_;
      }
      ptr_ = 0;
    }
  }


  template <class U>
  U* GetRawPtr(const OSSmartPtr<U>& smart_ptr)
  {
    return smart_ptr.ptr_;
  }

  template <class U>
  OSSmartPtr<const U> ConstPtr(const OSSmartPtr<U>& smart_ptr)
  {
    // compiler should implicitly cast
    return GetRawPtr(smart_ptr);
  }

  template <class U>
  bool IsValid(const OSSmartPtr<U>& smart_ptr)
  {
    return !IsNull(smart_ptr);
  }

  template <class U>
  bool IsNull(const OSSmartPtr<U>& smart_ptr)
  {
    return (smart_ptr.ptr_ == 0);
  }


  template <class U1, class U2>
  bool ComparePointers(const U1* lhs, const U2* rhs)
  {
    if (lhs == rhs) {
      return true;
    }

    // Even if lhs and rhs point to the same object
    // with different interfaces U1 and U2, we cannot guarantee that
    // the value of the pointers will be equivalent. We can
    // guarantee this if we convert to void*
    const void* v_lhs = static_cast<const void*>(lhs);
    const void* v_rhs = static_cast<const void*>(rhs);
    if (v_lhs == v_rhs) {
      return true;
    }

    // They must not be the same
    return false;
  }

  template <class U1, class U2>
  bool operator==(const OSSmartPtr<U1>& lhs, const OSSmartPtr<U2>& rhs)
  {
    U1* raw_lhs = GetRawPtr(lhs);
    U2* raw_rhs = GetRawPtr(rhs);
    return ComparePointers(raw_lhs, raw_rhs);
  }

  template <class U1, class U2>
  bool operator==(const OSSmartPtr<U1>& lhs, U2* raw_rhs)
  {
    U1* raw_lhs = GetRawPtr(lhs);
    return ComparePointers(raw_lhs, raw_rhs);
  }

  template <class U1, class U2>
  bool operator==(U1* raw_lhs, const OSSmartPtr<U2>& rhs)
  {
    const U2* raw_rhs = GetRawPtr(rhs);
    return ComparePointers(raw_lhs, raw_rhs);
  }

  template <class U1, class U2>
  bool operator!=(const OSSmartPtr<U1>& lhs, const OSSmartPtr<U2>& rhs)
  {
    bool retValue = operator==(lhs, rhs);
    return !retValue;
  }

  template <class U1, class U2>
  bool operator!=(const OSSmartPtr<U1>& lhs, U2* raw_rhs)
  {
    bool retValue = operator==(lhs, raw_rhs);
    return !retValue;
  }

  template <class U1, class U2>
  bool operator!=(U1* raw_lhs, const OSSmartPtr<U2>& rhs)
  {
    bool retValue = operator==(raw_lhs, rhs);
    return !retValue;
  }

#endif

