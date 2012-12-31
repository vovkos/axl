
/* #line 1 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
#include "stdafx.h"
#include "axl_pg_Lexer.h"

// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)

namespace axl {
namespace pg { 

//.............................................................................


/* #line 2 "axl_pg_Lexer.cpp" */
static const char _axl_pg_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 7, 1, 8, 1, 9, 1, 
	10, 1, 11, 1, 12, 1, 13, 1, 
	14, 1, 15, 1, 16, 1, 17, 1, 
	18, 1, 19, 1, 26, 1, 27, 1, 
	28, 1, 29, 1, 30, 1, 31, 1, 
	32, 1, 33, 1, 34, 1, 35, 1, 
	36, 1, 37, 1, 38, 1, 39, 1, 
	40, 1, 41, 1, 61, 1, 63, 1, 
	64, 1, 65, 1, 66, 1, 67, 1, 
	68, 1, 69, 1, 70, 1, 71, 1, 
	72, 1, 73, 1, 74, 1, 75, 1, 
	76, 2, 0, 7, 2, 0, 8, 2, 
	0, 14, 2, 0, 26, 2, 0, 27, 
	2, 0, 33, 2, 0, 62, 2, 0, 
	63, 2, 0, 64, 2, 3, 4, 2, 
	3, 5, 2, 3, 6, 2, 3, 20, 
	2, 3, 21, 2, 3, 22, 2, 3, 
	23, 2, 3, 24, 2, 3, 25, 2, 
	3, 42, 2, 3, 43, 2, 3, 44, 
	2, 3, 45, 2, 3, 46, 2, 3, 
	47, 2, 3, 48, 2, 3, 49, 2, 
	3, 50, 2, 3, 51, 2, 3, 52, 
	2, 3, 53, 2, 3, 54, 2, 3, 
	55, 2, 3, 56, 2, 3, 57, 2, 
	3, 58, 2, 3, 59, 2, 3, 60
	
};

static const short _axl_pg_key_offsets[] = {
	0, 3, 6, 8, 11, 17, 20, 23, 
	26, 29, 56, 59, 62, 63, 66, 67, 
	69, 70, 74, 76, 82, 89, 97, 105, 
	113, 121, 129, 137, 145, 153, 161, 169, 
	177, 185, 193, 202, 210, 218, 226, 234, 
	242, 250, 258, 266, 274, 282, 290, 298, 
	306, 315, 323, 331, 339, 347, 355, 363, 
	371, 379, 387, 395, 403, 412, 420, 428, 
	436, 444, 452, 460, 468, 476, 484, 492, 
	500, 508, 516, 524, 532, 540, 548, 556, 
	564, 572, 580, 588, 596, 606, 609, 610, 
	613, 614, 616, 617, 618, 631, 634, 635, 
	644, 646, 653, 661, 669, 677, 685, 693, 
	701, 704, 705, 707, 708
};

