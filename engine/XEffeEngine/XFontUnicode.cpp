//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFontTTF.h"
#include "XFontUnicode.h"
#include "XBasicOpenGL.h"
#include "XResourcePack.h"
#include "XLogBook.h"
#include "XBasicWindow.h"
#include "XObjectManager.h"

#define FONT_UNICODE_CONFIG_FILENAME ("FontUnicodeConfig.dat")

//int testE = 0;
_XFontUnicode::_XFontUnicode()
:m_texture(NULL)
//,m_isAcopy(0)
,m_fontPageText(NULL)
,m_pageSum(0)
,m_pageW(0)
,m_pageH(0)
,m_libFontSum(0)
,m_layout(0,0)
,m_cp(NULL)
{
	m_textPosition = createArrayMem<_XVector2>(m_maxStringLen);
	m_textRect = createArrayMem<_XRect>(m_maxStringLen);
	m_textPageOrder = createArrayMem<int>(m_maxStringLen);

//	testE ++;
//	printf("%d\n",testE);
}
_XFontUnicode::_XFontUnicode(int maxStrLen)
:_XFontBasic(maxStrLen)
,m_texture(NULL)
//,m_isAcopy(0)
,m_fontPageText(NULL)
,m_pageSum(0)
,m_pageW(0)
,m_pageH(0)
,m_libFontSum(0)
,m_layout(0,0)
,m_cp(NULL)
{
	m_textPosition = createArrayMem<_XVector2>(m_maxStringLen);
	m_textRect = createArrayMem<_XRect>(m_maxStringLen);
	m_textPageOrder = createArrayMem<int>(m_maxStringLen);

//	testE ++;
//	printf("%d\n",testE);
}
_XFontUnicode::~_XFontUnicode()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		for(int i = 0;i < m_pageSum;++ i)
		{
			m_texture[i].release();
		}
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);

		XDELETE(m_cp);
	}
//	testE --;
//	printf("%d\n",testE);
	XDELETE_ARRAY(m_textPosition);
	XDELETE_ARRAY(m_textRect);
	XDELETE_ARRAY(m_textPageOrder);

