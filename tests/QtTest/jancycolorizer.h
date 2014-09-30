#ifndef JANCYCOLORIZER_H
#define JANCYCOLORIZER_H

class jancyColorizer: public log::CRagelColorizerT <jancyColorizer>
{
public:
	void init();
	void exec();
};

#endif