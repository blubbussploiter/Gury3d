#include "content.h"

RBX::ContentProvider* provider;

RBX::ContentProvider* RBX::ContentProvider::singleton()
{ 
	if (!provider) provider = new ContentProvider();
	return provider;
}