#if WITH_OBJECT_MANAGER	//���������������ע����Щ���
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif

	release();
}
_XBool _XFontUnicode::init(const char *filenameIn,_XVector2 size,_XVector2 layout,int pageSum,_XResourcePosition resoursePosition)
{
	if(m_isInited) 
	{
		AddLogInfoStr("The font have initted!\n");
		return XTrue;
	}
	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}

	if(pageSum <= 0 || pageSum >= UNICODE_FONT_PAGE_SUM || pageSum >= 100) return XFalse;
	m_pageSum = pageSum;
	if((m_texture = createArrayMem<_XTextureData>(m_pageSum)) == NULL)
	{
		XDELETE(m_cp);
		return XFalse;
	}
	if((m_fontPageText = createArrayMem<char>(MAX_UNICODE_TEXT_SUM)) == NULL)
	{
		XDELETE_ARRAY(m_texture);
		XDELETE(m_cp);
		return XFalse;
	}

	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	m_resoursePosition = resoursePosition;

	m_string[0] = '\0';
	//����ͼƬ
	char *filename = NULL;
	if((filename = createArrayMem<char>(MAX_FONT_UNICODE_FILE_NAME_LENGTH)) == NULL)
	{
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);
		XDELETE(m_cp);
		return XFalse;
	}

	strcpy(filename,filenameIn);
	int fileNameLength = strlen(filename);
	for(int i = 0;i < m_pageSum;++ i)
	{
		if( i< 10)
		{
			filename[fileNameLength - 5] = i%10 + '0';
		}else
		if( i< 100)
		{
			filename[fileNameLength - 5] = i%10 + '0';
			filename[fileNameLength - 6] = (i/10)%10 + '0';
		}else
		{
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return XFalse;
		}

	//	if(TextureLoad(&m_texture[i],filename,1,&m_pageW,&m_pageH,m_resoursePosition) == 0)
		if(m_texture[i].load(filename,m_resoursePosition) == 0)
		{
			AddLogInfoNull("pic:%s load error!\n",filename);
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return XFalse;
		}
	}

	m_pageW = m_texture[0].texture.m_w;
	m_pageH = m_texture[0].texture.m_h;

	//��ʼ������
	if(!m_sprite.init(m_pageW,m_pageH,1,POINT_LEFT_TOP)) 
	{
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);
		XDELETE_ARRAY(filename);
		XDELETE(m_cp);
		return XFalse;
	}
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//���������������ע����Щ���
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif
	//��ȡ������Ӧ���ı�
	FILE *fp = NULL;
	filename[fileNameLength - 5] = '0';
	filename[fileNameLength - 6] = '0';
	filename[fileNameLength - 3] = 't';
	filename[fileNameLength - 2] = 'x';
	filename[fileNameLength - 1] = 't';
	if(m_resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		if((fp = fopen(filename,"r")) == NULL)
		{//�ļ���ʧ��
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return XFalse;
		}
		unsigned char fileReadChar;
		m_libFontSum = 0;
		for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
		{
			if(fread(&fileReadChar,1,1,fp) == 0) break;
			if(feof(fp)) break;	//�ļ���ȡʧ�����˳�
			if(fileReadChar == '\n') continue;
			if(fileReadChar <128)
			{//ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = ' ';
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				m_libFontSum ++;
			}else
			{//��ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				if(fread(&fileReadChar,1,1,fp) == 0) return 0;
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}
		}
		if(m_libFontSum == 0)
		{//�ַ�����û������Ԫ��
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			fclose(fp);
			return XFalse;
		}
		fclose(fp);
	}else
	{
/*		static int checkedresource = 0;
		if(checkedresource == 0)
		{
			checkedresource = 1;
			if(ResourceTemp.checkCheckData() == 0)
			{//���صĴ������ﶨ����Ϸ
				DebugShow("Resource Data Error!\n");
				while(1)
				{
					Sleep(100);
				}
			}
		}*/
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(filename);
		unsigned char *p = _XResourcePack::GetInstance().getFileData(filename);
		if(p == NULL)
		{
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return XFalse;
		}

		unsigned char fileReadChar;
		m_libFontSum = 0;
		int nowPosition = 0;
		for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
		{
			if(nowPosition == lengthTemp) break;
			fileReadChar = p[nowPosition];
			++ nowPosition;
			if(fileReadChar == '\n') continue;
			if(fileReadChar <128)
			{//ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = ' ';
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				m_libFontSum ++;
			}else
			{//��ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				if(nowPosition == lengthTemp) 
				{
					XDELETE_ARRAY(m_texture);
					XDELETE_ARRAY(m_fontPageText);
					XDELETE_ARRAY(filename);
					XDELETE(m_cp);
					XDELETE_ARRAY(p);
					return XFalse;
				}
				fileReadChar = p[nowPosition];
				++ nowPosition;
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}
		}
		if(m_libFontSum == 0)
		{//�ַ�����û������Ԫ��
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		XDELETE_ARRAY(p);
	}
	m_position.set(0,0);	
	m_setPosition.set(0,0);	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	m_angle = 0;			
	m_distance = 0;;		
	m_size = size;			
	m_layout = layout;
	m_showSize.x = 1.0f;
	m_showSize.y = 1.0f;
	m_sprite.setSize(m_showSize);
	m_isPassword = XFalse;

	XDELETE_ARRAY(filename);
	m_isInited = XTrue;		

	m_isCliped = XFalse;
	return XTrue;
}
_XBool _XFontUnicode::initEx(const char *filenameIn,_XResourcePosition resoursePosition)
{
	if(m_isInited) 
	{
		AddLogInfoStr("The font have initted!\n");
		return XTrue;
	}
	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}

	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	m_resoursePosition = resoursePosition;
	//�����ȡ��ظ�ʽ����
	char *filename = NULL;
	if((filename = createArrayMem<char>(MAX_FONT_UNICODE_FILE_NAME_LENGTH)) == NULL)
	{
		XDELETE(m_cp);
		return XFalse;
	}
	sprintf(filename,"%s/%s",filenameIn,FONT_UNICODE_CONFIG_FILENAME);
	char txtureFilename[MAX_FILE_NAME_LENGTH];

	FILE *fp = NULL;
	_XVector2 size(0,0);
	_XVector2 layout(0,0);
	int pageSum = 0;
	if(m_resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		if((fp = fopen(filename,"r")) == NULL)
		{//�ļ�����ʧ��
			XDELETE(m_cp);
			XDELETE_ARRAY(filename);
			return XFalse;
		}
		if(fscanf(fp,"%s\n",txtureFilename) != 1)
		{
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%f,%f,%f,%f,%d,",&size.x,&size.y,&layout.x,&layout.y,&pageSum) != 5)
		{
			fclose(fp);
			return XFalse;
		}
		fclose(fp);
	}else
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(filename);
		if(p == NULL)
		{
			XDELETE(m_cp);
			XDELETE_ARRAY(filename);
			return XFalse;
		}
		if(sscanf((char *)p,"%s\n",txtureFilename) != 1)
		{
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)p + strlen(txtureFilename),"%f,%f,%f,%f,%d,",&size.x,&size.y,&layout.x,&layout.y,&pageSum) != 5)
		{
			XDELETE_ARRAY(p);
			return XFalse;
		}
		XDELETE_ARRAY(p);
	}

	if(pageSum <= 0 || pageSum >= UNICODE_FONT_PAGE_SUM || pageSum >= 100) 
	{
		XDELETE(m_cp);
		XDELETE_ARRAY(filename);
		return XFalse;
	}
	m_pageSum = pageSum;
	sprintf(filename,"%s/%s",filenameIn,txtureFilename);
	int fileNameLength = strlen(filename);

	if((m_texture = createArrayMem<_XTextureData>(m_pageSum)) == NULL)
	{
		XDELETE(m_cp);
		XDELETE(filename);
		return XFalse;
	}
	if((m_fontPageText = createArrayMem<char>(MAX_UNICODE_TEXT_SUM)) == NULL)
	{
		XDELETE(m_cp);
		XDELETE(filename);
		XDELETE_ARRAY(m_texture);
		return XFalse;
	}
	m_string[0] = '\0';
	//����ͼƬ
	for(int i = 0;i < m_pageSum;++ i)
	{
		if( i< 10)
		{
			filename[fileNameLength - 5] = i%10 + '0';
		}else
		if( i< 100)
		{
			filename[fileNameLength - 5] = i%10 + '0';
			filename[fileNameLength - 6] = (i/10)%10 + '0';
		}else
		{
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return XFalse;
		}

	//	if(TextureLoad(&m_texture[i],filename,1,&m_pageW,&m_pageH,m_resoursePosition) == 0)
		if(m_texture[i].load(filename,m_resoursePosition) == 0)
		{
			AddLogInfoNull("pic:%s load error!\n",filename);
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return XFalse;
		}
	}

	m_pageW = m_texture[0].texture.m_w;
	m_pageH = m_texture[0].texture.m_h;

	//��ʼ������
	if(!m_sprite.init(m_pageW,m_pageH,1,POINT_LEFT_TOP)) 
	{
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);
		XDELETE_ARRAY(filename);
		XDELETE(m_cp);
		return XFalse;
	}
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//���������������ע����Щ���
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif
	//��ȡ������Ӧ���ı�
	filename[fileNameLength - 5] = '0';
	filename[fileNameLength - 6] = '0';
	filename[fileNameLength - 3] = 't';
	filename[fileNameLength - 2] = 'x';
	filename[fileNameLength - 1] = 't';
	if(m_resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		if((fp = fopen(filename,"r")) == NULL)
		{//�ļ���ʧ��
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return XFalse;
		}
		unsigned char fileReadChar;
		m_libFontSum = 0;
		for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
		{
			if(fread(&fileReadChar,1,1,fp) == 0) break;
			if(feof(fp)) break;	//�ļ���ȡʧ�����˳�
			if(fileReadChar == '\n') continue;
			if(fileReadChar <128)
			{//ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = ' ';
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				m_libFontSum ++;
			}else
			{//��ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				if(fread(&fileReadChar,1,1,fp) == 0) return 0;
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}
		}
		if(m_libFontSum == 0)
		{//�ַ�����û������Ԫ��
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			fclose(fp);
			return XFalse;
		}
		fclose(fp);
	}else
	{
//		static int checkedresource = 0;
//		if(checkedresource == 0)
//		{
//			checkedresource = 1;
//			if(ResourceTemp.checkCheckData() == 0)
//			{//���صĴ������ﶨ����Ϸ
//				DebugShow("Resource Data Error!\n");
//				while(1)
//				{
//					Sleep(100);
//				}
//			}
//		}

		int lengthTemp = _XResourcePack::GetInstance().getFileLength(filename);
		unsigned char *p = _XResourcePack::GetInstance().getFileData(filename);
		if(p == NULL)
		{
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			return XFalse;
		}

		unsigned char fileReadChar;
		m_libFontSum = 0;
		int nowPosition = 0;
		for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
		{
			if(nowPosition == lengthTemp) break;
			fileReadChar = p[nowPosition];
			++ nowPosition;
			if(fileReadChar == '\n') continue;
			if(fileReadChar <128)
			{//ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = ' ';
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				m_libFontSum ++;
			}else
			{//��ASCII
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
				if(nowPosition == lengthTemp) 
				{
					XDELETE_ARRAY(m_texture);
					XDELETE_ARRAY(m_fontPageText);
					XDELETE_ARRAY(filename);
					XDELETE(m_cp);
					XDELETE_ARRAY(p);
					return XFalse;
				}
				fileReadChar = p[nowPosition];
				++ nowPosition;
				m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
				m_libFontSum ++;
			}
		}
		if(m_libFontSum == 0)
		{//�ַ�����û������Ԫ��
			XDELETE_ARRAY(m_texture);
			XDELETE_ARRAY(m_fontPageText);
			XDELETE_ARRAY(filename);
			XDELETE(m_cp);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		XDELETE_ARRAY(p);
	}
	m_position.set(0,0);	
	m_setPosition.set(0,0);	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	m_angle = 0;			
	m_distance = 0;;		
	m_size = size;			
	m_layout = layout;
	m_showSize.x = 1.0f;
	m_showSize.y = 1.0f;
	m_sprite.setSize(m_showSize);
	m_isPassword = XFalse;

	XDELETE_ARRAY(filename);
	m_isInited = XTrue;		

	m_isCliped = XFalse;
	return XTrue;
}
_XBool _XFontUnicode::initFromTTF(const char *filenameIn,int fontSize,_XResourcePosition resoursePosition,bool withOutLine)
{
	if(m_isInited) 
	{
		AddLogInfoStr("The font have initted!\n");
		return XTrue;
	}
	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}

	m_resoursePosition = resoursePosition;
	//�����ȡ��ظ�ʽ����
	char *filename = NULL;
	if((filename = createArrayMem<char>(MAX_FONT_UNICODE_FILE_NAME_LENGTH)) == NULL)
	{
		XDELETE(m_cp);
		return XFalse;
	}
	strcpy(filename,filenameIn);
	int fileNameLength = strlen(filename);

	int fontOrder = _XFontTTF::GetInstance().loadTTFFile(filename,fontSize,m_resoursePosition);
	if(fontOrder < 0) return XFalse;
	//_XFontTTF::GetInstance().setOutline(fontOrder,1);	//Ч���ǳ�������
	unsigned int tex[128];
	int pageSum = 0;
	_XVector2 layout;
	if(_XFontTTF::GetInstance().getTextureFontUnicode(fontOrder,tex,pageSum,layout,withOutLine) == 0) return 0;
	//�ͷ���Դ
	_XFontTTF::GetInstance().releaseTTFFile(fontOrder);
	_XVector2 size(fontSize,fontSize);
	int w = getMinWellSize2n(layout.x * fontSize);
	int h = getMinWellSize2n(layout.y * fontSize);

	if(pageSum <= 0 || pageSum >= UNICODE_FONT_PAGE_SUM || pageSum >= 100) 
	{
		XDELETE(m_cp);
		XDELETE_ARRAY(filename);
		return XFalse;
	}
	m_pageSum = pageSum;

	if((m_texture = createArrayMem<_XTextureData>(m_pageSum)) == NULL)
	{
		XDELETE(m_cp);
		XDELETE(filename);
		return XFalse;
	}
	if((m_fontPageText = createArrayMem<char>(MAX_UNICODE_TEXT_SUM)) == NULL)
	{
		XDELETE(m_cp);
		XDELETE(filename);
		XDELETE_ARRAY(m_texture);
		return XFalse;
	}
	m_string[0] = '\0';

	filename[fileNameLength - 3] = filenameIn[fileNameLength - 3];
	filename[fileNameLength - 2] = filenameIn[fileNameLength - 2];
	filename[fileNameLength - 1] = filenameIn[fileNameLength - 1];
	//����ͼƬ
	for(int i = 0;i < m_pageSum;++ i)
	{
		m_texture[i].createWithTexture(w,h,COLOR_RGBA,tex[i]);
	}

	m_pageW = m_texture[0].texture.m_w;
	m_pageH = m_texture[0].texture.m_h;

	//��ʼ������
	if(!m_sprite.init(m_pageW,m_pageH,1,POINT_LEFT_TOP)) 
	{
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);
		XDELETE_ARRAY(filename);
		XDELETE(m_cp);
		return XFalse;
	}
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//���������������ע����Щ���
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif
	//������Ҫת�ַ�
	char tempStr[] = TEXT_FONT_UNICODE_MODE;
	int length = strlen(TEXT_FONT_UNICODE_MODE);
	int j = 0;
	m_libFontSum = 0;
	for(int i = 0;i < length;)
	{
		if(tempStr[i] < 0)
		{
			m_fontPageText[j] = tempStr[i];
			m_fontPageText[j + 1] = tempStr[i + 1];
			j += 2;
			i += 2;
			++ m_libFontSum;
		}else
		{
			m_fontPageText[j + 1] = ' ';
			m_fontPageText[j] = tempStr[i];
			j += 2;
			i ++;
			++ m_libFontSum;
		}
	}
	m_fontPageText[j] = '\0';
	m_fontPageText[j + 1] = '\0';
	m_position.set(0,0);	
	m_setPosition.set(0,0);	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	m_angle = 0;			
	m_distance = 0;;		
	m_size = size;			
	m_layout = layout;
	m_showSize.x = 1.0f;
	m_showSize.y = 1.0f;
	m_sprite.setSize(m_showSize);
	m_isPassword = XFalse;

	XDELETE_ARRAY(filename);
	m_isInited = XTrue;		

	m_isCliped = XFalse;
	return XTrue;
}
void _XFontUnicode::draw()
{
//	if((m_isCliped != 0 && m_clipRect.top >= m_size.y)
//		|| (m_isCliped != 0 && m_clipRect.bottom <= 0))
	if(m_isCliped && m_clipRect.bottom <= 0)
	{//�������ʲôҲ���û�
		return;
	}
	if(!m_isVisiable) return;
	if(m_needUpdateData)
	{
		m_needUpdateData = XFalse;
		updateData();
	}
	for(int i = 0;i < m_needShowTextSum;++ i)
	{
		m_sprite.setClipRect(m_textRect[i]);
		m_sprite.setPosition(m_textPosition[i]);

		m_sprite.draw(&(m_texture[m_textPageOrder[i]]));
	}
}
void _XFontUnicode::updateData()
{
	m_needShowTextSum = 0;
	m_maxPixelWidth = 0;
	m_maxPixelHeight = 0;
	int now_x = 0;
	int now_y = 0;
	char tempChar[2];
	int charPoint = 0;	//�ַ�����λ�õ�ָ��
	int pageNumble = -1;
	int wordsSumD = 0;		//˫�ֽ�
	int wordsSumS = 0;		//���ֽ�
	_XBool nowIsD = XFalse;		//��ǰ��ʾ���������ʱ���ֽڻ��Ƕ��ֽ�
	//Ϊ�˶��뷽ʽ������������Ҫ�����ʵ��Ĵ���
	//1��ͳ��������
	int allLineSum = 1;
	int nowLineWidthD = 0;	//��ǰ���е������ַ�����
	int nowLineWidthS = 0;	//��ǰ���е�ASCII�ַ�����
	for(int i =0;i < m_maxStringLen;++ i)
	{
		if(m_string[i] == '\0') break;
		if((unsigned char)m_string[i] < 128) 
		{//ASCII
			if(m_string[i] == '\n') allLineSum ++;	//����������
			else 
			{
				if(allLineSum == 1) nowLineWidthS ++; //ͳ�Ƶ�һ�еĿ���
			}
		}else
		{//��ASCII
			if(allLineSum == 1) nowLineWidthD ++; //ͳ�Ƶ�һ�еĿ���
			++ i;
		}
	}

	m_lineSum = 0;	//����ͳ������
	float textPixelDX = (m_size.x + m_distance) * m_showSize.x;			//˫�ֽ��ַ��Ŀ���
	float textPixelSX = (m_size.x * 0.5f + m_distance) * m_showSize.x;	//���ֽ��ַ��Ŀ���
	float textPixelY = (m_size.y + m_distanceY) * m_showSize.y;			//�ַ��Ŀ���
	for(int i =0;i < m_maxStringLen;++ i)
	{
		if(m_string[i] == '\0') break;	//�ַ�������
		if((unsigned char)m_string[i] < 128)
		{//ASCII
			if(m_isPassword == 0)
			{//������ģʽ
				tempChar[0] = m_string[i];
				tempChar[1] = ' ';
			}else
			{//����ģʽ
				tempChar[0] = '*';
				tempChar[1] = ' ';
			}
			//++ wordsSumS;
			nowIsD = XFalse;
			if(m_string[i] == '\n')
			{//����ǻ��з���ʼ����
				m_lineSum ++;
				wordsSumS = 0;
				wordsSumD = 0;
				//��������ʱͳ�Ƶ�ǰ�е��ַ�������
				nowLineWidthD = 0;	//��ǰ���е������ַ�����
				nowLineWidthS = 0;	//��ǰ���е�ASCII�ַ�����
				for(int j = i + 1;j < m_maxStringLen;++ j)
				{
					if((unsigned char)m_string[j] < 128) 
					{//ASCII
						if(m_string[j] == '\n' || m_string[j] == '\0') break;	//��ǰ�н���
						else nowLineWidthS ++; //ͳ�Ƶ�һ�еĿ���
					}else
					{//��ASCII
						nowLineWidthD ++; //ͳ�Ƶ�һ�еĿ���
						++ j;
					}
				}
				continue;	//���з�����Ҫ���κδ�����ֱ�Ӽ���
			}
		}else
		{//��ASCII
			if(m_isPassword == 0)
			{//������ģʽ
				tempChar[0] = m_string[i];
				tempChar[1] = m_string[i + 1];
				++ i;
				//++ wordsSumD;
				nowIsD = XTrue;
			}else
			{//����ģʽ
				tempChar[0] = '*';
				tempChar[1] = ' ';
				++ i;
				//++ wordsSumS;
				nowIsD = XFalse;
			}
		}
		//�����ַ�
		charPoint = 0;
		for(charPoint = 0;charPoint < m_libFontSum;++ charPoint)
		{
			if(m_fontPageText[UNICODE_BYTES_WIDTH * charPoint] == tempChar[0]
				&& m_fontPageText[UNICODE_BYTES_WIDTH * charPoint + 1] == tempChar[1])
			{
				break;
			}
		}
		if(charPoint == m_libFontSum)
		{//����Ҳ�������ַ�����Ϊ�ո�������
			//charPoint = 0;
			if(nowIsD) ++ wordsSumD;
			else ++wordsSumS;
			continue;
		}
		//�����ַ���ҳ�棬�������������
		pageNumble = charPoint / (int)(m_layout.x * m_layout.y);
		now_x = (charPoint % (int)(m_layout.x)) * m_size.x;

		if(m_layout.y == 0) now_y = 0;
		else now_y = ((charPoint / (int)(m_layout.x)) % (int)(m_layout.y)) * m_size.y;

		if(!m_isCliped)
		{
			m_textRect[m_needShowTextSum].set(now_x,now_y,now_x + m_size.x,now_y + m_size.y);
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_LEFT)
			{//����룬ɶҲ������
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_UP)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * m_lineSum);
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_DOWN)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
				}
			}else
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//����룬ɶҲ������
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_UP)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * m_lineSum);
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_DOWN)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
				}
			}else
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_RIGHT)
			{//����룬ɶҲ������
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_UP)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						+ m_angleCos * textPixelY * m_lineSum);
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_DOWN)
				{
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
				}
			}

		//	m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
		//		- m_angleSin * textPixelY * m_lineSum,
		//		m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
		//		+ m_angleCos * textPixelY * m_lineSum);
			m_textPageOrder[m_needShowTextSum] = pageNumble;
			m_needShowTextSum ++;

			if(textPixelDX * (wordsSumD + 1) + textPixelSX * wordsSumS > m_maxPixelWidth)
			{
				m_maxPixelWidth = textPixelDX * (wordsSumD + 1) + textPixelSX * wordsSumS;
			}
			if(textPixelY * (m_lineSum + 1) > m_maxPixelHeight)
			{
				m_maxPixelHeight = textPixelY * (m_lineSum + 1);
			}
		}else
		{
			int left,right,top,bottom;
			//�����ж�y�����Ƿ񳬳���Χ��Ŀǰ��δʵ��
		//	if(m_clipRect.top <= 0)
		//	{
		//		top = 0;
		//	}else
		//	{
		//		top = m_clipRect.top;
		//	}
			float tempY = 0.0f;
			if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_UP)
			{//����룬ɶҲ������
				tempY = (m_size.y + m_distanceY) * m_lineSum;
			}else
			if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_MIDDLE)
			{//���У�ÿ���������ư������
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum * 0.5f);
			}else
			if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_DOWN)
			{//�Ҷ��룬ÿ������������������
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum);
			}
			//float tempY = (m_size.y + m_distanceY) * m_lineSum;	//������ߵľ���
			if(m_clipRect.bottom < tempY + m_distance)
			{//����֮�²���ʾ
				if(nowIsD) ++ wordsSumD;
				else ++wordsSumS;
				continue;
			}
			//if(m_clipRect.top >= tempY + (m_size.y + m_distanceY))
			if(m_clipRect.top >= tempY + m_size.y)
			{//����֮�ϲ���ʾ
				if(nowIsD) ++ wordsSumD;
				else ++wordsSumS;
				continue;
			}
			//�����ж�x�����Ƿ񳬳���Χ�����������Χ����ʾ
			float tempX = 0.0f;
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_LEFT)
			{//����룬ɶҲ������
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS;
			}else
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//���У�ÿ���������ư������
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS - (m_size.x + m_distance) * nowLineWidthD * 0.5f - (m_size.x * 0.5f + m_distance) * nowLineWidthS * 0.5f;
			}else
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_RIGHT)
			{//�Ҷ��룬ÿ������������������
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS - (m_size.x + m_distance) * nowLineWidthD - (m_size.x * 0.5f + m_distance) * nowLineWidthS;
			}
			//float tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS;	//������ߵľ���
			if(tempX + m_distance >= m_clipRect.right) 
			{//�ұ�֮�Ҳ���ʾ
				if(nowIsD) ++ wordsSumD;
				else ++wordsSumS;
				continue;	//�ַ�Χ���治��Ҫ������
			}
		//	if((nowIsD == 0 && temp + (m_size.x * 0.5 + m_distance) <= m_clipRect.left) 
			//if((nowIsD == 0 && temp + (m_size.x + m_distance) <= m_clipRect.left) //������ʵ��Χ��ֱ�Ӳ�����
			//	|| (nowIsD == 1 && temp + (m_size.x + m_distance) <= m_clipRect.left))	//??
			//if(tempX + (m_size.x + m_distance) <= m_clipRect.left)
			if(tempX + m_size.x <= m_clipRect.left)
			{//���֮����ʾ
				if(nowIsD) ++ wordsSumD;
				else ++wordsSumS;
				continue;
			}
			if(tempY >= m_clipRect.top && tempY + (m_size.y + m_distanceY) <= m_clipRect.bottom)
			{//��Ҫȫ���軭
				top = 0;
				bottom = 0;
			}else
			{
				if(tempY < m_clipRect.top)
				{
					top = m_clipRect.top - tempY;
				}else
				{
					top = 0;
				}
				if(tempY + (m_size.y + m_distanceY) > m_clipRect.bottom)
				{
					bottom = tempY + (m_size.y + m_distanceY) - m_clipRect.bottom;
				}else
				{
					bottom = 0;
				}
			}
			//��Ӣ���ַ��ü���ʱ���������⣬����������ַ����ﲻ��������
			if(tempX >= m_clipRect.left && tempX + (m_size.x + m_distance) <= m_clipRect.right)
			{//��Ҫȫ���軭
				left = 0;
				right = 0;
			}else
			{
				if(tempX < m_clipRect.left)
				{
					left = m_clipRect.left - tempX;
				}else
				{
					left = 0;
				}
				if(tempX + (m_size.x + m_distance) > m_clipRect.right)
				{
					right = tempX + (m_size.x + m_distance) - m_clipRect.right;
				}else
				{
					right = 0;
				}
			}
			m_textRect[m_needShowTextSum].set(now_x + left,now_y + top,now_x + m_size.x - right,now_y + m_size.y - bottom);
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_LEFT)
			{//����룬ɶҲ������
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_UP)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_DOWN)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
				}
			}else
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//����룬ɶҲ������
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_UP)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_DOWN)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD * 0.5f - textPixelSX * nowLineWidthS * 0.5f));
				}
			}else
			if(m_alignmentModeX == FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//����룬ɶҲ������
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_UP)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS));
				}else
				if(m_alignmentModeY == FONT_ALIGNMENT_MODE_Y_DOWN)
				{//����룬ɶҲ������
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * nowLineWidthD - textPixelSX * nowLineWidthS));
				}
			}
		//	m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
		//		- m_angleSin * textPixelY * m_lineSum
		//		+ m_angleCos * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
		//		m_position.y + m_angleCos * top * m_showSize.x 
		//		+ m_angleCos * textPixelY * m_lineSum
		//		+ m_angleSin * (left * m_showSize.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
			m_textPageOrder[m_needShowTextSum] = pageNumble;
			++m_needShowTextSum;
		}
		if(nowIsD) ++ wordsSumD;
		else ++wordsSumS;
	}
}
_XFontUnicode& _XFontUnicode::operator = (const _XFontUnicode& temp)
{
	if(this == &temp) return *this;		//��ֹ��������
	if(temp.m_isInited == 0) return *this;	//���Ŀ��û�г�ʼ����ֱ���˳�
	
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		for(int i = 0;i < m_pageSum;++ i)
		{
			m_texture[i].release();
		}
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);

		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	//release();

	_XFontBasic::operator =(temp);
