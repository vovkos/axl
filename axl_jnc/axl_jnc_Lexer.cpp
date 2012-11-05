
/* #line 1 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
#include "stdafx.h"
#include "axl_jnc_Lexer.h"

// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)

namespace axl {
namespace jnc {

//.............................................................................


/* #line 2 "axl_jnc_Lexer.cpp" */
static const char _axl_jnc_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 62, 1, 63, 1, 64, 1, 
	65, 1, 66, 1, 67, 1, 68, 1, 
	69, 1, 70, 1, 71, 1, 72, 1, 
	73, 1, 74, 1, 75, 1, 76, 1, 
	77, 1, 78, 1, 79, 1, 80, 1, 
	81, 1, 82, 1, 83, 1, 85, 1, 
	86, 1, 87, 1, 88, 1, 89, 1, 
	90, 1, 91, 1, 92, 1, 93, 1, 
	94, 1, 95, 1, 96, 1, 97, 1, 
	98, 1, 99, 1, 100, 1, 101, 2, 
	0, 81, 2, 0, 82, 2, 0, 84, 
	2, 3, 4, 2, 3, 5, 2, 3, 
	6, 2, 3, 7, 2, 3, 8, 2, 
	3, 9, 2, 3, 10, 2, 3, 11, 
	2, 3, 12, 2, 3, 13, 2, 3, 
	14, 2, 3, 15, 2, 3, 16, 2, 
	3, 17, 2, 3, 18, 2, 3, 19, 
	2, 3, 20, 2, 3, 21, 2, 3, 
	22, 2, 3, 23, 2, 3, 24, 2, 
	3, 25, 2, 3, 26, 2, 3, 27, 
	2, 3, 28, 2, 3, 29, 2, 3, 
	30, 2, 3, 31, 2, 3, 32, 2, 
	3, 33, 2, 3, 34, 2, 3, 35, 
	2, 3, 36, 2, 3, 37, 2, 3, 
	38, 2, 3, 39, 2, 3, 40, 2, 
	3, 41, 2, 3, 42, 2, 3, 43, 
	2, 3, 44, 2, 3, 45, 2, 3, 
	46, 2, 3, 47, 2, 3, 48, 2, 
	3, 49, 2, 3, 50, 2, 3, 51, 
	2, 3, 52, 2, 3, 53, 2, 3, 
	54, 2, 3, 55, 2, 3, 56, 2, 
	3, 57, 2, 3, 58, 2, 3, 59, 
	2, 3, 60, 2, 3, 61
};

static const short _axl_jnc_key_offsets[] = {
	0, 3, 6, 7, 9, 12, 18, 62, 
	65, 66, 69, 70, 71, 73, 76, 77, 
	78, 80, 82, 83, 86, 87, 91, 93, 
	99, 101, 102, 103, 105, 106, 113, 114, 
	122, 130, 138, 148, 156, 164, 172, 180, 
	188, 196, 204, 212, 220, 228, 236, 244, 
	255, 263, 271, 279, 287, 295, 303, 311, 
	319, 328, 336, 344, 352, 360, 368, 377, 
	385, 393, 401, 409, 417, 425, 433, 441, 
	449, 458, 466, 474, 483, 491, 499, 507, 
	515, 523, 533, 541, 549, 557, 565, 573, 
	581, 589, 597, 605, 615, 623, 631, 639, 
	647, 655, 667, 675, 683, 691, 699, 707, 
	715, 723, 731, 740, 748, 756, 764, 772, 
	780, 788, 796, 804, 812, 820, 828, 836, 
	845, 853, 861, 869, 877, 885, 893, 901, 
	909, 917, 927, 935, 943, 952, 960, 968, 
	976, 984, 992, 1000, 1008, 1016, 1024, 1032, 
	1040, 1048, 1056, 1064, 1072, 1080, 1088, 1096, 
	1109, 1117, 1125, 1133, 1141, 1149, 1157, 1166, 
	1174, 1182, 1190, 1198, 1206, 1214, 1223, 1231, 
	1239, 1247, 1255, 1263, 1271, 1279, 1287, 1295, 
	1303, 1312, 1320, 1328, 1336, 1344, 1353, 1361, 
	1369, 1377, 1386, 1395, 1403, 1411, 1420, 1428, 
	1436, 1444, 1452, 1460, 1468, 1476, 1484, 1492, 
	1501, 1509, 1517, 1525, 1533, 1541, 1550, 1558, 
	1566, 1574, 1582, 1590, 1598, 1607, 1615, 1623, 
	1631, 1638, 1646, 1654, 1662, 1670
};

