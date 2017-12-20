#pragma once

#include <string>
#include <time.h>
#include <memory>

struct Head;	// 消息头
struct RealMsg;	// 消息结构
typedef std::shared_ptr<RealMsg> Msg;	// 包含消息结构的智能指针，推荐使用

#pragma pack(1)
// 消息头
struct Head
{	
	unsigned int seq;
	unsigned int timestamp;	// 发送时的时间戳
	unsigned int rto;	// 重传超时	
	unsigned int size;
	bool ACK;	// 是否是ack消息
};

struct RealMsg
{
	Head m_head;	// 消息头
	std::string m_data;	// 消息体（保存数据实体）

	// 默认构造函数
	RealMsg();

	// 根据消息体，构造消息
	RealMsg(const char* data, int len, unsigned int rto=2);

	// 从序列化的消息结构中，构造消息
	RealMsg(std::string buf);

	// 生成确认包
	RealMsg* GetACK();

	// 序列化消息
	std::string Serialize();

	// 反序列化消息
	void Deserialize(std::string buf);
};
#pragma pack()
