### 1. 资产管理
#### 任务目标
* 显示所有客户机的资产信息列表
* 显示选中客户机的资产信息，包括现信息、原信息以及更改历史
* 导出全部客户机资产信息列表(.csv)
* 全部和单项确认变更
* 开启该功能？？？

#### 任务实施
(1) 信息获取
**采用WMI API获取计算机的硬件信息**，相关语法如下:
* 客户机名
	ROOT\\cimv2--> SELECT * FROM Win32_ComputerSystem
* 主板
	ROOT\\WMI-->SELECT * FROM MSSmBios_RawSMBiosTables
* CPU
	ROOT\\cimv2-->SELECT * FROM Win32_Processor
* 显卡
	ROOT\\cimv2-->SELECT * FROM Win32_VideoController
* 网卡
* 内存
	ROOT\\cimv2-->SELECT * FROM Win32_PhysicalMemory
* 硬盘
	ROOT\\cimv2-->SELECT * FROM Win32_DiskDrive

(2) 信息存储
	主要包括三方面，当前硬件信息、历史硬件信息、历史硬件更改信息
	接口：
	**addHardwareInfo(clientName)
	getCurrentHardwareInfo(clientName)
	getRecentlyHistoryHardwareInfo(clientName)
	getRecentHistoryChangedLog(clientName)**

(3) 资源协调
	数据库设计，已和高志林沟通

### 2. 开机命令管理
#### 任务目标
* 实现开机命令的增删改
* 实现开机命令的分组查看功能

#### 任务实施
(1) 明确开机启动项结构
	目前无盘、启动项的添加界面、显示界面三方信息极不统一

* 无盘服务启动项信息
> 任务名称
要执行的程序全路径
要执行程序的启动参数(不带进程全路径)
延时运行(单位:秒),0表示不延时
是否在XP下执行
是否在WIN7下执行
是否在WIN10下执行
是否在其它系统下执行

* 添加启动项界面信息
> 任务名称
任务类型
服务端目录
执行程序
运行参数
延时执行
执行范围
系统范围

* 显示启动项界面信息
> 是否启动
任务名称
添加时间
延时执行
大小(MB)/个数
执行范围
系统范围

(2) 接口设计
**addClientStartOptions()
modifyClientStartOptions()
deleteClientStartOptions()**

(3) 资源协调
* UI及内容需重新设计
* 数据库设计

### 3. 桌面快捷管理
#### 任务目标
* 快捷方式资源管理
* 按组实现桌面快捷方式的增、删、查
* 根据分组信息为指定客户机添加左面快捷方式

#### 任务实施
(1) 快捷方式类型
* 游戏快捷
> 根据游戏名查找快捷方式
* 分类快捷
* IE自定义
> 添加、修改、删除
* 系统工具

(2) 快捷方式属性
* 目标类型
* 目标位置(完整路径或URL)
* 命令行参数
* 名称
* 图标(icon路径)
* 存放路径
* 备注

(3) 实现思路
* 接口设计
**
// 根据快捷方式类型和搜索字符串得到快捷方式资源(注：只有游戏快捷才有搜索选项)
getResourceShortCutByType(ShortCutsSearchRequest *pRequest)
// 添加IE快捷方式到资源
addIEShortCutToResource(ShortCutInfo)
// 修改资源中的IE快捷方式
modifyIEShortCutToResource(ShortCutInfo)
// 删除资源中的IE快捷方式
deleteIEShortCutToResource(ShortCutInfo)
// 将快捷方式列表添加到指定的组
addShortCutToGroup(ShortCutForGroup[])
// 根据组标识得到快捷方式列表
getShortCutByGroup(wchar_t* groupIdentifie)
// 删除指定组中的快捷方式
deleteShortCutToGroup(ShortCutForGroup[])
** 
</hr>

* 资源对话框设计逻辑
a. 根据不同的资源类型获取资源快捷方式
b. 记录各类型中已选中的和当前组已拥有的快捷方式
c. 切换类型时保存选中状态
</hr>

* 快捷方式显示对话框设计逻辑

(4) 资源协调
a. 讨论前移和后移的设计的必要性,建议快捷方式的显示按照字母的排序。
b. 添加UI快捷对话框
c. 多对多数据库设计

### 4. 桌面壁纸管理
#### 任务目标
a. 从资源库(云壁纸库、本地壁纸库)中获取壁纸信息，将选定的壁纸添加到指定组的指定分类中(默认随机、指定随机)
b. 删除指定组的指定分类中的壁纸
c. 根据分组信息为指定客户机设定桌面壁纸

#### 任务实施
(1) 关键结构设计
> 	typedef struct tagWallPaperForGroupRequest
	{
		TCHAR groupIdentifie[MAX_GROUP_INDENTFILE_LENGTH]; // 组标识
		SecondType type;  // 二级分类(默认随机，自定义随机)
		WallPaperInfo wallPaperInfo;
	}WallPaperForGroupRequest,*WallPaperForGroupRequestPtr;

(2) 接口设计
**addWallPaperToGroup
getWallPaperByGroup**
<font color='red'>注：设置客户机壁纸的前提是必须明确最后选则的SecondType</font>
</hr>

(3) 壁纸显示风格
> enum class WallpaperStyle //壁纸显示的类型
	{
		Tile,    //平铺
		Center,  //居中
		Stretch, //拉伸
		Fit,     //适应
		Fill     //填充
	};

(4) 资源协调
a. 对UI进行优化
b. 数据库设计

### 5. 桌面主题管理
#### 任务目标
根据主题类型设置游戏菜单主题

#### 任务实施
(1) 数据结构
> typedef struct tagThemeInfo
	{
		TCHAR ThemeName[16];
		TCHAR imgPath[MAX_PATH];
	}ThemeInfo,*ThemeInfoPtr;

(2) 主题类型
	由游戏菜单确定

(3) 资源协调
a. 游戏菜单主题设置
b. 无盘与客户机通信

### 6. 帮助-配置向导
* 由于配置功能在其他相关模块都有实现，待功能测试通过后再进行统一处理
### 7. 帮助辅助工具、在线帮助、关于
