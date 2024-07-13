#include <G3DAll.h>
#include "GLG3D/ToneMap.h"

#include "instance.h"
#include "service.h"

namespace RBX
{
	namespace Time
	{
		extern GameTime fromString(const char* c);
		extern std::string toString(int seconds);
	}

	class Lighting :
		public RBX::Instance,
		public RBX::Service<RBX::Lighting>
	{
	private:
		float latitude;

		Color3 ambientTop, ambientBottom;
		Color4 clear_color;

		GameTime timeOfDay;
	public:

		LightingParameters params;
		LightingRef lighting;

		void setTopAmbient(Color3 c)
		{
			lighting->ambientTop = c;
			ambientTop = c;
		}

		void setBottomAmbient(Color3 c) 
		{
			lighting->ambientBottom = c;
			ambientBottom = c;
		}

		void setClearColor(Color3 c) 
		{
			clear_color = c;
		}

		void setSpotLight(Color3 c)
		{
			params.lightColor = c;
		}

		void setTimeOfDay(std::string t) 
		{
			timeOfDay = Time::fromString(t.c_str());
			params.setTime(timeOfDay);
		}

		void setGeoLatitude(float l)
		{ 
			latitude = l;
			params.setLatitude(latitude);
		}

		std::string getTimeOfDay() { return Time::toString(timeOfDay); }
		Color3 getSpotLight() { return params.lightColor; }
		Color3 getTopAmbient() { return ambientTop; }
		Color3 getBottomAmbient() { return ambientBottom; }
		Color3 getClearColor() { return clear_color; }

		float getGeoLatitude() { return latitude; }

		static Lighting* singleton();

		Lighting()
		{
			params = LightingParameters();

			setGeoLatitude(41.7f);
			setTimeOfDay("14:0:0");
			setSpotLight(Color3(0.59607846f, 0.53725493f, 0.40000001f));

			lighting = G3D::Lighting::create();

			clear_color = Color4(0.0f, 0.0f, 0.0f);
			setTopAmbient(Color3(0.81960785f, 0.81568629f, 0.8509804f));
			setBottomAmbient(Color3(0.47843137f, 0.52549022f, 0.47058824f));

			setName("Lighting");
			setClassName("Lighting");
		}
		RTTR_ENABLE(RBX::Instance);
	};
}