# 结对编程项目
一个带有GUI支持的几何对象交点求解及画图软件。
主要特点有：
- 基于Qt和QCustomplot实现的GUI界面。支持用户在GUI中手动输入参数添加图形和从txt文件批量导入图形，同时支持选定图形及其产生交点的删除。
- 支持命令行标准输入输出和文件输入输出，此时输出的是交点个数。
- 支持异常检测和提醒，包括输入格式错误、参数范围超限、图形不合法、图形有重叠导致无穷多交点等。
- 实现了GUI模块和核心计算模块的松耦合。核心计算模块被导出成为动态链接库。核心模块面向用户为一组简洁的C风格接口，其定义详见[StdInterface.h](https://github.com/BUAASoftEngineering/PairProgrammingHW/blob/master/src/StdInterface.h)。
- 只需引用`#include "StdInterface.h"`并链接动态链接库，就可以基于核心计算模块二次开发。
- 跨平台，在 MS Windows 和 MacOS 上均通过测试。

## 使用方法
保证`bin/`文件夹中的动态链接库文件齐全。在MacOS下所需的动态链接库为`libgCore.dylib`，在MS Windows下为`libgCore.dll`

### 命令行程序
在控制台中运行`./bin/intersect.exe`

默认接受标准输入输出，也可以追加以下命令行参数进行文件输入输出：
- `-i <path to input file>` (optional)，重定向标准输入到文件输入
- `-o <path to output file>` (optional)，重定向标准输出到文件输出

#### 命令行程序松耦合测试

使用`bin/libgCore_lpx_for_loose_coupling_test.dll`替换同级目录下的`bin/libgCore.dll`，再运行`intersect.exe`即可

### GUI程序

GUI的代码仓库见https://github.com/BUAASoftEngineering/PairProgrammingHWGui

本仓库中提供了GUI程序编译好的二进制文件

解压`bin/gui.zip`，进入解压后的文件目录，运行`BUAASoftwareEngineeringPairProgrammingHWGui.exe`即可

#### 命令行程序松耦合测试

使用`bin/libgCore_lpx_for_loose_coupling_test.dll`替换解压后程序根目录下的`libgCore.dll`，再运行即可

## 说明
本项目使用CLion开发、单元测试、效能分析，再使用Visual Studio Community 2019进行代码分析。

为保证项目代码的跨平台性和通用性，test/下的单元测试文件使用[Google Test](https://github.com/google/googletest)而非VS2019自带的单元测试。 Google Test被CLion, Visual Studio等IDE兼容。
