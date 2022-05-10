//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_gui_AnsiAttrParser.h"

namespace axl {
namespace gui {

//..............................................................................

uint_t AnsiAttrParser::m_color256Table[256] = {
	0x000000,  // [0] Black              rgb(0,0,0)
	0x800000,  // [1] Maroon             rgb(128,0,0)
	0x008000,  // [2] Green              rgb(0,128,0)
	0x808000,  // [3] Olive              rgb(128,128,0)
	0x000080,  // [4] Navy               rgb(0,0,128)
	0x800080,  // [5] Purple             rgb(128,0,128)
	0x008080,  // [6] Teal               rgb(0,128,128)
	0xc0c0c0,  // [7] Silver             rgb(192,192,192)
	0x808080,  // [8] Grey               rgb(128,128,128)
	0xff0000,  // [9] Red                rgb(255,0,0)
	0x00ff00,  // [10] Lime              rgb(0,255,0)
	0xffff00,  // [11] Yellow            rgb(255,255,0)
	0x0000ff,  // [12] Blue              rgb(0,0,255)
	0xff00ff,  // [13] Fuchsia           rgb(255,0,255)
	0x00ffff,  // [14] Aqua              rgb(0,255,255)
	0xffffff,  // [15] White             rgb(255,255,255)
	0x000000,  // [16] Grey0             rgb(0,0,0)
	0x00005f,  // [17] NavyBlue          rgb(0,0,95)
	0x000087,  // [18] DarkBlue          rgb(0,0,135)
	0x0000af,  // [19] Blue3             rgb(0,0,175)
	0x0000d7,  // [20] Blue3             rgb(0,0,215)
	0x0000ff,  // [21] Blue1             rgb(0,0,255)
	0x005f00,  // [22] DarkGreen         rgb(0,95,0)
	0x005f5f,  // [23] DeepSkyBlue4      rgb(0,95,95)
	0x005f87,  // [24] DeepSkyBlue4      rgb(0,95,135)
	0x005faf,  // [25] DeepSkyBlue4      rgb(0,95,175)
	0x005fd7,  // [26] DodgerBlue3       rgb(0,95,215)
	0x005fff,  // [27] DodgerBlue2       rgb(0,95,255)
	0x008700,  // [28] Green4            rgb(0,135,0)
	0x00875f,  // [29] SpringGreen4      rgb(0,135,95)
	0x008787,  // [30] Turquoise4        rgb(0,135,135)
	0x0087af,  // [31] DeepSkyBlue3      rgb(0,135,175)
	0x0087d7,  // [32] DeepSkyBlue3      rgb(0,135,215)
	0x0087ff,  // [33] DodgerBlue1       rgb(0,135,255)
	0x00af00,  // [34] Green3            rgb(0,175,0)
	0x00af5f,  // [35] SpringGreen3      rgb(0,175,95)
	0x00af87,  // [36] DarkCyan          rgb(0,175,135)
	0x00afaf,  // [37] LightSeaGreen     rgb(0,175,175)
	0x00afd7,  // [38] DeepSkyBlue2      rgb(0,175,215)
	0x00afff,  // [39] DeepSkyBlue1      rgb(0,175,255)
	0x00d700,  // [40] Green3            rgb(0,215,0)
	0x00d75f,  // [41] SpringGreen3      rgb(0,215,95)
	0x00d787,  // [42] SpringGreen2      rgb(0,215,135)
	0x00d7af,  // [43] Cyan3             rgb(0,215,175)
	0x00d7d7,  // [44] DarkTurquoise     rgb(0,215,215)
	0x00d7ff,  // [45] Turquoise2        rgb(0,215,255)
	0x00ff00,  // [46] Green1            rgb(0,255,0)
	0x00ff5f,  // [47] SpringGreen2      rgb(0,255,95)
	0x00ff87,  // [48] SpringGreen1      rgb(0,255,135)
	0x00ffaf,  // [49] MediumSpringGreen rgb(0,255,175)
	0x00ffd7,  // [50] Cyan2             rgb(0,255,215)
	0x00ffff,  // [51] Cyan1             rgb(0,255,255)
	0x5f0000,  // [52] DarkRed           rgb(95,0,0)
	0x5f005f,  // [53] DeepPink4         rgb(95,0,95)
	0x5f0087,  // [54] Purple4           rgb(95,0,135)
	0x5f00af,  // [55] Purple4           rgb(95,0,175)
	0x5f00d7,  // [56] Purple3           rgb(95,0,215)
	0x5f00ff,  // [57] BlueViolet        rgb(95,0,255)
	0x5f5f00,  // [58] Orange4           rgb(95,95,0)
	0x5f5f5f,  // [59] Grey37            rgb(95,95,95)
	0x5f5f87,  // [60] MediumPurple4     rgb(95,95,135)
	0x5f5faf,  // [61] SlateBlue3        rgb(95,95,175)
	0x5f5fd7,  // [62] SlateBlue3        rgb(95,95,215)
	0x5f5fff,  // [63] RoyalBlue1        rgb(95,95,255)
	0x5f8700,  // [64] Chartreuse4       rgb(95,135,0)
	0x5f875f,  // [65] DarkSeaGreen4     rgb(95,135,95)
	0x5f8787,  // [66] PaleTurquoise4    rgb(95,135,135)
	0x5f87af,  // [67] SteelBlue         rgb(95,135,175)
	0x5f87d7,  // [68] SteelBlue3        rgb(95,135,215)
	0x5f87ff,  // [69] CornflowerBlue    rgb(95,135,255)
	0x5faf00,  // [70] Chartreuse3       rgb(95,175,0)
	0x5faf5f,  // [71] DarkSeaGreen4     rgb(95,175,95)
	0x5faf87,  // [72] CadetBlue         rgb(95,175,135)
	0x5fafaf,  // [73] CadetBlue         rgb(95,175,175)
	0x5fafd7,  // [74] SkyBlue3          rgb(95,175,215)
	0x5fafff,  // [75] SteelBlue1        rgb(95,175,255)
	0x5fd700,  // [76] Chartreuse3       rgb(95,215,0)
	0x5fd75f,  // [77] PaleGreen3        rgb(95,215,95)
	0x5fd787,  // [78] SeaGreen3         rgb(95,215,135)
	0x5fd7af,  // [79] Aquamarine3       rgb(95,215,175)
	0x5fd7d7,  // [80] MediumTurquoise   rgb(95,215,215)
	0x5fd7ff,  // [81] SteelBlue1        rgb(95,215,255)
	0x5fff00,  // [82] Chartreuse2       rgb(95,255,0)
	0x5fff5f,  // [83] SeaGreen2         rgb(95,255,95)
	0x5fff87,  // [84] SeaGreen1         rgb(95,255,135)
	0x5fffaf,  // [85] SeaGreen1         rgb(95,255,175)
	0x5fffd7,  // [86] Aquamarine1       rgb(95,255,215)
	0x5fffff,  // [87] DarkSlateGray2    rgb(95,255,255)
	0x870000,  // [88] DarkRed           rgb(135,0,0)
	0x87005f,  // [89] DeepPink4         rgb(135,0,95)
	0x870087,  // [90] DarkMagenta       rgb(135,0,135)
	0x8700af,  // [91] DarkMagenta       rgb(135,0,175)
	0x8700d7,  // [92] DarkViolet        rgb(135,0,215)
	0x8700ff,  // [93] Purple            rgb(135,0,255)
	0x875f00,  // [94] Orange4           rgb(135,95,0)
	0x875f5f,  // [95] LightPink4        rgb(135,95,95)
	0x875f87,  // [96] Plum4             rgb(135,95,135)
	0x875faf,  // [97] MediumPurple3     rgb(135,95,175)
	0x875fd7,  // [98] MediumPurple3     rgb(135,95,215)
	0x875fff,  // [99] SlateBlue1        rgb(135,95,255)
	0x878700,  // [100] Yellow4          rgb(135,135,0)
	0x87875f,  // [101] Wheat4           rgb(135,135,95)
	0x878787,  // [102] Grey53           rgb(135,135,135)
	0x8787af,  // [103] LightSlateGrey   rgb(135,135,175)
	0x8787d7,  // [104] MediumPurple     rgb(135,135,215)
	0x8787ff,  // [105] LightSlateBlue   rgb(135,135,255)
	0x87af00,  // [106] Yellow4          rgb(135,175,0)
	0x87af5f,  // [107] DarkOliveGreen3  rgb(135,175,95)
	0x87af87,  // [108] DarkSeaGreen     rgb(135,175,135)
	0x87afaf,  // [109] LightSkyBlue3    rgb(135,175,175)
	0x87afd7,  // [110] LightSkyBlue3    rgb(135,175,215)
	0x87afff,  // [111] SkyBlue2         rgb(135,175,255)
	0x87d700,  // [112] Chartreuse2      rgb(135,215,0)
	0x87d75f,  // [113] DarkOliveGreen3  rgb(135,215,95)
	0x87d787,  // [114] PaleGreen3       rgb(135,215,135)
	0x87d7af,  // [115] DarkSeaGreen3    rgb(135,215,175)
	0x87d7d7,  // [116] DarkSlateGray3   rgb(135,215,215)
	0x87d7ff,  // [117] SkyBlue1         rgb(135,215,255)
	0x87ff00,  // [118] Chartreuse1      rgb(135,255,0)
	0x87ff5f,  // [119] LightGreen       rgb(135,255,95)
	0x87ff87,  // [120] LightGreen       rgb(135,255,135)
	0x87ffaf,  // [121] PaleGreen1       rgb(135,255,175)
	0x87ffd7,  // [122] Aquamarine1      rgb(135,255,215)
	0x87ffff,  // [123] DarkSlateGray1   rgb(135,255,255)
	0xaf0000,  // [124] Red3             rgb(175,0,0)
	0xaf005f,  // [125] DeepPink4        rgb(175,0,95)
	0xaf0087,  // [126] MediumVioletRed  rgb(175,0,135)
	0xaf00af,  // [127] Magenta3         rgb(175,0,175)
	0xaf00d7,  // [128] DarkViolet       rgb(175,0,215)
	0xaf00ff,  // [129] Purple           rgb(175,0,255)
	0xaf5f00,  // [130] DarkOrange3      rgb(175,95,0)
	0xaf5f5f,  // [131] IndianRed        rgb(175,95,95)
	0xaf5f87,  // [132] HotPink3         rgb(175,95,135)
	0xaf5faf,  // [133] MediumOrchid3    rgb(175,95,175)
	0xaf5fd7,  // [134] MediumOrchid     rgb(175,95,215)
	0xaf5fff,  // [135] MediumPurple2    rgb(175,95,255)
	0xaf8700,  // [136] DarkGoldenrod    rgb(175,135,0)
	0xaf875f,  // [137] LightSalmon3     rgb(175,135,95)
	0xaf8787,  // [138] RosyBrown        rgb(175,135,135)
	0xaf87af,  // [139] Grey63           rgb(175,135,175)
	0xaf87d7,  // [140] MediumPurple2    rgb(175,135,215)
	0xaf87ff,  // [141] MediumPurple1    rgb(175,135,255)
	0xafaf00,  // [142] Gold3            rgb(175,175,0)
	0xafaf5f,  // [143] DarkKhaki        rgb(175,175,95)
	0xafaf87,  // [144] NavajoWhite3     rgb(175,175,135)
	0xafafaf,  // [145] Grey69           rgb(175,175,175)
	0xafafd7,  // [146] LightSteelBlue3  rgb(175,175,215)
	0xafafff,  // [147] LightSteelBlue   rgb(175,175,255)
	0xafd700,  // [148] Yellow3          rgb(175,215,0)
	0xafd75f,  // [149] DarkOliveGreen3  rgb(175,215,95)
	0xafd787,  // [150] DarkSeaGreen3    rgb(175,215,135)
	0xafd7af,  // [151] DarkSeaGreen2    rgb(175,215,175)
	0xafd7d7,  // [152] LightCyan3       rgb(175,215,215)
	0xafd7ff,  // [153] LightSkyBlue1    rgb(175,215,255)
	0xafff00,  // [154] GreenYellow      rgb(175,255,0)
	0xafff5f,  // [155] DarkOliveGreen2  rgb(175,255,95)
	0xafff87,  // [156] PaleGreen1       rgb(175,255,135)
	0xafffaf,  // [157] DarkSeaGreen2    rgb(175,255,175)
	0xafffd7,  // [158] DarkSeaGreen1    rgb(175,255,215)
	0xafffff,  // [159] PaleTurquoise1   rgb(175,255,255)
	0xd70000,  // [160] Red3             rgb(215,0,0)
	0xd7005f,  // [161] DeepPink3        rgb(215,0,95)
	0xd70087,  // [162] DeepPink3        rgb(215,0,135)
	0xd700af,  // [163] Magenta3         rgb(215,0,175)
	0xd700d7,  // [164] Magenta3         rgb(215,0,215)
	0xd700ff,  // [165] Magenta2         rgb(215,0,255)
	0xd75f00,  // [166] DarkOrange3      rgb(215,95,0)
	0xd75f5f,  // [167] IndianRed        rgb(215,95,95)
	0xd75f87,  // [168] HotPink3         rgb(215,95,135)
	0xd75faf,  // [169] HotPink2         rgb(215,95,175)
	0xd75fd7,  // [170] Orchid           rgb(215,95,215)
	0xd75fff,  // [171] MediumOrchid1    rgb(215,95,255)
	0xd78700,  // [172] Orange3          rgb(215,135,0)
	0xd7875f,  // [173] LightSalmon3     rgb(215,135,95)
	0xd78787,  // [174] LightPink3       rgb(215,135,135)
	0xd787af,  // [175] Pink3            rgb(215,135,175)
	0xd787d7,  // [176] Plum3            rgb(215,135,215)
	0xd787ff,  // [177] Violet           rgb(215,135,255)
	0xd7af00,  // [178] Gold3            rgb(215,175,0)
	0xd7af5f,  // [179] LightGoldenrod3  rgb(215,175,95)
	0xd7af87,  // [180] Tan              rgb(215,175,135)
	0xd7afaf,  // [181] MistyRose3       rgb(215,175,175)
	0xd7afd7,  // [182] Thistle3         rgb(215,175,215)
	0xd7afff,  // [183] Plum2            rgb(215,175,255)
	0xd7d700,  // [184] Yellow3          rgb(215,215,0)
	0xd7d75f,  // [185] Khaki3           rgb(215,215,95)
	0xd7d787,  // [186] LightGoldenrod2  rgb(215,215,135)
	0xd7d7af,  // [187] LightYellow3     rgb(215,215,175)
	0xd7d7d7,  // [188] Grey84           rgb(215,215,215)
	0xd7d7ff,  // [189] LightSteelBlue1  rgb(215,215,255)
	0xd7ff00,  // [190] Yellow2          rgb(215,255,0)
	0xd7ff5f,  // [191] DarkOliveGreen1  rgb(215,255,95)
	0xd7ff87,  // [192] DarkOliveGreen1  rgb(215,255,135)
	0xd7ffaf,  // [193] DarkSeaGreen1    rgb(215,255,175)
	0xd7ffd7,  // [194] Honeydew2        rgb(215,255,215)
	0xd7ffff,  // [195] LightCyan1       rgb(215,255,255)
	0xff0000,  // [196] Red1             rgb(255,0,0)
	0xff005f,  // [197] DeepPink2        rgb(255,0,95)
	0xff0087,  // [198] DeepPink1        rgb(255,0,135)
	0xff00af,  // [199] DeepPink1        rgb(255,0,175)
	0xff00d7,  // [200] Magenta2         rgb(255,0,215)
	0xff00ff,  // [201] Magenta1         rgb(255,0,255)
	0xff5f00,  // [202] OrangeRed1       rgb(255,95,0)
	0xff5f5f,  // [203] IndianRed1       rgb(255,95,95)
	0xff5f87,  // [204] IndianRed1       rgb(255,95,135)
	0xff5faf,  // [205] HotPink          rgb(255,95,175)
	0xff5fd7,  // [206] HotPink          rgb(255,95,215)
	0xff5fff,  // [207] MediumOrchid1    rgb(255,95,255)
	0xff8700,  // [208] DarkOrange       rgb(255,135,0)
	0xff875f,  // [209] Salmon1          rgb(255,135,95)
	0xff8787,  // [210] LightCoral       rgb(255,135,135)
	0xff87af,  // [211] PaleVioletRed1   rgb(255,135,175)
	0xff87d7,  // [212] Orchid2          rgb(255,135,215)
	0xff87ff,  // [213] Orchid1          rgb(255,135,255)
	0xffaf00,  // [214] Orange1          rgb(255,175,0)
	0xffaf5f,  // [215] SandyBrown       rgb(255,175,95)
	0xffaf87,  // [216] LightSalmon1     rgb(255,175,135)
	0xffafaf,  // [217] LightPink1       rgb(255,175,175)
	0xffafd7,  // [218] Pink1            rgb(255,175,215)
	0xffafff,  // [219] Plum1            rgb(255,175,255)
	0xffd700,  // [220] Gold1            rgb(255,215,0)
	0xffd75f,  // [221] LightGoldenrod2  rgb(255,215,95)
	0xffd787,  // [222] LightGoldenrod2  rgb(255,215,135)
	0xffd7af,  // [223] NavajoWhite1     rgb(255,215,175)
	0xffd7d7,  // [224] MistyRose1       rgb(255,215,215)
	0xffd7ff,  // [225] Thistle1         rgb(255,215,255)
	0xffff00,  // [226] Yellow1          rgb(255,255,0)
	0xffff5f,  // [227] LightGoldenrod1  rgb(255,255,95)
	0xffff87,  // [228] Khaki1           rgb(255,255,135)
	0xffffaf,  // [229] Wheat1           rgb(255,255,175)
	0xffffd7,  // [230] Cornsilk1        rgb(255,255,215)
	0xffffff,  // [231] Grey100          rgb(255,255,255)
	0x080808,  // [232] Grey3            rgb(8,8,8)
	0x121212,  // [233] Grey7            rgb(18,18,18)
	0x1c1c1c,  // [234] Grey11           rgb(28,28,28)
	0x262626,  // [235] Grey15           rgb(38,38,38)
	0x303030,  // [236] Grey19           rgb(48,48,48)
	0x3a3a3a,  // [237] Grey23           rgb(58,58,58)
	0x444444,  // [238] Grey27           rgb(68,68,68)
	0x4e4e4e,  // [239] Grey30           rgb(78,78,78)
	0x585858,  // [240] Grey35           rgb(88,88,88)
	0x626262,  // [241] Grey39           rgb(98,98,98)
	0x6c6c6c,  // [242] Grey42           rgb(108,108,108)
	0x767676,  // [243] Grey46           rgb(118,118,118)
	0x808080,  // [244] Grey50           rgb(128,128,128)
	0x8a8a8a,  // [245] Grey54           rgb(138,138,138)
	0x949494,  // [246] Grey58           rgb(148,148,148)
	0x9e9e9e,  // [247] Grey62           rgb(158,158,158)
	0xa8a8a8,  // [248] Grey66           rgb(168,168,168)
	0xb2b2b2,  // [249] Grey70           rgb(178,178,178)
	0xbcbcbc,  // [250] Grey74           rgb(188,188,188)
	0xc6c6c6,  // [251] Grey78           rgb(198,198,198)
	0xd0d0d0,  // [252] Grey82           rgb(208,208,208)
	0xdadada,  // [253] Grey85           rgb(218,218,218)
	0xe4e4e4,  // [254] Grey89           rgb(228,228,228)
	0xeeeeee,  // [255] Grey93           rgb(238,238,238)
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AnsiAttrParser::AnsiAttrParser() {
	m_targetAttr = NULL;
	m_textColor = NULL;
	m_backColor = NULL;
}

size_t
AnsiAttrParser::parse(
	TextAttr* targetAttr,
	const TextAttr& baseAttr,
	const sl::StringRef& string
) {
	static SetAttrFuncPtr setAttrFuncTable[108] = { 0 };
	static bool once = false;

	if (!once) { // no need to use sl::callOnce here
		setAttrFuncTable[0] = &AnsiAttrParser::clear;
		setAttrFuncTable[1] = &AnsiAttrParser::setBoldOn;
		setAttrFuncTable[3] = &AnsiAttrParser::setItalicOn;
		setAttrFuncTable[4] = &AnsiAttrParser::setUnderlineOn;
		setAttrFuncTable[5] = &AnsiAttrParser::setBoldOn;
		setAttrFuncTable[7] = &AnsiAttrParser::setInverseOn;
		setAttrFuncTable[9] = &AnsiAttrParser::setStrikeoutOn;

		for (size_t i = 10; i < 20; i++)
			setAttrFuncTable[i] = &AnsiAttrParser::setFont;

		setAttrFuncTable[22] = &AnsiAttrParser::setBoldOff;
		setAttrFuncTable[23] = &AnsiAttrParser::setItalicOff;
		setAttrFuncTable[24] = &AnsiAttrParser::setUnderlineOff;
		setAttrFuncTable[27] = &AnsiAttrParser::setInverseOff;
		setAttrFuncTable[29] = &AnsiAttrParser::setStrikeoutOff;

		for (size_t i = 30; i < 38; i++)
			setAttrFuncTable[i] = &AnsiAttrParser::setTextColor;

		setAttrFuncTable[39] = &AnsiAttrParser::setBaseTextColor;

		for (size_t i = 40; i < 48; i++)
			setAttrFuncTable[i] = &AnsiAttrParser::setBackColor;

		setAttrFuncTable[49] = &AnsiAttrParser::setBaseBackColor;

		for (size_t i = 90; i < 98; i++)
			setAttrFuncTable[i] = &AnsiAttrParser::setBrightTextColor;

		for (size_t i = 100; i < 108; i++)
			setAttrFuncTable[i] = &AnsiAttrParser::setBrightBackColor;

		setAttrFuncTable[38] = &AnsiAttrParser::setColor256First;
		setAttrFuncTable[48] = &AnsiAttrParser::setColor256First;

		once = true;
	}

	m_targetAttr = targetAttr;
	m_baseAttr = baseAttr;
	m_textColor = &m_targetAttr->m_foreColor;
	m_backColor = &m_targetAttr->m_backColor;
	m_setAttrExFunc = NULL;
	m_attrExBuffer.clear();

	size_t attrCount = 0;

	const char* p = string.cp();
	const char* end = string.getEnd();

	while (p < end) {
		char* attrEnd;
		uint_t attr = strtoul(p, &attrEnd, 10);
		if (attrEnd == p)
			break;

		attrCount++;

		if (m_setAttrExFunc)
			(this->*m_setAttrExFunc)(attr);
		else if (attr < countof(setAttrFuncTable)) {
			SetAttrFuncPtr setAttrFunc = setAttrFuncTable[attr];
			if (setAttrFunc)
				(this->*setAttrFunc)(attr);
		}

		p = attrEnd;
		while (p < end) {
			if (*p == ';') {
				p++;
				break;
			}

			p++;
		}
	}

	if (!attrCount)
		*targetAttr = baseAttr;

	return attrCount;
}

void
AnsiAttrParser::clear(uint_t) {
	*m_targetAttr = m_baseAttr;
	m_textColor = &m_targetAttr->m_foreColor;
	m_backColor = &m_targetAttr->m_backColor;
}

void
AnsiAttrParser::setInverse(bool isInversed) {
	if (isInversed == (m_textColor == &m_targetAttr->m_backColor))
		return;

	uint_t c = m_targetAttr->m_foreColor;
	m_targetAttr->m_foreColor = m_targetAttr->m_backColor;
	m_targetAttr->m_backColor = c;

	uint_t* p = m_textColor;
	m_textColor = m_backColor;
	m_backColor = p;
}

void
AnsiAttrParser::setFontFlag(uint_t flag) {
	if (m_targetAttr->m_fontFlags & FontFlag_Undefined)
		m_targetAttr->m_fontFlags = flag;
	else
		m_targetAttr->m_fontFlags |= flag;
}

void
AnsiAttrParser::clearFontFlag(uint_t flag) {
	if (m_targetAttr->m_fontFlags & FontFlag_Undefined)
		return;

	if (m_baseAttr.m_fontFlags & FontFlag_Undefined)
		m_targetAttr->m_fontFlags &= ~flag;
	else
		m_targetAttr->m_fontFlags |= (m_baseAttr.m_fontFlags & flag);
}

void
AnsiAttrParser::setColor256Next(uint_t code) {
	size_t count = m_attrExBuffer.append(code);
	switch (count) {
	case 2:
		if (code != 2 && code != 5) // invalid sequence
			m_setAttrExFunc = NULL;
		break;

	case 3:
		if (m_attrExBuffer[1] == 5)
			finalizeColor256(m_color256Table[code & 0xff]);
		break;

	case 5:
		finalizeColor256(
			(m_attrExBuffer[2] & 0xff) << 16 |
			(m_attrExBuffer[3] & 0xff) << 8 |
			(m_attrExBuffer[4] & 0xff)
		);

	default:
		ASSERT(false);
		m_setAttrExFunc = NULL;
	}
}

void
AnsiAttrParser::finalizeColor256(uint_t color) {
	switch (m_attrExBuffer[0]) {
	case 38:
		*m_textColor = color;
		break;

	case 48:
		*m_backColor = color;
		break;
	}

	m_setAttrExFunc = NULL;
}

//..............................................................................

} // namespace gui
} // namespace axl
