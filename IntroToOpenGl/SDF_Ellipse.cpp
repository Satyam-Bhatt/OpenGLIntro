#include "SDF_Ellipse.h"

SDF_Ellipse SDF_Ellipse::instance;

SDF_Ellipse* SDF_Ellipse::GetInstance()
{
    return &instance;
}
