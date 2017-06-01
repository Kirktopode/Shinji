#ifndef Vector_h
#define Vector_h

/** @file */

#include "float.h"

/** Geometry vector, IE quantity with magnitude and direction. This is set up
    as a template, so that we can specify exactly how many components and what
    type each component has.
    @tparam n number of vector components
    @tparam T data type for component
*/
template<int n, typename T=fp>
class Vector {
public:
  T comp[n]; ///< Component array. This is the actual data for the vector, all else is code to act on this.
  /** Enables access to component array directly */
  T& operator[](int i) {return comp[i];};
  /** Construct a zero vector */
  Vector<n,T>() {for(int i=0;i<n;i++) comp[i]=0;};
  /** Assignment operator. Components of source vector are copied into this vector
    @param[in] rhs vector to copy components from
    @return a reference to this now-modified vector
  */
  Vector<n,T>& operator=(Vector<n> rhs) {
    for(int i=0;i<n;i++)comp[i]=rhs[i];
    return *this;
  }
  /** Vector addition, component by component 
    @param[in] rhs vector to add components from
    @return a reference to this now-modified vector
   */
  Vector<n,T>& operator+=(const Vector<n>& rhs) {
    for(int i=0;i<n;i++) comp[i]+=rhs.comp[i];
    return *this;
  }
  /** Vector-scalar addition. Add the scalar to each component of the vector 
    @param[in] rhs scalar to add to each component
    @return a reference to this now-modified vector
   */
  Vector<n,T>& operator+=(T rhs) {
    for(int i=0;i<n;i++) comp[i]+=rhs;
    return *this;
  }
  /** Vector subtraction, component by component 
    @param[in] rhs vector to subtract components from
    @return a reference to this now-modified vector
   */
  Vector<n,T>& operator-=(const Vector<n>& rhs) {
    for(int i=0;i<n;i++) comp[i]-=rhs.comp[i];
    return *this;
  }
  /** Vector-scalar subtraction. Subtract the scalar from each component of the vector 
    @param[in] rhs scalar to subtract from each component
    @return a reference to this now-modified vector
   */
  Vector<n,T>& operator-=(T rhs) {
    for(int i=0;i<n;i++) comp[i]-=rhs;
    return *this;
  }
  /** Vector component-by-component multiplication (not dot or cross product) 
    @param[in] rhs vector to multiply components by
    @return a reference to this now-modified vector
   */
  Vector<n,T>& operator*=(const Vector<n>& rhs) {
    for(int i=0;i<n;i++) comp[i]*=rhs.comp[i];
    return *this;
  }
  /** Vector-scalar multiplication. Multiply each component by the scalar 
    @param[in] rhs scalar to multiply each component by
    @return a reference to this now-modified vector
   */
  Vector<n,T>& operator*=(T rhs) {
    for(int i=0;i<n;i++) comp[i]*=rhs;
    return *this;
  }
  /** Vector component-by-component division 
    @param[in] rhs vector to divide components by
    @return a reference to this now-modified vector
   */
  Vector<n,T>& operator/=(const Vector<n>& rhs) {
    for(int i=0;i<n;i++) comp[i]/=rhs.comp[i];
    return *this;
  }
  /** Vector-scalar division. Divide each component by the scalar 
    @param[in] rhs scalar to divide each component by
    @return a reference to this now-modified vector
   */
  Vector<n,T>& operator/=(T rhs) {
    for(int i=0;i<n;i++) comp[i]/=rhs;
    return *this;
  }
};

/** Vector addition operator. Follows the operator overloading recipe
@relates Vector 
@param[in] lhs left hand operand (order doesn't matter as addition is commutative)
@param[in] rhs right hand operand 
@return a vector which is the sum of these two vectors*/
template<int n, typename T=fp> static inline Vector<n,T> operator+(Vector<n,T> lhs, const Vector<n,T>& rhs) {lhs+=rhs;return lhs;};
/** Vector-scalar addition operator. Follows the operator overloading recipe. 
  Note that since scalar addition is commutative, so is this operator and 
  scalar-vector addition will produce exactly the same result as vector-scalar
  addition.
@relates Vector 
@param[in] lhs left hand (vector) operand
@param[in] rhs right hand (scalar) operand 
@return a vector which is a copy of the input vector with the scalar added to each component*/
template<int n, typename T=fp> static inline Vector<n,T> operator+(Vector<n,T> lhs, const T rhs)            {lhs+=rhs;return lhs;};
/** Scalar-vector addition operator. Follows the operator overloading recipe.
  Note that since scalar addition is commutative, so is this operator and 
  scalar-vector addition will produce exactly the same result as vector-scalar
  addition.
@relates Vector 
@param[in] lhs left hand (scalar) operand
@param[in] rhs right hand (vector) operand 
@return a vector which is a copy of the input vector with the scalar added to each component*/
template<int n, typename T=fp> static inline Vector<n,T> operator+(const T& lhs, Vector<n,T> rhs)           {rhs+=lhs;return rhs;};

