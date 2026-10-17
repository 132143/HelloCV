#ifndef UNITLIB_H
#define UNITLIB_H

// unitlib —— 单位转换库
// 只放函数声明，实现见 src/unitlib.cpp

namespace unitlib {

// 温度
double celsiusToFahrenheit(double c);
double fahrenheitToCelsius(double f);

// 长度
double meterToFoot(double m);
double footToMeter(double ft);

// 质量
double kilogramToPound(double kg);
double poundToKilogram(double lb);

// 版本号（由 CMake 的 target_compile_definitions 注入）
const char* version();

}  // namespace unitlib

#endif  // UNITLIB_H
