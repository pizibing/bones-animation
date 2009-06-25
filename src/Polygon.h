#pragma once

class Polygon
{
public:
	Polygon(void);
	~Polygon(void);

private:
	// generic vector 3d
	struct vector3d {   
		GLfloat x;   
		GLfloat y;
		GLfloat z;
	};

	int m_num_vertices;
	struct vector3d *m_ptrs_vertices;

	//int m_num_normals; is like m_num_vertices
	bool m_has_normals;
	struct vector3d *m_ptrs_normals;

	// texture channels
	// At this moment, I´m going to use just ONE texture channel
	bool m_has_texcoords;
	struct vector3d *m_ptrs_texcoords;

	// material properties
	GLfloat m_mat_ambient[4];
	GLfloat m_mat_diffuse[4];
	GLfloat m_mat_specular[4];
	GLfloat m_mat_emission[4];
	GLfloat m_mat_shininess;
};
