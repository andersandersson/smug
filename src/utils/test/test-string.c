#include "string.h"

int main()
{
	String* str1;
	String* str2;
	String* str3;
	
	str1 = String_new("");
	String_delete(str1);
	str1 = String_new("Testar strings.");
	str2 = String_new("Testar strings.");

	if (String_length(str1) != 15) return 5;
	if (String_charAt(str1, 3) != 't') return 6;
	
	if (!String_equalAscii(str1, str2)) return 1;
	String_delete(str2);
	str2 = String_new("Testar strings...");
	if (String_equalAscii(str1, str2)) return 2;
	String_delete(str2);
	
	if (String_occurences(str1, 's') != 3) return 4;
	
	str2 = String_new("BLA");
	str3 = String_newReplaceChar(str1, 's', str2);
	String_delete(str1);
	str1 = String_new("TeBLAtar BLAtringBLA.");
	if (!String_equalAscii(str3, str1)) return 3;
	
	//String_delete(str1);
	String_delete(str2);
	String_delete(str3);
	
	str2 = String_newSubString(str1, 4, 7);
	str3 = String_new("Atar BL");
	if (!String_equalAscii(str3, str2)) return 7;
	String_delete(str1);
	
	str1 = String_newConcat(str2, str3);
	String_delete(str2);
	str2 = String_new("Atar BLAtar BL");
	if (!String_equalAscii(str1, str2)) return 8;
	String_delete(str3);
	
	String_crop(str1, 1, 8);
	str3 = String_new("tar BLAt");
	if (!String_equalAscii(str1, str3)) return 9;
	
	if (String_findChar(str3, 't') != 0) return 10;
	if (String_findCharFrom(str3, 't', 1) != 7) return 11;
	if (String_findCharFrom(str3, 't', 8) != -1) return 12;
	if (String_findCharFrom(str3, 'a', 2) != -1) return 13;
	if (String_findChar(str3, 'x') != -1) return 14;
	
    return 0;
}
