
#include <fstream>
#include "3ds.h"


using namespace _3ds;

unsigned long ReadMemory(char* value, const void* mem, unsigned long num)
{
	const unsigned long bytes = sizeof(char) * num;
	memcpy((void*)value, mem, bytes);
	return bytes;
}

unsigned long ReadMemory(unsigned char* value, const void* mem, unsigned long num)
{
	const unsigned long bytes = sizeof(unsigned char) * num;
	memcpy((void*)value, mem, bytes);
	return bytes;
}

unsigned long ReadMemory(short* value, const void* mem, unsigned long num)
{
	const unsigned long bytes = sizeof(short) * num;
	memcpy((void*)value, mem, bytes);
	return bytes;
}

unsigned long ReadMemory(unsigned short* value, const void* mem, unsigned long num)
{
	const unsigned long bytes = sizeof(unsigned short) * num;
	memcpy((void*)value, mem, bytes);
	return bytes;
}

unsigned long ReadMemory(unsigned long* value, const void* mem, unsigned long num)
{
	const unsigned long bytes = sizeof(unsigned long) * num;
	memcpy((void*)value, mem, bytes);
	return bytes;
}

unsigned long ReadMemory(float* value, const void* mem, unsigned long num)
{
	const unsigned long bytes = sizeof(float) * num;
	memcpy((void*)value, mem, bytes);
	return bytes;
}

struct Counter
{
	unsigned short num_materials;
	unsigned short num_meshs;
};

