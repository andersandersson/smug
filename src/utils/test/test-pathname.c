#include "pathname.h"
#include "sstring.h"

static const int NUM_TESTS = 4;

int runtest(int nr)
{
	String* str1;
	String* str2;
	String* str3;
    PathName* pn1;
    PathName* pn2;

    switch (nr)
    {
    case 0:
        str1 = String_new("/usr/bin/local");
        pn1 = PathName_new(str1, FALSE);
        pn2 = PathName_new(str1, TRUE);
        if (!PathName_isAbsolute(pn1)) return 1;
        if (!PathName_isAbsolute(pn2)) return 2;
        if (PathName_isFile(pn1)) return 3;
        if (!PathName_isFile(pn2)) return 4;
        if (PathName_getFileName(pn1) != NULL) return 5;
        if (PathName_getBareName(pn1) != NULL) return 6;
        if (PathName_getExtension(pn1) != NULL) return 7;
        if (PathName_getExtension(pn2) != NULL) return 8;
        PathName_delete(pn1);
        PathName_delete(pn2);
        String_delete(str1);
        break;

    case 1:
        str1 = String_new("usr/bin/local");
        pn1 = PathName_new(str1, FALSE);
        str2 = PathName_getAsString(pn1);
        if (!String_equalAscii(str1, str2)) return 10;
        if (PathName_isAbsolute(pn1)) return 11;
        String_delete(str1);
        String_delete(str2);
        str1 = String_new("usr-bin-local");
        str3 = String_new("-");
        PathName_setSeparator(str3);
        str2 = PathName_getAsString(pn1);
        if (!String_equalAscii(str1, str2)) return 12;
        String_delete(str3);
        str3 = String_new("/");
        PathName_setSeparator(str3);
        String_delete(str1);
        String_delete(str2);
        String_delete(str3);
        PathName_delete(pn1);
        break;

    case 2:
        str1 = String_new("/etc/bash.bashrc");
        pn1 = PathName_new(str1, TRUE);
        str2 = PathName_getAsString(pn1);
        if (str2 == NULL) return 20;
        if (!String_equalAscii(str1, str2)) return 21;
        String_delete(str1);
        String_delete(str2);
        str1 = String_new("bash.bashrc");
        str2 = PathName_getFileName(pn1);
        if (str2 == NULL) return 22;
        if (!String_equalAscii(str1, str2)) return 23;
        String_delete(str1);
        String_delete(str2);
        str1 = String_new("bash");
        str2 = PathName_getBareName(pn1);
        if (str2 == NULL) return 24;
        if (!String_equalAscii(str1, str2)) return 25;
        String_delete(str1);
        String_delete(str2);
        str1 = String_new("bashrc");
        str2 = PathName_getExtension(pn1);
        if (str2 == NULL) return 26;
        if (!String_equalAscii(str1, str2)) return 27;
        String_delete(str1);
        String_delete(str2);
        PathName_delete(pn1);
        break;

    case 3:
        str1 = String_new("C:\\My Documents\\");    // Absolute windows path.
        pn1 = PathName_new(str1, FALSE);
        if (pn1 == NULL) return 30;
        String_delete(str1);
        PathName_delete(pn1);
        str1 = String_new("\\My Documents\\");      // Absolute Unix path.
        pn1 = PathName_new(str1, FALSE);
        if (pn1 == NULL) return 31;
        String_delete(str1);
        PathName_delete(pn1);
        str1 = String_new("C:/My Documents\\");     // Absolute Windows path.
        pn1 = PathName_new(str1, FALSE);
        if (pn1 == NULL) return 32;
        String_delete(str1);
        PathName_delete(pn1);
        str1 = String_new("C/My Documents\\");      // Relative Unix path.
        pn1 = PathName_new(str1, FALSE);
        if (pn1 == NULL) return 33;
        if (PathName_isAbsolute(pn1)) return 30;
        String_delete(str1);
        PathName_delete(pn1);
        str1 = String_new("C/:My Documents\\");     // Disallowed dirname character.
        pn1 = PathName_new(str1, FALSE);
        if (pn1 != NULL) return 34;
        String_delete(str1);
        PathName_delete(pn1);
        str1 = String_new("C:/My Documents/");      // Not a file name.
        pn1 = PathName_new(str1, TRUE);
        if (pn1 != NULL) return 35;
        String_delete(str1);
        PathName_delete(pn1);
        break;
    default:
        break;
    }

    return 0;
}

int main()
{
    int i;
    int ret;
    for (i = 0; i < NUM_TESTS; i++)
    {
        ret = runtest(i);
        if (ret != 0) return ret;
    }
    return 0;
}
