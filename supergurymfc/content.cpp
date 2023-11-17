#include <iostream>
#include <sstream>
#include <fstream>

#include "strings.h"
#include "content.h"
#include "decode.h"
#include "md5.h"

#include "stdout.h"

RBX::Content RBX::Content::fromImageFile(std::string file)
{
	/* jsut read it as binary then encode to base64 */
	/*
	
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
	*/
	return Content::fromContent(file);
}

bool RBX::Content::resolve()
{
	if (isBinary)
	{
		std::string data = base64_decode(binary);
		contentLength = data.length();

		content = new G3D::uint8[contentLength];
		std::copy(data.begin(), data.end(), content);

		return true;
	}

	if (isStored)
	{
		RBX::ContentProvider::singleton()->downloadContent(*this);
		return true;
	}

	return false;
}

std::string RBX::Content::resolveContentHash()
{
	std::string base64 = base64_encode(content, contentLength);
	return md5(base64);
}
