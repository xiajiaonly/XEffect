INLINE XBool XImageList::canGetFocus(const XVec2& p)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
	!m_isActive ||		//没有激活的控件不接收控制
	!m_isVisible ||	//如果不可见直接退出
	!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XImageList::disable()		//使控件无效
{
	m_leftBtn.disable();	//左边的按钮
	m_rightBtn.disable();	//右边的按钮
	m_imageSld.disable();	//滑动条
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XImageList::enable()		//使控件有效
{
	m_leftBtn.enable();	//左边的按钮
	m_rightBtn.enable();	//右边的按钮
	m_imageSld.enable();	//滑动条
	m_isEnable = XTrue;
}
INLINE void XImageList::setVisible()
{
	m_leftBtn.setVisible();	//左边的按钮
	m_rightBtn.setVisible();	//右边的按钮
	m_imageSld.setVisible();	//滑动条
	m_isVisible = XTrue;
}
INLINE void XImageList::disVisible()
{
	m_leftBtn.disVisible();	//左边的按钮
	m_rightBtn.disVisible();	//右边的按钮
	m_imageSld.disVisible();	//滑动条
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
}
//INLINE void XImageList::setCallbackFun(void (* funInit)(void *,int),
//		void (* funRelease)(void *,int),
//		void (* funSelectChange)(void *,int),
//		void *pClass)
//{
//	m_funInit = funInit;
//	m_funRelease = funRelease;
//	m_funSelectChange = funSelectChange;
//	if(pClass != NULL) m_pClass = pClass;
//	else m_pClass = this;
//}	
//INLINE void XImageList::setLostFocus()	//设置失去焦点
//{
//	if(!m_isInited ||	//如果没有初始化直接退出
//		!m_isActive ||		//没有激活的控件不接收控制
//		!m_isVisible ||	//如果不可见直接退出
//		!m_isEnable) return;		//如果无效则直接退出
//
//	//m_leftBtn.setLostFocus();
//	//m_rightBtn.setLostFocus();
//	//m_imageSld.setLostFocus();
//	m_isBeChoose = XFalse;
//}
INLINE void XImageList::setPosition(const XVec2& p)
{
	m_position = p;
	m_leftBtn.setPosition(m_position);
	m_rightBtn.setPosition(m_position.x + (m_buttonWidth + m_imageSize.x * m_showImageSum) * m_scale.x,m_position.y);
	m_imageSld.setPosition(m_position.x + m_buttonWidth * m_scale.x,m_position.y);
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
	updateState(false);
}
INLINE XBool XImageList::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XImageList::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XImageList::setColor(const XFColor& c)
{
	m_color = c;
	m_leftBtn.setColor(m_color);
	m_rightBtn.setColor(m_color);
	m_imageSld.setColor(m_color);
	for(auto it = m_imageList.begin();it != m_imageList.end();++ it)
	{
		(*it)->setColor(m_color);
	}
}
INLINE void XImageList::setAlpha(float a) 
{
	m_color.setA(a);
	m_leftBtn.setAlpha(a);
	m_rightBtn.setAlpha(a);
	m_imageSld.setAlpha(a);
	for (auto it = m_imageList.begin(); it != m_imageList.end(); ++it)
	{
		(*it)->setAlpha(a);
	}
}
INLINE void XImageList::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
	m_imageSld.drawUp();
}
INLINE void XImageList::update(float stepTime)
{
	m_leftBtn.update(stepTime);
	m_rightBtn.update(stepTime);
	m_imageSld.update(stepTime);
}