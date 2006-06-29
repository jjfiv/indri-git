/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.29
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace Indri {

using System;
using System.Runtime.InteropServices;

public class QueryAnnotationNodeVector : IDisposable, System.Collections.IEnumerable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal QueryAnnotationNodeVector(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(QueryAnnotationNodeVector obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~QueryAnnotationNodeVector() {
    Dispose();
  }

  public virtual void Dispose() {
    if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
      swigCMemOwn = false;
      indriPINVOKE.delete_QueryAnnotationNodeVector(swigCPtr);
    }
    swigCPtr = new HandleRef(null, IntPtr.Zero);
    GC.SuppressFinalize(this);
  }

  public QueryAnnotationNodeVector(System.Collections.ICollection c) : this() {
    if (c == null)
      throw new ArgumentNullException("c");
    foreach (QueryAnnotationNode element in c) {
      this.Add(element);
    }
  }

  public bool IsFixedSize {
    get {
      return false;
    }
  }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public QueryAnnotationNode this[int index]  {
    get {
      return getitem(index);
    }
    set {
      setitem(index, value);
    }
  }

  public int Capacity {
    get {
      return (int)capacity();
    }
    set {
      if (value < size())
        throw new ArgumentOutOfRangeException("Capacity");
      reserve((uint)value);
    }
  }

  public int Count {
    get {
      return (int)size();
    }
  }

  public bool IsSynchronized {
    get {
      return false;
    }
  }

  public void CopyTo(System.Array array) {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(System.Array array, int arrayIndex) {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, System.Array array, int arrayIndex, int count) {
    if (array == null)
      throw new ArgumentNullException("array");
    if (index < 0)
      throw new ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new ArgumentException("Multi dimensional array.");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

  // Type-safe version of IEnumerable.GetEnumerator
  System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
    return new QueryAnnotationNodeVectorEnumerator(this);
  }

  public QueryAnnotationNodeVectorEnumerator GetEnumerator() {
    return new QueryAnnotationNodeVectorEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class QueryAnnotationNodeVectorEnumerator : System.Collections.IEnumerator {
    private QueryAnnotationNodeVector collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public QueryAnnotationNodeVectorEnumerator(QueryAnnotationNodeVector collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public QueryAnnotationNode Current {
      get {
        if (currentIndex == -1)
          throw new InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new InvalidOperationException("Collection modified.");
        return (QueryAnnotationNode)currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    public bool MoveNext() {
      int size = collectionRef.Count;
      bool moveOkay = (currentIndex+1 < size) && (size == currentSize);
      if (moveOkay) {
        currentIndex++;
        currentObject = collectionRef[currentIndex];
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new InvalidOperationException("Collection modified.");
      }
    }
  }

  public void Clear() {
    indriPINVOKE.QueryAnnotationNodeVector_Clear(swigCPtr);
  }

  public void Add(QueryAnnotationNode value) {
    indriPINVOKE.QueryAnnotationNodeVector_Add(swigCPtr, QueryAnnotationNode.getCPtr(value));
  }

  private uint size() {
    uint ret = indriPINVOKE.QueryAnnotationNodeVector_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = indriPINVOKE.QueryAnnotationNodeVector_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    indriPINVOKE.QueryAnnotationNodeVector_reserve(swigCPtr, n);
  }

  public QueryAnnotationNodeVector() : this(indriPINVOKE.new_QueryAnnotationNodeVector__SWIG_0(), true) {
  }

  public QueryAnnotationNodeVector(int capacity) : this(indriPINVOKE.new_QueryAnnotationNodeVector__SWIG_1(capacity), true) {
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  private QueryAnnotationNode getitemcopy(int index) {
    IntPtr cPtr = indriPINVOKE.QueryAnnotationNodeVector_getitemcopy(swigCPtr, index);
    QueryAnnotationNode ret = (cPtr == IntPtr.Zero) ? null : new QueryAnnotationNode(cPtr, false);
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private QueryAnnotationNode getitem(int index) {
    IntPtr cPtr = indriPINVOKE.QueryAnnotationNodeVector_getitem(swigCPtr, index);
    QueryAnnotationNode ret = (cPtr == IntPtr.Zero) ? null : new QueryAnnotationNode(cPtr, false);
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, QueryAnnotationNode value) {
    indriPINVOKE.QueryAnnotationNodeVector_setitem(swigCPtr, index, QueryAnnotationNode.getCPtr(value));
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(QueryAnnotationNodeVector values) {
    indriPINVOKE.QueryAnnotationNodeVector_AddRange(swigCPtr, QueryAnnotationNodeVector.getCPtr(values));
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  public QueryAnnotationNodeVector GetRange(int index, int count) {
    IntPtr cPtr = indriPINVOKE.QueryAnnotationNodeVector_GetRange(swigCPtr, index, count);
    QueryAnnotationNodeVector ret = (cPtr == IntPtr.Zero) ? null : new QueryAnnotationNodeVector(cPtr, true);
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, QueryAnnotationNode value) {
    indriPINVOKE.QueryAnnotationNodeVector_Insert(swigCPtr, index, QueryAnnotationNode.getCPtr(value));
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, QueryAnnotationNodeVector values) {
    indriPINVOKE.QueryAnnotationNodeVector_InsertRange(swigCPtr, index, QueryAnnotationNodeVector.getCPtr(values));
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    indriPINVOKE.QueryAnnotationNodeVector_RemoveAt(swigCPtr, index);
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    indriPINVOKE.QueryAnnotationNodeVector_RemoveRange(swigCPtr, index, count);
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  public static QueryAnnotationNodeVector Repeat(QueryAnnotationNode value, int count) {
    IntPtr cPtr = indriPINVOKE.QueryAnnotationNodeVector_Repeat(QueryAnnotationNode.getCPtr(value), count);
    QueryAnnotationNodeVector ret = (cPtr == IntPtr.Zero) ? null : new QueryAnnotationNodeVector(cPtr, true);
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    indriPINVOKE.QueryAnnotationNodeVector_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    indriPINVOKE.QueryAnnotationNodeVector_Reverse__SWIG_1(swigCPtr, index, count);
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, QueryAnnotationNodeVector values) {
    indriPINVOKE.QueryAnnotationNodeVector_SetRange(swigCPtr, index, QueryAnnotationNodeVector.getCPtr(values));
    if (indriPINVOKE.SWIGPendingException.Pending) throw indriPINVOKE.SWIGPendingException.Retrieve();
  }

}

}