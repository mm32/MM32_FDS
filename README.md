# MM32-FDS

MM32 固件开发套件 (以下简称 MM32-FDS) 框架由 MM32 外设驱动库 (包含硬件描述层 HDL，硬件抽象层 HAL)，
设备驱动层 (DRV)，板级支持包 (BSP) 和应用程序实例构成。

MM32 固件开发套件支持 MM32F, MM32L 及 MM32SPIN 全系列 MCU 的软件开发，包含 MM32F103/MM32L3xx,
MM32F031C8_CB/MM32L0xx 版本，MM32SPIN2x, MM32F031C4_C6/MM32F003 版本等多款 MCU。

MM32固件开发套件提供以下方式进行软件开发：

1. 使用固件开发套件应用程序例程进行移植。
2. 用户软件调用设备驱动层的接口。
3. 访问硬件抽象层 (HAL) 函数接口。

## MM32-FDS 特点

### AllInOne 架构，同一个项目支持 MM32 全系列 MCU

- 项目MCU选择宏决定编译器核 (Core) 类型
- MCU选择宏决定外设配置
- 可选择系统资源和用户资源配置
- 多项目例程中断向量冲突处理
- 非阻塞方式代码设计

### 简化应用程序编程，访问硬件外设接口高度抽象

- 外设资源设备控制块 dcb
- 外设资源配置函数 `CreateFile()`, `DeleteFile()`
- 打开文件/关闭文件 `OpenFile()`, `CloseFile()`
- 文件读取/文件写入操作 `ReadFile()`, `WriteFile()`
- 直接内存访问输入/输出函数 `inp()`, `outp()`

### 支持同步/异步方式编程

- 回调函数接口Callback()
- 文件操作函数支持可重入

### MM32-FDS App例程

MM32-FDS 应用程序例程存放在 app 路径下，头文件存放在 `inc` 路径下。MM32-FDS App 所有的程序
实例，均按以下规则编写：

### 头文件

- 增加防重入嵌套语句 `#ifndef / #define   ...  #endif`
- 定义宏，枚举，结构
- 全局变量，静态变量防重入条件编译语句，参见下方
- 函数原型定义

```c
#ifdef _UART_C_                     // 在 C 程序代码的第一个有效行中定义
#define GLOBAL                      // 定义 GLOBAL 为空

GLOBAL bool txSuccess = false;      // 定义变量，并初始化
GLOBAL bool rxSuccess = false;

static bool toggle = false;         // 当前模块静态变量，其它模块不可使用

#else
#define GLOBAL extern               // 定义 GLOBAL 为 extern

GLOBAL bool txSuccess;              // 非当前模块之外的定义为外部变量
GLOBAL bool rxSuccess;

#endif

GLOBAL u32 g_Count;                 // 定义全局变量

#undef GLOBAL                       // 释放GLOBAL定义
```

### 应用程序

