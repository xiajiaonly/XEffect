#ifndef _JIA_XFRAMEWORK_
#define _JIA_XFRAMEWORK_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
//框架的主类
#include "XErrorReporter.h"
#if WITH_LOG
#include "XLogBook.h"
#else
#include "XTimer.h"
#endif
#include "XEngineCommon.h"
#include "XMouseAndKeyBoardDefine.h"
#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
#include "XInputEventBuff.h"
#include "XMath\XMoveData.h"
#include "XCritical.h"
class XLicenseBase
{
public:
	virtual std::string createStr() = 0;	//生成认证码
	virtual std::string createKey() = 0;	//生成验证码
	virtual bool certification() = 0;		//是否验证通过
	virtual std::string getCodeStr()const = 0;
	virtual std::string getKeyStr()const = 0;
	virtual void setKey(const char * str) = 0;	//设置激活码
	virtual void setCodeStr(const char *code) = 0;

	virtual bool saveCode() = 0;
	virtual bool readCode() = 0;
	virtual bool saveKey() = 0;
	virtual bool readKey() = 0;
	virtual bool init() = 0;
};
namespace XE{
class XFrameWork;
class XMusic;
class XSound;
class XConfigManager;
class X3DWorld;
class XFontTTF;
class XOperateManager;
class XSoundCore;
class XWindowCore;
class XTextureResourceManager;
class XResourcePack;
class XResourceManager;
class XControlManager;
#if WITH_OBJECT_MANAGER
class XObjectManager;
#endif
//下面是框架全局的东西
struct XFrameWorkData
{
	XFrameWork *pFrameWork;			//框架的指针
#if WITH_OBJECT_MANAGER
	XObjectManager *pObjManager;		//物件管理器
#endif
	XControlManager *pCtrlManager;	//控件管理器
#if WITH_LOG
	XLogbook *pLog;					//日志的指针
#endif
	XResourceManager *pResManager;	//资源管理器
	XResourcePack *pResPack;			//资源封包器
	XTextureResourceManager *pTexManager;		//贴图管理器
	XErrorReporter *pErrReporter;	//错误接收器
	XMusic *pMusicPlayer;			//音乐播放器
	XSound *pSoundPlayer;			//音效播放器
	XConfigManager *pCfgManager;
	X3DWorld *p3DWorld;
	XFontTTF *pFontTTF;
	XOperateManager *pOperateManager;
	XSoundCore *pSoundCore;			//音效核心的指针
	XWindowCore *pWindowCore;		//窗口的核心指针
	XLicenseBase *pLicense;		//许可容器
	XFrameWorkData()
		:pFrameWork(NULL)
#if WITH_OBJECT_MANAGER
		, pObjManager(NULL)
#endif
		, pCtrlManager(NULL)
#if WITH_LOG
		, pLog(NULL)					//日志的指针
#endif
		, pResManager(NULL)
		, pResPack(NULL)
		, pTexManager(NULL)
		, pErrReporter(NULL)
		, pMusicPlayer(NULL)			//音乐播放器
		, pSoundPlayer(NULL)			//音效播放器
		, pCfgManager(NULL)
		, p3DWorld(NULL)
		, pFontTTF(NULL)
		, pOperateManager(NULL)
		, pSoundCore(NULL)
		, pWindowCore(NULL)
		, pLicense(NULL)
	{}
#if WITH_LOG
	bool create(bool isIniLog = false, XLogBookMode mode = LOGBOOK_MODE_SIMPLE,
		const std::string& serverFilename = "../LogServer/LogServer.exe");
#else
	bool create();
#endif
	void release();
};
extern XFrameWorkData gFrameworkData;	//框架的全局变量
//日志的相关接口
#if WITH_LOG
#define LogStr gFrameworkData.pLog->addLogInfoStr
#define LogNull gFrameworkData.pLog->addLogInfoNull
#define LogExp gFrameworkData.pLog->addLogInfoExp
#define LogPause gFrameworkData.pLog->disable
#define LogResume gFrameworkData.pLog->enable
#define LogRelease gFrameworkData.pLog->release
#else
#define LogStr(p) (void)(p)
#define LogNull (void)
#define LogExp (void)
#define LogPause (void)
#define LogResume (void)
#define LogRelease (void)
#endif
inline void reportError(const std::string& errorDescription, const std::string& filename,
	int line, const std::string& funName)
{
	gFrameworkData.pErrReporter->reportError(errorDescription, filename, line, funName);
}
#define REPORT_ERROR(p) reportError(p,__FILE__,__LINE__,__FUNCDNAME__)
//为了访问时的书写方便，下面定义一些宏，下面这些宏都会因为指针的无效而造成crash，需要重新封装
#define XEG (*gFrameworkData.pFrameWork)	//这种用法逻辑上不严谨可能因为指针没有初始化而造成crash，需要重新封装
#define XCtrlManager (*gFrameworkData.pCtrlManager)
#define XResManager (*gFrameworkData.pResManager)
#if WITH_OBJECT_MANAGER
#define XObjManager (*gFrameworkData.pObjManager)
#endif
#define XResPack (*gFrameworkData.pResPack)
#define XTexManager (*gFrameworkData.pTexManager)
#define XErrReporter (*gFrameworkData.pErrReporter)
#define XCFGManager (*gFrameworkData.pCfgManager)
#define XTTFFont (*gFrameworkData.pFontTTF)
#define XOpManager (*gFrameworkData.pOperateManager)
#define XCurSndCore (*gFrameworkData.pSoundCore)
#define XWindow (*gFrameworkData.pWindowCore)
#define XSoundPlayer (*gFrameworkData.pSoundPlayer)
#define XMusicPlayer (*gFrameworkData.pMusicPlayer)
#define X3DWld (*gFrameworkData.p3DWorld)
#define pXLicense (gFrameworkData.pLicense)
enum XColorStyle
{
	COLOR_STYLE_NORMAL,
	COLOR_STYLE_LIGHT,
};
class XSoftBoard;
class XPressDataDouble;
class XCheatModule;
class XConfigManager;
class XWindowTitle;
struct XInputEvent;
class XSimpleTimer;
class XCaregiver;
class XBabyBase;
class XPBO;
class XProjectionCorrection;
class XMaskCorrection;
inline void XEE_Error(const char *p){ REPORT_ERROR(p); }
//这样设计的目的是减少单子实例的数量，只存在这一个单子实例，其他的所有单子实例都作为这个实例的内部成员
//这一部优化目前并未进行，因为存在很多需要考虑的因素
enum XCorrectionStage
{
	CSTAGE_NULL,
	CSTAGE_PROJECTION,
	CSTAGE_MASK,
};
class XFrameWork
{
public:
	XFrameWork();
	virtual ~XFrameWork();
protected:
	XFrameWork(const XFrameWork&);
	XFrameWork &operator= (const XFrameWork&);
private:
	float m_rightBtnDownTimer;
	bool m_isInited;
	bool m_withVirtualMouse;	//是否使用虚拟鼠标
	bool m_isPassKeyLicense;	//是否通过认证
	XPressDataDouble *m_moveX;
	XPressDataDouble *m_moveY;
	XProjectionCorrection *m_projCorrection;
	XMaskCorrection *m_maskCorrection;
	XCorrectionStage m_corStage;
#if LIMITED_VERSION == 1
	float m_limitedTimer;
#endif
	XCaregiver *m_pCaregiver;	//看护类
public:
	bool addABaby(XBabyBase *baby, int resetTimer = 1000);
	static bool cmdProc(void *, const char *, std::string &);
	//所有的全局变量，全局函数全部都要归总到这个类里面
	XCheatModule *m_cheatModule;	//命令输入行的成员
	XSoftBoard *m_softBoard;		//虚拟键盘的成员
	bool initCommon();		//初始化一些系统的资源，这个在最开始调用
	void updateCommon(float stepTime);
	bool initCommonAfterSet();	//在用户调用之后进行相应的初始化
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是引擎对外开放的组件
//private:
//	XConfigManager *m_pConfigMng;
//public:
//	XConfigManager &getConfigManager(){return *m_pConfigMng;}
	//--------------------------------------------------------------
	XWindowInfo m_windowData;		//窗口的相关数据

