# 自动编译桥说明文档 (Build Bridge)

本文档说明了如何使用自动化脚本，实现在主机（Host）修改代码并在 Windows 7 虚拟机（VM）中自动触发 VS2015 MSBuild 编译的完整流程。

## 1. 整体工作流程

1. **VS Code 修改代码**：你在主机上的 VS Code 或通过 Agent 编写、修改 C++ 源码。
2. **主机脚本同步代码**：按 Ctrl+Shift+B 触发主机上的 equest_vm_build.ps1 脚本，它使用 obocopy 增量同步最新的代码到 VMware 共享目录（避开 .git、x64、.vs 等不需要同步的目录）。
3. **创建编译请求**：主机脚本在共享目录创建 uild.request 状态文件。
4. **虚拟机监听**：虚拟机中常驻运行的 m_build_watcher.bat 每隔2秒轮询检查一次，当发现 uild.request 时被唤醒。
5. **虚拟机拉取代码**：虚拟机脚本先将共享目录中的源码通过 obocopy 复制到虚拟机的**本地磁盘**，避免通过网络驱动器直接编译造成 I/O 拥堵和冲突。
6. **执行编译**：虚拟机脚本加载 cvarsall.bat 环境，并调用 MSBuild.exe 编译本地的 .sln 文件，编译结果和错误实时记录在 uild_latest.log 中。
7. **写回状态和日志**：编译结束后，虚拟机脚本将日志文件写回共享目录，并生成 uild.done (成功) 或 uild.failed (失败) 文件。
8. **主机读取并展示**：主机脚本一直在等待这些状态文件，一旦发现编译结束，便从共享目录读取 uild_latest.log 打印在 VS Code 终端中。通过 $msCompile problemMatcher，可以直接在 VS Code 中点击错误跳转到对应的文件和行数。

## 2. 主机端需要修改哪些路径

打开主机脚本：	ools_autoVS2015\request_vm_build.ps1

你需要在脚本开头的【配置区域】检查并修改以下变量：
- $HostSource：主机实际存放 FeatureSpy260714 的本地绝对路径。
  - *当前默认为：* D:\VMwareSHARE\HRproject\31project\_Source\FeatureSpy260714
- $SharedRoot：VMware 共享文件夹在主机侧的根目录路径。
  - *当前默认为：* D:\VMwareSHARE\HRproject\31project

## 3. 虚拟机端需要修改哪些路径

打开虚拟机脚本：	ools_autoVS2015\vm_build_watcher.bat

你需要在脚本开头的【配置区域】检查并修改以下变量：
- SHARED_ROOT：共享目录在虚拟机中映射的盘符路径（如 Z: 盘下的对应目录）。
  - *当前默认为：* Z:\VMwareSHARE\HRproject\31project
- LOCAL_SOURCE：虚拟机中用于接收源码并编译的**本地路径**（放桌面或C盘下）。
  - *当前默认为：* C:\Users\ug12\Desktop\HuNXTool\_Source\FeatureSpy260714
- VCVARSALL_PATH：虚拟机内 Visual Studio 2015 环境批处理的路径。
  - *当前默认为：* C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat
- MSBUILD_PATH：虚拟机内 MSBuild 的执行路径。
  - *当前默认为：* C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe
- SLN_PATH：虚拟机本地源码中 .sln 解决方案的具体路径。
  - *当前默认为：* %LOCAL_SOURCE%\FeatureSpy260714.sln

## 4. 如何启动与使用

1. **虚拟机端（只需要启动一次）**：
   - 打开 Windows 7 虚拟机。
   - 导航到共享文件夹的 	ools_autoVS2015 目录。
   - 双击运行 m_build_watcher.bat。
   - **请保持这个黑色的 CMD 窗口始终打开**，它会自动在后台安静地等待请求，不占 CPU。
2. **主机端（每次改完代码后）**：
   - 在主机 VS Code 中，按组合键 Ctrl + Shift + B。
   - VS Code 会自动调出终端运行构建任务，并等待虚拟机返回结果。
   - 编译完成后，所有的 Error 或 Warning 都会在终端输出，并且会集成到 VS Code 的“问题 (Problems)”面板中。

## 5. 状态文件含义 (位于 build_bridge_autoVS2015 目录)

- uild.request：主机生成的请求信号，等待虚拟机处理。
- uild.running：虚拟机确认收到请求，正在同步代码和编译。
- uild.done：虚拟机报告**编译成功**。
- uild.failed：虚拟机报告**编译失败**（可能是代码报错，也可能是同步失败）。
- uild_latest.log：包含完整 MSBuild 输出和时间的最新编译日志。

## 6. 常见问题排查

- **共享目录不存在或拒绝访问**：确保 VMware 设置的共享文件夹处于激活状态，虚拟机网络映射（Z盘）连接正常。
- **MSBuild 或 sln 路径错误**：如果 uild_latest.log 提示找不到 MSBuild 或者报文件找不到，请对照【第3节】去虚拟机检查路径配置。
- **虚拟机监听脚本没有启动**：如果主机终端卡在“等待虚拟机处理编译请求...”且超时退出，检查虚拟机里那个 .bat 窗口是否不小心关了或者挂起了。
- **build.running 残留**：如果虚拟机强行断电或闪退，可能残留 uild.running 导致主机报错。直接手动删除 uild_bridge_autoVS2015 目录下的该文件即可。
- **robocopy 返回值不是普通的0或1**：Robocopy 返回 0-7 都是成功的状态码（如 1 代表复制了新文件，3 代表有新增和跳过等）。返回 8 以上才是出错，如果报错会反映在日志中。
- **中文日志乱码**：虚拟机编译时使用了 GBK 编码（cmd chcp 936），主机的 PowerShell 脚本读取时已配置 Encoding Default 自动适配，一般不会乱码。
- **NX 正在占用 DLL**：如果你还在虚拟机开着 NX 并且加载了旧版的这个插件 DLL，MSBuild 可能会报权限被拒绝（Access Denied），请在编译前卸载 NX 插件或者关闭 NX。