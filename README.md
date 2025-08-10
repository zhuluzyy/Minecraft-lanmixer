# Minecraft双语资源包&lanmixer双语混合器

作者:zhuluzyy  
资源包支持版本:1.21.1及以下(以上也可以用,只是缺少新版本的内容).  
lanmixer支持所有版本以及模组,支持正则表达式筛选.  
在minecraft中启用本资源包并选择语言简体中文(中国大陆)即可.  
本双语资源包采用英语在前,中文在后的排列方式.显示物品,方块,效果,附魔的双语名称.可以在各种存储管理mod中按首字母排列物品.无需任何前置mod,无论是fabric,forge还是原版都可以用.  
内置了双语混合器,可以自行制作mod和原版的双语文件.  

## lanmixer双语混合器自定义双语文件

作者:zhuluzyy  
lanmixer是用于制作minecraft双语资源包的有力工具.无论版本,mod,mod加载器,只要能加载资源包的版本都行.提供正则表达式支持,能让用户更自由的创建双语资源包.  
如果你只需要原版的双语对照下面就不用看了,本压缩包本身就是一个已经可用的双语资源包,直接加载即可.  

### DIY双语资源包教程

你要对json以及minecraft资源包有初步了解,可以上minecraft wiki查看教程.如果有正则表达式基础那就更好了.  

<!--教程参见视频<https://www.bilibili.com/unknow>.  -->

从蓝奏云[下载](https://wwxp.lanzouu.com/b00tbb409g)MinecraftBilingualResourcepack.密码:bt1f  

解压压缩包,这时的文件结构应该是这样的:  
```
\---MinecraftBilingualResourcepack
    |   license.txt
    |   pack.mcmeta
    |   pack.png
    |   README.html
    |   README.md
    \---assets
        \---minecraft
            \---lang
                    a.json
                    b.json
                    lanmixer.exe
                    out.json
                    zh_cn.json
```

一切之前,先让你的文件资源管理器显示文件扩展名(后缀名).  

首先,先搞到两个需要的语言文件.网上有很多教程,请自行查找.  

如果你有json排序器的话(vscode的插件chen86860.json-sorter也行),可以把两个文件按key排序,可以大大加快lanmixer运行速度(只需三秒钟).没有也不用特意去下载,原版文件顶多三分钟.  

<!--已无效,该功能已支持
用记事本(或者其他文本编辑器)打开将要混合的两个语言文件,查看是否有中文或者非ascii字符(如果看见`\u8d44\u6e90`之类的字符,那么恭喜你,这一步不用做了).如果有,将文件上传至<https://www.json.cn/unicode/>(或者任何json转义器)(拖进文本框就行),取消勾选`英文数字是否转义`,点击`中文转Unicode`,再下载.两个都如此处理(英语文件也要处理).-->  

把处理好的两个语言文件放于`MinecraftBilingualResourcepack\assets\minecraft\lang`目录下.  

要显示在前面的语言文件重命名为`a.json`,要显示在后面的语言文件重命名为`b.json`.比如将`en_us.json`改为`a.json`,`zh_cn.json`改为`b.json`;就可以实现英文在前,中文在后的文本显示`Chicken 鸡`.  

双击运行`lanmixer.exe`,输入匹配key的正则表达式并回车.  
一些正则表达式示例:  
仅匹配方块,物品,效果,附魔:`(block|item|effect|enchantment)\.[\S\s]*`  
匹配所有(包括菜单页面):`[\S\s]*`  

如果程序提示`XXX cannot open`,那么检查两个文件的文件名是否一个为`a.json`,一个为`b.json`,并重启程序;  
<!--如果程序提示`this file is unsuited for this version. enter 'no' to continue (ignore this hint), or enter any other (such as 'yes') to try using lanmixerold:`,输入`yes`并回车;-->  
如果程序提示`the key "XXX" not found`,表明该项存在于`a.json`中但不存在于`b.json中`;  
如果程序提示`the end. you can close the window`,意思就是结束了可以关闭窗口;  

程序结束之后,在`MinecraftBilingualResourcepack\assets\minecraft\lang`下会生成一个最终语言文件`out.json`,将其重命名为`zh_cn.json`(未匹配的项以zh_cn语言显示).  

如果你处理的是mod的语言文件,将其移动至mod命名空间下的相应位置(从哪里来就放回哪里去).  

修改`MinecraftBilingualResourcepack\pack.mcmeta`使其版本与源语言文件一致.  

将整个`MinecraftBilingualResourcepack`文件夹复制到你的`.minecraft\resourcepacks`(mc资源包文件夹)下,启动minecraft,启用`MinecraftBilingualResourcepack`资源包.并保证语言为简体中文(中国大陆)  

如果你打算发布自己制作的资源包的话,可以在最终语言文件中搜索`"resourcePack.title":`.如果搜不到,将`"resourcePack.title":"Select Resource Packs \u9009\u62e9\u8d44\u6e90\u5305",`(按照你使用的两个语言)粘贴进文档里(或者直接在正则表达式中选中这个键).这只是为了让用户更方便的确认资源包是否正确安装,如果正确安装,资源包选择界面会显示`Select Resource Packs 选择资源包`.也别忘了在资源包里写上`使用zhuluzyy的lanmixer创建`.  

最后,minecraft~启动!  

### 程序内数据
```none
lanmixer作者:zhuluzyy  
时间240806~250206  
在a.json和b.json键排列相近时使用lanmixer能大幅提升速度(比如两者都按字母顺序排序)  
输入参数:lanmixer.exe regex a.json b.json  
默认文件输入:a.json b.json 输出:out.json  
输出:{"Key":"EnvSeparatorZhv",*}  
正则匹配使用的是regex_match(),要求整个键完全匹配正则表达式  
仅方块,物品,效果,附魔:(block|item|effect|enchantment)\.\[\S\s]*  
匹配所有:[\S\s]*  

//配置常量
const string SEPARATOR=" ";//两语言间的分隔符
const short OUTPUTINTERVAL=16;//每处理n项向控制台输出一次,提高该值有助于改善性能
```
