/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#ifndef _ROMI_V3_H_
#define _ROMI_V3_H_

#include <cstddef>
#include <stdexcept>

namespace romi {
        
        double *smul(double *w, const double *v, double s);
        double *sdiv(double *w, const double *v, double s);
        double *sadd(double *w, const double *v, double s);
        
        double *vadd(double *r, const double *a, const double *b);
        double *vsub(double *r, const double *a, const double *b);
        double *vmul(double *r, const double *a, const double *b);
        double *vdiv(double *r, const double *a, const double *b);
        double vdot(const double *a, const double *b);
        double *vcross(double *r, const double *a, const double *b);

        double vmax(const double *a);
        double vmin(const double *a);
        
        double *vabs(double *r, const double *a);
        double *vcopy(double *r, const double *a);
        double *vzero(double *r);
        double *vset(double *r, double v);
        
        double vnorm(const double *v);
        double *normalize(double *w, const double *v);
        
        double vdist(const double *a, const double *b);
        bool veq(const double *a, const double *b);
        bool vnear(double *a, double *b, double epsilon);
        
        double *vclamp(double *r, const double *v, const double *lo, const double *hi);
        
        //

        enum {
                kX = 0,
                kY = 1,
                kZ = 2
        };
        
        class v3
        {
        protected:
                double x_[3];
                        
                
        public:
                
                v3() {
                        set(0.0);
                }
                
                v3(double v) {
                        set(v);
                }
                
                v3(double x, double y, double z) {
                        set(x, y, z);
                }
                
                v3(const double *values) {
                        set(values);
                }
                
                /* v3(const v3& v) { */
                /*         set(v.x_); */
                /* } */
                
                double x() const {
                        return x_[kX];
                }
                
                double y() const {
                        return x_[kY];
                }
                
                double z() const {
                        return x_[kZ];
                }

                void x(double value) {
                        x_[kX] = value;
                }
                
                void y(double value) {
                        x_[kY] = value;
                }
                
                void z(double value) {
                        x_[kZ] = value;
                }

                double& operator[](int index) {
                        if (index < 0 || index > 2) {
                                throw std::runtime_error("Index out of bound");
                        }
                        return x_[index];
                }
                
                //
                v3& operator=(double v) {
                        set(v);
                        return *this;
                }

                v3& operator=(const double *v) {
                        set(v);
                        return *this;
                }
                
                void set(double v) {
                        vset(x_, v);
                }
                
                void set(size_t i , double v) {
                        if (i < 3)
                                x_[i] = v;
                }
                
                void set(const double *v) {
                        vcopy(x_, v);
                }
                
                void set(double x, double y, double z) {
                        x_[kX] = x;
                        x_[kY] = y;
                        x_[kZ] = z;
                }
                //
                                
                const double *values() const {
                        return x_;
                }

                v3 operator+(const v3& b) const {
                        v3 r;
                        vadd(r.x_, x_, b.x_);
                        return r;
                }
                
                v3 operator-(const v3& b) const {
                        v3 r;
                        vsub(r.x_, x_, b.x_);
                        return r;
                }

                v3 operator*(const v3& b) const {
                        v3 r;
                        vmul(r.x_, x_, b.x_);
                        return r;
                }

                double dot(const v3& b) const {
                        return vdot(x_, b.x_);
                }
                
                v3 operator+(double b) const {
                        v3 r;
                        sadd(r.x_, x_, b);
                        return r;
                }
                
                v3 operator-(double b) const {
                        v3 r;
                        sadd(r.x_, x_, -b);
                        return r;
                }
                
                v3 operator*(double b) const {
                        v3 r;
                        smul(r.x_, x_, b);
                        return r;
                }
                
                v3 operator/(double b) const {
                        v3 r;
                        sdiv(r.x_, x_, b);
                        return r;
                }
                
                double norm() const {
                        return vnorm(x_);
                }
                
                v3 normalize() const {
                        v3 r;
                        sdiv(r.x_, x_, norm());
                        return r;
                }
                
                double dist(const v3& a) const {
                        return vdist(x_, a.x_);
                }
                
                v3 clamp(const v3& lo, const v3& hi) const {
                        v3 r;
                        vclamp(r.x_, x_, lo.x_, hi.x_);
                        return r;
                }

                bool operator==(const v3& b) const {
                        return veq(x_, b.x_);
                }
        };
        
        inline double norm(const v3& v) {
                return v.norm();
        }

}

#endif // _ROMI_V3_H_
