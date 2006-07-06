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

public class TagExtent : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal TagExtent(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(TagExtent obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~TagExtent() {
    Dispose();
  }

  public virtual void Dispose() {
    if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
      swigCMemOwn = false;
      indri_csharpPINVOKE.delete_TagExtent(swigCPtr);
    }
    swigCPtr = new HandleRef(null, IntPtr.Zero);
    GC.SuppressFinalize(this);
  }

  public string name {
    set {
      indri_csharpPINVOKE.TagExtent_name_set(swigCPtr, value);
    } 
    get {
      string ret = indri_csharpPINVOKE.TagExtent_name_get(swigCPtr);
      return ret;
    } 
  }

  public uint begin {
    set {
      indri_csharpPINVOKE.TagExtent_begin_set(swigCPtr, value);
    } 
    get {
      uint ret = indri_csharpPINVOKE.TagExtent_begin_get(swigCPtr);
      return ret;
    } 
  }

  public uint end {
    set {
      indri_csharpPINVOKE.TagExtent_end_set(swigCPtr, value);
    } 
    get {
      uint ret = indri_csharpPINVOKE.TagExtent_end_get(swigCPtr);
      return ret;
    } 
  }

  public long number {
    set {
      indri_csharpPINVOKE.TagExtent_number_set(swigCPtr, value);
    } 
    get {
      long ret = indri_csharpPINVOKE.TagExtent_number_get(swigCPtr);
      return ret;
    } 
  }

  public TagExtent parent {
    set {
      indri_csharpPINVOKE.TagExtent_parent_set(swigCPtr, TagExtent.getCPtr(value));
    } 
    get {
      IntPtr cPtr = indri_csharpPINVOKE.TagExtent_parent_get(swigCPtr);
      TagExtent ret = (cPtr == IntPtr.Zero) ? null : new TagExtent(cPtr, false);
      return ret;
    } 
  }

  public AttributeValuePairVector attributes {
    set {
      indri_csharpPINVOKE.TagExtent_attributes_set(swigCPtr, AttributeValuePairVector.getCPtr(value));
    } 
    get {
      IntPtr cPtr = indri_csharpPINVOKE.TagExtent_attributes_get(swigCPtr);
      AttributeValuePairVector ret = (cPtr == IntPtr.Zero) ? null : new AttributeValuePairVector(cPtr, false);
      return ret;
    } 
  }

  public TagExtent() : this(indri_csharpPINVOKE.new_TagExtent(), true) {
  }

}

}
