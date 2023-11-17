
// 3DStudioDoc.cpp : implementation of the CMy3DStudioDoc class
//

#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "3DStudio.h"
#endif

#include "3DStudioDoc.h"

#include <propkey.h>

// CMy3DStudioDoc

IMPLEMENT_DYNCREATE(CMy3DStudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy3DStudioDoc, CDocument)
END_MESSAGE_MAP()

// CMy3DStudioDoc construction/destruction

CMy3DStudioDoc::CMy3DStudioDoc() noexcept
{
	// TODO: add one-time construction code here

}

CMy3DStudioDoc::~CMy3DStudioDoc()
{
}

BOOL CMy3DStudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// CMy3DStudioDoc serialization

void CMy3DStudioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}