static const char _axl_jnc_trans_keys[] = {
	10, 34, 92, 10, 39, 92, 46, 10, 
	42, 10, 42, 47, 48, 57, 65, 70, 
	97, 102, 9, 10, 13, 32, 33, 34, 
	37, 38, 39, 42, 43, 45, 46, 47, 
	48, 60, 61, 62, 94, 95, 97, 98, 
	99, 100, 101, 102, 103, 105, 108, 110, 
	112, 114, 115, 116, 117, 118, 119, 124, 
	49, 57, 65, 90, 104, 122, 9, 13, 
	32, 61, 10, 34, 92, 10, 61, 38, 
	61, 10, 39, 92, 10, 61, 43, 61, 
	45, 61, 46, 42, 47, 61, 10, 88, 
	120, 48, 57, 48, 57, 48, 57, 65, 
	70, 97, 102, 60, 61, 61, 61, 61, 
	62, 61, 95, 48, 57, 65, 90, 97, 
	122, 61, 95, 117, 48, 57, 65, 90, 
	97, 122, 95, 116, 48, 57, 65, 90, 
	97, 122, 95, 111, 48, 57, 65, 90, 
	97, 122, 95, 105, 111, 114, 48, 57, 
	65, 90, 97, 122, 95, 103, 48, 57, 
	65, 90, 97, 122, 95, 101, 48, 57, 
	65, 90, 97, 122, 95, 110, 48, 57, 
	65, 90, 97, 122, 95, 100, 48, 57, 
	65, 90, 97, 122, 95, 105, 48, 57, 
	65, 90, 97, 122, 95, 97, 48, 57, 
	65, 90, 98, 122, 95, 110, 48, 57, 
	65, 90, 97, 122, 95, 111, 48, 57, 
	65, 90, 97, 122, 95, 108, 48, 57, 
	65, 90, 97, 122, 95, 101, 48, 57, 
	65, 90, 97, 122, 95, 97, 48, 57, 
	65, 90, 98, 122, 95, 107, 48, 57, 
	65, 90, 97, 122, 95, 97, 104, 108, 
	111, 48, 57, 65, 90, 98, 122, 95, 
	115, 48, 57, 65, 90, 97, 122, 95, 
	101, 48, 57, 65, 90, 97, 122, 95, 
	97, 48, 57, 65, 90, 98, 122, 95, 
	114, 48, 57, 65, 90, 97, 122, 95, 
	97, 48, 57, 65, 90, 98, 122, 95, 
	115, 48, 57, 65, 90, 97, 122, 95, 
	115, 48, 57, 65, 90, 97, 122, 95, 
	110, 48, 57, 65, 90, 97, 122, 95, 
	115, 116, 48, 57, 65, 90, 97, 122, 
	95, 116, 48, 57, 65, 90, 97, 122, 
	95, 105, 48, 57, 65, 90, 97, 122, 
	95, 110, 48, 57, 65, 90, 97, 122, 
	95, 117, 48, 57, 65, 90, 97, 122, 
	95, 101, 48, 57, 65, 90, 97, 122, 
	95, 101, 111, 48, 57, 65, 90, 97, 
	122, 95, 102, 48, 57, 65, 90, 97, 
	122, 95, 97, 48, 57, 65, 90, 98, 
	122, 95, 117, 48, 57, 65, 90, 97, 
	122, 95, 108, 48, 57, 65, 90, 97, 
	122, 95, 116, 48, 57, 65, 90, 97, 
	122, 95, 117, 48, 57, 65, 90, 97, 
	122, 95, 98, 48, 57, 65, 90, 97, 
	122, 95, 108, 48, 57, 65, 90, 97, 
	122, 95, 101, 48, 57, 65, 90, 97, 
	122, 95, 108, 110, 48, 57, 65, 90, 
	97, 122, 95, 115, 48, 57, 65, 90, 
	97, 122, 95, 101, 48, 57, 65, 90, 
	97, 122, 95, 100, 117, 48, 57, 65, 
	90, 97, 122, 95, 105, 48, 57, 65, 
	90, 97, 122, 95, 97, 48, 57, 65, 
	90, 98, 122, 95, 110, 48, 57, 65, 
	90, 97, 122, 95, 109, 48, 57, 65, 
	90, 97, 122, 95, 99, 48, 57, 65, 
	90, 97, 122, 95, 97, 108, 111, 48, 
	57, 65, 90, 98, 122, 95, 108, 48, 
	57, 65, 90, 97, 122, 95, 115, 48, 
	57, 65, 90, 97, 122, 95, 101, 48, 
	57, 65, 90, 97, 122, 95, 111, 48, 
	57, 65, 90, 97, 122, 95, 97, 48, 
	57, 65, 90, 98, 122, 95, 116, 48, 
	57, 65, 90, 97, 122, 95, 114, 48, 
	57, 65, 90, 97, 122, 95, 101, 48, 
	57, 65, 90, 97, 122, 95, 116, 48, 
	57, 65, 90, 97, 122, 95, 102, 109, 
	110, 48, 57, 65, 90, 97, 122, 95, 
	112, 48, 57, 65, 90, 97, 122, 95, 
	111, 48, 57, 65, 90, 97, 122, 95, 
	114, 48, 57, 65, 90, 97, 122, 95, 
	116, 48, 57, 65, 90, 97, 122, 95, 
	116, 48, 57, 65, 90, 97, 122, 49, 
	51, 54, 56, 95, 101, 48, 57, 65, 
	90, 97, 122, 54, 95, 48, 57, 65, 
	90, 97, 122, 50, 95, 48, 57, 65, 
	90, 97, 122, 52, 95, 48, 57, 65, 
	90, 97, 122, 95, 114, 48, 57, 65, 
	90, 97, 122, 95, 102, 48, 57, 65, 
	90, 97, 122, 95, 97, 48, 57, 65, 
	90, 98, 122, 95, 99, 48, 57, 65, 
	90, 97, 122, 95, 101, 48, 57, 65, 
	90, 97, 122, 95, 105, 111, 48, 57, 
	65, 90, 97, 122, 95, 116, 48, 57, 
	65, 90, 97, 122, 95, 116, 48, 57, 
	65, 90, 97, 122, 95, 108, 48, 57, 
	65, 90, 97, 122, 95, 101, 48, 57, 
	65, 90, 97, 122, 95, 101, 48, 57, 
	65, 90, 97, 122, 95, 110, 48, 57, 
	65, 90, 97, 122, 95, 100, 48, 57, 
	65, 90, 97, 122, 95, 105, 48, 57, 
	65, 90, 97, 122, 95, 97, 48, 57, 
	65, 90, 98, 122, 95, 110, 48, 57, 
	65, 90, 97, 122, 95, 110, 48, 57, 
	65, 90, 97, 122, 95, 103, 48, 57, 
	65, 90, 97, 122, 95, 97, 117, 48, 
	57, 65, 90, 98, 122, 95, 109, 48, 
	57, 65, 90, 97, 122, 95, 101, 48, 
	57, 65, 90, 97, 122, 95, 115, 48, 
	57, 65, 90, 97, 122, 95, 112, 48, 
	57, 65, 90, 97, 122, 95, 97, 48, 
	57, 65, 90, 98, 122, 95, 99, 48, 
	57, 65, 90, 97, 122, 95, 101, 48, 
	57, 65, 90, 97, 122, 95, 108, 48, 
	57, 65, 90, 97, 122, 95, 108, 48, 
	57, 65, 90, 97, 122, 95, 97, 114, 
	117, 48, 57, 65, 90, 98, 122, 95, 
	99, 48, 57, 65, 90, 97, 122, 95, 
	107, 48, 57, 65, 90, 97, 122, 95, 
	105, 111, 48, 57, 65, 90, 97, 122, 
	95, 118, 48, 57, 65, 90, 97, 122, 
	95, 97, 48, 57, 65, 90, 98, 122, 
	95, 116, 48, 57, 65, 90, 97, 122, 
	95, 101, 48, 57, 65, 90, 97, 122, 
	95, 112, 48, 57, 65, 90, 97, 122, 
	95, 101, 48, 57, 65, 90, 97, 122, 
	95, 114, 48, 57, 65, 90, 97, 122, 
	95, 116, 48, 57, 65, 90, 97, 122, 
	95, 121, 48, 57, 65, 90, 97, 122, 
	95, 98, 48, 57, 65, 90, 97, 122, 
	95, 108, 48, 57, 65, 90, 97, 122, 
	95, 105, 48, 57, 65, 90, 97, 122, 
	95, 99, 48, 57, 65, 90, 97, 122, 
	95, 101, 48, 57, 65, 90, 97, 122, 
	95, 116, 48, 57, 65, 90, 97, 122, 
	95, 117, 48, 57, 65, 90, 97, 122, 
	95, 114, 48, 57, 65, 90, 97, 122, 
	95, 110, 48, 57, 65, 90, 97, 122, 
	95, 97, 101, 104, 105, 116, 119, 48, 
	57, 65, 90, 98, 122, 95, 102, 48, 
	57, 65, 90, 97, 122, 95, 101, 48, 
	57, 65, 90, 97, 122, 95, 116, 48, 
	57, 65, 90, 97, 122, 95, 111, 48, 
	57, 65, 90, 97, 122, 95, 114, 48, 
	57, 65, 90, 97, 122, 95, 116, 48, 
	57, 65, 90, 97, 122, 95, 103, 122, 
	48, 57, 65, 90, 97, 121, 95, 110, 
	48, 57, 65, 90, 97, 122, 95, 101, 
	48, 57, 65, 90, 97, 122, 95, 100, 
	48, 57, 65, 90, 97, 122, 95, 101, 
	48, 57, 65, 90, 97, 122, 95, 111, 
	48, 57, 65, 90, 97, 122, 95, 102, 
	48, 57, 65, 90, 97, 122, 95, 97, 
	114, 48, 57, 65, 90, 98, 122, 95, 
	116, 48, 57, 65, 90, 97, 122, 95, 
	105, 48, 57, 65, 90, 97, 122, 95, 
	99, 48, 57, 65, 90, 97, 122, 95, 
	117, 48, 57, 65, 90, 97, 122, 95, 
	99, 48, 57, 65, 90, 97, 122, 95, 
	116, 48, 57, 65, 90, 97, 122, 95, 
	105, 48, 57, 65, 90, 97, 122, 95, 
	116, 48, 57, 65, 90, 97, 122, 95, 
	99, 48, 57, 65, 90, 97, 122, 95, 
	104, 48, 57, 65, 90, 97, 122, 95, 
	114, 121, 48, 57, 65, 90, 97, 122, 
	95, 117, 48, 57, 65, 90, 97, 122, 
	95, 101, 48, 57, 65, 90, 97, 122, 
	95, 112, 48, 57, 65, 90, 97, 122, 
	95, 101, 48, 57, 65, 90, 97, 122, 
	95, 100, 111, 48, 57, 65, 90, 97, 
	122, 95, 101, 48, 57, 65, 90, 97, 
	122, 95, 102, 48, 57, 65, 90, 97, 
	122, 95, 102, 48, 57, 65, 90, 97, 
	122, 95, 110, 115, 48, 57, 65, 90, 
	97, 122, 95, 105, 115, 48, 57, 65, 
	90, 97, 122, 95, 111, 48, 57, 65, 
	90, 97, 122, 95, 110, 48, 57, 65, 
	90, 97, 122, 95, 97, 105, 48, 57, 
	65, 90, 98, 122, 95, 102, 48, 57, 
	65, 90, 97, 122, 95, 101, 48, 57, 
	65, 90, 97, 122, 95, 103, 48, 57, 
	65, 90, 97, 122, 95, 110, 48, 57, 
	65, 90, 97, 122, 95, 101, 48, 57, 
	65, 90, 97, 122, 95, 100, 48, 57, 
	65, 90, 97, 122, 95, 105, 48, 57, 
	65, 90, 97, 122, 95, 110, 48, 57, 
	65, 90, 97, 122, 95, 103, 48, 57, 
	65, 90, 97, 122, 95, 97, 111, 48, 
	57, 65, 90, 98, 122, 95, 114, 48, 
	57, 65, 90, 97, 122, 95, 105, 48, 
	57, 65, 90, 97, 122, 95, 97, 48, 
	57, 65, 90, 98, 122, 95, 110, 48, 
	57, 65, 90, 97, 122, 95, 116, 48, 
	57, 65, 90, 97, 122, 95, 105, 108, 
	48, 57, 65, 90, 97, 122, 95, 100, 
	48, 57, 65, 90, 97, 122, 95, 97, 
	48, 57, 65, 90, 98, 122, 95, 116, 
	48, 57, 65, 90, 97, 122, 95, 105, 
	48, 57, 65, 90, 97, 122, 95, 108, 
	48, 57, 65, 90, 97, 122, 95, 101, 
	48, 57, 65, 90, 97, 122, 95, 99, 
	104, 48, 57, 65, 90, 97, 122, 95, 
	104, 48, 57, 65, 90, 97, 122, 95, 
	97, 48, 57, 65, 90, 98, 122, 95, 
	114, 48, 57, 65, 90, 97, 122, 95, 
	48, 57, 65, 90, 97, 122, 95, 116, 
	48, 57, 65, 90, 97, 122, 95, 105, 
	48, 57, 65, 90, 97, 122, 95, 108, 
	48, 57, 65, 90, 97, 122, 95, 101, 
	48, 57, 65, 90, 97, 122, 61, 124, 
	0
};

