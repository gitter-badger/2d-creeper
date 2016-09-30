#ifndef LINUX_H
#define LINUX_H
typedef unsigned short WORD;
typedef struct CHAR_INFO
{
	union {
		unsigned short UnicodeChar;
		char AsciiChar;
	} Char;
	WORD Attributes;
} CHAR_INFO;

#endif
