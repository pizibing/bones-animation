//simple vector4D class which contain four private value and its get and set method
class FCVector4D
{
public:
	FCVector4D(float x, float y, float z, float w);
	FCVector4D();
	~FCVector4D(void);
	float GetX(void);
	float GetY(void);
	float GetZ(void);
	float GetW(void);
	void Set(float x, float y, float z, float w);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetW(float w);

private:
	float m_x;
	float m_y; 
	float m_z;
	float m_w;
};