static const char _axl_jnc_single_lengths[] = {
	3, 3, 1, 2, 3, 0, 38, 3, 
	1, 3, 1, 1, 2, 3, 1, 1, 
	2, 2, 1, 3, 1, 2, 0, 0, 
	2, 1, 1, 2, 1, 1, 1, 2, 
	2, 2, 4, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 5, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	3, 2, 2, 2, 2, 2, 3, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	3, 2, 2, 3, 2, 2, 2, 2, 
	2, 4, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 4, 2, 2, 2, 2, 
	2, 6, 2, 2, 2, 2, 2, 2, 
	2, 2, 3, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 3, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 4, 2, 2, 3, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 7, 
	2, 2, 2, 2, 2, 2, 3, 2, 
	2, 2, 2, 2, 2, 3, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	3, 2, 2, 2, 2, 3, 2, 2, 
	2, 3, 3, 2, 2, 3, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 3, 
	2, 2, 2, 2, 2, 3, 2, 2, 
	2, 2, 2, 2, 3, 2, 2, 2, 
	1, 2, 2, 2, 2, 2
};

static const char _axl_jnc_range_lengths[] = {
	0, 0, 0, 0, 0, 3, 3, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 1, 3, 
	0, 0, 0, 0, 0, 3, 0, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 0
};

