#ifndef JANCYCOLORIZER_H
#define JANCYCOLORIZER_H

class JancyColorizer: public log::CRagelColorizerT <JancyColorizer>
{
public:
	void Init();
	void Exec();
};

#endif