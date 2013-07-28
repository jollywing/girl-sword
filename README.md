girl-sword
==========

A little Chinese RPG (Role Playing Game). (Chinese name is 越女剑)

Current version: 1.2


# 游戏说明 #

## 下载资源包 ##

不管是Linux用户还是Windows用户，都要先下载
[girl-sword-resource.tar.gz](https://github.com/jollywing/girl-sword/blob/master/release/girl-sword-resource.tar.gz),
下载后解压，解压后生成的目录就是 *游戏目录*。

## 下载二进制包，解压到此目录 ##

对于Linux用户，下载
[girl-sword-binary-1.2-linux.tar.gz](https://github.com/jollywing/girl-sword/blob/master/release/girl-sword-binary-1.2-linux.tar.gz),
在游戏目录下解压：`tar xzvf girl-sword-binary-1.2-linux.tar.gz`.
(目前只提供32bit的二进制下载，如果需要64bit的二进制，可以自己 make.)

对于Windows用户，下载
[girl-sword-binary-1.2-win32.tar.gz](https://github.com/jollywing/girl-sword/blob/master/release/girl-sword-binary-1.2-win32.tar.gz)
到游戏目录, 用解压缩软件解压到此处。

## 设置字体 ##

对于Linux用户, 进入此游戏目录后，有两种方法设置字体:

1. 复制字体 cp `/PATH/TO/YOUR/FONT.ttf` font.ttf
2. 建立字体的符号链接 ln -s `/PATH/TO/YOUR/FONT.ttf` font.ttf

对于Windows用户，从 c:\windows\fonts 目录下复制一个中文字体到游戏目录，改名为 font.ttf。

## 开始玩吧 ##

Linux用户：`./girl-sword` 将全屏运行游戏，`./girl-sword -w`将以窗口形式运行游戏。

Windows用户：双击 `girl-sword.exe` 将全屏运行游戏，双击 `play-windowed.cmd` 将以窗口形式运行游戏。

游戏目录内的 *README.txt* 是游戏的操作说明及游戏攻略。

## TODO ##

+ 系统预留前两个剧情变量：用作保存选择结果 (SELECT) 和战斗结果 (WIN).
+ 将自动剧情、触发剧情、场景切换完全通过脚本自动化
+ 按照命名约定更改代码
+ 在显示消息时，中英文混合的字符串智能截断
+ [DONE] 脚本IF, ELSE的完善
+ 增加脚本的英文版
+ 去除程序内部的剧情标志
+ 将精灵、地图、战斗角色等以数组组织
+ 将游戏说明存储在外部文本文件中
+ 增加一个序幕：我叫阿青，...
+ 将游戏中字符串变成外部可载入的资源
+ 修改阿青妈的剧本