	//下面是主动式的帧率控制，效果是控制的比较准确，缺点是空载时CPU占用率较高
	//XSimpleTimer m_fpsTimer;
private:
	bool m_isMouseLBDown;	//是否左键按下
	bool m_isMouseRBDown;	//是否右键按下
public:
	bool getIsMouseLBDown()const { return m_isMouseLBDown; }
	bool getIsLBDrag(const XInputEvent &event)const;
	bool getIsMouseRBDown()const { return m_isMouseRBDown; }
	bool getIsRBDrag(const XInputEvent &event)const;
	//下面是engine的一些参数
	void setForceToPaint(bool flag) { m_forceToPaint = flag; }
	bool setIsForceToPaint()const { return m_forceToPaint; }
private:
	HWND m_hwnd;			//窗口的句柄
	HDC m_wHDC;				//窗口容器的指针
	HGLRC m_wCurHGLRC;		//当前的上下文
	HGLRC m_wCopyHGLRC;		//当前上下文的副本
	bool m_forceToPaint;	//如果设置会false则窗口最小化之后会跳过窗口绘制，否则强制绘制
	bool m_isGameExit;		//是否准备退出程序
	int m_windowWidth;		//当前窗口的宽度
	int m_windowHeight;		//当前窗口的高度
	int m_scenePosX;			//场景的位置	//目前这个定义尚不完整，在很多模式下并没有效果
	int m_scenePosY;			//场景的位置
	int m_sceneWidth;		//场景的宽度
	int m_sceneHeight;		//场景的高度
	float m_sceneScaleX;		//窗口的缩放比例
	float m_sceneScaleY;		//窗口的缩放比例
	float m_viewAngle3D;		//3D观察的角度
	XVec2 m_mousePosition;	//当前的鼠标位置，经过填充矫正
	XVec2 m_touchPosition;	//当前触摸的点的位置，经过填充矫正
	XVec2 m_upMousePosition;	//上一帧的鼠标位置
	std::string m_defWorkPath;	//默认的工作路径，有的插件会修改工作路径导致相对路径的操作会造成错误，所以这里需要该项来做修正
	//+++++音频参数+++++
	int m_audioSampleRate;	//音频初始化的频率
	int m_audioFormat;		//音频的格式
	int m_audioChannel;	//音频的声道
	//-----音频参数-----
	unsigned int m_mainThreadHandle;	//主线程的ID

