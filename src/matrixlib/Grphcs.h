 
///////////////////////////////////////////////////////////////////////////////////////// 
// 
//								Grphcs.h     
// 
///////////////////////////////////////////////////////////////////////////////////////// 
// 

#ifndef __grphcs_h_ 
#ifndef __GRPHCS_H__ 
 
#define __grphcs_h_ 
#define __GRPHCS_H__ 
 
#ifdef __cplusplus 
extern "C" { 
#endif 
 
//////////////////////////////////////////////////////////////////////////////// 
 
//*******************		some parameters       ********************* 
 
//////////////////////////////////////////////////////////////////////////////// 
 
////////////////////////////math parameter///////////////////// 
 
#define		PIE		3.14159265358979323846 
 
////////////////////////////graphcs design///////////////////// 
 
/* edition number */ 
#define		GRAPHICS_DESIGN_VERSION_1_0			1 
 
//��ҵ��ɫģ�� 
 
 
#define		G3D_PIXEL_COLOR_RGB						0X00000000 
#define		G3D_PIXEL_COLOR_RGBA					0X00000001 
#define		G3D_PIXEL_COLOR_INDEX					0X00000002 
 
//ֱ������������������λ�ù�ϵ,ע������ֻ�������ҹ�ϵ,���������¹�ϵ 
//������ɨ����� 
#define		G3D_TRIANGLE_ON_LINE_RIGHT				0X00000100 
#define		G3D_TRIANGLE_ON_LINE_LEFT				0X00000101 
 
//����˷����ڲ����㽻����,�����ڴ˶�����˻��ҳ˲��� 
#define		G3D_MATRIX_MULTIPLIER_PRECONCAT			0X00000200 
#define		G3D_MATRIX_MULTIPLIER_POSTCONCAT		0X00000201 
 
 
//������ɫģ�� 
#define		G3D_RENDER_VERTICES						0X00000300 
#define		G3D_RENDER_WIRE							0X00000301 
#define		G3D_RENDER_WIRE_LIGHTED					0X00000302 
#define		G3D_RENDER_FLAT							0X00000303 
#define		G3D_RENDER_GOURAUD_SMOOTH				0X00000304 
 
 
//���ʺ͹�Դ�Ļ�������: 
#define		G3D_AMBIENT								0X00000400 
#define		G3D_DIFFUSE								0X00000401 
#define		G3D_SPECULAR							0X00000402 
 
//���λ�� 
#define		G3D_POSITION							0X00000403 
 
//�۹����� 
#define		G3D_SPOT_DIRECTION						0X00000404 
#define		G3D_SPOT_CUTOFF							0X00000405 
#define		G3D_SPOT_EXPONENT						0X00000406 
 
//���˥������ 
#define		G3D_CONSTANT_ATTENUATION				0X00000407 
#define		G3D_LINEAR_ATTENUATION					0X00000408 
#define		G3D_QUADRATIC_ATTENUATION				0X00000409 
#define		G3D_LIGHT_ON_OFF						0X0000040A 
 
//����ר�� 
#define     G3D_AMBIENT_AND_DIFFUSE					0X00000501 
#define     G3D_EMISSION							0X00000502 
#define     G3D_SHININESS							0X00000503 
 
 
//����ģ�� 
//ȫ�ֻ����� 
#define		G3D_LIGHT_MODEL_AMBIENT					0X00000600 
//���ӵ�������Զ�ӵ� 
#define     G3D_LIGHT_MODEL_LOCAL_VIEWER			0X00000601 
//˫����� 
#define		G3D_LIGHT_MODEL_TWO_SIDE				0X00000602 
 
//ָ������ǰ�桢���棬����˫�� 
#define		G3D_FRONT								0X00000700	 
#define		G3D_BACK								0X00000701 
#define		G3D_FRONT_AND_BACK						0X00000702 
 
//��Ȳ������� 
#define		G3D_LESS								0X00000800 
#define		G3D_LEQUAL								0X00000801 
#define		G3D_EQUAL								0X00000802 
#define		G3D_GEQUAL								0X00000803 
#define		G3D_GREATER								0X00000804 
#define		G3D_NOTEQUAL							0X00000805 
#define		G3D_NEVER								0X00000806 
#define		G3D_ALWAYS								0X00000807 
 
 
 
 
 
//////////////////////////////////////////////////////////////////////////////// 
 
//*******************		�ṹ����       ********************* 
 
//////////////////////////////////////////////////////////////////////////////// 
 
//������ɫ:RGB 
typedef struct tagFLOATCOLORRGB 
{ 
	float red; 
	float green; 
	float blue; 
}FLOATCOLORRGB; 
 
//������ɫ:RGBA 
typedef struct tagFLOATCOLORRGBA 
{ 
	float red; 
	float green; 
	float blue; 
	float alpha; 
}FLOATCOLORRGBA; 
 
 
//��ά�������� 
typedef struct tagVECTOR3D 
{ 
	float x; 
	float y; 
	float z; 
}VECTOR3D; 
 
//��ά�������� 
typedef struct tagVERTEX3D 
{ 
	float x; 
	float y; 
	float z; 
}VERTEX3D; 
 
 
//�������(Homogeneous Coordinate) 
typedef struct tagHOMOCOORD 
{ 
	float x; 
	float y; 
	float z; 
	float w; 
}HOMOCOORD; 
 
 
//�򵥵�͸��ͶӰ,��������豸����任,�����۲���� 
 
//��ȡ����(���ڹ۲�����) 
typedef struct tagVIEWFINDER 
{ 
	float     fDistView;		//	�Ӿ�,�ӵ㵽ͶӰ��Ļ֮��ľ��� 
	float	  xScale;			//	�����ӿڱ任ʱ,������ű��� 
	float     yScale;			//	�����ӿڱ任ʱ,�߶����ű��� 
	//POINT	  ptCenter;			//	����,��������ϵ����ԭ��ͶӰ����Ļ�ϵ�λ�� 
}VIEWFINDER; 
 
 
//////////////////////////////////////////////////////////////////////////////// 
 
//*******************		��������       ********************* 
 
//////////////////////////////////////////////////////////////////////////////// 
 
//�ضϺ��� 
#define BOUND(x,a,b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x))) 
 
//����ֵ���� 
#define ABS(x)	(((x) < 0) ? -(x) : (((x) > 0) ? (x) : 0)) 
 
#define MAX(a,b)	(((a) > (b)) ? (a) : (b)) 
#define MIN(a,b)	(((a) < (b)) ? (a) : (b))  
 
#ifdef __cplusplus 
} 
#endif 
 
#endif /* __GRPHCS_H__ */ 
#endif /* __grphcs_h_ */