static const char _axl_pg_trans_keys[] = {
	10, 34, 92, 10, 39, 92, 10, 42, 
	10, 42, 47, 48, 57, 65, 70, 97, 
	102, 10, 34, 92, 10, 39, 92, 10, 
	34, 92, 10, 39, 92, 9, 10, 13, 
	32, 34, 39, 47, 48, 76, 95, 97, 
	99, 100, 101, 105, 108, 110, 112, 114, 
	115, 117, 49, 57, 65, 90, 98, 122, 
	9, 13, 32, 10, 34, 92, 10, 10, 
	39, 92, 10, 42, 47, 10, 88, 120, 
	48, 57, 48, 57, 48, 57, 65, 70, 
	97, 102, 95, 48, 57, 65, 90, 97, 
	122, 76, 95, 48, 57, 65, 90, 97, 
	122, 95, 110, 48, 57, 65, 90, 97, 
	122, 95, 121, 48, 57, 65, 90, 97, 
	122, 95, 108, 48, 57, 65, 90, 97, 
	122, 95, 97, 48, 57, 65, 90, 98, 
	122, 95, 115, 48, 57, 65, 90, 97, 
	122, 95, 115, 48, 57, 65, 90, 97, 
	122, 95, 101, 48, 57, 65, 90, 97, 
	122, 95, 102, 48, 57, 65, 90, 97, 
	122, 95, 97, 48, 57, 65, 90, 98, 
	122, 95, 117, 48, 57, 65, 90, 97, 
	122, 95, 108, 48, 57, 65, 90, 97, 
	122, 95, 116, 48, 57, 65, 90, 97, 
	122, 95, 110, 112, 48, 57, 65, 90, 
	97, 122, 95, 116, 48, 57, 65, 90, 
	97, 122, 95, 101, 48, 57, 65, 90, 
	97, 122, 95, 114, 48, 57, 65, 90, 
	97, 122, 95, 115, 48, 57, 65, 90, 
	97, 122, 95, 105, 48, 57, 65, 90, 
	97, 122, 95, 108, 48, 57, 65, 90, 
	97, 122, 95, 111, 48, 57, 65, 90, 
	97, 122, 95, 110, 48, 57, 65, 90, 
	97, 122, 95, 109, 48, 57, 65, 90, 
	97, 122, 95, 112, 48, 57, 65, 90, 
	97, 122, 95, 111, 48, 57, 65, 90, 
	97, 122, 95, 114, 48, 57, 65, 90, 
	97, 122, 95, 116, 48, 57, 65, 90, 
	97, 122, 95, 101, 111, 48, 57, 65, 
	90, 97, 122, 95, 97, 48, 57, 65, 
	90, 98, 122, 95, 118, 48, 57, 65, 
	90, 97, 122, 95, 101, 48, 57, 65, 
	90, 97, 122, 95, 99, 48, 57, 65, 
	90, 97, 122, 95, 97, 48, 57, 65, 
	90, 98, 122, 95, 108, 48, 57, 65, 
	90, 97, 122, 95, 111, 48, 57, 65, 
	90, 97, 122, 95, 97, 48, 57, 65, 
	90, 98, 122, 95, 115, 48, 57, 65, 
	90, 97, 122, 95, 116, 48, 57, 65, 
	90, 97, 122, 95, 114, 48, 57, 65, 
	90, 97, 122, 95, 97, 105, 48, 57, 
	65, 90, 98, 122, 95, 103, 48, 57, 
	65, 90, 97, 122, 95, 109, 48, 57, 
	65, 90, 97, 122, 95, 97, 48, 57, 
	65, 90, 98, 122, 95, 111, 48, 57, 
	65, 90, 97, 122, 95, 114, 48, 57, 
	65, 90, 97, 122, 95, 105, 48, 57, 
	65, 90, 97, 122, 95, 116, 48, 57, 
	65, 90, 97, 122, 95, 121, 48, 57, 
	65, 90, 97, 122, 95, 101, 48, 57, 
	65, 90, 97, 122, 95, 115, 48, 57, 
	65, 90, 97, 122, 95, 111, 48, 57, 
	65, 90, 97, 122, 95, 108, 48, 57, 
	65, 90, 97, 122, 95, 118, 48, 57, 
	65, 90, 97, 122, 95, 101, 48, 57, 
	65, 90, 97, 122, 95, 114, 48, 57, 
	65, 90, 97, 122, 95, 116, 48, 57, 
	65, 90, 97, 122, 95, 97, 48, 57, 
	65, 90, 98, 122, 95, 114, 48, 57, 
	65, 90, 97, 122, 95, 116, 48, 57, 
	65, 90, 97, 122, 95, 115, 48, 57, 
	65, 90, 97, 122, 95, 105, 48, 57, 
	65, 90, 97, 122, 95, 110, 48, 57, 
	65, 90, 97, 122, 95, 103, 48, 57, 
	65, 90, 97, 122, 10, 34, 39, 46, 
	60, 62, 123, 125, 40, 41, 10, 34, 
	92, 10, 10, 39, 92, 10, 62, 125, 
	46, 46, 10, 34, 36, 39, 44, 46, 
	59, 60, 62, 123, 125, 40, 41, 10, 
	34, 92, 10, 95, 97, 108, 48, 57, 
	65, 90, 98, 122, 48, 57, 95, 48, 
	57, 65, 90, 97, 122, 95, 114, 48, 
	57, 65, 90, 97, 122, 95, 103, 48, 
	57, 65, 90, 97, 122, 95, 111, 48, 
	57, 65, 90, 97, 122, 95, 99, 48, 
	57, 65, 90, 97, 122, 95, 97, 48, 
	57, 65, 90, 98, 122, 95, 108, 48, 
	57, 65, 90, 97, 122, 10, 39, 92, 
	10, 62, 125, 46, 46, 0
};

