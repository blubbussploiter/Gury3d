#include "content.h"

RBX::ContentProvider* provider;

void RBX::ContentProvider::downloadContent(RBX::Content& content)
{

}

RBX::ContentProvider* RBX::ContentProvider::singleton()
{ 
	if (!provider) provider = new ContentProvider();
	return provider;
}