//lanmixer作者:zhuluzyy
//240806~251116
//在a.json和b.json键排列相近时使用本程序能大幅提升速度
//输入参数:lanmixer.exe regex a.json b.json separator out.json
//文件输入:a.json b.json 输出:out.json
//输出:{"Key":"EnvseparatorZhv",*}
//正则匹配使用的是regex_match(),要求整个键完全匹配正则表达式
//仅方块,物品,效果,附魔:(block|item|effect|enchantment)\.[\S\s]*
//匹配所有:[\S\s]*
/*更新内容:v1.2
在输出文件中包含正则表达式
支持
	仅提取单个语言文件中符合键的内容
	自定义分隔符
	自定义输出文件名
*/
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
using namespace std;
//配置常量
const short OUTPUTINTERVAL=16;//每处理n项向控制台输出一次,提高该值有助于改善性能

bool discard(ifstream &fin,const char target)//丢弃直到(丢弃target)(eof时返回true)
{
	char lastlastship=0;
	char lastship=0;
	char ship=0;
	stop2:;
	do
	{
		lastlastship=lastship;
		lastship=ship;
	}while(!(((ship=fin.get())==target)||fin.eof()));
	if(fin.eof())
	{
		return true;
	}

	//处理转义
	if(lastlastship!='\\'&&lastship=='\\'&&ship=='\"')
	{
		goto stop2;
	}
	return false;
}

bool readstr(ifstream &fin,const char target,string &str)//读取直到(丢弃target)(eof时返回true)
{
	str="";
	char lastlastship=0;
	char lastship=0;
	char ship=0;
	stop3:;
	while((lastlastship=lastship,lastship=ship),!(((ship=fin.get())==target)||fin.eof()))
	{
		str+=ship;
	}
	if(fin.eof())
	{
		return true;
	}

	//处理转义
	if(lastlastship!='\\'&&lastship=='\\'&&ship=='\"')
	{
		str+="\"";
		goto stop3;
	}
	return false;
}

string findzh(ifstream &zh,const string key/*键*/,string &lastkey/*上一次zh指针停留的键*/,unsigned long &eofn/*eof次数*/)//在zh里找值
{
	string zhv="";
	string tkey="\"";
	while(tkey!=lastkey)//主查找循环
	{
		//"tkey":"zhv",
		//读
		if(discard(zh,'\"'))//读到eof
		{
			++eofn;
			zh.clear();
			zh.seekg(0,ios::beg);//移至文件开头
			if(lastkey=="")//如果上一次没找到就从头开始
			{
				break;
			}
			continue;
		}
		readstr(zh,'\"',tkey);
		if(key==tkey)
		{
			discard(zh,'\"');//弃 :"
			readstr(zh,'\"',zhv);//读 zhv"
			lastkey=tkey;
			return zhv;
		}
		else
		{
			discard(zh,'\"');//弃 :"
			discard(zh,'\"');//弃 zhv"
		}
	}
	cout<<"the key \""<<key<<"\" not found"<<endl;//没找到
	lastkey="";
	return "";
}

string escape(const string& s)//转义
{
    string res;
    for (char c:s) {
        switch (c) {
            case '\\': res += "\\\\"; break;
            case '"': res += "\\\""; break;
            default: res += c; break;
        }
    }
    return res;
}

