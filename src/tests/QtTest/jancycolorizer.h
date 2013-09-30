#ifndef JANCYCOLORIZER_H
#define JANCYCOLORIZER_H

class JancyColorizer: public log::CRagelColorizerT <JancyColorizer>
{
public:
	AXL_OBJ_CLASS_0 (JancyColorizer, IColorizer)

public:
	void Init();
	void Exec();
};

#endif