	bool m_isFrameTimerEscape;	//时间帧是否发生偷跑
public:
	bool getFrameTimerEscape()const { return m_isFrameTimerEscape; }
	//设置帧时间偷跑，避免画面跳跃
	void setFrameTimerEscape() { m_isFrameTimerEscape = true; }
private:
	float m_frameTime;	//上一帧的时间,单位为毫秒
	float m_curFPS;	//当前的逻辑帧率
	float m_maxFrameStep;	//帧时间差
	int m_rendDelay;	//显示的刷新时间，单位为ms，默认为33毫秒属性一次，太快的画面刷新速度实际上没有意义，逻辑帧率快才有意义
	XBool m_stopFlag;	//是否暂停
	int m_manualFPS;	//-1是不设置帧率
	float m_manualFrameTime;	//设置的每帧的时间

	XResPos m_defResPos;	//默认资源的位置

	XBool m_isOpenGL2Supported;	//OpenGL是否支持2.0是否支持GLSL
	XBool m_isLineSmooth;		//是否使用划线的平滑处理
	XBool m_isMultiSampleSupported;	//是否支持多重采样
	XBool m_isFboSupported;	//是否支持FBO
	int m_maxTextureSize;		//显卡支持的最大贴图尺寸
	int m_maxTextureSum;	//一次能同时使用的贴图的数量
	int m_multiSampleSum;	//多重采样的数量

