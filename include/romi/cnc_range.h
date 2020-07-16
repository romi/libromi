/*
  libromi

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  Libromi provides common abstractions and functions for ROMI
  applications.

  libromi is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */
#ifndef _ROMI_RANGE_H_
#define _ROMI_RANGE_H_

#include <r.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _cnc_range_t {
        double x[2];
        double y[2];
        double z[2];
} cnc_range_t;

int cnc_range_parse(cnc_range_t *range, json_object_t r);
int cnc_range_set_minmax(cnc_range_t *range, int axis, double min, double max);
int cnc_range_is_valid(cnc_range_t *range, double x, double y, double z);

#ifdef __cplusplus
}
#endif

#endif // _ROMI_ROVER_H_
