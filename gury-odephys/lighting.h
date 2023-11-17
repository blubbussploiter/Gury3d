#include <G3DAll.h>
#include "instance.h"

namespace RBX
{

	namespace Time
	{
		extern GameTime fromString(const char* c);
		extern std::string toString(int seconds);
	}

	class Lighting :
		public RBX::Instance
	{
	private:
		float latitude;

		Color3 top_ambient, bottom_ambient, spotLight_color;
		Color4 clear_color;

		LightingParameters* params;

		Vector3 lightDir2;
		Vector3 toLight;

		GameTime timeOfDay;

	public:

		void begin(RenderDevice* renderDevice);
		void end(RenderDevice* renderDevice);

		void setSpotLight(Color3 c) { spotLight_color = c; }
		void setTopAmbient(Color3 c) { top_ambient = c; }
		void setBottomAmbient(Color3 c) { bottom_ambient = c; }
		void setTimeOfDay(std::string t) { timeOfDay = Time::fromString(t.c_str()); }
		void setGeoLatitude(float l) { latitude = l; }

		Color3 getSpotLight() { return spotLight_color; }
		Color3 getTopAmbient() { return top_ambient; }
		Color3 getBottomAmbient() { return bottom_ambient; }
		std::string getTimeOfDay() { return Time::toString(timeOfDay); }
		float getGeoLatitude() { return latitude; }

		LightingParameters getParameters();
		static Lighting* singleton();

		Lighting()
		{
			latitude = 41.7f;

			clear_color = Color4(1.0f, 1.0f, 1.0f);
			top_ambient = Color3(0.81960785f, 0.81568629f, 0.8509804f);
			bottom_ambient = Color3(0.47843137f, 0.52549022f, 0.47058824f);
			spotLight_color = Color3(0.59607846f, 0.53725493f, 0.40000001f);

			lightDir2 = Vector3::unitY();
			toLight = Vector3(-0.40000001f, -1.0f, 0.1f);

			timeOfDay = toSeconds(14, 0, PM);

			setName("Lighting");
			setClassName("Lighting");
		}
		RTTR_ENABLE(RBX::Instance);
	};
}