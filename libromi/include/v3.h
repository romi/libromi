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
        
        class v3
        {
        protected:
                double _x[3];
                        
                
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
                /*         set(v._x); */
                /* } */
                
                virtual ~v3() = default;
                
                double& x() {
                        return _x[0];
                }
                
                double& y() {
                        return _x[1];
                }
                
                double& z() {
                        return _x[2];
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
                        vset(_x, v);
                }
                
                void set(size_t i , double v) {
                        if (i < 3)
                                _x[i] = v;
                }
                
                void set(const double *v) {
                        vcopy(_x, v);
                }
                
                void set(double x, double y, double z) {
                        _x[0] = x;
                        _x[1] = y;
                        _x[2] = z;
                }
                //
                                
                const double *values() const {
                        return _x;
                }

                v3 operator+(const v3& b) const {
                        v3 r;
                        vadd(r._x, _x, b._x);
                        return r;
                }
                
                v3 operator-(const v3& b) const {
                        v3 r;
                        vsub(r._x, _x, b._x);
                        return r;
                }

                v3 operator*(const v3& b) const {
                        v3 r;
                        vmul(r._x, _x, b._x);
                        return r;
                }

                double dot(const v3& b) const {
                        return vdot(_x, b._x);
                }
                
                v3 operator+(double b) const {
                        v3 r;
                        sadd(r._x, _x, b);
                        return r;
                }
                
                v3 operator-(double b) const {
                        v3 r;
                        sadd(r._x, _x, -b);
                        return r;
                }
                
                v3 operator*(double b) const {
                        v3 r;
                        smul(r._x, _x, b);
                        return r;
                }
                
                v3 operator/(double b) const {
                        v3 r;
                        sdiv(r._x, _x, b);
                        return r;
                }
                
                double norm() const {
                        return vnorm(_x);
                }
                
                v3 normalize() const {
                        v3 r;
                        sdiv(r._x, _x, norm());
                        return r;
                }
                
                double dist(const v3& a) const {
                        return vdist(_x, a._x);
                }
                
                v3 clamp(const v3& lo, const v3& hi) const {
                        v3 r;
                        vclamp(r._x, _x, lo._x, hi._x);
                        return r;
                }

                bool operator==(const v3& b) const {
                        return veq(_x, b._x);
                }
        };
        
        inline double norm(const v3& v) {
                return v.norm();
        }

}

#endif // _ROMI_V3_H_
