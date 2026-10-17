#include "unitlib.h"

#ifndef UNITLIB_VERSION
#define UNITLIB_VERSION "unknown"
#endif

namespace unitlib {

// 华氏度 = 摄氏度 * 9/5 + 32
double celsiusToFahrenheit(double c) { return c * 9.0 / 5.0 + 32.0; }
double fahrenheitToCelsius(double f) { return (f - 32.0) * 5.0 / 9.0; }

// 1 米 = 3.280839895 英尺
double meterToFoot(double m)  { return m * 3.280839895; }
double footToMeter(double ft) { return ft / 3.280839895; }

// 1 千克 = 2.2046226218 磅
double kilogramToPound(double kg) { return kg * 2.2046226218; }
double poundToKilogram(double lb) { return lb / 2.2046226218; }

const char* version() { return UNITLIB_VERSION; }

}  // namespace unitlib