static const short _axl_jnc_index_offsets[] = {
	0, 4, 8, 10, 13, 17, 21, 63, 
	67, 69, 73, 75, 77, 80, 84, 86, 
	88, 91, 94, 96, 100, 102, 106, 108, 
	112, 115, 117, 119, 122, 124, 129, 131, 
	137, 143, 149, 157, 163, 169, 175, 181, 
	187, 193, 199, 205, 211, 217, 223, 229, 
	238, 244, 250, 256, 262, 268, 274, 280, 
	286, 293, 299, 305, 311, 317, 323, 330, 
	336, 342, 348, 354, 360, 366, 372, 378, 
	384, 391, 397, 403, 410, 416, 422, 428, 
	434, 440, 448, 454, 460, 466, 472, 478, 
	484, 490, 496, 502, 510, 516, 522, 528, 
	534, 540, 550, 556, 562, 568, 574, 580, 
	586, 592, 598, 605, 611, 617, 623, 629, 
	635, 641, 647, 653, 659, 665, 671, 677, 
	684, 690, 696, 702, 708, 714, 720, 726, 
	732, 738, 746, 752, 758, 765, 771, 777, 
	783, 789, 795, 801, 807, 813, 819, 825, 
	831, 837, 843, 849, 855, 861, 867, 873, 
	884, 890, 896, 902, 908, 914, 920, 927, 
	933, 939, 945, 951, 957, 963, 970, 976, 
	982, 988, 994, 1000, 1006, 1012, 1018, 1024, 
	1030, 1037, 1043, 1049, 1055, 1061, 1068, 1074, 
	1080, 1086, 1093, 1100, 1106, 1112, 1119, 1125, 
	1131, 1137, 1143, 1149, 1155, 1161, 1167, 1173, 
	1180, 1186, 1192, 1198, 1204, 1210, 1217, 1223, 
	1229, 1235, 1241, 1247, 1253, 1260, 1266, 1272, 
	1278, 1283, 1289, 1295, 1301, 1307
};

