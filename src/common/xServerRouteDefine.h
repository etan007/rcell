#pragma once

// 默认路由层级
#define ROUTE_NODE_LEVEL_NORMAL 3


// 负载定义
#define SERVER_LOAD_WARNING 2000
#define SERVER_LOAD_FULL 4000

// 协议注册的执行线程类型
enum CMD_RUN_THREAD_TYPE
{
    // 网络线程执行
    CMD_RUN_THREAD_NET = 0,
    // 逻辑线程执行
    CMD_RUN_THREAD_LOGIC,

    CMD_RUN_THREAD_MAX,
};

// 服务器类型ID定义 
// 注意 需要和 t_servername 中的定义一一对应
enum SERVER_ROUTE_SID_DEF
{
    /* *******************************************
     *
     *    SManager 和 SLauncher 服务器为管理服务器并非游戏逻辑服务器
     *
     */


    SERVER_SID_LOGIN_GATE = 1,    // 登录服务器 
    SERVER_SID_DATE_RECORD = 2,    // 数据服务器 
    // 大厅服务器
    SERVER_SID_LOBBY_GATE = 3,    // 大厅网关服务器 
    SERVER_SID_LOBBY_CENTER = 103,  // 大厅中心服务器 
    // 家园服务器
    SERVER_SID_HOME_LOGIC = 4,    // 家园逻辑服务器 
    SERVER_SID_HOME_CENTER = 104,  // 家园中心服务器 

    SERVER_SID_BATTLRREPORT_LOGIC = 5,    // 战报服务器 

    // 匹配服务器
    SERVER_SID_MATCH = 10,   // 匹配服务器 

    // 平台好友服务器组
    SERVER_SID_FRIEND_LOGIC = 11,   // 好友服务器 逻辑服务器
    SERVER_SID_FRIEND_CENTER = 111,  // 好友服务器 中心服务器
    SERVER_SID_FRIEND_ROUTE = 12,   // 好友服务器 全局路由服务器

    // 平台支付服务器组
    SERVER_SID_MARKET_LOGIC = 15,   // 支付服务器 逻辑服务器
    SERVER_SID_MARKET_CENTER = 115,  // 支付服务器 中心服务器 对应一个数据中心服务的玩家业务
    SERVER_SID_MARKET_ROUTE = 16,   // 支付服务器 全局路由服务器

    SERVER_SID_MAIL_SERVER = 18, // 邮件服务器
    SERVER_SID_GM_SERVER = 19, // GM 服务器

    SERVER_SID_GAME_LOG         = 20,   // 游戏日志服务器 
	//交易行服务器
	SERVER_SID_TRADEBANK = 21,	//交易行服务器

	// 公会服务器组
	SERVER_SID_GUILD_LOGIC = 21,   // 公会服务器 逻辑服务器
	SERVER_SID_GUILD_CENTER = 121,  // 公会服务器 中心服务器 对应一个数据中心服务的玩家业务
	SERVER_SID_GUILD_ROUTE = 22,   // 公会服务器 全局路由服务器
};

