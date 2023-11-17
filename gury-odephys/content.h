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
		bool isEmpty;

		MimeTypes contentType;

		std::string binary;
		std::string internalContentUrl, contentUrl;

	public:

		const G3D::uint8* content;
		size_t contentLength;

		static Content fromImageFile(std::string file);
		std::string resolveContentHash();

		Content(std::string content) : isBinary(0), contentUrl(content) 
		{
			isEmpty = 0;
		}
		Content(bool isBinary, std::string binary) : isBinary(isBinary), binary(binary) 
		{
			isEmpty = 0;
		}
		Content(bool isBinary) : isBinary(isBinary) 
		{
			isEmpty = 0;
		}
		Content() : isBinary(0), isEmpty(1) {}
	};

	class ContentProvider
	{
	public:
		void downloadContent(Content& content, std::string& contentPath);
		void cleanupContent(Content content);
		static ContentProvider* singleton();
	};
}

#endif