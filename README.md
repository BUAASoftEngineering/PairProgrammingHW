# 结对编程项目
一个带有GUI支持的几何对象交点求解及画图软件。
![](https://images.cnblogs.com/cnblogs_com/FuturexGO/1677884/o_200324010618painted.png)

主要特点有：
- 基于Qt和QCustomplot实现的GUI界面。支持用户在GUI中手动输入参数添加图形和从txt文件批量导入图形，同时支持选定图形及其产生交点的删除。
- 支持命令行标准输入输出和文件输入输出，此时输出的是交点个数。
- 支持异常检测和提醒，包括输入格式错误、参数范围超限、图形不合法、图形有重叠导致无穷多交点等。
- 实现了GUI模块和核心计算模块的松耦合。核心计算模块被导出成为动态链接库。核心模块面向用户为一组简洁的C风格接口，其定义详见[StdInterface.h](https://github.com/BUAASoftEngineering/PairProgrammingHW/blob/master/src/StdInterface.h)。
- 由于实现了松耦合，因此可以使用其他团队开发的`libgCore.dll`替换`bin/`下的dll，实现即插即用、可随时替换的核心模块。
- 只需引用`#include "StdInterface.h"`并链接动态链接库，就可以基于核心计算模块二次开发。
- 跨平台，在 MS Windows 和 MacOS 上均通过测试。

## 使用方法
保证`exe`所在目录的动态链接库文件齐全。在MS Windows下，命令行程序依赖的DLL为`libgCore.dll`（核心计算库）、`libgcc_s_seh-1.dll, libstdc++-6.dll, libwinpthread-1.dll`（MinGW的C++标准库）。

对于`bin/`下的命令行程序和解压的GUI程序，应保证所有所需的dll（包括上述的4个dll和其他Qt与Windows相关dll）与对应的exe在同一目录下。

### 命令行程序
在控制台中运行`./bin/intersect.exe`。

默认接受标准输入输出，也可以追加以下命令行参数进行文件输入输出：
- `-i <path to input file>` (optional)，重定向标准输入到文件输入
- `-o <path to output file>` (optional)，重定向标准输出到文件输出

### GUI程序

GUI的代码仓库见https://github.com/BUAASoftEngineering/PairProgrammingHWGui 。

本仓库中提供了编译好的GUI可执行程序。

解压`bin/gui.zip`，进入解压后的文件目录，运行`BUAASoftwareEngineeringPairProgrammingHWGui.exe`即可。

支持的功能有：
- 文件批量导入模式。点击菜单栏的File， 点击子菜单的Open...，通过文件选择浏览器选择`.txt`或`.in`文件进行导入。
- 手动添加模式。使用右上方的选单选择图形类别，再在4个或3个参数输入框中输入图形参数。
- 删除图形。在画布中点击选中图形按下`Delete`，或在右侧列表中以多选模式选中图形再点击删除按钮。

## 说明
本项目使用CLion开发、单元测试、效能分析，再使用Visual Studio Community 2019进行代码分析。

为保证项目代码的跨平台性和通用性，test/下的单元测试文件使用[Google Test](https://github.com/google/googletest)而非VS2019自带的单元测试。 Google Test被CLion, Visual Studio等IDE兼容。
