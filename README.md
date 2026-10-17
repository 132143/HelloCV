# HelloCV 纳新任务总仓库

## 语雀笔记链接汇总
- Linux系统环境搭建实践笔记：https://www.yuque.com/g/adse-xrk7i/bgxged/rkpsi6wak47lh4vg/collaborator/join?token=BFswVFhZxuQEN3Wt&source=doc_collaborator#
- Git计算器开发实践笔记：https://www.yuque.com/g/adse-xrk7i/bgxged/kalyfpdzrhnzx12s/collaborator/join?token=mFDl8FBFHKkekKty&source=doc_collaborator#
- LLM网关实践笔记：https://www.yuque.com/g/adse-xrk7i/bgxged/ma7o0fogcw926ac2/collaborator/join?token=0C4zR688tv460OiM&source=doc_collaborator&goto=%2Fadse-xrk7i%2Fbgxged%2Fma7o0fogcw926ac2%2Fedit#
- cmake学习笔记：https://www.yuque.com/g/adse-xrk7i/bgxged/fga39heue0p6fckt/collaborator/join?token=FUtLBj1m6OnN5q6w&source=doc_collaborator#
- cmake实践笔记：https://www.yuque.com/g/adse-xrk7i/bgxged/zpf5biuexxwx193s/collaborator/join?token=mY9UvR5OxjPL33wq&source=doc_collaborator# 

## 相关项目仓库
- git_training计算器练习仓库：https://github.com/132143/git_training

---

## 第二次任务 · CMake 部分：UnitConverter

单位转换器：支持温度（℃↔℉）、长度（米↔英尺）、质量（千克↔磅）三组互转，
用户通过菜单自主选择转换方向并输入数值。

### 项目结构
```
UnitConverter/
├── CMakeLists.txt              # 根目录 CMake 构建文件
├── src/
│   └── main.cpp                # 主程序（交互逻辑）
├── unitlib/                    # 转换库
│   ├── CMakeLists.txt          # 库的 CMake 构建文件
│   ├── include/
│   │   └── unitlib.h           # 头文件（函数声明）
│   └── src/
│       └── unitlib.cpp         # 转换函数具体实现
└── build/                      # 编译目录（自行创建，不提交）
```

### 环境配置
```bash
sudo apt update
sudo apt install -y build-essential cmake git
```

### 构建与运行（源码外构建）
```bash
cd UnitConverter
mkdir build && cd build
cmake ..
make
./unit_converter
```

### 使用方法
按菜单选择：`1/2` 摄氏度↔华氏度，`3/4` 米↔英尺，`5/6` 千克↔磅，`0` 退出。
输入格式为「先输菜单编号，再输数值」。

### 涉及的 CMake 知识点
- `cmake_minimum_required()` / `project()` / `set()`
- `add_executable()` / `add_library()`
- `target_link_libraries()` / `target_include_directories()` / `target_compile_definitions()`
- `find_package()`（`Threads` 必需依赖 + `fmt` 可选依赖）
- `add_subdirectory()`（`unitlib` 作为独立模块）
- `install()`
- 源码外构建：`mkdir build && cd build && cmake .. && make`

### 语雀笔记
- CMake 学习记录：<待填写>
- CMake 实践任务记录：<待填写>

---

## 第二次任务 · LLM 部分：补全 gateway（skeleton）

把一行 JSON 请求转发给 **DeepSeek 官方 API**，把模型返回的任意文本整理成一行
严格合法的 JSON，并在网络抖动、上游报错、模型胡说、输入损坏时都不崩。

### 协议
- 输入（每行）：`{"time": "<本轮时间>", "history": ["rock", "paper"]}`，`history` 可选
- 输出（每行）：`{"time": "<与输入一致>", "option": "rock|scissor|paper"}`，只允许这两个键
- 不变式：**一行进、一行出；任何情况下都回一行合法 JSON，绝不抛异常**

### 目录结构
```
skeleton/
├── gateway/                    # 网关包
│   ├── config.py               # 配置（API Key 从环境变量读取）
│   ├── prompts.py              # 预设提示词
│   ├── json_guard.py           # JSON 容错层
│   ├── rules.py                # 协议规则、取值归一化、本地兜底
│   ├── client.py               # HTTP 转发与分类重试
│   ├── app.py                  # 总编排
│   └── game.py                 # 对局与裁判
├── run_gateway.py              # CLI：一行进一行出
├── play_game.py                # 和模型玩一局
├── mock_llm_server.py          # 本地假上游（返回 12 种坏数据）
├── selfcheck.py                # 自检脚本
├── samples/requests.jsonl      # 样例输入
└── tests/                      # 93 个验收测试
```

### 环境配置步骤
```bash
sudo apt install -y python3 python3-venv python3-pip

cd skeleton
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt

# 配置 DeepSeek API Key（不要写进代码，也不要提交到仓库）
export DEEPSEEK_API_KEY=sk-xxxxxxxx
```

### 使用方法
```bash
python selfcheck.py                    # 自检 + 跑全部 93 个测试

# 离线模式（不调用 API，只用本地兜底）
python run_gateway.py --offline --input samples/requests.jsonl

# 本地假上游（不花钱、可复现）
python mock_llm_server.py --port 8801 &
python run_gateway.py --api-base-url http://127.0.0.1:8801 --api-key test --input samples/requests.jsonl
kill %1

# 真实 DeepSeek API
python run_gateway.py --input samples/requests.jsonl
python play_game.py --rounds 10 --strategy counter
```

### 容错设计（三级降级）
1. 直接解析原始文本
2. 剥掉 markdown 代码围栏后再解析
3. 从自由文本里抠出括号配平的 `{...}`（字符串感知）
4. 文本级修复：去注释、`None/True/False`、单引号、尾随逗号、补右括号
5. 全部失败 → 本地兜底出拳 → 出口再自检一次

### HTTP 错误码分类（决定要不要重试）
| 状态码 | 处理 |
|---|---|
| `200` | 取 `content`（**不用** `reasoning_content`） |
| `400` / `401` / `403` | 不可重试，立刻失败 |
| `408` / `409` / `425` / `429` / `5xx` | 退避重试 |
| 空 `content` / 网络异常 | 退避重试 |

### 语雀笔记
- LLM 网关实践任务记录：<待填写>