- 定义与文件名类似的宏，提供当前模块头文件预编译使用：`#define _UART_C_`，供头文件预编译使用。
- 应用程序 Systick 函数 `void AppTaskTick()`，用于应用程序的系统调度。
- 应用程序回调函数 `void TxCallback(void* fPtr)`，在同步方式下中断的快速处理。
- 主程序 `int main(void)`
  - step 1: 设置系统时钟 `MCUID = SetSystemClock(emSYSTICK_On, (u32*)&AppTaskTick); `
    - 形参 1： `emSYSTICK_On / emSYSTICK_Off ` 表示系统是否使用 SysTick (在 32 位 Cortex-Mx 下编程，建议打开)。
    - 形参 2： 应用程序是否使用系统 SysTick 的回调 (在 32 位 Cortex-Mx 下编程，建议打开)，不使用时选择 NULL。
    - 返回值：为当前MCU类别。
  - step 2: 定义句柄变量、创建文件，`HANDLE hUART = CreateFile(emIP_UART) `
    - 形参为使用套件资源的枚举，在 drv.h 中定义，返回值如果不为空表示创建成功。
    - 当需要删除句柄时，可以使用 `DeleteFile(emIP_UART)` 命令删除句柄。
  - step 3: 定义设备控制块 `tAPP_UART_DCB dcb`，并为其初始化。
  - step 4: 打开文件 `OpenFile(hUART, (void*)&dcb)`，用于对外设的初始化，打开文件对于子句柄 (hSub) 来说不分先后顺序。
    - 形参1: 创建的文件句柄，
    - 形参2: 设备控制块结构的地址，注意 hSub 为子句柄，通常是一个枚举，如 UART1 为 0， UART2 为 1。
    - 打开文件后可以通过 `CloseFile(hUART)` 命令关闭当前设备。
  - step 5: 写文件 `WriteFile(hUART, emFILE_UART1, newTxBuffer, sizeof(newTxBuffer)) == sizeof(newTxBuffer))`
    - 形参 1：设备句柄，类型为 HANDLE，由创建文件产生。
    - 形参 2：为设备的子句柄，类型为 s8， 从 "0" 开始， "-1" 表示无效。对应指定的外设，如 UART1。
    - 形参 3：写文件数据指针，类型为 u8*。
    - 形参 4：写文件数据数据长度，类型为 u16。
    - 返回值：为int类型，与驱动程序相关。通常小于"0"为错误代码，大于"0"为写入长度。
  - step 6: 读文件 `ReadFile(hUART, emFILE_UART1, newTxBuffer, sizeof(newTxBuffer)) == sizeof(newTxBuffer))`
    - 形参 1：设备句柄，类型为 HANDLE，由创建文件产生。
    - 形参 2：为设备的子句柄，类型为 s8，从 "0" 开始，"-1" 表示无效。对应指定的外设，如 UART1。
    - 形参 3：读文件数据指针，类型为 u8*。
    - 形参 4：读文件数据数据长度，类型为 u16。
    - 返回值：为 int 类型，与驱动程序相关。通常小于 "0" 为错误代码，大于 "0" 为写入长度。

### 设备控制块

以下是一个设备控制块的实例：

```c
tAPP_UART_DCB dcb = {
    .hSub       = emFILE_UART1,         // 使用 UART1
    .type       = emTYPE_DMA,           // UART1 使用 DMA 方式，可选中断或查询方式

    .cbTx       = (u32)&TxCallback,     // 发送完成回调函数地址
    .cbRx       = (u32)&RxCallback,     // 接收完成回调函数地址

    .block      = emTYPE_Non_Block,     // 读文件/写文件是否采用阻塞方式，建议使用非阻塞方式
    .sync       = emTYPE_Sync,          // 使用同步方式处理发送或接收事件
    .remapEn    = false,                // 允许/关闭端口重映象功能
    .remapIdx   = 0,                    // 重映象端口选择
    .timeOut    = 0,                    // 允许超时，0 表示不允许超时，单位毫秒

    .baudRate   = 115200,               // 波特率
    .width      = 8,                    // 传输字长，8位
    .stop       = 0,                    // 停止位长度 0 表示一位，可选 1，2，3 分别表示 2 位， 0.5 位， 1.5 位
    .parity     = emUART_PARITY_None,   // 校验方式
    .flow       = 0,                    // 0 表示不使用流控制
    .mode       = emTXRX_TxRx           // 允许发送和接受
};
```

对于不同的外设，dcb是不同的。具体说明详见外设驱动模块说明 (待增加)。
在 drv.h 中已经定义了绝大部分外设的 dcb，在后续版本升级中将陆续增加。

## MM32-FDS DRV, BSP 与 HAL 模块

1. MM32-FDS 设备驱动层代码放在 `driver` 路径下，头文件存放在 `inc` 路径下。MM32-FDS driver 所有的程序代码，编程规则同 app。
2. MM32-FDS 板级支持包代码放在 `bsp` 路径下，头文件存放在 `inc` 路径下。MM32-FDS bsp 所有的程序代码，编程规则同 app。
3. MM32-FDS 硬件抽象层代码放在 `lib/hal_lib` 路径下，头文件存放在 `inc` 路径下。MM32-FDS hal 所有的程序代码，编程规则同 app。
4. MM32-FDS 硬件抽象层代码支持 MM32 系列所有 Flash 版本的 MCU，用户只需通过改变项目预定义的宏即可切换到选择的 MCU，所有 drv, bsp 和 hal 代码无须重新加载，实现 All in One 功能。

