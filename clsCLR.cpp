#include "clsCLR.h"



bool clsRGBa::operator==(const clsRGBa& rhs) const{
	return (r == rhs.r && g == rhs.g && b == rhs.b && alpha == rhs.alpha);
}
bool clsRGBa::operator!=(const clsRGBa& rhs) const {
	return !(*this == rhs);
}