static const char _axl_pg_single_lengths[] = {
	3, 3, 2, 3, 0, 3, 3, 3, 
	3, 21, 3, 3, 1, 3, 1, 2, 
	1, 2, 0, 0, 1, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 3, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	3, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 3, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 8, 3, 1, 3, 
	1, 2, 1, 1, 11, 3, 1, 3, 
	0, 1, 2, 2, 2, 2, 2, 2, 
	3, 1, 2, 1, 1
};

static const char _axl_pg_range_lengths[] = {
	0, 0, 0, 0, 3, 0, 0, 0, 
	0, 3, 0, 0, 0, 0, 0, 0, 
	0, 1, 1, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 1, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 3, 
	1, 3, 3, 3, 3, 3, 3, 3, 
	0, 0, 0, 0, 0
};

static const short _axl_pg_index_offsets[] = {
	0, 4, 8, 11, 15, 19, 23, 27, 
	31, 35, 60, 64, 68, 70, 74, 76, 
	79, 81, 85, 87, 91, 96, 102, 108, 
	114, 120, 126, 132, 138, 144, 150, 156, 
	162, 168, 174, 181, 187, 193, 199, 205, 
	211, 217, 223, 229, 235, 241, 247, 253, 
	259, 266, 272, 278, 284, 290, 296, 302, 
	308, 314, 320, 326, 332, 339, 345, 351, 
	357, 363, 369, 375, 381, 387, 393, 399, 
	405, 411, 417, 423, 429, 435, 441, 447, 
	453, 459, 465, 471, 477, 487, 491, 493, 
	497, 499, 502, 504, 506, 519, 523, 525, 
	532, 534, 539, 545, 551, 557, 563, 569, 
	575, 579, 581, 584, 586
};

