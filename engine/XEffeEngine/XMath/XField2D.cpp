#include "XStdHead.h"
#include "XField2D.h"
#include "XBasicOpenGL.h"
namespace XE{
void XField2D::Draw(const XVec2& size, float scale)	//描绘场
{
	XVec2 posB;
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLineWidth(1);
	glBegin(GL_LINES);
	int index = 0;
	XVec2 tmpSize = size / XVec2(m_w, m_h);
	for (int h = 0; h < m_h; ++h)
	{
		for (int w = 0; w < m_w; ++w, ++index)
		{
			posB = tmpSize * XVec2(w, h);
			glColor3fv(XFColor::blue);
			glVertex2fv(posB);
			glColor3fv(XFColor::red);
			posB += m_pVectors[index] * scale;
			glVertex2fv(posB);
		}
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void XField2D::SetField2DType(XField2DType type)
{
	if (!m_isInited || m_type == type) return;
	m_type = type;
	float len;
	//下面初始化场的相关数据
	switch (m_type)
	{
	case FIELD2D_NULL:break;
	case FIELD2D_DOWN:
		for (int i = 0; i < m_w * m_h; ++i)
		{
			m_pVectors[i].set(0.0f, -2.5f);
		}
		break;
	case FIELD2D_WHIRL:	//尚未完成
		{
			int index = 0;
			XVec2 tmp(m_w >> 1, m_h >> 1);
			XVec2 tmpx;
			for (int h = 0; h < m_h; ++h)
			{
				for (int w = 0; w < m_w; ++w, ++index)
				{
					len = 1.0f + tmp.getLength(w, h);
					len = 50.0f * pow(1.0 / len, 0.25);
					//	len = 10.0f;
					tmpx = tmp - XVec2(w, h);
					tmpx.set(tmpx.y, -tmpx.x);
					tmpx.normalize();
					m_pVectors[index] = tmpx * len;
				}
			}
		}
		break;
	case FIELD2D_RAND:
		for (int i = 0; i < m_w * m_h; ++i)
		{
			m_pVectors[i].set(XRand::randomf(-20, 20), XRand::randomf(-20, 20));
		}
		break;
	}
}
bool XField2D::Save2File(const char* filename)
{
	if (m_pVectors == nullptr) return false;
	FILE* fp = nullptr;
	if ((fp = fopen(filename, "wb")) == nullptr) return false;
	if (fwrite(m_pVectors, 1, m_sum * sizeof(XVec2), fp) <= 0) return false;
	return true;
}
bool XField2D::LoadFromFile(const char* filename)
{
	if (m_pVectors == nullptr) return false;
	FILE* fp = nullptr;
	if ((fp = fopen(filename, "rb")) == nullptr) return false;
	if (fread(m_pVectors, 1, m_sum * sizeof(XVec2), fp) <= 0) return false;
	return true;
}
#if !WITH_INLINE_FILE
#include "XField2D.inl"
#endif
}