/** Vector subtraction operator. Follows the operator overloading recipe
@relates Vector 
@param[in] lhs left hand operand
@param[in] rhs right hand operand 
@return a vector which is the difference between these two vectors*/
template<int n, typename T=fp> static inline Vector<n,T> operator-(Vector<n,T> lhs, const Vector<n,T>& rhs) {lhs-=rhs;return lhs;};
/** Vector-scalar subtraction operator. Follows the operator overloading recipe 
@relates Vector 
@param[in] lhs left hand (vector) operand
@param[in] rhs right hand (scalar) operand 
@return a vector which is a copy of the input vector with the scalar subtracted from each component*/
template<int n, typename T=fp> static inline Vector<n,T> operator-(Vector<n,T> lhs, const T rhs)            {lhs-=rhs;return lhs;};
/** Scalar-vector addition operator. Follows the operator overloading recipe 
@relates Vector 
@param[in] lhs left hand (scalar) operand
@param[in] rhs right hand (vector) operand 
@return a vector which is a copy of the input vector with the scalar subtracted from each component*/
template<int n, typename T=fp> static inline Vector<n,T> operator-(const T& lhs, Vector<n,T> rhs)           {rhs-=lhs;return rhs;};

/** Vector component multiplication operator. Performs component-by-component multiplication, and follows the operator overloading recipe
@relates Vector 
@param[in] lhs left hand operand (order doesn't matter as multiplication is commutative)
@param[in] rhs right hand operand 
@return a vector which is the component-by-component product of these two vectors*/
template<int n, typename T=fp> static inline Vector<n,T> operator*(Vector<n,T> lhs, const Vector<n,T>& rhs) {lhs*=rhs;return lhs;};
/** Vector-scalar multiplication operator. Follows the operator overloading recipe.
  Note that since scalar multiplication is commutative, so is this operator and 
  scalar-vector multiplication will produce exactly the same result as vector-scalar
  multiplication.
@relates Vector 
@param[in] lhs left hand (vector) operand
@param[in] rhs right hand (scalar) operand 
@return a vector which is scaled by the scalar*/
template<int n, typename T=fp> static inline Vector<n,T> operator*(Vector<n,T> lhs, const T rhs)            {lhs*=rhs;return lhs;};
/** Scalar-vector multiplication operator. Follows the operator overloading recipe 
  Note that since scalar multiplication is commutative, so is this operator and 
  scalar-vector multiplication will produce exactly the same result as vector-scalar
  multiplication.
@relates Vector 
@param[in] lhs left hand (vector) operand
@param[in] rhs right hand (scalar) operand 
@return a vector which is scaled by the scalar*/
template<int n, typename T=fp> static inline Vector<n,T> operator*(const T& lhs, Vector<n,T> rhs)           {rhs*=lhs;return rhs;};

/** Vector component division operator. Performs component-by-component division, and follows the operator overloading recipe
@relates Vector 
@param[in] lhs left hand operand
@param[in] rhs right hand operand 
@return a vector which is the component-by-component quotient of these two vectors*/
template<int n, typename T=fp> static inline Vector<n,T> operator/(Vector<n,T> lhs, const Vector<n,T>& rhs) {lhs/=rhs;return lhs;};
/** Vector-scalar division operator. Follows the operator overloading recipe.
    Note that there isn't a scalar-vector division operator, as it doesn't make
    sense to divide a single scalar by several vector components.
@relates Vector 
@param[in] lhs left hand (vector) operand
@param[in] rhs right hand (scalar) operand 
@return a vector which is scaled by the reciprocal of the scalar*/
template<int n, typename T=fp> static inline Vector<n,T> operator/(Vector<n,T> lhs, const T rhs)            {lhs/=rhs;return lhs;};
// No operator/(fp,Vector). This operation doesn't make sense

/** Scalar (dot) product of two vectors.
@relates Vector
@param[in] lhs left hand operand
@param[in] rhs right hand operand 
@return sum of component-by-component products of the two vectors
*/
template<int n, typename T=fp> static inline T dot(const Vector<n,T>& lhs, const Vector<n,T>& rhs) {T result=0;for(int i=0;i<n;i++) result+=lhs.comp[i]*rhs.comp[i];return result;};
//If we wanted a cross product, we would include it similar to dot() above
#endif
