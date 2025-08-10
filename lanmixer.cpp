//lanmixer作者:zhuluzyy
//240806~250207
//在a.json和b.json键排列相近时使用本程序能大幅提升速度
//输入参数:lanmixer.exe regex a.json b.json
//文件输入:a.json b.json 输出:out.json
//输出:{"Key":"EnvSEPARATORZhv",*}
//正则匹配使用的是regex_match(),要求整个键完全匹配正则表达式
//仅方块,物品,效果,附魔:(block|item|effect|enchantment)\.[\S\s]*
//匹配所有:[\S\s]*

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <stdlib.h>
using namespace std;

//配置常量
const string SEPARATOR=" ";//两语言间的分隔符
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

int main(int argc,char* argv[])//0main 1regex 2a.json 3b.json
{
	//作者声明
	cout<<"lanmixer by zhuluzyy"<<endl;
	//检查参数
	if(argc==3)
	{
		cerr<<"the number of arguments is incorrect. please use 0, 1 or 3 arguments\a"<<endl;
		cin.get();
		cin.get();
		return -1;
	}

	//获得正则表达式
	string rgin="";
	if(argc==1)
	{
		cout<<"regex for key:";
		cin>>rgin;
	}
	else
	{
		rgin=argv[1];//第一个参数为正则表达式
	}
	regex pattern(rgin);//转换正则表达式格式

	//获得文件名
	string enf="a.json";//英语文件名
	string zhf="b.json";//中文文件名
	if(argc>=4)
	{
		enf=argv[2];//第二个参数为a.json
		zhf=argv[3];//第三个参数为b.json
	}
	//打开文件
	ifstream en(enf.c_str());//英语文件
	if(!en.is_open())
	{
		cerr<<enf<<" cannot open\a"<<endl;
		cin.get();
		cin.get();
		return -1;
	}
	ifstream zh(zhf.c_str());//中文文件
	if(!zh.is_open())
	{
		cerr<<zhf<<" cannot open\a"<<endl;
		cin.get();
		cin.get();
		return -1;
	}
	ofstream out("out.json");//输出文件
	if(!out.is_open())
	{
		cerr<<"out.json cannot open\a"<<endl;
		cin.get();
		cin.get();
		return -1;
	}

	//前事
	string key="";//键
	string env="";//en值
	string zhv="";//zh值
	string zhlastkey="";//zh上一次key
	unsigned long zheofn=0;//zh eof的次数
	unsigned long outn=0;//输出的条目数
	out<<'{';
	discard(en,'\"');//弃 {"
	system("echo %time%");
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
			if(env.find("%s")==-1)//去除含参数的项
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
					if(/*const*/outn%256==8)//电子水印
					{
						out<<",\"this.lanfile.created."<<outn<<"by.lanmixer\":\"the author of lanmixer is zhuluzyy \\u9010\\u9e7f\\u4e2d\\u539f\\u539f\"";
					}
					if(outn!=0)//防止前随逗号
					{
						out<<',';//,
					}
					out<<'\"'<<key<<"\":\""<<env<<SEPARATOR<<zhv<<'\"';//,"Key":"EnvSEPARATORZhv"
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
	system("echo %time%");
	cin.get();
	cin.get();
	return 0;
}
