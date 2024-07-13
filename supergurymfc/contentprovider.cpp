#include "content.h"

RBX::ContentProvider* provider;

RBX::ContentProvider* RBX::ContentProvider::get()
{ 
	if (!provider) provider = new ContentProvider();
	return provider;
}