#pragma once

#include <G3DAll.h>
#include "GuiRoot.h"

namespace RBX
{
	namespace Diagnostics
	{
		class Renderer
		{
		private:

			Gui::GuiBox diag_box0;
			Gui::GuiLabel diag_label0;
			Gui::GuiLabel diag_label1;
			Gui::GuiLabel diag_label2;
			Gui::GuiLabel diag_label3;
			Gui::GuiButton diag_button0;
			Gui::GuiButton diag_button1;
			Gui::GuiButton diag_button2;
			Gui::GuiButton diag_button3;
			Gui::GuiButton diag_button4;

			bool addedButtons;

		public:

			bool diagnostics_enabled;

			bool kernel_shouldRenderObjects;
			bool joints_shouldRenderOutlines;
			bool joints_shouldRenderWebs;

			void render(RenderDevice* rd);
			void preRender(RenderDevice* rd);
			void render2D(RenderDevice* rd);

			static void diag_jointsWeb(Gui::GuiButton* button);
			static void diag_jointsOutline(Gui::GuiButton* button);

			Renderer()
			{
				diag_box0.position = Vector2(20, 60);
				diag_box0.size = Vector2(200, 140);

				diag_label0.position = diag_box0.position;
				diag_label1.position = diag_label0.position + Vector2(0, 20);
				diag_label2.position = diag_label1.position + Vector2(0, 20);
				diag_label3.position = diag_label2.position + Vector2(0, 20);
				diag_button0.position = diag_label3.position + Vector2(0, 20);
				diag_button1.position = diag_button0.position + Vector2(50, 0);
				diag_button2.position = diag_button1.position + Vector2(50, 0);
				diag_button3.position = diag_label3.position + Vector2(0, 40);
				diag_button4.position = diag_button3.position + Vector2(0, 20);

				diag_label0.title = "gury diagnostics :)";

				diag_button3.renderColor = Color4::clear();
				diag_button3.hoverColor = Color4::clear();
				diag_button3.clickedColor = Color3::yellow();
				diag_button3.onClick = &diag_jointsWeb;

				diag_button4.renderColor = Color4::clear();
				diag_button4.hoverColor = Color4::clear();
				diag_button4.clickedColor = Color3::yellow();
				diag_button4.onClick = &diag_jointsOutline;

				diag_button0.size = Vector2(50, 20);
				diag_button1.size = Vector2(50, 20);
				diag_button2.size = Vector2(100, 20);
				diag_button3.size = Vector2(200, 20);
				diag_button4.size = Vector2(200, 20);

				diag_label0.textColor = Color3::blue();
				diag_label1.textColor = Color3::black();
				diag_label2.textColor = Color3::black();
				diag_label3.textColor = Color3::black();

				/* settings */

				kernel_shouldRenderObjects = 1;

				joints_shouldRenderOutlines = 0;
				joints_shouldRenderWebs = 0;
				
			}
		};

		extern Renderer* get_Renderer();
	}
}