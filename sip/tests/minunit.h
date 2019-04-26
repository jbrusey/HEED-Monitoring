/*
    This file is part of LSip.

    LSip is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LSip is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LSip.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef MINUNIT_H
#define MINUNIT_H

#define mu_strify1(s) #s
#define mu_strify(s) mu_strify1(s)
#define mu_assert(message, test) do { if (!(test)) return __FILE__ ":" mu_strify(__LINE__) ": assertion failed: " message; } while (0)
#define mu_run_test(test) do { const char *message = test(); tests_run++; if (message) return message; } while (0)
extern int tests_run;

#endif