static const unsigned char _axl_pg_indicies[] = {
	2, 3, 4, 1, 6, 7, 8, 5, 
	11, 12, 10, 11, 12, 13, 10, 15, 
	15, 15, 14, 18, 19, 20, 17, 22, 
	23, 24, 21, 27, 28, 29, 26, 31, 
	32, 33, 30, 35, 36, 35, 35, 37, 
	38, 39, 40, 43, 42, 44, 45, 46, 
	47, 48, 49, 50, 51, 52, 53, 54, 
	41, 42, 42, 34, 35, 35, 35, 55, 
	2, 3, 4, 1, 57, 1, 6, 7, 
	8, 5, 58, 5, 10, 59, 56, 60, 
	59, 62, 62, 41, 61, 41, 61, 15, 
	15, 15, 63, 42, 42, 42, 42, 0, 
	65, 42, 42, 42, 42, 64, 42, 66, 
	42, 42, 42, 64, 42, 67, 42, 42, 
	42, 64, 42, 68, 42, 42, 42, 64, 
	42, 69, 42, 42, 42, 64, 42, 70, 
	42, 42, 42, 64, 42, 71, 42, 42, 
	42, 64, 42, 72, 42, 42, 42, 64, 
	42, 73, 42, 42, 42, 64, 42, 74, 
	42, 42, 42, 64, 42, 75, 42, 42, 
	42, 64, 42, 76, 42, 42, 42, 64, 
	42, 77, 42, 42, 42, 64, 42, 78, 
	79, 42, 42, 42, 64, 42, 80, 42, 
	42, 42, 64, 42, 81, 42, 42, 42, 
	64, 42, 82, 42, 42, 42, 64, 42, 
	83, 42, 42, 42, 64, 42, 84, 42, 
	42, 42, 64, 42, 85, 42, 42, 42, 
	64, 42, 86, 42, 42, 42, 64, 42, 
	87, 42, 42, 42, 64, 42, 88, 42, 
	42, 42, 64, 42, 89, 42, 42, 42, 
	64, 42, 90, 42, 42, 42, 64, 42, 
	91, 42, 42, 42, 64, 42, 92, 42, 
	42, 42, 64, 42, 93, 94, 42, 42, 
	42, 64, 42, 95, 42, 42, 42, 64, 
	42, 96, 42, 42, 42, 64, 42, 97, 
	42, 42, 42, 64, 42, 98, 42, 42, 
	42, 64, 42, 99, 42, 42, 42, 64, 
	42, 100, 42, 42, 42, 64, 42, 101, 
	42, 42, 42, 64, 42, 102, 42, 42, 
	42, 64, 42, 103, 42, 42, 42, 64, 
	42, 104, 42, 42, 42, 64, 42, 105, 
	42, 42, 42, 64, 42, 106, 107, 42, 
	42, 42, 64, 42, 108, 42, 42, 42, 
	64, 42, 109, 42, 42, 42, 64, 42, 
	110, 42, 42, 42, 64, 42, 111, 42, 
	42, 42, 64, 42, 112, 42, 42, 42, 
	64, 42, 113, 42, 42, 42, 64, 42, 
	114, 42, 42, 42, 64, 42, 115, 42, 
	42, 42, 64, 42, 116, 42, 42, 42, 
	64, 42, 117, 42, 42, 42, 64, 42, 
	118, 42, 42, 42, 64, 42, 119, 42, 
	42, 42, 64, 42, 120, 42, 42, 42, 
	64, 42, 121, 42, 42, 42, 64, 42, 
	122, 42, 42, 42, 64, 42, 123, 42, 
	42, 42, 64, 42, 124, 42, 42, 42, 
	64, 42, 125, 42, 42, 42, 64, 42, 
	126, 42, 42, 42, 64, 42, 127, 42, 
	42, 42, 64, 42, 128, 42, 42, 42, 
	64, 42, 129, 42, 42, 42, 64, 42, 
	130, 42, 42, 42, 64, 132, 133, 134, 
	136, 137, 135, 138, 135, 135, 131, 18, 
	19, 20, 17, 140, 17, 22, 23, 24, 
	21, 141, 21, 142, 143, 139, 145, 144, 
	146, 144, 148, 149, 150, 151, 152, 153, 
	152, 154, 152, 155, 152, 152, 147, 27, 
	28, 29, 26, 157, 26, 160, 161, 162, 
	159, 160, 160, 158, 159, 163, 160, 160, 
	160, 160, 25, 160, 165, 160, 160, 160, 
	164, 160, 166, 160, 160, 160, 164, 160, 
	167, 160, 160, 160, 164, 160, 168, 160, 
	160, 160, 164, 160, 169, 160, 160, 160, 
	164, 160, 170, 160, 160, 160, 164, 31, 
	32, 33, 30, 171, 30, 172, 173, 156, 
	175, 174, 176, 174, 0
};

static const char _axl_pg_trans_targs[] = {
	9, 0, 9, 9, 12, 1, 9, 9, 
	14, 9, 2, 2, 3, 9, 9, 19, 
	84, 5, 84, 84, 86, 6, 84, 84, 
	88, 92, 7, 92, 92, 94, 8, 92, 
	92, 105, 9, 10, 9, 11, 13, 15, 
	17, 18, 20, 21, 22, 24, 28, 34, 
	43, 48, 55, 59, 69, 76, 80, 9, 
	9, 9, 9, 16, 9, 9, 4, 9, 
	9, 20, 23, 20, 25, 26, 27, 20, 
	29, 30, 31, 32, 33, 20, 35, 38, 
	36, 37, 20, 39, 40, 41, 42, 20, 
	44, 45, 46, 47, 20, 49, 52, 50, 
	51, 20, 53, 54, 20, 56, 57, 58, 
	20, 60, 61, 64, 62, 63, 20, 65, 
	66, 67, 68, 20, 70, 71, 72, 73, 
	74, 75, 20, 77, 78, 79, 20, 81, 
	82, 83, 20, 84, 84, 85, 87, 84, 
	89, 90, 91, 84, 84, 84, 84, 84, 
	84, 84, 84, 92, 92, 93, 95, 104, 
	92, 106, 107, 108, 92, 92, 92, 96, 
	97, 98, 100, 92, 92, 99, 97, 101, 
	102, 103, 97, 92, 92, 92, 92, 92, 
	92
};

