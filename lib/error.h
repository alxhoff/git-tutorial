/****************************************************************************
   ----------------------------------------------------------------------
    Copyright (C) Alexander Hoffman, 2019

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 ****************************************************************************/

/**
 * @file error.h
 * @author Alex Hoffman
 * @copyright GNU GPL v3
 * */

#ifndef ERROR_H_
#define ERROR_H_

typedef enum {
  ENOMEM = 1,
  ENOINIT = 2,
  ENOENT = 3,
  EINVAL = 4,
} err_codes_t;

#endif /* ERROR_H_ */