	bool m_isProjectAsDaemon;	//是否程序作为后台窗口运行

	char m_autoShutDownState;			//自动关机的状态，只有跨越设定时间才会触发自动关机
	XSimpleTimer m_autoShutDownTimer;			//自动关机的计时
	float m_showVersionTimer;			//显示版本号的计时器
	XFontUnicode *m_showVersionFont;	//显示版本号的字符串
public:
	//下面是关于自定义标题栏的支持
	XWindowTitle *m_customWinTitle;	//自定义的窗口标题
	XFontUnicode *m_systemFont;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对描绘字体的支持
//	bool drawFont(float x, float y, const char *str);
	bool drawFont(const XVec2& pos, const char *str);
	bool drawFont(const XVec2& pos, const char *str, const XFColor&c);
private:
	XMoveData m_fontMD;
	std::string m_topMsgStr;
public:
	void setTopMsgStr(const std::string &str,bool withLog = true);
	//----------------------------------------------------------------
	float m_errorExitTimer;	//发送错误时退出的计时
	XBool m_haveSystemFont;

	bool m_deamonWithConsole;	//托盘菜单中是否有控制台
	NOTIFYICONDATA m_nid;			//当关闭控制台时会造成这里菜单没有正常关闭
	LRESULT(*m_customWinProc)(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, bool &end);	//自定义的系统消息处理函数
	LRESULT daemonProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, bool &end);

	XFColor m_defBGColor;	//默认的背景颜色
	std::string m_programPropName;	//进程属性的名字
	HANDLE m_programPropValue;		//进程属性的值
	XBool m_isSuspend;	//程序是否进入中断，如果进入中断的话要退出loading的多线程
private:
	bool m_textureNeedFit2N;	//贴图是否会自适应2的n次方的尺寸(目前这个功能并不完善，需要进一步全面严谨逻辑)
	XCritical m_hdcMutex;
public:
	void hdcLock() { m_hdcMutex.Lock(); }
	void hdcUnlock() { m_hdcMutex.Unlock(); }
	void setTexNeedFit2N(bool flag) { m_textureNeedFit2N = flag; }		//(目前这个功能并不完善，需要进一步全面严谨逻辑)
	bool getTexNeedFit2N()const { return m_textureNeedFit2N; }			//(目前这个功能并不完善，需要进一步全面严谨逻辑)
	HDC getHDC()const { return m_wHDC; }
	HGLRC getCopyHGLRC()const { return m_wCopyHGLRC; }
	HWND getHWND()const { return m_hwnd; }
	bool getIsGameExit()const { return m_isGameExit; }
	void setGameExit() { m_isGameExit = true; }		//设置游戏退出
	float getViewAngle()const { return m_viewAngle3D; }
	void setViewAngle(float angle) { m_viewAngle3D = angle; }
	int getWinW()const { return m_windowWidth; }
	int getWinH()const { return m_windowHeight; }
	int getScenePosX()const { return m_scenePosX; }
	int getScenePosY()const { return m_scenePosY; }
	int getSceneW()const { return m_sceneWidth; }
	int getSceneH()const { return m_sceneHeight; }
	XVec2 getWindowSize()const { return XVec2(m_windowWidth, m_windowHeight); }
	XVec2 getSceneSize()const { return XVec2(m_sceneWidth, m_sceneHeight); }
	XVec2 getSceneCenter()const { return XVec2(m_sceneWidth, m_sceneHeight) * 0.5f; }
	float getLastFrameTime()const { return m_frameTime; }	//返回上一帧使用的时间单位ms
	float getElapsedTime()const { return XTime::getCurrentTicks(); }	//返回程序运行的时间，单位为ms
	float getCurFps()const { return m_curFPS; }
	XResPos getDefResPos()const { return m_defResPos; }
	XBool getIsFBOSupported()const { return m_isFboSupported; }		//判断底层是否支持FBO
	XBool getIsMultiSampleSupported()const { return m_isMultiSampleSupported; }		//是否支持多重采样
	XBool getIsLineSmooth()const { return m_isLineSmooth; }		//是否使用线性平滑
	int getMaxTextureSum()const { return m_maxTextureSum; }		//返回最多支持的贴图数量
	int getMaxTextureSize()const { return m_maxTextureSize; }		//返回所支持的贴图最大尺寸
	int getMultiSampleSum()const { return m_multiSampleSum; }
	XVec2 getMousePos()const { return m_mousePosition; }		//获取当前鼠标的位置
	XVec2 getTouchPos()const { return m_touchPosition; }		//获取当前鼠标的位置
	XVec2 getUpMousePos()const { return m_upMousePosition; }		//获取当前鼠标的位置

