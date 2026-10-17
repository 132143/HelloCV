// main.cpp —— 单位转换器主程序（交互逻辑）
// 只负责：打印菜单 / 读输入 / 调用 unitlib / 打印结果

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "unitlib.h"

#ifdef HAVE_FMT
#include <fmt/format.h>
#endif

namespace {

void printMenu() {
    std::cout << "\n========== 单位转换器 (unitlib v" << unitlib::version()
              << ") ==========\n"
              << " 1. 摄氏度 -> 华氏度\n"
              << " 2. 华氏度 -> 摄氏度\n"
              << " 3. 米     -> 英尺\n"
              << " 4. 英尺   -> 米\n"
              << " 5. 千克   -> 磅\n"
              << " 6. 磅     -> 千克\n"
              << " 0. 退出\n"
              << "--------------------------------------\n"
              << "请输入选项 [0-6]: ";
}

void printResult(const std::string& fromUnit, double in,
                 const std::string& toUnit, double out) {
#ifdef HAVE_FMT
    std::cout << fmt::format("  {:.4f} {} = {:.4f} {}\n", in, fromUnit, out, toUnit);
#else
    std::cout << "  " << std::fixed << std::setprecision(4)
              << in << " " << fromUnit << " = " << out << " " << toUnit << "\n";
#endif
}

double readNumber() {
    double v = 0.0;
    while (!(std::cin >> v)) {
        if (std::cin.eof()) {
            std::cout << "\n检测到输入结束，程序退出。\n";
            std::exit(EXIT_SUCCESS);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  输入不是数字，请重新输入: ";
    }
    return v;
}

}  // namespace

int main() {
    std::cout << "欢迎使用单位转换器！支持温度、长度、质量三组单位互转。\n";

    int choice = -1;
    while (choice != 0) {
        printMenu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  请输入 0-6 之间的数字。\n";
            continue;
        }
        if (choice == 0) break;

        double value = 0.0;
        switch (choice) {
            case 1:
                std::cout << "请输入摄氏温度: "; value = readNumber();
                printResult("C", value, "F", unitlib::celsiusToFahrenheit(value));
                break;
            case 2:
                std::cout << "请输入华氏温度: "; value = readNumber();
                printResult("F", value, "C", unitlib::fahrenheitToCelsius(value));
                break;
            case 3:
                std::cout << "请输入长度(米): "; value = readNumber();
                printResult("m", value, "ft", unitlib::meterToFoot(value));
                break;
            case 4:
                std::cout << "请输入长度(英尺): "; value = readNumber();
                printResult("ft", value, "m", unitlib::footToMeter(value));
                break;
            case 5:
                std::cout << "请输入质量(千克): "; value = readNumber();
                printResult("kg", value, "lb", unitlib::kilogramToPound(value));
                break;
            case 6:
                std::cout << "请输入质量(磅): "; value = readNumber();
                printResult("lb", value, "kg", unitlib::poundToKilogram(value));
                break;
            default:
                std::cout << "  无效选项，请输入 0-6。\n";
                break;
        }
    }
    std::cout << "已退出，再见！\n";
    return 0;
}
