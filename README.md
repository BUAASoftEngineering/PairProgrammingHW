# 松耦合测试

## 核心模块gCore

在编译时，核心模块的动态链接库被链接，其名称已被指定为`libgCore.dll`。

因此在`bin/`目录下提供了所需的`libgCore.dll`，为本结对小组的核心模块DLL。为了便于在测试中进行拷贝、覆盖替换等操作，我们还附上了另一份拷贝`xwl_libgCore(default).dll`。

我们与另一小组进行合作，设计了核心模块的标准接口，使得两组的DLL文件都符合相同的接口规范。因此，我们在`bin/`目录下还提供了另一组（其他开发团队）开发编译出的`lpx_libgCore(alternative).dll`。

## 使用命令行程序测试前端与核心之间的松耦合

要测试本项目开发的核心模块，请在`bin/`下使用
```bash
cp "xwl_libgCore(default).dll" libgCore.dll
```
更新动态链接库文件。

要测试另一团队开发的核心模块，请在`bin/`下使用
```bash
cp "lpx_libgCore(alternative).dll" libgCore.dll
```
更新动态链接库文件。

之后保障其他所需的DLL齐全（`libgcc_s_seh-1.dll, libstdc++-6.dll, libwinpthread-1.dll`），在根目录下运行命令行程序
```bash
./bin/intersect.exe -i <TestFilePath>
```
即可在控制台收到运行的输出。

其中`<TestFilePath>`为测试文件路径，在`data/`中已经给出了`same_ans.txt`、`different_ans.txt`和`exception.txt`三组样例。两个团队的动态链接库在第一份测试中表现应相同，在后两份测试中应当表现不同。

## 使用GUI程序测试前端与核心之间的松耦合

首先解压`bin/gui.zip`到`bin/gui/`。

要测试本项目开发的核心模块，请在`bin/`下使用
```bash
cp "xwl_libgCore(default).dll" gui/libgCore.dll
```
更新动态链接库文件。

要测试另一团队开发的核心模块，请在`bin/`下使用
```bash
cp "lpx_libgCore(alternative).dll" gui/libgCore.dll
```
更新动态链接库文件。

之后保障其他所需的DLL齐全（`libgcc_s_seh-1.dll, libstdc++-6.dll, libwinpthread-1.dll`和其他Qt与Windows链接库），双击或通过控制台运行程序
```bash
./bin/gui/BUAASoftwareEngineeringPairProgrammingHWGui.exe
```
即可运行GUI程序。
可以通过文件导入`data/exception.txt`，观察弹出窗口的错误提示内容的不同，和通过文件导入正常样例`data/samll_case_GUI.txt`，观察验证两个团队的动态链接库的功能性。