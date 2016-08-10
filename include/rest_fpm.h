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

#ifndef _REST_FPM_H_
#define _REST_FPM_H_

#include <stddef.h>

#ifndef __GNUC__
	#error "Certain GNU extensions are required for this program to function"
#endif

#define VERSION "0"

#define MAX_FCGI_PARAMS 17

//
// @FCGI_PARAMS
// These parameters are passed to the application by NGINX. They are set in the
// server configuration file.
//
// @see: /etc/nginx/sites-available/<server-name>
//
const char * const FCGI_PARAMS[MAX_FCGI_PARAMS] =
{
	"GATEWAY_INTERFACE",
	"SERVER_SOFTWARE",
	"QUERY_STRING",
	"REQUEST_METHOD",
	"CONTENT_TYPE",
	"CONTENT_LENGTH",
	"SCRIPT_FILENAME",
	"SCRIPT_NAME",
	"REQUEST_URI",
	"DOCUMENT_URI",
	"DOCUMENT_ROOT",
	"SERVER_PROTOCOL",
	"REMOTE_ADDR",
	"REMOTE_PORT",
	"SERVER_ADDR",
	"SERVER_PORT",
	"SERVER_NAME"
};

//
// @struct query_parser: stores information about an API query
// 
// @count: Number of individual queries
// @tokens: Collection of token cordinates for a string
//
struct query_parser
{
	size_t count;
	struct token *tokens;
};

struct token
{
	size_t start;
	size_t end;
	size_t size;
};

#endif // _REST_FPM_H
