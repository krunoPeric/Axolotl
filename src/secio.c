/**
 * Harlan James <root@aboyandhisgnu.org> <osurobotics.club>
 *
 * This file is part of Axolotl.
 *
 * Axolotl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Axolotl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Axolotl.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "secio.h"

#include "fcgi_stdio.h"

#include <stdio.h>
#include <unistd.h>

void
print_s(const char * const buf)
{
	if (buf == NULL)
	{
		return;
	}

	int i = 0;
	while (buf[i] > 0 && buf[i] <= '~')
	{
		fwrite((void *)&buf[i++], sizeof(char), 1, FCGI_stdout);
	}
}
