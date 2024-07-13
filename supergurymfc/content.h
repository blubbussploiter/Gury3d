#ifndef CONTENT_H
#define CONTENT_H

#include <string>
#include <G3D/GImage.h>
#include <G3D/BinaryOutput.h>

#include "encode.h"

namespace RBX
{
	enum MimeTypes
	{
		Mesh,
		Png,
		Jpg,
		Bmp,
		Tga
	};

	class Content
	{
	public:
		bool isBinary;
		bool isStored; /* on disk, web */
		bool isEmpty;

		MimeTypes contentType;

		std::string binary;
		std::string internalContentUrl, contentUrl;

	public:

		G3D::uint8* content;
		size_t contentLength;

		static Content fromImageFile(std::string file);
		bool resolve();

		std::string resolveContentHash();

		static Content fromBinary(std::string binary)
		{
			Content c;
			c.isEmpty = 0;
			c.isBinary = true;
			c.binary = binary;
			return c;
		}

		static Content fromContent(std::string content)
		{
			Content c;
			c.isEmpty = 0;
			c.isStored = true;
			c.contentUrl = content;
			return c;
		}

		Content() : isBinary(0), isStored(0), isEmpty(1) {}
	};

	class ContentProvider
	{
	public:
		static ContentProvider* get();
	};
}

#endif