#ifndef _REST_FPM_H_
#define _REST_FPM_H_

#ifndef __GNUC__
	#error "Certain GNU extensions are required for this program to function"
#endif

#include <stddef.h>

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
	"SCRPIE_FILENAME",
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
