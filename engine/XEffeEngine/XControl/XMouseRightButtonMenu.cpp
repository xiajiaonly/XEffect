//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMouseRightButtonMenu.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourceManager.h"

_XMouseRightButtonMenu::_XMouseRightButtonMenu()
:m_isInited(XFalse)
,m_menu(NULL)				//��ť��ָ��
,m_menuSum(0)				//��ť������
,m_nowChoose(-1)			//��ǰѡ���ֵ��-1ʲôҲû��ѡ��//��ʹ�ü���ѡ��˵�ʱ���ֵ����Ч
,m_lastChoose(-1)			//����ѡ���ֵ
,m_funChooseChange(NULL)	//�ı�ѡֵʱ����
,m_funChooseOver(NULL)		//����ȷ��ѡֵʱ����
,m_resInfo(NULL)
{
}
_XMouseRightButtonMenu::~_XMouseRightButtonMenu()
{
	release();
}
inline void _XMouseRightButtonMenu::release()	//�ͷ���Դ
{
	if(!m_isInited) return;
	XDELETE_ARRAY(m_menu);
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = XFalse;
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
_XBool _XMouseRightButtonMenu::init(int menuSum,	//�˵��е��������
		const _XVector2& position,	//�˵���λ��
		const _XRect& Area,	//�˵���������Ӧ��Χ
		const _XMouseRightButtonMenuTexture &tex,	//�˵�����ͼ
		const _XFontUnicode &font,float captionSize, const _XVector2& textPosition)		//�˵�������
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(tex.buttonNormal == NULL || tex.buttonOn == NULL) return XFalse;//�⼸����ͼ�Ǳ���Ҫ�е�
	if(menuSum <= 0) return XFalse;	//û�в˵���ĳ�ʼ����ʧ�ܵ�
	m_position = position;
	m_mouseRect = Area;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�

	//���ø����˵���
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//�ڴ����ʧ����ֱ�ӷ���ʧ��

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(m_menu[i].init(_XVector2(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y),m_mouseRect,tex," ",font,captionSize,textPosition) == 0)
		{
			XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setSize(m_size);
	//����Щ����������������ע����
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//����ѡ���ֵ

	m_isVisiable = XFalse;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMouseRightButtonMenu::initEx(int menuSum,	//�˵��е��������
	const _XVector2& position,	//�˵���λ��
	const _XMouseRightButtonMenuTexture &tex,	//�˵�����ͼ
	const _XFontUnicode &font,float captionSize)		//�˵�������
{
	if(m_isInited) return XFalse;
	if(tex.m_mouseRect.getHeight() <= 0 || tex.m_mouseRect.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(tex.buttonNormal == NULL || tex.buttonOn == NULL) return XFalse;//�⼸����ͼ�Ǳ���Ҫ�е�
	if(menuSum <= 0) return XFalse;	//û�в˵���ĳ�ʼ����ʧ�ܵ�
	m_position = position;
	m_mouseRect = tex.m_mouseRect;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�

	//���ø����˵���
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//�ڴ����ʧ����ֱ�ӷ���ʧ��

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(m_menu[i].initEx(_XVector2(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y),tex," ",font,captionSize) == 0)
		{
			XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setSize(m_size);
	//����Щ����������������ע����
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//����ѡ���ֵ

	m_isVisiable = XFalse;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMouseRightButtonMenu::initPlus(const char * path,int menuSum,	//�˵��е��������
	const _XFontUnicode &font,float captionSize,_XResourcePosition resoursePosition)		//�˵�������
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XButtonTexture * tex = (_XButtonTexture *)m_resInfo->m_pointer;

	if(tex->m_mouseRect.getHeight() <= 0 || tex->m_mouseRect.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(tex->buttonNormal == NULL || tex->buttonOn == NULL) return XFalse;//�⼸����ͼ�Ǳ���Ҫ�е�
	if(menuSum <= 0) return XFalse;	//û�в˵���ĳ�ʼ����ʧ�ܵ�
	m_position.set(0.0f,0.0f);
	m_mouseRect = tex->m_mouseRect;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�

	//���ø����˵���
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//�ڴ����ʧ����ֱ�ӷ���ʧ��

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(!m_menu[i].initEx(_XVector2(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y),(*tex)," ",font,captionSize))
		{
			XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setSize(m_size);
	//����Щ����������������ע����
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//����ѡ���ֵ

	m_isVisiable = XFalse;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMouseRightButtonMenu::initWithoutTex(int menuSum,const _XRect& area,
	const _XFontUnicode &font,float captionSize, const _XVector2& textPosition)
{
	if(m_isInited) return XFalse;
	if(menuSum <= 0) return XFalse;	//û�в˵���ĳ�ʼ����ʧ�ܵ�
	m_position.set(0.0f,0.0f);
	m_mouseRect = area;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�

	//���ø����˵���
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//�ڴ����ʧ����ֱ�ӷ���ʧ��

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(m_menu[i].initWithoutTex(" ",font,captionSize,m_mouseRect,textPosition) == 0)
		{
			XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setSize(m_size);
	//����Щ����������������ע����
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//����ѡ���ֵ

	m_isVisiable = XFalse;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XMouseRightButtonMenu::draw()
{
	if(!m_isInited) return;		//û�г�ʼ��ʱ����ʾ
	if(!m_isVisiable) return;	//���ɼ�ʱ����ʾ
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].draw();
	}
}
void _XMouseRightButtonMenu::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y);
	}
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�
	m_nowChoose = -1;
	updateChildPos();
}
void _XMouseRightButtonMenu::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y);
		m_menu[i].setSize(m_size);
	}
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�
	m_nowChoose = -1;
	updateChildSize();
}
_XBool _XMouseRightButtonMenu::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
_XBool _XMouseRightButtonMenu::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(!m_isVisiable)
	{//û����ʾ��ʱ����Ҫͨ���������Ҽ�������Ƿ񵯳��˵�
		if(mouseState == MOUSE_RIGHT_BUTTON_UP)
		{//�Ҽ���������ʾ�˵�
			m_isVisiable = XTrue;
			setPosition(x,y);
		}
	}else
	{//��ʾʱ�԰�����������Ӧ
		for(int i = 0;i < m_menuSum;++ i)
		{
			m_menu[i].mouseProc(x,y,mouseState);
		}
		if(m_allArea.isInRect(x,y))
		{
			if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y);
				if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
				//�ָ����а�����״̬Ϊ��ͨ״̬
				for(int i = 0;i < m_menuSum;++ i)
				{
					m_menu[i].setState(BUTTON_STATE_NORMAL);
				}
				m_isVisiable = XFalse;
			}
		//	m_upMousePoint.set(x,y);
			//m_upMousePoint.y = (int)((y - m_objRect.top) / m_objRect.getHeight() * m_size.y) * m_objRect.getHeight() * m_size.y + m_objRect.getHeight() * m_size.y * 0.5;
		}else
		{//������Ĳ���
			if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = -1;
				if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
				m_isVisiable = XFalse;
			}
		}
	}
	return XTrue;
}
_XBool _XMouseRightButtonMenu::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(!m_isVisiable) return XFalse;	//���̲���ֻ������ʾ������²�����ʾ
	if(keyState == KEY_STATE_UP)
	{
		if(keyOrder == XKEY_UP)
		{//���ϼ�
			-- m_nowChoose;
			if(m_nowChoose < 0) m_nowChoose = m_menuSum -1;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_nowChoose + 0.5f)) * m_size.y;

		//	m_upMousePoint.y -= m_objRect.getHeight() * m_size.y;
		//	if(m_upMousePoint.y < m_allArea.top)
		//	{
		//		m_upMousePoint.y += m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
		}else
		if(keyOrder == XKEY_DOWN)
		{//���¼�
			++ m_nowChoose;
			if(m_nowChoose >= m_menuSum) m_nowChoose = 0;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_nowChoose + 0.5f)) * m_size.y;

		//	m_upMousePoint.y += m_objRect.getHeight() * m_size.y;
		//	if(m_upMousePoint.y > m_allArea.bottom)
		//	{
		//		m_upMousePoint.y -= m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
		}else
		if(keyOrder ==  XKEY_RETURN)
		{//�س���
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(m_menu[i].getState() == BUTTON_STATE_ON)
				{
					m_lastChoose = i;
					if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
					m_isVisiable = XFalse;
					return XTrue;
				}
			}
		}else
		{
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(keyOrder == m_menu[i].getHotKey() && m_menu[i].getState() != BUTTON_STATE_DISABLE)
				{//���������˵��Ŀ�ݼ���ͬ�����Ҳ˵���״̬������Ч
					m_lastChoose = i;
					if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
					m_isVisiable = XFalse;
					return XTrue;
				}
			}
		}
	}
	return XTrue;
}
_XBool _XMouseRightButtonMenu::setACopy(const _XMouseRightButtonMenu &temp)
{
	if(& temp == this) return XTrue;	//��ֹ������ֵ
	if(!temp.m_isInited) return XFalse;	//���Ŀ��û�г�ʼ����ֱ�ӷ���
	if(m_isInited)	release();//��������Ѿ���ʼ��,�ͷŵ���Ӧ����Դ
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	//���ø����˵���
	m_menuSum = temp.m_menuSum;		//��ť������
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//�ڴ����ʧ����ֱ�ӷ���ʧ��

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(!m_menu[i].setACopy(temp.m_menu[i])) return XFalse;
		//����Щ�ؼ��ӿؼ���������ע����
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}

	m_nowChoose = temp.m_nowChoose;		//��ǰѡ���ֵ��-1ʲôҲû��ѡ��//��ʹ�ü���ѡ��˵�ʱ���ֵ����Ч
	m_lastChoose = temp.m_lastChoose;		//����ѡ���ֵ

	m_funChooseChange = temp.m_funChooseChange;	//�ı�ѡֵʱ����
	m_funChooseOver = temp.m_funChooseOver;		//����ȷ��ѡֵʱ����

	m_allArea = temp.m_allArea;		//��궯������Ӧ����
	m_upMousePoint = temp.m_upMousePoint;	//�ϴμ�¼������ڷ�Χ�ڵ�λ��

	return XTrue;
}
_XBool _XMouseRightButtonMenu::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XMouseRightButtonMenu::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_allArea.left,m_allArea.top);else
	if(order == 1) ret.set(m_allArea.right,m_allArea.top);else
	if(order == 2) ret.set(m_allArea.right,m_allArea.bottom);else
	if(order == 3) ret.set(m_allArea.left,m_allArea.bottom);
	return ret;
}