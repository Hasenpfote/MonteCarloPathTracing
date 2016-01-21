//==============================================================================
/*!
	@file	3ds.h
	@brief	3DS ÉtÉ@ÉCÉãëÄçÏ
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __3DS_H_
#define __3DS_H_

#include <string>
#include <list>
#include <vector>
#include "lib/math/vector.h"

#define CONVERT_LFCS	// left handed coordinate system
#define CONVERT_CM_TO_M	// cm to m

namespace _3ds
{

struct Color
{
	float r, g, b;
};

struct Material
{
	std::string	name;
	Color		ambient;
	Color		diffuse;
	Color		specular;
	float		shininess;
	float		shine;
	float		transparency;
};

struct Coord
{
	float u;
	float v;
};

struct Face
{
	unsigned long	sg;	//!< smoothing group
	unsigned short	a;
	unsigned short	b;
	unsigned short	c;
	unsigned short	mtrl_id;
	Vector3			n;
	Vector3			n0;
	Vector3			n1;
	Vector3			n2;
};

struct Mesh
{
public:
	Mesh();
	~Mesh();

public:
	std::string		name;
	unsigned short	num_vertices;
	Vector3*		vertices;
	unsigned short	num_coords;
	Coord*			coords;
	unsigned short	num_faces;
	Face*			faces;
	Vector3			local_x;
	Vector3			local_y;
	Vector3			local_z;
	Vector3			center;
};

typedef std::list<Material*>	material_list;
typedef std::list<Mesh*>		mesh_list;

typedef std::vector<Material*>	material_array;
typedef std::vector<Mesh*>		mesh_array;

class Geometry
{
public:
	Geometry();
	~Geometry();
	bool Load(const std::string& filename);
	void Release();

	material_array& GetMaterials(){ return materials; }
	mesh_array& GetMeshes(){ return meshes; }

private:
	unsigned long Read(unsigned long length);
	unsigned long ReadMaterial(Material& material, unsigned long pos, unsigned long length);
	unsigned long ReadMesh(Mesh& mesh, unsigned long pos, unsigned long length);
	unsigned long ReadColor(Color& color, unsigned long pos);
	unsigned long ReadQuantity(float& q, unsigned long pos);
	unsigned long ReadName(std::string& name, unsigned long pos);
	void CalcNormal(float angle);

private:
	material_array	materials;
	mesh_array		meshes;
	char*			memory;
};

}

#endif // __3DS_H_