void Count(Counter& counter, const char* mem, unsigned long length)
{
	counter.num_materials = 0;
	counter.num_meshs = 0;

	unsigned long pos = 0;
	unsigned long end = length;
	while(pos < end)
	{
		unsigned short chunk_id;
		unsigned long chunk_length;
		pos += ReadMemory(&chunk_id, (const void*)&mem[pos], 1);
		pos += ReadMemory(&chunk_length, (const void*)&mem[pos], 1);

		switch(chunk_id)
		{
		// MAIN CHUNK
		case 0x4D4D:
		// 3D EDITOR CHUNK
		case 0x3D3D:
			break;
		// MATERIAL BLOCK
		case 0xAFFF:
			counter.num_materials++;
			pos += chunk_length - 6;
			break;
		// OBJECT BLOCK
		case 0x4000:
			{
				char c;
				do
				{
					pos += ReadMemory(&c, (const void*)&mem[pos], 1);
				}while(c != '\0');
			}
			break;
		// TRIANGULAR MESH
		case 0x4100:
			counter.num_meshs++;
			pos += chunk_length - 6;
			break;
		// SKIP
		default:
			pos += chunk_length - 6;
			break;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////

Mesh::Mesh()
{
	num_vertices = 0;
	vertices = NULL;
	num_coords = 0;
	coords = NULL;
	num_faces = 0;
	faces = NULL;
}

Mesh::~Mesh()
{
	if(vertices)
		delete[] vertices;
	if(coords)
		delete[] coords;
	if(faces)
		delete[] faces;
}

////////////////////////////////////////////////////////////////////////////////

Geometry::Geometry()
{
	memory = NULL;
}

Geometry::~Geometry()
{
	Release();
}

bool Geometry::Load(const std::string& filename)
{
	Release();

	std::ifstream ifs;
	ifs.open(filename.c_str(), std::ios::in | std::ios::binary);
	if(!ifs)
		return false;

	ifs.seekg(0, std::ios_base::end);
	const unsigned long length = ifs.tellg();
	memory = new char[length];
	if(!memory)
	{
		ifs.close();
		return false;
	}
	ifs.seekg(0, std::ios_base::beg);
	ifs.read(memory, length);
	ifs.close();

	Counter counter;
	Count(counter, memory, length);
	materials.reserve(counter.num_materials);
	meshes.reserve(counter.num_meshs);

	Read(length);

	delete[] memory;
	memory = NULL;

	const float smoothing_angle = PI / 4;	// 暫定で 45 度
	CalcNormal(smoothing_angle);

	return true;
}

void Geometry::Release()
{
	for(material_array::iterator it = materials.begin(); it != materials.end(); it++)
	{
		delete (*it);
	}
	material_array().swap(materials);

	for(mesh_array::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		delete (*it);
	}
	mesh_array().swap(meshes);

	if(memory)
	{
		delete[] memory;
		memory = NULL;
	}
}

unsigned long Geometry::Read(unsigned long length)
{
	unsigned long pos = 0;
	unsigned long end = length;
	std::string name;
	while(pos < end)
	{
		unsigned short chunk_id;
		unsigned long chunk_length;
		pos += ReadMemory(&chunk_id, (const void*)&memory[pos], 1);
		pos += ReadMemory(&chunk_length, (const void*)&memory[pos], 1);

		switch(chunk_id)
		{
		// MAIN CHUNK
		case 0x4D4D:
			break;
		// 3D EDITOR CHUNK
		case 0x3D3D:
			break;
		// MATERIAL BLOCK
		case 0xAFFF:
			{
				Material* mtrl = new Material;
				memset((void*)mtrl, 0, sizeof(Material));
				materials.push_back(mtrl);
				pos = ReadMaterial(*mtrl, pos, chunk_length-6);
			}
			break;
		// OBJECT BLOCK
		case 0x4000:
			name.clear();
			pos = ReadName(name, pos);
			break;
		// TRIANGULAR MESH
		case 0x4100:
			{
				Mesh* mesh = new Mesh;
				memset((void*)mesh, 0, sizeof(Mesh));
				mesh->name = name;
				meshes.push_back(mesh);
				pos = ReadMesh(*mesh, pos, chunk_length-6);
			}
			break;
		// SKIP
		default:
			pos += chunk_length - 6;
			break;
		}
	}
	return 0;
}

unsigned long Geometry::ReadMaterial(Material& material, unsigned long pos, unsigned long length)
{
	unsigned long end = pos + length;
	while(pos < end)
	{
		unsigned short chunk_id;
		unsigned long chunk_length;
		pos += ReadMemory(&chunk_id, (const void*)&memory[pos], 1);
		pos += ReadMemory(&chunk_length, (const void*)&memory[pos], 1);

		switch(chunk_id)
		{
		// MATERIAL NAME
		case 0xA000:
			pos = ReadName(material.name, pos);
			break;
		// AMBIENT COLOR
		case 0xA010:
			pos = ReadColor(material.ambient, pos);
			break;
		// DIFFUSE COLOR
		case 0xA020:
			pos = ReadColor(material.diffuse, pos);
			break;
		// SPECULAR COLOR
		case 0xA030:
			pos = ReadColor(material.specular, pos);
			break;
		// SHHININESS PERCENT
		case 0xA040:
			pos = ReadQuantity(material.shininess, pos);
			break;
		// SHINE STRENGTH
		case 0xA041:
			pos = ReadQuantity(material.shine, pos);
			break;
		// TRANSPARENCY
		case 0xA050:
			pos = ReadQuantity(material.transparency, pos);
			break;
		// SKIP
		default:
			pos += chunk_length - 6;
			break;
		}
	}
	return pos;
}

unsigned long Geometry::ReadMesh(Mesh& mesh, unsigned long pos, unsigned long length)
{
	const unsigned long end = pos + length;
	while(pos < end)
	{
		unsigned short chunk_id;
		unsigned long chunk_length;
		pos += ReadMemory(&chunk_id, (const void*)&memory[pos], 1);
		pos += ReadMemory(&chunk_length, (const void*)&memory[pos], 1);

		switch(chunk_id)
		{
		// VERTICES LIST
		case 0x4110:
			{
				unsigned short num_vertices;
				pos += ReadMemory(&num_vertices, (const void*)&memory[pos], 1);
				mesh.num_vertices = num_vertices;
				if(num_vertices == 0)
				{
					mesh.vertices = NULL;
					break;
				}
				mesh.vertices = new Vector3[num_vertices];
				for(unsigned short i = 0; i < num_vertices; i++)
				{
					float e[3];
					pos += ReadMemory(e, (const void*)&memory[pos], 3);
 #ifdef CONVERT_LFCS
					mesh.vertices[i].set(e[0], e[2], e[1]);
  #ifdef CONVERT_CM_TO_M
					Vec3Scale(&mesh.vertices[i], &mesh.vertices[i], 0.01f);
  #endif // CONVERT_CM_TO_M
 #else
					mesh.vertices[i].set(e[0], e[1], e[2]);
 #endif // CONVERT_LFCS
				}
			}
			break;
		// FACES DESCRIPTION
		case 0x4120:
			{
				unsigned short num_faces;
				pos += ReadMemory(&num_faces, (const void*)&memory[pos], 1);
				mesh.num_faces = num_faces;
				if(num_faces == 0)
				{
					mesh.faces = NULL;
					break;
				}
				mesh.faces = new Face[num_faces];
				memset((void*)mesh.faces, 0, sizeof(Face)*num_faces);
				for(unsigned short i = 0; i < num_faces; i++)
				{
					unsigned short e[4];
					pos += ReadMemory(e, (const void*)&memory[pos], 4);
 #ifdef CONVERT_LFCS
					mesh.faces[i].a = e[0];
					mesh.faces[i].b = e[2];
					mesh.faces[i].c = e[1];
 #else
					mesh.faces[i].a = e[0];
					mesh.faces[i].b = e[1];
					mesh.faces[i].c = e[2];
 #endif // CONVERT_LFCS
				}
			}
			break;
		// FACE MATERIAL
		case 0x4130:
			{
				std::string name;
				pos = ReadName(name, pos);

				unsigned short mtrl_id = 0;
				for(material_array::const_iterator it = materials.begin(); it != materials.end(); it++)
				{
					if(name == (*it)->name)
					{
						break;
					}
					mtrl_id++;
				}

				unsigned short num_faces;
				pos += ReadMemory(&num_faces, (const void*)&memory[pos], 1);
				for(unsigned short i = 0; i < num_faces; i++)
				{
					unsigned short face_id;
					pos += ReadMemory(&face_id, (const void*)&memory[pos], 1);
					if(i == face_id)
					{
						mesh.faces[i].mtrl_id = mtrl_id;
					}
				}
			}
			break;
		// MAPPING COORDINATES LIST
		case 0x4140:
			{
				unsigned short num_coords;
				pos += ReadMemory(&num_coords, (const void*)&memory[pos], 1);
				mesh.num_coords = num_coords;
				if(num_coords == 0)
				{
					mesh.coords = NULL;
					break;
				}
				mesh.coords = new Coord[num_coords];
				for(unsigned short i = 0; i < num_coords; i++)
				{
					float coord[2];
					pos += ReadMemory(coord, (const void*)&memory[pos], 2);
					mesh.coords[i].u = coord[0];
					mesh.coords[i].v = coord[1];
				}
			}
			break;
		// FACE SMOOTHING GROUP
		case 0x4150:
			{
				const unsigned short num_faces = mesh.num_faces;
				for(unsigned short i = 0; i < num_faces; i++)
				{
					unsigned long sg;
					pos += ReadMemory(&sg, (const void*)&memory[pos], 1);
					mesh.faces[i].sg = sg;
				}
			}
			break;
		// LOCAL AXIS
		case 0x4160:
			{
				float e[3];
				pos += ReadMemory(e, (const void*)&memory[pos], 3);
				mesh.local_x.set(e[0], e[1], e[2]);
				pos += ReadMemory(e, (const void*)&memory[pos], 3);
				mesh.local_y.set(e[0], e[1], e[2]);
				pos += ReadMemory(e, (const void*)&memory[pos], 3);
				mesh.local_z.set(e[0], e[1], e[2]);
				pos += ReadMemory(e, (const void*)&memory[pos], 3);
				mesh.center.set(e[0], e[1], e[2]);
			}
			break;
		// SKIP
		default:
			pos += chunk_length - 6;
			break;
		}
	}
	return pos;
}

unsigned long Geometry::ReadColor(Color& color, unsigned long pos)
{
	unsigned short chunk_id;
	unsigned long chunk_length;
	pos += ReadMemory(&chunk_id, (const void*)&memory[pos], 1);
	pos += ReadMemory(&chunk_length, (const void*)&memory[pos], 1);

	switch(chunk_id)
	{
	// FLOATING POINT
	case 0x0010:
		{
			float e[3];
			pos += ReadMemory(e, (const void*)&memory[pos], 3);
			color.r = e[0];
			color.g = e[1];
			color.b = e[2];
		}
		break;
	// BYTE
	case 0x0011:
	case 0x0012:
		{
			unsigned char e[3];
			pos += ReadMemory(e, (const void*)&memory[pos], 3);
			color.r = e[0] / 255.0f;
			color.g = e[1] / 255.0f;
			color.b = e[2] / 255.0f;
		}
		break;
	// SKIP
	default:
		pos += chunk_length - 6;
		break;
	}
	return pos;
}

unsigned long Geometry::ReadQuantity(float& q, unsigned long pos)
{
	unsigned short chunk_id;
	unsigned long chunk_length;
	pos += ReadMemory(&chunk_id, (const void*)&memory[pos], 1);
	pos += ReadMemory(&chunk_length, (const void*)&memory[pos], 1);

	switch(chunk_id)
	{
	// HALF
	case 0x0030:
		{
			short value;
			pos += ReadMemory(&value, (const void*)&memory[pos], 1);
			q = (float)value;
		}
		break;
	// FLOATING POINT
	case 0x0031:
		{
			float value;
			pos += ReadMemory(&value, (const void*)&memory[pos], 1);
			q = value;
		}
		break;
	// SKIP
	default:
		pos += chunk_length - 6;
		break;
	}
	return pos;
}

unsigned long Geometry::ReadName(std::string& name, unsigned long pos)
{
	char c;
	do
	{
		pos += ReadMemory(&c, (const void*)&memory[pos], 1);
		name.append(1, c);
	}while(c != '\0');

	return pos;
}

/*!
	@brief		法線ベクトルの算出
	@param[i]	angle: スムージングアングル [rad]
 */
void Geometry::CalcNormal(float angle)
{
	// 面法線
	for(mesh_array::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		const Vector3* vertices = (*it)->vertices;
		const unsigned short num_faces = (*it)->num_faces;
		for(unsigned short i = 0; i < num_faces; i++)
		{
			Face* face = &(*it)->faces[i];
			Vector3 e0, e1, n;
			Vec3Subtract(&e0, &vertices[face->b], &vertices[face->a]);
			Vec3Subtract(&e1, &vertices[face->c], &vertices[face->a]);
			Vec3OuterProduct(&n, &e0, &e1);
			Vec3Normalize(&n, &n);
			face->n  = n;
		}
	}
	// 頂点法線
	for(mesh_array::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		Face* faces = (*it)->faces;
		const unsigned short num_faces = (*it)->num_faces;
		for(unsigned short i = 0; i < num_faces; i++)
		{
			Vector3 n0, n1, n2;
			n0 = faces[i].n;
			n1 = faces[i].n;
			n2 = faces[i].n;
			for(unsigned short j = 0; j < num_faces; j++)
			{
				if((i == j) || (faces[i].sg != faces[j].sg))
					continue;
				if((faces[i].a == faces[j].a) || (faces[i].a == faces[j].b) || (faces[i].a == faces[j].c))
				{
					if(acosf(Vec3InnerProduct(&faces[i].n, &faces[j].n)) <= angle)
						Vec3Add(&n0, &n0, &faces[j].n);
				}
				if((faces[i].b == faces[j].a) || (faces[i].b == faces[j].b) || (faces[i].b == faces[j].c))
				{
					if(acosf(Vec3InnerProduct(&faces[i].n, &faces[j].n)) <= angle)
						Vec3Add(&n1, &n1, &faces[j].n);
				}
				if((faces[i].c == faces[j].a) || (faces[i].c == faces[j].b) || (faces[i].c == faces[j].c))
				{
					if(acosf(Vec3InnerProduct(&faces[i].n, &faces[j].n)) <= angle)
						Vec3Add(&n1, &n1, &faces[j].n);
				}
			}
			Vec3Normalize(&faces[i].n0, &n0);
			Vec3Normalize(&faces[i].n1, &n1);
			Vec3Normalize(&faces[i].n2, &n2);
		}
	}
}