static const short _axl_jnc_indicies[] = {
	2, 3, 4, 1, 6, 7, 8, 5, 
	10, 9, 12, 13, 11, 12, 13, 14, 
	11, 16, 16, 16, 15, 18, 19, 18, 
	18, 20, 21, 22, 23, 24, 25, 26, 
	27, 28, 29, 30, 32, 33, 34, 36, 
	35, 37, 38, 39, 40, 41, 42, 43, 
	44, 45, 46, 47, 48, 49, 50, 51, 
	52, 53, 54, 31, 35, 35, 17, 18, 
	18, 18, 55, 57, 56, 2, 3, 4, 
	1, 58, 1, 59, 56, 60, 61, 56, 
	6, 7, 8, 5, 62, 5, 63, 56, 
	64, 65, 56, 66, 67, 56, 68, 56, 
	11, 69, 70, 56, 71, 69, 73, 73, 
	31, 72, 31, 72, 16, 16, 16, 74, 
	75, 76, 56, 78, 77, 79, 56, 80, 
	81, 56, 83, 82, 35, 35, 35, 35, 
	0, 84, 56, 35, 86, 35, 35, 35, 
	85, 35, 87, 35, 35, 35, 85, 35, 
	88, 35, 35, 35, 85, 35, 89, 90, 
	91, 35, 35, 35, 85, 35, 92, 35, 
	35, 35, 85, 35, 93, 35, 35, 35, 
	85, 35, 94, 35, 35, 35, 85, 35, 
	95, 35, 35, 35, 85, 35, 96, 35, 
	35, 35, 85, 35, 97, 35, 35, 35, 
	85, 35, 98, 35, 35, 35, 85, 35, 
	99, 35, 35, 35, 85, 35, 100, 35, 
	35, 35, 85, 35, 101, 35, 35, 35, 
	85, 35, 102, 35, 35, 35, 85, 35, 
	103, 35, 35, 35, 85, 35, 104, 105, 
	106, 107, 35, 35, 35, 85, 35, 108, 
	35, 35, 35, 85, 35, 109, 35, 35, 
	35, 85, 35, 110, 35, 35, 35, 85, 
	35, 111, 35, 35, 35, 85, 35, 112, 
	35, 35, 35, 85, 35, 113, 35, 35, 
	35, 85, 35, 114, 35, 35, 35, 85, 
	35, 115, 35, 35, 35, 85, 35, 116, 
	117, 35, 35, 35, 85, 35, 118, 35, 
	35, 35, 85, 35, 119, 35, 35, 35, 
	85, 35, 120, 35, 35, 35, 85, 35, 
	121, 35, 35, 35, 85, 35, 122, 35, 
	35, 35, 85, 35, 123, 124, 35, 35, 
	35, 85, 35, 125, 35, 35, 35, 85, 
	35, 126, 35, 35, 35, 85, 35, 127, 
	35, 35, 35, 85, 35, 128, 35, 35, 
	35, 85, 35, 129, 35, 35, 35, 85, 
	35, 131, 35, 35, 35, 130, 35, 132, 
	35, 35, 35, 85, 35, 133, 35, 35, 
	35, 85, 35, 134, 35, 35, 35, 85, 
	35, 135, 136, 35, 35, 35, 85, 35, 
	137, 35, 35, 35, 85, 35, 138, 35, 
	35, 35, 85, 35, 139, 140, 35, 35, 
	35, 85, 35, 141, 35, 35, 35, 85, 
	35, 142, 35, 35, 35, 85, 35, 143, 
	35, 35, 35, 85, 35, 144, 35, 35, 
	35, 85, 35, 146, 35, 35, 35, 145, 
	35, 147, 148, 149, 35, 35, 35, 85, 
	35, 150, 35, 35, 35, 85, 35, 151, 
	35, 35, 35, 85, 35, 152, 35, 35, 
	35, 85, 35, 153, 35, 35, 35, 85, 
	35, 154, 35, 35, 35, 85, 35, 155, 
	35, 35, 35, 85, 35, 156, 35, 35, 
	35, 85, 35, 157, 35, 35, 35, 85, 
	35, 158, 35, 35, 35, 85, 35, 159, 
	160, 161, 35, 35, 35, 85, 35, 162, 
	35, 35, 35, 85, 35, 163, 35, 35, 
	35, 85, 35, 164, 35, 35, 35, 85, 
	35, 165, 35, 35, 35, 85, 35, 166, 
	35, 35, 35, 85, 168, 169, 170, 171, 
	35, 172, 35, 35, 35, 167, 173, 35, 
	35, 35, 35, 85, 174, 35, 35, 35, 
	35, 85, 175, 35, 35, 35, 35, 85, 
	35, 176, 35, 35, 35, 85, 35, 177, 
	35, 35, 35, 85, 35, 178, 35, 35, 
	35, 85, 35, 179, 35, 35, 35, 85, 
	35, 180, 35, 35, 35, 85, 35, 181, 
	182, 35, 35, 35, 85, 35, 183, 35, 
	35, 35, 85, 35, 184, 35, 35, 35, 
	85, 35, 185, 35, 35, 35, 85, 35, 
	186, 35, 35, 35, 85, 35, 187, 35, 
	35, 35, 85, 35, 188, 35, 35, 35, 
	85, 35, 189, 35, 35, 35, 85, 35, 
	190, 35, 35, 35, 85, 35, 191, 35, 
	35, 35, 85, 35, 192, 35, 35, 35, 
	85, 35, 193, 35, 35, 35, 85, 35, 
	194, 35, 35, 35, 85, 35, 195, 196, 
	35, 35, 35, 85, 35, 197, 35, 35, 
	35, 85, 35, 198, 35, 35, 35, 85, 
	35, 199, 35, 35, 35, 85, 35, 200, 
	35, 35, 35, 85, 35, 201, 35, 35, 
	35, 85, 35, 202, 35, 35, 35, 85, 
	35, 203, 35, 35, 35, 85, 35, 204, 
	35, 35, 35, 85, 35, 205, 35, 35, 
	35, 85, 35, 206, 207, 208, 35, 35, 
	35, 85, 35, 209, 35, 35, 35, 85, 
	35, 210, 35, 35, 35, 85, 35, 211, 
	212, 35, 35, 35, 85, 35, 213, 35, 
	35, 35, 85, 35, 214, 35, 35, 35, 
	85, 35, 215, 35, 35, 35, 85, 35, 
	216, 35, 35, 35, 85, 35, 217, 35, 
	35, 35, 85, 35, 218, 35, 35, 35, 
	85, 35, 219, 35, 35, 35, 85, 35, 
	220, 35, 35, 35, 85, 35, 221, 35, 
	35, 35, 85, 35, 222, 35, 35, 35, 
	85, 35, 223, 35, 35, 35, 85, 35, 
	224, 35, 35, 35, 85, 35, 225, 35, 
	35, 35, 85, 35, 226, 35, 35, 35, 
	85, 35, 227, 35, 35, 35, 85, 35, 
	228, 35, 35, 35, 85, 35, 229, 35, 
	35, 35, 85, 35, 230, 35, 35, 35, 
	85, 35, 231, 232, 233, 234, 235, 236, 
	35, 35, 35, 85, 35, 237, 35, 35, 
	35, 85, 35, 238, 35, 35, 35, 85, 
	35, 239, 35, 35, 35, 85, 35, 240, 
	35, 35, 35, 85, 35, 241, 35, 35, 
	35, 85, 35, 242, 35, 35, 35, 85, 
	35, 243, 244, 35, 35, 35, 85, 35, 
	245, 35, 35, 35, 85, 35, 246, 35, 
	35, 35, 85, 35, 247, 35, 35, 35, 
	85, 35, 248, 35, 35, 35, 85, 35, 
	249, 35, 35, 35, 85, 35, 250, 35, 
	35, 35, 85, 35, 251, 252, 35, 35, 
	35, 85, 35, 253, 35, 35, 35, 85, 
	35, 254, 35, 35, 35, 85, 35, 255, 
	35, 35, 35, 85, 35, 256, 35, 35, 
	35, 85, 35, 257, 35, 35, 35, 85, 
	35, 258, 35, 35, 35, 85, 35, 259, 
	35, 35, 35, 85, 35, 260, 35, 35, 
	35, 85, 35, 261, 35, 35, 35, 85, 
	35, 262, 35, 35, 35, 85, 35, 263, 
	264, 35, 35, 35, 85, 35, 265, 35, 
	35, 35, 85, 35, 266, 35, 35, 35, 
	85, 35, 267, 35, 35, 35, 85, 35, 
	268, 35, 35, 35, 85, 35, 269, 270, 
	35, 35, 35, 85, 35, 271, 35, 35, 
	35, 85, 35, 272, 35, 35, 35, 85, 
	35, 273, 35, 35, 35, 85, 35, 274, 
	275, 35, 35, 35, 85, 35, 276, 277, 
	35, 35, 35, 85, 35, 278, 35, 35, 
	35, 85, 35, 279, 35, 35, 35, 85, 
	35, 280, 281, 35, 35, 35, 85, 35, 
	282, 35, 35, 35, 85, 35, 283, 35, 
	35, 35, 85, 35, 284, 35, 35, 35, 
	85, 35, 285, 35, 35, 35, 85, 35, 
	286, 35, 35, 35, 85, 35, 287, 35, 
	35, 35, 85, 35, 288, 35, 35, 35, 
	85, 35, 289, 35, 35, 35, 85, 35, 
	290, 35, 35, 35, 85, 35, 291, 292, 
	35, 35, 35, 85, 35, 293, 35, 35, 
	35, 85, 35, 294, 35, 35, 35, 85, 
	35, 295, 35, 35, 35, 85, 35, 296, 
	35, 35, 35, 85, 35, 297, 35, 35, 
	35, 85, 35, 298, 299, 35, 35, 35, 
	85, 35, 300, 35, 35, 35, 85, 35, 
	301, 35, 35, 35, 85, 35, 302, 35, 
	35, 35, 85, 35, 303, 35, 35, 35, 
	85, 35, 304, 35, 35, 35, 85, 35, 
	305, 35, 35, 35, 85, 35, 306, 307, 
	35, 35, 35, 85, 35, 308, 35, 35, 
	35, 85, 35, 309, 35, 35, 35, 85, 
	35, 310, 35, 35, 35, 85, 311, 35, 
	35, 35, 85, 35, 312, 35, 35, 35, 
	85, 35, 313, 35, 35, 35, 85, 35, 
	314, 35, 35, 35, 85, 35, 315, 35, 
	35, 35, 85, 316, 317, 56, 0
};

