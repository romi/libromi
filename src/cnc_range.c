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

#include "romi/cnc_range.h"

int cnc_range_set_minmax(cnc_range_t *range, int axis, double min, double max)
{
        if (max < min) {
                r_err("cnc_range_set_minmax: max > min for axis %d", axis);
                return -1;
        }
        switch (axis) {
        case 0: range->x[0] = min;
                range->x[1] = max;
                break;
        case 1: range->y[0] = min;
                range->y[1] = max;
                break;
        case 2: range->z[0] = min;
                range->z[1] = max;
                break;                
        }
        return 0;
}

static int cnc_range_parse_minmax(cnc_range_t *range, json_object_t a, int axis)
{
        int err = 0;
        double min = json_array_getnum(a, 0);
        double max = json_array_getnum(a, 1);
        if (isnan(min) || isnan(max)) {
                r_err("Invalid min or max values for axis %d", axis);
                err = -1;
        } else {
                err = cnc_range_set_minmax(range, axis, min, max);
        }
        return err;
}

static int cnc_range_parse_axis(cnc_range_t *range, json_object_t r, int axis)
{
        int err = 0;
        json_object_t a = json_array_get(r, axis);
        if (!json_isarray(a)) {
                r_err("Range of axis %d is not an array", axis);
                err = -1;
        } else if (json_array_length(a) != 2) {
                r_err("Invalid array for axis %d", axis);
                err = -1;
        } else {
                err = cnc_range_parse_minmax(range, a, axis);
        }
        return err;
}

static int cnc_range_parse_axes(cnc_range_t *range, json_object_t r)
{
        int err = 0;
        for (int axis = 0; axis < 3; axis++) {
                if (cnc_range_parse_axis(range, r, axis) != 0) {
                        err = -1;
                        break;
                }
        }
        return err;
}

int cnc_range_parse(cnc_range_t *range, json_object_t r)
{
        int err = 0;
        if (!json_isarray(r)) {
                r_err("Invalid range, expected an array");
                err = -1;
        } else if (json_array_length(r) != 3) {
                r_err("Invalid range, expected an array with 3 axes");
                err = -1;
        } else {
                err = cnc_range_parse_axes(range, r);
        }
        return err;
}

int cnc_range_is_valid(cnc_range_t *range, double x, double y, double z)
{
        return ((x >= range->x[0]) && (x <= range->x[1])
                && (y >= range->y[0]) && (y <= range->y[1])
                && (z >= range->z[0]) && (z <= range->z[1]));
}