static const unsigned char _axl_pg_trans_actions[] = {
	95, 0, 121, 71, 199, 0, 118, 69, 
	196, 93, 0, 1, 0, 67, 91, 0, 
	33, 0, 100, 11, 127, 0, 97, 9, 
	124, 65, 0, 109, 37, 136, 0, 106, 
	35, 133, 73, 0, 115, 205, 205, 7, 
	7, 0, 202, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 77, 
	89, 81, 79, 0, 75, 85, 0, 87, 
	83, 151, 0, 190, 0, 0, 0, 160, 
	0, 0, 0, 0, 0, 166, 0, 0, 
	0, 0, 172, 0, 0, 0, 0, 193, 
	0, 0, 0, 0, 154, 0, 0, 0, 
	0, 175, 0, 0, 169, 0, 0, 0, 
	163, 0, 0, 0, 0, 0, 181, 0, 
	0, 0, 0, 187, 0, 0, 0, 0, 
	0, 0, 184, 0, 0, 0, 178, 0, 
	0, 0, 157, 23, 103, 130, 130, 13, 
	0, 0, 0, 31, 27, 25, 21, 17, 
	29, 19, 15, 49, 112, 148, 0, 148, 
	39, 0, 0, 0, 63, 53, 59, 0, 
	145, 0, 0, 55, 57, 0, 139, 0, 
	0, 0, 142, 51, 47, 43, 61, 45, 
	41
};

static const unsigned char _axl_pg_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 3, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 3, 0, 0, 0, 
	0, 0, 0, 0, 3, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0
};

static const unsigned char _axl_pg_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 5, 0, 0, 0, 
	0, 0, 0, 0, 5, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0
};

static const short _axl_pg_eof_trans[] = {
	1, 1, 10, 10, 15, 17, 17, 26, 
	26, 0, 56, 57, 58, 57, 59, 57, 
	61, 62, 62, 64, 1, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 0, 140, 141, 140, 
	142, 140, 145, 145, 0, 157, 158, 159, 
	164, 26, 165, 165, 165, 165, 165, 165, 
	157, 172, 157, 175, 175
};

static const int axl_pg_start = 9;
static const int axl_pg_first_final = 9;
static const int axl_pg_error = -1;

static const int axl_pg_en_user_code = 84;
static const int axl_pg_en_user_code_2nd_pass = 92;
static const int axl_pg_en_main = 9;


/* #line 127 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */


//.............................................................................

void 
CLexer::Init ()
{
	
/* #line 380 "axl_pg_Lexer.cpp" */
	{
	cs = axl_pg_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 135 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
}

bool
CLexer::Exec ()
{
	
/* #line 386 "axl_pg_Lexer.cpp" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
_resume:
	_acts = _axl_pg_actions + _axl_pg_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 2:
/* #line 1 "NONE" */
	{ts = p;}
	break;
/* #line 403 "axl_pg_Lexer.cpp" */
		}
	}

	_keys = _axl_pg_trans_keys + _axl_pg_key_offsets[cs];
	_trans = _axl_pg_index_offsets[cs];

	_klen = _axl_pg_single_lengths[cs];
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

	_klen = _axl_pg_range_lengths[cs];
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
	_trans = _axl_pg_indicies[_trans];