static const unsigned char _axl_jnc_trans_targs[] = {
	6, 0, 6, 6, 10, 1, 6, 6, 
	14, 6, 6, 3, 3, 4, 6, 6, 
	23, 6, 7, 6, 8, 9, 11, 12, 
	13, 15, 16, 17, 18, 19, 21, 22, 
	24, 26, 27, 29, 30, 31, 34, 47, 
	62, 72, 81, 89, 91, 106, 119, 129, 
	146, 151, 176, 185, 199, 212, 221, 6, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 6, 2, 20, 6, 6, 
	6, 5, 6, 25, 6, 6, 6, 6, 
	6, 28, 6, 6, 6, 6, 32, 33, 
	29, 35, 42, 44, 36, 37, 38, 39, 
	40, 41, 29, 43, 29, 45, 46, 29, 
	48, 50, 52, 55, 49, 29, 51, 29, 
	53, 54, 29, 56, 57, 58, 29, 59, 
	60, 61, 29, 63, 68, 64, 65, 66, 
	67, 29, 6, 69, 70, 71, 29, 73, 
	75, 74, 29, 76, 79, 77, 78, 29, 
	80, 6, 29, 82, 85, 88, 83, 84, 
	29, 86, 87, 29, 29, 90, 29, 29, 
	92, 96, 93, 94, 95, 29, 97, 6, 
	98, 99, 100, 29, 101, 29, 29, 29, 
	102, 103, 104, 105, 29, 107, 117, 108, 
	109, 110, 111, 112, 113, 114, 115, 116, 
	29, 118, 29, 120, 127, 121, 122, 123, 
	124, 125, 126, 29, 128, 29, 130, 132, 
	142, 131, 29, 133, 137, 134, 135, 136, 
	29, 138, 139, 140, 141, 29, 143, 144, 
	145, 29, 147, 148, 149, 150, 29, 152, 
	154, 155, 158, 165, 172, 153, 29, 29, 
	156, 157, 29, 159, 162, 160, 161, 29, 
	163, 164, 29, 166, 169, 167, 168, 29, 
	170, 171, 29, 173, 174, 175, 29, 177, 
	179, 178, 29, 180, 181, 182, 184, 183, 
	29, 29, 186, 196, 187, 189, 188, 29, 
	190, 192, 191, 29, 193, 194, 195, 29, 
	197, 198, 29, 200, 205, 201, 202, 203, 
	204, 29, 206, 207, 29, 208, 209, 210, 
	211, 29, 213, 218, 214, 215, 216, 217, 
	29, 219, 220, 29, 6, 6
};

static const short _axl_jnc_trans_actions[] = {
	85, 0, 90, 49, 264, 0, 87, 47, 
	261, 83, 45, 0, 1, 0, 51, 81, 
	0, 53, 0, 93, 0, 267, 0, 0, 
	267, 0, 0, 0, 7, 7, 7, 0, 
	0, 0, 0, 258, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 77, 
	79, 11, 69, 29, 19, 39, 67, 25, 
	21, 31, 23, 33, 0, 0, 27, 75, 
	71, 0, 73, 0, 13, 61, 35, 9, 
	15, 0, 63, 37, 41, 65, 0, 0, 
	147, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 138, 0, 156, 0, 0, 231, 
	0, 0, 0, 0, 0, 243, 0, 177, 
	0, 0, 198, 0, 0, 0, 123, 0, 
	0, 0, 234, 0, 0, 0, 0, 0, 
	0, 246, 59, 0, 0, 0, 174, 0, 
	0, 0, 222, 0, 0, 0, 0, 108, 
	0, 57, 189, 0, 0, 0, 0, 0, 
	252, 0, 0, 171, 225, 0, 207, 219, 
	0, 0, 0, 0, 0, 96, 0, 55, 
	0, 0, 0, 159, 0, 162, 165, 168, 
	0, 0, 0, 0, 201, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	135, 0, 183, 0, 0, 0, 0, 0, 
	0, 0, 0, 99, 0, 255, 0, 0, 
	0, 0, 105, 0, 0, 0, 0, 0, 
	120, 0, 0, 0, 0, 204, 0, 0, 
	0, 117, 0, 0, 0, 0, 237, 0, 
	0, 0, 0, 0, 0, 0, 141, 210, 
	0, 0, 180, 0, 0, 0, 0, 129, 
	0, 0, 213, 0, 0, 0, 0, 114, 
	0, 0, 192, 0, 0, 0, 240, 0, 
	0, 0, 249, 0, 0, 0, 0, 0, 
	111, 216, 0, 0, 0, 0, 0, 195, 
	0, 0, 0, 144, 0, 0, 0, 132, 
	0, 0, 102, 0, 0, 0, 0, 0, 
	0, 153, 0, 0, 150, 0, 0, 0, 
	0, 126, 0, 0, 0, 0, 0, 0, 
	186, 0, 0, 228, 43, 17
};

static const short _axl_jnc_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 3, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0
};

static const short _axl_jnc_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 5, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0
};

static const short _axl_jnc_eof_trans[] = {
	1, 1, 10, 10, 10, 16, 0, 56, 
	57, 57, 59, 57, 57, 57, 63, 57, 
	57, 57, 57, 57, 72, 73, 73, 75, 
	57, 78, 57, 57, 83, 1, 57, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 131, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	146, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 168, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 57
};

static const int axl_jnc_start = 6;
static const int axl_jnc_first_final = 6;
static const int axl_jnc_error = -1;

static const int axl_jnc_en_main = 6;


/* #line 165 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */


//.............................................................................

void 
CLexer::Init ()
{
	
/* #line 728 "axl_jnc_Lexer.cpp" */
	{
	cs = axl_jnc_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 173 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
}

bool
CLexer::Exec ()
{
	
/* #line 734 "axl_jnc_Lexer.cpp" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
_resume:
	_acts = _axl_jnc_actions + _axl_jnc_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 2:
/* #line 1 "NONE" */
	{ts = p;}
	break;
/* #line 751 "axl_jnc_Lexer.cpp" */
		}
	}

	_keys = _axl_jnc_trans_keys + _axl_jnc_key_offsets[cs];
	_trans = _axl_jnc_index_offsets[cs];

	_klen = _axl_jnc_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _axl_jnc_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _axl_jnc_indicies[_trans];