//************************************************************
int main(int argc,char* argv[])//0main 1regex 2a.json 3b.json 4separator 5out.json
{
	cout<<"lanmixer by zhuluzyy"<<endl;//作者声明
	cout<<"arguments:lanmixer.exe regex a.json b.json separator out.json"<<endl<<endl;

	string rgin="";//正则表达式
	string enf="a.json";//英语文件名
	string zhf="b.json";//中文文件名
	string separator=" ";//两语言间的分隔符
	string outf="out.json";//输出文件名
	bool singlefile=false;//单文件模式

	//收集参数
	switch(argc)
	{
		default:
		case 6:outf=argv[5];
		case 5:separator=escape((string)argv[4]);
		case 4:zhf=argv[3];
		case 3:enf=argv[2];
		case 2:rgin=argv[1];
		break;
		case 1:
			cout<<"regex for key:";
			cin>>rgin;
		break;
	}
	if(argc==3)
	{
		singlefile=true;
	}
	regex pattern(rgin);//转换正则表达式格式

/*
	//获得正则表达式1
	if(argc==1)
	{
		cout<<"regex for key:";
		cin>>rgin;
	}
	else
	{
		rgin=argv[1];//第一个参数为正则表达式
	}
	//获得文件名345
	if(argc>=4)
	{
		enf=argv[2];//第二个参数为a.json
		zhf=argv[3];//第三个参数为b.json
	}
*/

	//打开文件
	ifstream en(enf.c_str());//英语文件
	if(!en.is_open())
	{
		cerr<<enf<<" cannot open\a"<<endl;
		cin.get();
		cin.get();
		return 0;
	}
	ifstream zh(zhf.c_str());//中文文件
	if(!singlefile&&!zh.is_open())
	{
		cerr<<zhf<<" cannot open\a"<<endl;
		cin.get();
		cin.get();
		return 0;
	}
	ofstream out(outf.c_str());//输出文件
	if(!out.is_open())
	{
		cerr<<outf<<" cannot open\a"<<endl;
		cin.get();
		cin.get();
		return 0;
	}

	//前事
	string key="";//键
	string env="";//en值
	string zhv="";//zh值
	string zhlastkey="";//zh上一次key
	unsigned long zheofn=0;//zh eof的次数
	unsigned long outn=0;//输出的条目数
	out<<"{\"lanmixer.regex\":\""<<escape(rgin)<<'\"';//输出开头括号与正则表达式
	discard(en,'\"');//弃 {"
	//system("echo %time%");
	cout<<"start"<<endl;

	//主读取循环
	while(!en.eof())
	{
		//读
		readstr(en,'\"',key);//读 key
		//"key":"env zhv",
		if(regex_match(key,pattern))//正则匹配key
		{
			discard(en,'\"');//弃 :"
			readstr(en,'\"',env);//读 env
			discard(en,'\"');//弃 ,"
			if(singlefile)//单文件模式
			{
				//输出至终端
				if(outn%OUTPUTINTERVAL==0)//每16输出一次key
				{
					cout<<outn<<"	"<<key<<endl;//输出当前key
				}
				//写
				out<<",\""<<key<<"\":\""<<env<<'\"';//,"Key":"Env"
				++outn;
			}
			else if(env.find("%s")==-1)//去除含参数的项
			{
				zhv=findzh(zh,key,zhlastkey,zheofn);//读 zhv
				if(zhv!=""&&zhv!=env)//去除空值和重复值
				{
					//输出至终端
					if(outn%OUTPUTINTERVAL==0)//每16输出一次key
					{
						cout<<outn<<"	"<<key<<endl;//输出当前key
					}
/*
					if(outn==/*const在n时抽查/128&&zheofn>=/*const抽查通过阈值/64)//建议跳转lanmixerold
					{
						cout<<"this file is unsuited for this version. enter 'no' to continue (ignore this hint), or enter any other (such as 'yes') to try using lanmixerold:\a";
						string ans="";
						cin>>ans;
						if(ans!="no")
						{
							en.close();
							zh.close();
							out.close();
							system(("start \"lanmixerold.exe\" \"lanmixerold.exe\" \""+rgin+"\" \""+enf+"\" \""+zhf+"\"").c_str());
							return 0;
						}
						cout<<"continue"<<endl;
					}
*/
					//写
					if(/*const*/outn%256==16)//水印
					{
						out<<",\"lanmixer.watermark."<<outn<<"\":\"the author of lanmixer is zhuluzyy\"";
					}
					out<<",\""<<key<<"\":\""<<env<<separator<<zhv<<'\"';//,"Key":"EnvseparatorZhv"
					++outn;
				}
			}
		}
		else
		{
			discard(en,',');//弃,
			discard(en,'\"');//弃"
		}
	}
	out<<"}\n";

	//后事
	en.close();
	zh.close();
	out.close();
	cout<<"total output is "<<outn<<endl;
	cout<<"total eof is "<<zheofn<<endl;
	cout<<"regex for key is "<<rgin<<endl;
	cout<<"the end. you can close the window\a"<<endl;
	//system("echo %time%");
	cin.get();
	cin.get();
	return 0;
}
