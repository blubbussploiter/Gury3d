#include "diagnosticsWorldDrawer.h"
#include "jointsservice.h"

RBX::Diagnostics::Renderer* renderer;

RBX::Diagnostics::Renderer* RBX::Diagnostics::get_Renderer()
{
	if (!renderer)
	{
		renderer = new RBX::Diagnostics::Renderer();
	}
	return renderer;
}

void RBX::Diagnostics::Renderer::render(RenderDevice* rd)
{
	if (!diagnostics_enabled) return;

	/* render Joints */

	if (joints_shouldRenderWebs)
	{

		RBX::Instances* joints = JointsService::singleton()->getChildren();
		for (size_t i = 0; i < joints->size(); i++)
		{
			RBX::Connector* joint = toInstance<Connector>(joints->at(i));
			if (joint)
			{
				joint->diagRender(rd);
			}
		}

	}

	/* render kernel stuff */

	Kernel::get()->diag_renderObjects(rd);

	/* -- render roblox stuff probably.. -- */


}

void RBX::Diagnostics::Renderer::preRender(RenderDevice* rd)
{
	if (!diagnostics_enabled) return;

	/* render outlines for joint's primitives */

	if (joints_shouldRenderOutlines)
	{

		RBX::Instances* joints = JointsService::singleton()->getChildren();
		for (size_t i = 0; i < joints->size(); i++)
		{
			RBX::Connector* joint = toInstance<Connector>(joints->at(i));
			if (joint)
			{
				joint->diagRenderPrimitiveOutlines(rd);
			}
		}

	}
}

void RBX::Diagnostics::Renderer::render2D(RenderDevice* rd)
{
	if (!diagnostics_enabled) return;

	if (!addedButtons)
	{
		RBX::Gui::singleton()->add(&diag_button3);
		RBX::Gui::singleton()->add(&diag_button4);
		addedButtons = true;
	}

	diag_label1.title = RBX::Format("Joints in world: %d", JointsService::singleton()->getChildren()->size());
	diag_label2.title = RBX::Format("Primitives in world: %d", Kernel::get()->objects.size());
	diag_label3.title = RBX::Format("Kernel: good");
	diag_button3.title = RBX::Format("joints_renderWebs: %d", joints_shouldRenderWebs);
	diag_button4.title = RBX::Format("joints_renderOutlines: %d", joints_shouldRenderOutlines);

	diag_label0.render(rd);
	diag_label1.render(rd);
	diag_label2.render(rd);
	diag_label3.render(rd);
	diag_box0.render(rd);

}

void RBX::Diagnostics::Renderer::diag_jointsWeb(Gui::GuiButton* button)
{
	Diagnostics::get_Renderer()->joints_shouldRenderWebs = !
		Diagnostics::get_Renderer()->joints_shouldRenderWebs;
}

void RBX::Diagnostics::Renderer::diag_jointsOutline(Gui::GuiButton* button)
{
	Diagnostics::get_Renderer()->joints_shouldRenderOutlines = !
		Diagnostics::get_Renderer()->joints_shouldRenderOutlines;
}