_eof_trans:
	cs = _axl_jnc_trans_targs[_trans];

	if ( _axl_jnc_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _axl_jnc_actions + _axl_jnc_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 44 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{ NewLine (p + 1); }
	break;
	case 3:
/* #line 1 "NONE" */
	{te = p+1;}
	break;
	case 4:
/* #line 58 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 1;}
	break;
	case 5:
/* #line 59 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 2;}
	break;
	case 6:
/* #line 60 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 3;}
	break;
	case 7:
/* #line 61 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 4;}
	break;
	case 8:
/* #line 62 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 5;}
	break;
	case 9:
/* #line 64 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 6;}
	break;
	case 10:
/* #line 65 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 7;}
	break;
	case 11:
/* #line 67 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 8;}
	break;
	case 12:
/* #line 68 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 9;}
	break;
	case 13:
/* #line 70 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 10;}
	break;
	case 14:
/* #line 71 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 11;}
	break;
	case 15:
/* #line 72 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 12;}
	break;
	case 16:
/* #line 73 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 13;}
	break;
	case 17:
/* #line 74 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 14;}
	break;
	case 18:
/* #line 75 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 15;}
	break;
	case 19:
/* #line 76 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 16;}
	break;
	case 20:
/* #line 77 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 17;}
	break;
	case 21:
/* #line 79 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 18;}
	break;
	case 22:
/* #line 80 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 19;}
	break;
	case 23:
/* #line 81 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 20;}
	break;
	case 24:
/* #line 82 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 21;}
	break;
	case 25:
/* #line 83 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 22;}
	break;
	case 26:
/* #line 84 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 23;}
	break;
	case 27:
/* #line 85 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 24;}
	break;
	case 28:
/* #line 86 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 25;}
	break;
	case 29:
/* #line 87 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 26;}
	break;
	case 30:
/* #line 88 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 27;}
	break;
	case 31:
/* #line 90 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 29;}
	break;
	case 32:
/* #line 91 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 30;}
	break;
	case 33:
/* #line 92 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 31;}
	break;
	case 34:
/* #line 93 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 32;}
	break;
	case 35:
/* #line 96 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 34;}
	break;
	case 36:
/* #line 97 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 35;}
	break;
	case 37:
/* #line 98 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 36;}
	break;
	case 38:
/* #line 99 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 37;}
	break;
	case 39:
/* #line 100 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 38;}
	break;
	case 40:
/* #line 101 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 39;}
	break;
	case 41:
/* #line 102 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 40;}
	break;
	case 42:
/* #line 103 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 41;}
	break;
	case 43:
/* #line 105 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 42;}
	break;
	case 44:
/* #line 106 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 43;}
	break;
	case 45:
/* #line 108 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 44;}
	break;
	case 46:
/* #line 109 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 45;}
	break;
	case 47:
/* #line 110 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 46;}
	break;
	case 48:
/* #line 111 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 47;}
	break;
	case 49:
/* #line 113 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 49;}
	break;
	case 50:
/* #line 114 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 50;}
	break;
	case 51:
/* #line 115 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 51;}
	break;
	case 52:
/* #line 116 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 52;}
	break;
	case 53:
/* #line 117 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 53;}
	break;
	case 54:
/* #line 118 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 54;}
	break;
	case 55:
/* #line 120 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 55;}
	break;
	case 56:
/* #line 121 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 56;}
	break;
	case 57:
/* #line 122 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 57;}
	break;
	case 58:
/* #line 150 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 79;}
	break;
	case 59:
/* #line 151 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 80;}
	break;
	case 60:
/* #line 152 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 81;}
	break;
	case 61:
/* #line 161 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{act = 87;}
	break;
	case 62:
/* #line 126 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_Eq); }}
	break;
	case 63:
/* #line 127 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_Ne); }}
	break;
	case 64:
/* #line 128 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_Le); }}
	break;
	case 65:
/* #line 129 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_Ge); }}
	break;
	case 66:
/* #line 130 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_LogicalOr); }}
	break;
	case 67:
/* #line 131 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_LogicalAnd); }}
	break;
	case 68:
/* #line 132 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_Inc); }}
	break;
	case 69:
/* #line 133 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_Dec); }}
	break;
	case 70:
/* #line 136 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_MulAssign); }}
	break;
	case 71:
/* #line 137 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_DivAssign); }}
	break;
	case 72:
/* #line 138 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_ModAssign); }}
	break;
	case 73:
/* #line 139 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_AddAssign); }}
	break;
	case 74:
/* #line 140 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_SubAssign); }}
	break;
	case 75:
/* #line 141 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_ShlAssign); }}
	break;
	case 76:
/* #line 142 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_ShrAssign); }}
	break;
	case 77:
/* #line 143 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_AndAssign); }}
	break;
	case 78:
/* #line 144 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_XorAssign); }}
	break;
	case 79:
/* #line 145 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_OrAssign); }}
	break;
	case 80:
/* #line 146 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_Ellipsis); }}
	break;
	case 81:
/* #line 151 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateCharToken (EToken_Integer); }}
	break;
	case 82:
/* #line 152 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateStringToken (EToken_Literal, 1, 1); }}
	break;
	case 83:
/* #line 157 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;}
	break;
	case 84:
/* #line 159 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;}
	break;
	case 85:
/* #line 161 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p+1;{ CreateToken (ts [0]); }}
	break;
	case 86:
/* #line 89 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateToken (EToken_Int); }}
	break;
	case 87:
/* #line 95 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateToken (EToken_Enum); }}
	break;
	case 88:
/* #line 112 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateToken (EToken_Do); }}
	break;
	case 89:
/* #line 134 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateToken (EToken_Shl); }}
	break;
	case 90:
/* #line 135 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateToken (EToken_Shr); }}
	break;
	case 91:
/* #line 150 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateStringToken (EToken_Identifier); }}
	break;
	case 92:
/* #line 151 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateCharToken (EToken_Integer); }}
	break;
	case 93:
/* #line 152 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateStringToken (EToken_Literal, 1, 1); }}
	break;
	case 94:
/* #line 153 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateIntegerToken (10); }}
	break;
	case 95:
/* #line 154 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateIntegerToken (16, 2); }}
	break;
	case 96:
/* #line 156 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;}
	break;
	case 97:
/* #line 159 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;}
	break;
	case 98:
/* #line 161 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{te = p;p--;{ CreateToken (ts [0]); }}
	break;
	case 99:
/* #line 153 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{{p = ((te))-1;}{ CreateIntegerToken (10); }}
	break;
	case 100:
/* #line 161 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */
	{{p = ((te))-1;}{ CreateToken (ts [0]); }}
	break;
	case 101:
/* #line 1 "NONE" */
	{	switch( act ) {
	case 1:
	{{p = ((te))-1;} CreateToken (EToken_Import); }
	break;
	case 2:
	{{p = ((te))-1;} CreateToken (EToken_Namespace); }
	break;
	case 3:
	{{p = ((te))-1;} CreateToken (EToken_Using); }
	break;
	case 4:
	{{p = ((te))-1;} CreateToken (EToken_Pack); }
	break;
	case 5:
	{{p = ((te))-1;} CreateToken (EToken_Endian); }
	break;
	case 6:
	{{p = ((te))-1;} CreateToken (EToken_Typedef); }
	break;
	case 7:
	{{p = ((te))-1;} CreateToken (EToken_Static); }
	break;
	case 8:
	{{p = ((te))-1;} CreateToken (EToken_Public); }
	break;
	case 9:
	{{p = ((te))-1;} CreateToken (EToken_Private); }
	break;
	case 10:
	{{p = ((te))-1;} CreateToken (EToken_Const); }
	break;
	case 11:
	{{p = ((te))-1;} CreateToken (EToken_Volatile); }
	break;
	case 12:
	{{p = ((te))-1;} CreateToken (EToken_Signed); }
	break;
	case 13:
	{{p = ((te))-1;} CreateToken (EToken_Unsigned); }
	break;
	case 14:
	{{p = ((te))-1;} CreateToken (EToken_LittleEndian); }
	break;
	case 15:
	{{p = ((te))-1;} CreateToken (EToken_BigEndian); }
	break;
	case 16:
	{{p = ((te))-1;} CreateToken (EToken_Safe); }
	break;
	case 17:
	{{p = ((te))-1;} CreateToken (EToken_Unsafe); }
	break;
	case 18:
	{{p = ((te))-1;} CreateToken (EToken_Auto); }
	break;
	case 19:
	{{p = ((te))-1;} CreateToken (EToken_Void); }
	break;
	case 20:
	{{p = ((te))-1;} CreateToken (EToken_Variant); }
	break;
	case 21:
	{{p = ((te))-1;} CreateToken (EToken_Bool); }
	break;
	case 22:
	{{p = ((te))-1;} CreateToken (EToken_Int8); }
	break;
	case 23:
	{{p = ((te))-1;} CreateToken (EToken_Int16); }
	break;
	case 24:
	{{p = ((te))-1;} CreateToken (EToken_Int32); }
	break;
	case 25:
	{{p = ((te))-1;} CreateToken (EToken_Int64); }
	break;
	case 26:
	{{p = ((te))-1;} CreateToken (EToken_Float); }
	break;
	case 27:
	{{p = ((te))-1;} CreateToken (EToken_Double); }
	break;
	case 29:
	{{p = ((te))-1;} CreateToken (EToken_Char); }
	break;
	case 30:
	{{p = ((te))-1;} CreateToken (EToken_Short); }
	break;
	case 31:
	{{p = ((te))-1;} CreateToken (EToken_Long); }
	break;
	case 32:
	{{p = ((te))-1;} CreateToken (EToken_WChar); }
	break;
	case 34:
	{{p = ((te))-1;} CreateToken (EToken_EnumC); }
	break;
	case 35:
	{{p = ((te))-1;} CreateToken (EToken_Struct); }
	break;
	case 36:
	{{p = ((te))-1;} CreateToken (EToken_Union); }
	break;
	case 37:
	{{p = ((te))-1;} CreateToken (EToken_Class); }
	break;
	case 38:
	{{p = ((te))-1;} CreateToken (EToken_Interface); }
	break;
	case 39:
	{{p = ((te))-1;} CreateToken (EToken_Property); }
	break;
	case 40:
	{{p = ((te))-1;} CreateToken (EToken_Get); }
	break;
	case 41:
	{{p = ((te))-1;} CreateToken (EToken_Set); }
	break;
	case 42:
	{{p = ((te))-1;} CreateToken (EToken_SizeOf); }
	break;
	case 43:
	{{p = ((te))-1;} CreateToken (EToken_TypeOf); }
	break;
	case 44:
	{{p = ((te))-1;} CreateToken (EToken_If); }
	break;
	case 45:
	{{p = ((te))-1;} CreateToken (EToken_Else); }
	break;
	case 46:
	{{p = ((te))-1;} CreateToken (EToken_For); }
	break;
	case 47:
	{{p = ((te))-1;} CreateToken (EToken_While); }
	break;
	case 49:
	{{p = ((te))-1;} CreateToken (EToken_Break); }
	break;
	case 50:
	{{p = ((te))-1;} CreateToken (EToken_Continue); }
	break;
	case 51:
	{{p = ((te))-1;} CreateToken (EToken_Return); }
	break;
	case 52:
	{{p = ((te))-1;} CreateToken (EToken_Switch); }
	break;
	case 53:
	{{p = ((te))-1;} CreateToken (EToken_Case); }
	break;
	case 54:
	{{p = ((te))-1;} CreateToken (EToken_Default); }
	break;
	case 55:
	{{p = ((te))-1;} CreateToken (EToken_True); }
	break;
	case 56:
	{{p = ((te))-1;} CreateToken (EToken_False); }
	break;
	case 57:
	{{p = ((te))-1;} CreateToken (EToken_Null); }
	break;
	case 79:
	{{p = ((te))-1;} CreateStringToken (EToken_Identifier); }
	break;
	case 80:
	{{p = ((te))-1;} CreateCharToken (EToken_Integer); }
	break;
	case 81:
	{{p = ((te))-1;} CreateStringToken (EToken_Literal, 1, 1); }
	break;
	case 87:
	{{p = ((te))-1;} CreateToken (ts [0]); }
	break;
	}
	}
	break;
/* #line 1292 "axl_jnc_Lexer.cpp" */
		}
	}

_again:
	_acts = _axl_jnc_actions + _axl_jnc_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 1:
/* #line 1 "NONE" */
	{ts = 0;}
	break;
/* #line 1303 "axl_jnc_Lexer.cpp" */
		}
	}

	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _axl_jnc_eof_trans[cs] > 0 ) {
		_trans = _axl_jnc_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	}

/* #line 179 "C:/root/Projects/Ninja/PRJ_V.GIT/axl3/axl_jnc/axl_jnc_Lexer.rl" */

	bool Result = cs != axl_jnc_error;
	if (!Result)
	{
		CToken* pToken = CreateToken (EToken_Error);
		pToken->m_Data.m_Error = err::CError ("lexer error");
	}

	return Result;
}


//.............................................................................

} // namespace axl {
} // namespace jnc {
