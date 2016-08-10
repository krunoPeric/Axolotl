#ifndef _TERMIO_H_
#define _TERMIO_H_

#ifdef HEADLESS
	#warn "Printing to the terminal has no effect for headless instances"
#endif

#define _tprintfb(t,b) tprintf("\033[1m%s\033[0m\t%s", t, b)
#define tprintfb(t,b) tprintf("\033[1m%s\033[0m\t%s\n", t, b)

void tprintf(const char *format, ...);

#endif // _TERMIO_H_