	int getAudioSampleRate()const { return m_audioSampleRate; }	//获取音频采样率
	int getAudioChannelSum()const { return m_audioChannel; }		//获取音频通道数
	int getAudioFormat()const { return m_audioFormat; }			//获取默认音频格式
	void setAudioSampleRate(int r) { m_audioSampleRate = r; }	//设置音频采样率
	void setAudioChannelSum(int c) { m_audioChannel = c; }		//设置音频通道数
	void setAudioFormat(int f) { m_audioFormat = f; }			//设置默认音频格式

	void setStop(int temp = -1);		//停止 0,play,1,stop,-1,change state
	XBool getStopState()const { return m_stopFlag; }
	//设置游戏的逻辑帧率，实际上显示帧率已经被限制，只有逻辑帧率是开放的，如果需要限制逻辑帧率的话可以通过这个函数
	void setFPS(int temp);				//设置游戏帧率，小于0为不限制帧率(目前这个函数好像没有起到应有的作用)
	void setRendFPS(int tmp);			//设置图像的渲染频率，这个函数必须在初始化函数中调用才能生效
	void setBGColor(const XFColor& c);	//设置背景的颜色
	void setStyle(XColorStyle style);	//选择颜色的配色方案
	unsigned int getMainThreadID()const { return m_mainThreadHandle; }	//获取主线程的ID

	void releaseIcon();	//释放icon资源
	void setProjectAsDaemon() { m_isProjectAsDaemon = true; }		//设置项目为后台项目
	bool getIsProjectAsDaemon()const { return m_isProjectAsDaemon; }	//判断项目是否为后台程序
	XBool setSystemFont(const char * filename);
private:
	void gameProc(void *game);	//游戏的主流程
public:
	XBool readSystemConfigData(XWindowInfo &data,bool forceTitle = false);			//从引擎默认的配置文件中读取配置数据
	XBool saveSystemConfigData(const XWindowInfo &data);	//将引擎默认的配置数据保存到配置文件
	const char *getDefWorkPath()const { return m_defWorkPath.c_str(); }
private:
	bool createWindow(int width, int height, const char *windowTitle = NULL,
		bool isFullScreen = false, bool withFrame = true);	//初始化SDL窗口
	XBool initWindow();
	XBool initWindowEx(const XWindowInfo& wData);	//这个接口已经废弃
	XBool initWindowEx(void);	//这个接口已经废弃
	XBool initGL();				//初始化OpenGL的数据
	void initGlew();
	void initWindowMatrix();	//初始化窗口的矩阵
	XBool initOpenGL3D();	//3D窗口的初始化一些openGL属性
	XBool initOpenGL2D();	//可以拉伸和旋转的窗口的初始化一些openGL属性

	void engineMove(float steptime);	//引擎的内部更新
	XBool engineInit();		//窗口建立之前的初始化
	void engineSysInit();	//窗口建立之后的初始化

