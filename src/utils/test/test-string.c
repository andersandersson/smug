#include "sstring.h"
#include "linkedlist.h"

static const int NUM_TESTS = 100;

/*
    TODO: Test String_splitStr()
*/

int runtest(int nr)
{
	String* str1;
	String* str2;
	String* str3;
    String* nullStr;
    LinkedList* ls1;
    LinkedList* ls2;
    
    nullStr = String_new("");

    switch (nr)
    {
    case 0:
        str1 = String_new("");
        String_delete(str1);
        break;
    case 1:
        str1 = String_new("Testar strings.");
        str2 = String_new("Testar strings.");
        if (String_length(str1) != 15) return 10;
        if (String_charAt(str1, 3) != 't') return 11;
        if (!String_equalAscii(str1, str2)) return 12;
        String_delete(str1);
        String_delete(str2);
        break;
    case 2:
        str1 = String_new("Testar strings.");
        str2 = String_new("Testar strings...");
        if (String_equalAscii(str1, str2)) return 20;
        String_delete(str1);
        String_delete(str2);
        break;
    case 3:
        str1 = String_new("Testar strings.");
        if (String_occurences(str1, 's') != 3) return 30;
        String_delete(str1);
        break;
    case 4:
        str1 = String_new("Testar strings.");
        str2 = String_new("BLA");
        str3 = String_newReplaceChar(str1, 's', str2);
        String_delete(str1);
        str1 = String_new("TeBLAtar BLAtringBLA.");
        if (!String_equalAscii(str3, str1)) return 40;
        String_delete(str1);
        String_delete(str2);
        String_delete(str3);
        break;
    case 5:
        str1 = String_new("TeBLAtar BLAtringBLA.");
        str2 = String_newSubString(str1, 4, 7);
        str3 = String_new("Atar BL");
        if (!String_equalAscii(str3, str2)) return 50;
        String_delete(str1);
        str1 = String_newConcat(str2, str3);
        String_delete(str2);
        str2 = String_new("Atar BLAtar BL");
        if (!String_equalAscii(str1, str2)) return 51;
        String_delete(str1);
        String_delete(str2);
        String_delete(str3);
        break;
    case 6:
        str1 = String_new("Testar strings.");
        str2 = String_new("");
        str3 = String_newConcat(str1, str2);
        if (!String_equalAscii(str1, str3)) return 60;
        String_delete(str1);
        String_delete(str2);
        String_delete(str3);
        break;
    case 7:
        str1 = String_new("Testar strings.");
        str2 = String_new("estar st");
        String_crop(str1, 1, 8);
        if (!String_equalAscii(str1, str2)) return 70;
        String_delete(str1);
        String_delete(str2);
        break;
    case 8:
        str1 = String_new("Testar strings.");
        if (String_findChar(str1, 'T') != 0) return 80;
        if (String_findCharFrom(str1, 't', 3) != 3) return 81;
        if (String_findCharFrom(str1, 't', 9) != -1) return 82;
        if (String_findCharFrom(str1, 't', 4) != 8) return 83;
        if (String_findChar(str1, 'x') != -1) return 84;
        String_delete(str1);
        break;
    case 9:
        str1 = String_new("detta/är/ett/långt/katalognamn");
        str2 = String_new("/detta/är/ett/långt//katalognamn/");
        ls1 = String_split(str1, '/');
        ls2 = String_split(str2, '/');
        if (LinkedList_length(ls1) != 5) return 90;
        if (LinkedList_length(ls2) != 8) return 91;
        if (String_length((String*)ls2->first->item) != 0) return 92;
        String_delete(str1);
        String_delete(str2);
        str1 = String_newJoin(ls1, nullStr);
        str2 = String_newJoin(ls2, nullStr);
        str3 = String_new("dettaärettlångtkatalognamn");
        if (!String_equalAscii(str1, str2)) return 93;
        if (!String_equalAscii(str1, str3)) return 94;
        String_delete(str1);
        String_delete(str2);
        String_delete(str3);
        str3 = String_new("/");
        str1 = String_newJoin(ls1, str3);
        str2 = String_newJoin(ls2, str3);
        String_delete(str3);
        str3 = String_new("detta/är/ett/långt/katalognamn");
        if (!String_equalAscii(str1, str3)) return 95;
        String_delete(str3);
        str3 = String_new("/detta/är/ett/långt//katalognamn/");
        if (!String_equalAscii(str2, str3)) return 96;
        String_delete(str1);
        String_delete(str2);
        String_delete(str3);
        LinkedList_deleteContents(ls1, String_delete);
        LinkedList_deleteContents(ls2, String_delete);
        LinkedList_delete(ls1);
        LinkedList_delete(ls2);
        break;
    case 10:
        str1 = String_new("babab");
        str2 = String_new("ba");
        if (String_occurencesStr(str1, str2) != 2) return 100;
        String_delete(str2);
        str2 = String_new("bab");
        if (String_occurencesStr(str1, str2) != 1) return 101;
        if (String_occurencesStrOverlap(str1, str2) != 2) return 102;
        String_delete(str1);
        String_delete(str2);
        break;
    default:
        break;
    }
    String_delete(nullStr);
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
