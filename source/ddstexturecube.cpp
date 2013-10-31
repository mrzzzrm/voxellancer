
#include <iostream>
#include <fstream>

#include "ddstexturecube.h"


glow::Texture * DdsTextureCube::load(std::string pathXp, std::string pathXn, std::string pathYp, std::string pathYn, std::string pathZp, std::string pathZn)
{
	glow::Texture *texture = new glow::Texture(GL_TEXTURE_CUBE_MAP);

	loadSingleTexture(pathXp, texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	loadSingleTexture(pathXn, texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	loadSingleTexture(pathYp, texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	loadSingleTexture(pathYn, texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	loadSingleTexture(pathZp, texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	loadSingleTexture(pathZn, texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	return texture;
}


void DdsTextureCube::loadSingleTexture(std::string path, glow::Texture *texture, GLenum target){
	char header[124];


	/* try to open the file */
	std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		std::cout << "DdsTexture: could not read:" << path;
		return;
	}

	file.seekg(0, std::ios::beg);

	/* verify the type of file */
	char filecode[4];
	file.read(filecode, 4);

	if (strncmp(filecode, "DDS ", 4) != 0) {
		file.close();
		std::cout << "DdsTexture: not a dds file:" << path;
		return;
	}

	/* get the surface desc */
	file.read(header, 124);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	/* how big is it going to be including all mipmaps? */
	unsigned int bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	char * buffer = new char[bufsize * sizeof(char)];
	file.read(buffer, bufsize);
	/* close the file pointer */
	file.close();

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		delete[] buffer;
		std::cout << "DdsTexture: not a supported dds format:" << path;
		return;
	}

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		texture->compressedImage2D(level, format, width, height, 0, size, buffer + offset, target);
		offset += size;
		width /= 2;
		height /= 2;
	}
	delete[] buffer;


}