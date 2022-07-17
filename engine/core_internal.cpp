#include "core_internal.h"

void Panic(char const *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vprintf(fmt, va);
	va_end(va);
	exit(1);
}

char* LoadFileText(const char *path)
{
	FILE *f = fopen(path, "rb");

	if(f == NULL)
	{
		Panic("Can't load file at %s", path);
	}

	char *text;
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	text = (char*)calloc(1, len);
	fread(text, 1, len, f);
	fclose(f);

	return text;
}