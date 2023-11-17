#include <iostream>
#include <sstream>
#include <fstream>

#include "strings.h"
#include "content.h"
#include "decode.h"
#include "md5.h"

RBX::ContentProvider* provider;

void RBX::ContentProvider::downloadContent(RBX::Content& content, std::string& contentPath) /* we wont use this in the future btw */
{
	std::string baseContentPath = ConFileInPath("/content/provider/");
	std::ofstream write;

	if (content.isEmpty) return;

	if (content.isBinary)
	{
		if (CreateDirectory(baseContentPath.c_str(), NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			std::string hash, decoded, url, str;
			std::istringstream stream;
			 
			if (content.binary.empty()) return;

			hash = md5(content.binary);
			decoded = base64_decode(content.binary);

			url = baseContentPath + hash;
			content.internalContentUrl = url;

			write = std::ofstream(url);
			stream = std::istringstream(decoded);

			while (std::getline(stream, str))
			{
				write << str;
			}
			
			write.close();

			contentPath = url;

		}
	}
}

void RBX::ContentProvider::cleanupContent(Content content)
{

}

RBX::ContentProvider* RBX::ContentProvider::singleton()
{
	if (!provider) provider = new ContentProvider();
	return provider;
}

RBX::Content RBX::Content::fromImageFile(std::string file)
{
	G3D::GImage data;
	G3D::GImage::Format format;

	G3D::BinaryOutput output;
	Content content(1, "");

	format = data.resolveFormat(file);

	data.load(file, format);
	data.encode(format, output);

	content.content = output.getCArray();
	content.contentLength = output.length();

	return content;
}

std::string RBX::Content::resolveContentHash()
{
	std::string base64 = base64_encode(content, contentLength);
	return md5(base64);
}
