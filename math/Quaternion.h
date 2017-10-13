#ifndef Quaternion_h
#define Quaternion_h

/** @file */

#include "Vector.h"

/** Quaternion class. This is a specialization of the Vector class, which adds
    the quaternion multiplication operator, along with a couple of methods to
    encourage use of a consistent convention when using this quaternion class
    for 3D orientation. */
class Quaternion:public Vector<4,fp> {
private:
  friend Quaternion operator*(Quaternion lhs, const Quaternion& rhs);
  /** Quaternion multipy and assign. Don't use this directly, instead use the
      plain multiply operator built upon this.

      The operator uses this as the left-hand operand,
      the passed-in quaternion as the right-hand operand, and writes
      the result back to the left-hand side, returning a reference to it.

      Stone bridge equation test - i^2==j^2==k^2==ijk==-1

          Quaternion i(1,0,0,0);
          printf("i: x=%f,y=%f,z=%f,w=%f\n",i.x(),i.y(),i.z(),i.w());
          Quaternion j(0,1,0,0);
          printf("j: x=%f,y=%f,z=%f,w=%f\n",j.x(),j.y(),j.z(),j.w());
          Quaternion k(0,0,1,0);
          printf("k: x=%f,y=%f,z=%f,w=%f\n",k.x(),k.y(),k.z(),k.w());
          Quaternion i2=i*i;
          printf("i^2: x=%f,y=%f,z=%f,w=%f\n",i2.x(),i2.y(),i2.z(),i2.w());
          Quaternion j2=j*j;
          printf("j^2: x=%f,y=%f,z=%f,w=%f\n",j2.x(),j2.y(),j2.z(),j2.w());
          Quaternion k2=k*k;
          printf("k^2: x=%f,y=%f,z=%f,w=%f\n",k2.x(),k2.y(),k2.z(),k2.w());
          Quaternion ijk=i*j*k;
          printf("ijk: x=%f,y=%f,z=%f,w=%f\n",ijk.x(),ijk.y(),ijk.z(),ijk.w());

      @param q right-hand side quaternion


  */
  Quaternion& operator*=(const Quaternion& q) {
    fp rx= w()*q.x()-z()*q.y()+y()*q.z()+x()*q.w();
    fp ry= z()*q.x()+w()*q.y()-x()*q.z()+y()*q.w();
    fp rz=-y()*q.x()+x()*q.y()+w()*q.z()+z()*q.w();
    fp rw=-x()*q.x()-y()*q.y()-z()*q.z()+w()*q.w();
    x()=rx;y()=ry;z()=rz;w()=rw;
    return *this;
  }
public:
  /** Assignment operator. Copy the components from the right hand side
      quaternion into this quaternion without affecting the right hand side
   @param[in] rhs Quaternion to copy components from
   */
  Quaternion& operator=(Quaternion rhs) {
    for(int i=0;i<4;i++)comp[i]=rhs[i];
    return *this;
  };
  /** Constructor with components specified
   @param[in] Lx initial x vector component
   @param[in] Ly initial y vector component
   @param[in] Lz initial z vector component
   @param[in] Lw initial scalar   component
  */
  Quaternion(fp Lx, fp Ly, fp Lz, fp Lw):Quaternion(){x()=Lx;y()=Ly;z()=Lz;w()=Lw;};
  /** Construct an identity quaternion, IE 0i+0j+0k+1 . This also represents an
      identity orientation */
  Quaternion() {x()=0;y()=0;z()=0;w()=1;};
  Quaternion(Vector<4,fp> other):Quaternion(other.comp[0],other.comp[1],other.comp[2],other.comp[3]) {};
  /** Construct a quaternion from the components of a 3D vector. Quaternion
      will be Lx*i+Ly*j+Lz*k+0 . Does not have to be a unit quaternion or unit
      vector.
   @param[in] Lx initial x vector component
   @param[in] Ly initial y vector component
   @param[in] Lz initial z vector component
   */
  Quaternion(fp Lx, fp Ly, fp Lz):Quaternion(Lx,Ly,Lz,0){};
  /** Quaternion conjugation (Good night, everybody!). This method operates 
      directly on this quaternion, changing its components (as opposed to 
      returning a copy which is conjugated). */
  void conjugate() {x()=-x();y()=-y();z()=-z();}

  void integrate(Vector<3> w, fp dt, int steps=1);
  /** Reciprocal of quaternion magnitude
   @return reciprocal of quaternion magnitude=1/sqrt(x^2+y^2+z^2+w^2)
   */
  fp rlength() {return Q_rsqrt(x()*x()+y()*y()+z()*z()+w()*w());};
  /** Force the quaternion to be unit length with the same direction */
  void normalize() {((Vector<4,fp>)(*this))*=rlength();};
  Quaternion r2b(Quaternion& vr);
  Quaternion b2r(Quaternion& vb);
};

/** Quaternion multiply and assign. Not a method because we need to follow the
    operator overloading recipe. Built on multiply-assign member operator.
  @param[in] lhs left-hand side operand
  @param[in] rhs right-hand side operator
  @return The quaternion product */
inline Quaternion operator*(Quaternion lhs, const Quaternion& rhs) {lhs*=rhs;return lhs;};

#endif
