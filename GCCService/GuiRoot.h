#ifndef GUIROOT_H
#define GUIROOT_H

#include <G3DAll.h>
#include <vector>

#include "instance.h"
#include "strings.h"

namespace RBX
{
	namespace Gui
	{

		/* Calling :remove or ->remove on any of these will crash ur game ok ty */

		Vector2 getPosition(float width, float height, Vector2 pos, bool alignRight, bool alignBottomRight, bool alignBottom);

		class GuiObject : public RBX::Instance
		{
		public:

			bool alignRight;
			bool alignBottomRight;
			bool alignBottom;

			Vector2 origin;
			Vector2 position;
			Vector2 size;

			bool visible;
			bool isButton;

			virtual void render(RenderDevice* d) = 0;
			GuiObject() : visible(true) {
				alignBottom = 0;
				alignRight = 0;
				alignBottomRight = 0;
				setClassName("GuiObject"); 
				setName("GuiObject");
			}
		};

		class GuiBox : public GuiObject
		{
		public:
			Color4 background;
			Color4 outline;
			virtual void render(RenderDevice* d);
			GuiBox() {background = Color4(0.5f, 0.5f, 0.5f, 0.3f); outline = Color4::CLEAR;}
		};

		class GuiList : public GuiObject
		{
		private:
			std::vector<GuiObject*> children;
		public:

			std::string listTitle;
			float startY;

			bool appliedTitle;

			virtual void render(RenderDevice* d);
			void addChild(GuiObject* o);
			void removeChild(GuiObject* o);
		};

		class GuiButton : public GuiBox
		{
		public:

			std::string title;
			Color4 titleColor, hoverColor, disabledColor, clickedColor;

			Vector2 titleOff;
			float sz;

			bool disabled;
			bool hovered;

			bool clicked;

			void(*onClick)(GuiButton* b);

			virtual void render(RenderDevice* d);
			virtual bool mouseIn(Vector2 mosPos);

			virtual void handleMouse(G3D::UserInput* ui);

			GuiButton() {
				isButton = true; sz = 12.f; 
				titleColor = Color4(0.25f, 0.25f, 0.25f, 0.95f);
				hoverColor = Color4(0.6f, 0.6f, 0.6f, 0.3f);
				disabledColor = Color4(0.6f, 0.6f, 0.6f, 0.6f);
				clickedColor = hoverColor;
			}
		};

		class GuiImage : public GuiObject
		{
		public:
			TextureRef texture;
			int width;
			int height;

			virtual void render(RenderDevice* d);
		};

		class GuiImageButton : public GuiButton
		{
		public:
			TextureRef texture;
			TextureRef hoverTexture;
			TextureRef clickTexture;

			int width;
			int height;

			virtual void render(RenderDevice* d);
			virtual bool mouseIn(Vector2 mosPos);
			virtual void handleMouse(G3D::UserInput* ui);
		};

		class GuiLabel : public GuiObject
		{
		public:
			float sz;
			std::string title;

			Color4 textColor;
			Color4 outlineColor;

			virtual void render(RenderDevice* d);
			GuiLabel() : title("GuiLabel"), sz(12.f), textColor(Color3::black()), outlineColor(Color4::CLEAR) {}
		};

		class GuiMessage : public GuiObject
		{
		public:
			std::string text;
			virtual void render(RenderDevice* d);
		};

		class GuiHint : public GuiObject
		{
		public:
			std::string text;
			virtual void render(RenderDevice* d);
		};

		class GuiRoot
		{
		private:
			std::vector<GuiObject*> objects;
		public:

			GFontRef font;

			void render(RenderDevice* d);

			void doButtonLogic(G3D::UserInput* ui, RenderDevice* d);

			void add(GuiObject* obj) { objects.push_back(obj); }

			void initFont()
			{
				if (font.isNull())
				{
					font = GFont::fromFile(GetFileInPath("/content/font/lighttrek-small.fnt"));
				}
			}

			GuiRoot() 
			{ 

			}
		};

		namespace CameraPanMenu
		{
			extern void onArrowUpBtnClick(GuiButton* btn);
			extern void onArrowDwnBtnClick(GuiButton* btn);
			extern void onZoomInBtnClick(GuiButton* btn);
			extern void onZoomOutBtnClick(GuiButton* btn);
		}

		namespace MenuBar
		{
			extern void onExitBtnClick(GuiButton* btn);
			extern void onFullscreenBtnClick(GuiButton* btn);
		}

		/* deprecated, use Datamodel->guiRoot */
		GuiRoot* singleton(); 
	}
}

#endif