	void engineDraw();	//描绘引擎相关的东西
	XBool m_needReset3DParam;	//是否需要回复3D参数的设置
public:
	void end2DDraw();	//结束2D描绘时回复环境状态
	XBool getWindowNeedDraw();	//获取当前窗口是否需要刷新
	XVec2 getMousePosFromWindow(int mouseX, int mouseY);
private:
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是框架的主要接口
	int suspendKeyProc();	//中断之后的输入事件处理

	XSimpleTimer *m_periodicLogTimer;	//周期性的日志计时器
	int m_curframeCounter;	//当前的总帧数
	XEE::XMemUsageInfo m_curMemInfo;	//当前的内存数据
	bool singletonProc();	//用于判断当前程序是否需要因为单例模式而退出。
public:
	//这是错误处理机制的前身，现在这个功能已经被XErrorReporter类替换，详细查看XErrorReporter的代码和说明
	void suspend(const char * str);	//中断并显示

	float engineIdle();		//引擎的自循环函数用于循环事件处理,返回两次调用之间的毫秒计时
	void engineRelease();	//引擎的资源释放
	void engineKeyProc(int key, XKeyState keyState);
	void engineMouseProc(int x, int y, XMouseState mouseState);

	void(*m_drawAfterCtrl)();	//描绘玩控件之后的描绘动作的回调函数
	void begin2DDraw();			//开始描绘2D元素时回复环境状态(在3D类型的项目中才需要这个操作)
	//withEngin是否描述引擎内部的数据
	void updateScreen(bool withEngin = true);		//更新屏幕的内容
	void *m_pGame;				//主游戏的指针
	void restartGame();			//重新运行当前程序
	void clearScreen();			//清除屏幕的内容
	void clearScreen(const XFColor& c);
	//void startGame(void *game,const XWindowInfo* windowInfo = NULL);	//不推荐使用
	template<class Type> void runGame(const XWindowInfo* windowInfo = NULL)
	{
		//先读取配置文件
		if (windowInfo == NULL)
		{
			if (!readSystemConfigData(m_windowData,true)) LogStr("读取配置文件失败!");
		}
		else
		{
			m_windowData = *windowInfo;
		}
		if (m_windowData.withException)
		{//处理异常
			__try {
				XException::initException();
				LogStr("程序开始");
				if (!initWindow())
					XEE_Error("建立窗体失败");
				if (!singletonProc())
				{
					Type *tmpGame = XMem::createMem<Type>();
					if (tmpGame == NULL) XEE_Error("游戏建立失败");
					gameProc(tmpGame);
					XMem::XDELETE(tmpGame);
				}
				else
				{
					LogStr("程序已经启动，不能重复启动!");
				}
				engineRelease();
			}
			__except (XException::unhandledExceptionFilter(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
			{//下面是关于异常的处理
				//XException::unhandledExceptionFilter(GetExceptionInformation());
				if (!m_isInited)
				{//如果没有初始化，则这里直接进行逻辑，不做处理
					LogStr("框架初始化失败!");
#if WITH_LOG
					LogRelease();	//保持日志文件的完整性
#endif
				//	Sleep(5000);	//等待10s钟，然后再做处理
				//	if(!XEG.getIsGameExit() && XEG.m_windowData.isRestartWhenCrash) XEG.restartGame();	//重新启动程序
					XEG.setGameExit();				//防止由于退出时再次产生异常
					exit(1);						//退出当前程序
				}
				else
				{//否则显示相关信息
					m_errorExitTimer = 0.0f;
					while (!inputEvent(false))
					{//程序主循环
						errorLoop();
					}
				}
			}
		}
		else
		{
			LogStr("程序开始");
			if (!initWindow())
				XEE_Error("建立窗体失败");
			if (!singletonProc())
			{
				Type *tmpGame = XMem::createMem<Type>();
				if (tmpGame == NULL) XEE_Error("游戏建立失败");
				gameProc(tmpGame);
				XMem::XDELETE(tmpGame);
			}
			else
			{
				LogStr("程序已经启动，不能重复启动!");
			}
			engineRelease();
		}
	}
	void touchProc(const TOUCHINPUT& ti);
private:
#if CREATE_WINDOW_METHOD == 0 || CREATE_WINDOW_METHOD == 2
	//send2Game是否将输入事件传递给游戏
	bool inputEvent(bool send2Game = true);
#endif
#if CREATE_WINDOW_METHOD == 1
	static void glutIdle();
	static void glutDraw();
	void inputProc(const XInputEvent &input);
	bool inputEvent(bool send2Game) { return false; }
	static void glutInput(unsigned char c, int x, int y);
	static void glutsInput(int k, int x, int y);
	static void glutMouseMove(int x, int y);
	static void glutMouseProc(int b, int s, int x, int y);
#endif
	bool m_isImmState;
public:
	bool inputEvent(const XInputEvent &tmpEvent, bool send2Game = true);
	void miniInputEvent(const XInputEvent &tmpEvent);
private:
	//为了方便统一SDL与Glut的封装，这里定义几个接口
	void mainLoop2D();	//2D游戏的循环
	void mainLoop3D();	//3D游戏的循环
	void errorLoop();	//发送错误的循环
	XSimpleTimer m_drawTimer;	//显示的计时器，显示太快实际上没有意义，逻辑线程可以跑很快，但是显示线程没有必要很快
	//--------------------------------------------------------------
public:
	//窗口相关的操作
	//设置窗口的 透明度
	void setWindowAlpha(float a);
	void getWindowPos(int &x, int &y);
	XVec2 getWindowPos();
	XVec2 getClientPos(const XVec2& screenPos);
	void getWindowSize(int &w, int &h);
	void minSizeWindow() { ShowWindow(m_hwnd, SW_MINIMIZE); }
	void setWindowSize(float x, float y);
	void setWindowPos(int x, int y, bool always = false);	//设置窗口的位置
	void setWindowHide() { ShowWindow(m_hwnd, SW_HIDE); }	//隐藏窗口
	void setWindowShow() { ShowWindow(m_hwnd, SW_NORMAL); }	//显示窗口
#ifdef XEE_OS_WINDOWS
	//该函数用于给程序设置一个属性，并通过该属性来判断该程序是否已经运行，从而保持程序运行的唯一性
	bool isProgramExist(const std::string& name);
#endif
	//++++++++++++++++++++++++++++++++++++
	//窗口和全屏切换的一个测试
	bool m_isSetToFullScreen;	//是否被设置为全屏模式
	void toggleFullScreen();	//目前这个方法尚不稳定(beta)
	//------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对一些系统数据的统计
	//1、总贴图数量
	//2、总贴图面积
	//3、总贴图占用的显存大小
	//4、每次刷新的贴图面积
	//5、内存使用带宽
#if WITH_ENGINE_STATISTICS
private:
	bool m_withEngineStatistics;	//是否开启引擎级别的统计
	int m_totalTextureSum;			//总的贴图数量
	int m_totalTextureArea;		//总的贴图面积
	int m_totalVideoMemUsage;		//总的显存使用量
	int m_totalFrameTextureArea;	//每帧刷新贴图的面积(尚未实现)
	float m_curVideoMemBandwidthUsage;	//当前内存带宽的使用(尚未实现)
public:
	void enableEngineStatistics() { m_withEngineStatistics = true; }
	void addStaticsticsTexInfo(unsigned int id, int level = 0, int type = GL_TEXTURE_2D);
	void decStaticsticsTexInfo(unsigned int id, int level = 0, int type = GL_TEXTURE_2D);
	void logEngineStatistics();			//将引擎统计数据输出到日志
#endif
	//--------------------------------------------------------------
	//withThread：是否使用多线程
	//将屏幕从点x，y开始的w和h矩形内的图像保存到png文件
	XBool gameShot(const char *fileName, int w, int h, bool withThread = true, float x = 0.0f, float y = 0.0f);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是关于透明窗口的定义
#if BACKGROUND_ALPHA_MODE == 1
private:
	bool m_isBGAlphaInited;
	RECT m_rc;
	POINT m_ptDest;
	SIZE m_sizeDest;
	HDC m_hMemDc;
	BITMAPINFO m_bmpinfo;
	void* m_pBits;
	HBITMAP m_hBitmap;
	HGDIOBJ m_oldGDIObj;
	BLENDFUNCTION m_blf;
	POINT m_ptSrc;
	XPBO *m_bgAlphaPbo;
	void initBgAlphaData();		//初始化透明背景需要的资源
	void releaseBhAlphaData();	//释放透明背景的资源
#endif
	//--------------------------------------------------------------
	//++++++++
private:
	XInputEventBuff m_inputEventBuff;
public:
	template <class T>
	bool addEventProc(T *pClass, void(T::*fun)(const XInputEvent &e))
	{
		return m_inputEventBuff.addEventProc<T>(pClass, fun);
	}
	template <class T>
	bool desEventProc(T *pClass, void(T::*fun)(const XInputEvent &e))
	{
		return m_inputEventBuff.desEventProc(pClass, fun);
	}
public:
	//--------
};
//下面使一些全局的方法的定义
#if WITH_LOG
template<class Type> void runGame(const XWindowInfo* windowInfo = NULL,
	const std::string& title = "",
	bool isIniLog = false,XLogBookMode mode = LOGBOOK_MODE_SIMPLE,
	const std::string& serverFilename = "../LogServer/LogServer.exe")
#else
template<class Type> void runGame(const XWindowInfo* windowInfo = NULL,
	const std::string& title = "")
#endif
{
#if WITH_LOG
	if (!gFrameworkData.create(isIniLog, mode, serverFilename)) return;
#else
	if (!gFrameworkData.create()) return;
#endif
	XEG.m_windowData.windowTitle = title;
	XEG.runGame<Type>(windowInfo);
	gFrameworkData.release();
}
inline std::string getCommonResPos()
{
	if(gFrameworkData.pFrameWork == NULL) return XString::gNullStr;
	else return gFrameworkData.pFrameWork->m_windowData.commonResourcePos;
}
inline XResPos getDefResPos()
{
	if(gFrameworkData.pFrameWork == NULL) return RES_AUTO;
	else return gFrameworkData.pFrameWork->getDefResPos();
}
inline int getWindowWidth()
{
	if(gFrameworkData.pFrameWork == NULL) return 0;
	else return gFrameworkData.pFrameWork->getWinW();
}
inline int getWindowHeight()
{
	if(gFrameworkData.pFrameWork == NULL) return 0;
	else return gFrameworkData.pFrameWork->getWinH();
}
inline XVec2 getWindowSize()
{
	if (gFrameworkData.pFrameWork == NULL) return XVec2::zero;
	else return gFrameworkData.pFrameWork->getWindowSize();
}
inline int getSceneWidth()
{
	if(gFrameworkData.pFrameWork == NULL) return 0;
	else return gFrameworkData.pFrameWork->getSceneW();
}
inline int getSceneHeight()
{
	if(gFrameworkData.pFrameWork == NULL) return 0;
	else return gFrameworkData.pFrameWork->getSceneH();
}
extern const XFontUnicode& getDefaultFont();
inline const XVec2 getMousePos()
{
	if(gFrameworkData.pFrameWork == NULL) return XVec2::zero;
	else return gFrameworkData.pFrameWork->getMousePos();
}
//制定格式的图片文件是否被支持
extern bool getIsImageSupport(const std::string& ext);
extern bool getIsSoundSupport(const std::string& ext);
#if WITH_INLINE_FILE
#include "XFrameWork.inl"
#endif
}

#endif