_eof_trans:
	cs = _axl_pg_trans_targs[_trans];

	if ( _axl_pg_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _axl_pg_actions + _axl_pg_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 44 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{ NewLine (p + 1); }
	break;
	case 3:
/* #line 1 "NONE" */
	{te = p+1;}
	break;
	case 4:
/* #line 58 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 1;}
	break;
	case 5:
/* #line 59 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 2;}
	break;
	case 6:
/* #line 65 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 8;}
	break;
	case 7:
/* #line 58 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;}
	break;
	case 8:
/* #line 59 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;}
	break;
	case 9:
/* #line 60 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (ts [0]); }}
	break;
	case 10:
/* #line 61 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_OpenBrace); }}
	break;
	case 11:
/* #line 62 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_CloseBrace); }}
	break;
	case 12:
/* #line 63 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_OpenChevron); }}
	break;
	case 13:
/* #line 64 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_CloseChevron); }}
	break;
	case 14:
/* #line 65 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;}
	break;
	case 15:
/* #line 58 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;}
	break;
	case 16:
/* #line 59 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;}
	break;
	case 17:
/* #line 60 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateToken (ts [0]); }}
	break;
	case 18:
/* #line 65 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;}
	break;
	case 19:
/* #line 1 "NONE" */
	{	switch( act ) {
	default:
	{{p = ((te))-1;}}
	break;
	}
	}
	break;
	case 20:
/* #line 75 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 9;}
	break;
	case 21:
/* #line 76 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 10;}
	break;
	case 22:
/* #line 78 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 12;}
	break;
	case 23:
/* #line 79 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 13;}
	break;
	case 24:
/* #line 80 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 14;}
	break;
	case 25:
/* #line 87 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 21;}
	break;
	case 26:
/* #line 75 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;}
	break;
	case 27:
/* #line 76 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;}
	break;
	case 28:
/* #line 82 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (ts [0]); }}
	break;
	case 29:
/* #line 83 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_OpenBrace); }}
	break;
	case 30:
/* #line 84 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_CloseBrace); }}
	break;
	case 31:
/* #line 85 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_OpenChevron); }}
	break;
	case 32:
/* #line 86 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (EToken_CloseChevron); }}
	break;
	case 33:
/* #line 87 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;}
	break;
	case 34:
/* #line 75 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;}
	break;
	case 35:
/* #line 76 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;}
	break;
	case 36:
/* #line 77 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateIntegerToken (10, 1); }}
	break;
	case 37:
/* #line 80 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateStringToken (EToken_Identifier, 1); }}
	break;
	case 38:
/* #line 81 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateConstIntegerToken (0); }}
	break;
	case 39:
/* #line 82 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateToken (ts [0]); }}
	break;
	case 40:
/* #line 87 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;}
	break;
	case 41:
/* #line 1 "NONE" */
	{	switch( act ) {
	case 12:
	{{p = ((te))-1;} CreateToken (EToken_Arg); }
	break;
	case 13:
	{{p = ((te))-1;} CreateToken (EToken_Local); }
	break;
	case 14:
	{{p = ((te))-1;} CreateStringToken (EToken_Identifier, 1); }
	break;
	default:
	{{p = ((te))-1;}}
	break;
	}
	}
	break;
	case 42:
/* #line 103 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 26;}
	break;
	case 43:
/* #line 104 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 27;}
	break;
	case 44:
/* #line 105 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 28;}
	break;
	case 45:
/* #line 106 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 29;}
	break;
	case 46:
/* #line 107 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 30;}
	break;
	case 47:
/* #line 108 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 31;}
	break;
	case 48:
/* #line 109 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 32;}
	break;
	case 49:
/* #line 110 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 33;}
	break;
	case 50:
/* #line 111 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 34;}
	break;
	case 51:
/* #line 112 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 35;}
	break;
	case 52:
/* #line 113 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 36;}
	break;
	case 53:
/* #line 114 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 37;}
	break;
	case 54:
/* #line 115 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 38;}
	break;
	case 55:
/* #line 116 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 39;}
	break;
	case 56:
/* #line 117 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 40;}
	break;
	case 57:
/* #line 118 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 41;}
	break;
	case 58:
/* #line 119 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 42;}
	break;
	case 59:
/* #line 120 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 43;}
	break;
	case 60:
/* #line 123 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{act = 46;}
	break;
	case 61:
/* #line 99 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;}
	break;
	case 62:
/* #line 101 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;}
	break;
	case 63:
/* #line 118 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateCharToken (EToken_Integer); }}
	break;
	case 64:
/* #line 119 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateStringToken (EToken_Literal, 1, 1); }}
	break;
	case 65:
/* #line 123 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p+1;{ CreateToken (ts [0]); }}
	break;
	case 66:
/* #line 98 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;}
	break;
	case 67:
/* #line 100 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;}
	break;
	case 68:
/* #line 118 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateCharToken (EToken_Integer); }}
	break;
	case 69:
/* #line 119 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateStringToken (EToken_Literal, 1, 1); }}
	break;
	case 70:
/* #line 120 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateStringToken (EToken_Identifier); }}
	break;
	case 71:
/* #line 121 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateIntegerToken (10); }}
	break;
	case 72:
/* #line 122 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateIntegerToken (16, 2); }}
	break;
	case 73:
/* #line 123 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{te = p;p--;{ CreateToken (ts [0]); }}
	break;
	case 74:
/* #line 121 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{{p = ((te))-1;}{ CreateIntegerToken (10); }}
	break;
	case 75:
/* #line 123 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */
	{{p = ((te))-1;}{ CreateToken (ts [0]); }}
	break;
	case 76:
/* #line 1 "NONE" */
	{	switch( act ) {
	case 26:
	{{p = ((te))-1;} CreateToken (EToken_LL); }
	break;
	case 27:
	{{p = ((te))-1;} CreateToken (EToken_Import); }
	break;
	case 28:
	{{p = ((te))-1;} CreateToken (EToken_Using); }
	break;
	case 29:
	{{p = ((te))-1;} CreateToken (EToken_Class); }
	break;
	case 30:
	{{p = ((te))-1;} CreateToken (EToken_NoAst); }
	break;
	case 31:
	{{p = ((te))-1;} CreateToken (EToken_Default); }
	break;
	case 32:
	{{p = ((te))-1;} CreateToken (EToken_Local); }
	break;
	case 33:
	{{p = ((te))-1;} CreateToken (EToken_Enter); }
	break;
	case 34:
	{{p = ((te))-1;} CreateToken (EToken_Leave); }
	break;
	case 35:
	{{p = ((te))-1;} CreateToken (EToken_Start); }
	break;
	case 36:
	{{p = ((te))-1;} CreateToken (EToken_Pragma); }
	break;
	case 37:
	{{p = ((te))-1;} CreateToken (EToken_Resolver); }
	break;
	case 38:
	{{p = ((te))-1;} CreateToken (EToken_Priority); }
	break;
	case 39:
	{{p = ((te))-1;} CreateToken (EToken_Any); }
	break;
	case 40:
	{{p = ((te))-1;} CreateToken (EToken_Epsilon); }
	break;
	case 41:
	{{p = ((te))-1;} CreateCharToken (EToken_Integer); }
	break;
	case 42:
	{{p = ((te))-1;} CreateStringToken (EToken_Literal, 1, 1); }
	break;
	case 43:
	{{p = ((te))-1;} CreateStringToken (EToken_Identifier); }
	break;
	case 46:
	{{p = ((te))-1;} CreateToken (ts [0]); }
	break;
	}
	}
	break;
/* #line 771 "axl_pg_Lexer.cpp" */
		}
	}

_again:
	_acts = _axl_pg_actions + _axl_pg_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 1:
/* #line 1 "NONE" */
	{ts = 0;}
	break;
/* #line 782 "axl_pg_Lexer.cpp" */
		}
	}

	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _axl_pg_eof_trans[cs] > 0 ) {
		_trans = _axl_pg_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	}

/* #line 141 "D:/Prj/Ninja/axl3/axl_pg/axl_pg_Lexer.rl" */

	bool Result = cs != axl_pg_error;
	if (!Result)
	{
		CToken* pToken = CreateToken (EToken_Error);
		pToken->m_Data.m_Error = err::CError ("lexer error");
	}

	return Result;
}

int
CLexer::GetMachineState (ELexerMachine Machine)
{
	switch (Machine)
	{
	case ELexerMachine_Main:
		return axl_pg_en_main;

	case ELexerMachine_UserCode:
		return axl_pg_en_user_code;

	case ELexerMachine_UserCode2ndPass:
		return axl_pg_en_user_code_2nd_pass;

	default:
		ASSERT (false);
		return axl_pg_en_main;
	}
}

//.............................................................................

} // namespace pg {
} // namespace axl {