## MM32-FDS 系统资源

MM32-FDS 系统资源代码放在 `lib/startup` 路径下，头文件存放在 `inc` 路径下。MM32-FDS 系统资源所有的程序代码，编写规则同 app。
MM32-FDS 系统资源包含以下文件

1. `startup_mm32.c`, `MM32-FDS` 启动文件，考虑到代码的兼容性和唯一性，使用 C 代码编程，没有使用汇编语言编程。
2. `system_mm32.c`, `MM32-FDS` 时钟初始化及 Systick 中断程序等文件文件。
3. `resource.c`, `MM32-FDS` 键盘和显示程序调度代码。
4. `mm32.h`, MM32 MCU 硬件描述层，是 MM32-FDS 固件开发套件唯一的关于 MCU 描述的文件，用户只需通过改变宏即可选择合适的 MCU，并得到其地址、结构寄存器及位段的描述。

## MM32-FDS 函数命名方式

代码库函数命名采用 `模块层级名 + 外设模块名 + 函数功能` 的 3 级命名方式。函数名中各层以下划线分隔。模块层级名及外设模块名可省略。模块层级名及外设模块名为大写，函数功能采用大驼峰命名法。

例如 `void DRV_ADC_Init(tDRV_ADC_DCB* dcb)`。

## MM32-FDS 变量命名方式

1. 代码库变量采用小驼峰命名法。如 `GLOBAL u32 keyStatus`。
2. 结构体类型定义采用模块层级名 + 外设模块名 + 结构体功能 + TypeDef 来定义。各级之间以下划线分隔。模块层级名及外设模块名采用大写形式。
3. 结构体功能采用首字母大写的形式。例如 `tDRV_EXTI_INSTANT`。
4. 枚举类型定义采用 `EM + 外设模块名 + 枚举类型功能` 的形式。各级之间以下划线分隔。外设模块名及枚举类型功能采用大写形式。如 `EM_UART_PARITY`。枚举成员定义采用 `em + 外设模块名 + 枚举成员功能` 的形式。各级之间以下划线分隔。外设模块名为大写形式。枚举成员功能为首字母大写。

## 用户模块例程说明

模块例程所在文件为 `/app/IP/`。例程通过调用 drv 层函数，实现用户代码与底层硬件相分离。用户所调用代码原型声明在 `drv.h` 中，函数原型定义在各 DRV 层 C 文件函数中。DRV 层通过调用 BSP 层及 HAL 层来实现硬件底层实现。

## 中断函数防止重复定义规则

因为 IP 中断函数原型名称只有一个无法修改，因此模块中加入中断函数实现代码时，通过在项目中添加宏定义 `__EX_IPxxx` ，通过预编译判断 `__EX_IPxxx` 是否已经定义来区分不同项目的中断函数。在 `drv_xxx.c` 的文件属性中，预定义防重入宏定义。

## C prepreprocessor macro definition description

|  No.  |       Definition type       |                         Description                         |                  Example                  |                           Notes                           |
| :---: | :-------------------------- | :---------------------------------------------------------- | :---------------------------------------- | :-------------------------------------------------------- |
|   1   | IDE selection               | Define the development environment                          | __KEIL, __IAR, __GCC                      |                                                           |
|   2   | Chip selection              | Define the selected chip                                    | __MM3N1, __MM0N1, __MM0P1, __MM0Q1        |                                                           |
|   3   | Module selection            | Select modules to protect duplicate definition content      | __EX_UART                                 |                                                           |
|   4   | Development board selection | Select development board                                    | __MM32_MINIBOARD, __MM32_EVB              |                                                           |
|   5   | System resource enable      | Select interrupt mode or query mode to use system resources | __SYS_RESOURCE_IT, __SYS_RESOURCE_POLLING | Double underline is enabled, single underline is disabled |
|   7   | Timeout return time         | Set the timeout return time, 0 means no timeout setting     | OVERTIME=0                                |                                                           |
|   8   | External clock setting      | Set the external clock, the unit is Hz                      | CLOCK=8000000                             | Only for IAR EWARM                                        |