#if WITH_OBJECT_MANAGER	//���������������ע����Щ���
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif

	//����ĸ����������Լ�����Դ����������Լ�����Դ����Ҫ�ͷŵ�
//	m_isAcopy = 1;

	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		
		
	m_pageSum = temp.m_pageSum;

	for(int i = 0;i < m_maxStringLen;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
		m_textPageOrder[i] =temp.m_textPageOrder[i];
	}

	m_layout = temp.m_layout;		
	return *this;
}
_XBool _XFontUnicode::setACopy(const _XFontUnicode & temp)
{
	if(this == &temp) return XFalse;		//��ֹ��������
	if(temp.m_isInited == 0) return XFalse;	//���Ŀ��û�г�ʼ����ֱ���˳�

	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		for(int i = 0;i < m_pageSum;++ i)
		{
			m_texture[i].release();
		}
		XDELETE_ARRAY(m_texture);
		XDELETE_ARRAY(m_fontPageText);

		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	//release();
	if(_XFontBasic::setACopy(temp) == 0) return 0;
#if WITH_OBJECT_MANAGER	//���������������ע����Щ���
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif

	//����ĸ����������Լ�����Դ����������Լ�����Դ����Ҫ�ͷŵ�
//	m_isAcopy = 1;

	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		
		
	m_pageSum = temp.m_pageSum;

	for(int i = 0;i < m_maxStringLen;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
		m_textPageOrder[i] =temp.m_textPageOrder[i];
	}

	m_layout = temp.m_layout;		
	return XTrue;
}
_XFontUnicode::_XFontUnicode(const _XFontUnicode & temp)
:_XFontBasic(temp)
{
	//if(this == &temp) return;		//��ֹ��������
#if WITH_OBJECT_MANAGER	//���������������ע����Щ���
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTUNICODE);
#endif
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;

	m_textPosition = createArrayMem<_XVector2>(m_maxStringLen);
	m_textRect = createArrayMem<_XRect>(m_maxStringLen);
	m_textPageOrder = createArrayMem<int>(m_maxStringLen);
	
//	m_isAcopy = 1;

	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		
	
	m_pageSum = temp.m_pageSum;

	for(int i = 0;i < m_maxStringLen;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
		m_textPageOrder[i] =temp.m_textPageOrder[i];
	}

	m_layout = temp.m_layout;		
}
void _XFontUnicode::setMaxStrLen(int maxStrLen)
{
	XDELETE_ARRAY(m_string);
	XDELETE_ARRAY(m_textPosition);
	XDELETE_ARRAY(m_textRect);
	XDELETE_ARRAY(m_textPageOrder);

	if(maxStrLen < 2) m_maxStringLen = 2;
	else m_maxStringLen = maxStrLen;
	m_string = createArrayMem<char>(m_maxStringLen);
	m_string[0] = '\0';
	m_textPosition = createArrayMem<_XVector2>(m_maxStringLen);
	m_textRect = createArrayMem<_XRect>(m_maxStringLen);
	m_textPageOrder = createArrayMem<int>(m_maxStringLen);

	m_needUpdateData = XTrue;
}