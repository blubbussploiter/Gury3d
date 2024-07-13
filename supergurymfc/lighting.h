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

		Color3 ambientTop, ambientBottom, spotLight;
		Color4 clear_color;

		void adjust() /* same color for top and front(maybe) sides */
		{
			params->lightDirection.x = -params->lightDirection.y;
			params->lightColor = spotLight;
		}

	public:

		GameTime timeOfDay;

		LightingParameters* params;
		LightingRef lighting;

		void setTopAmbient(Color3 c)
		{
			ambientTop = c;
			lighting->ambientTop = ambientTop;
		}

		void setBottomAmbient(Color3 c) 
		{
			ambientBottom = c;
			lighting->ambientBottom = ambientBottom;
		}

		void setClearColor(Color3 c) 
		{
			clear_color = c;
		}

		void setSpotLight(Color3 c)
		{
			spotLight = c;
			params->lightColor = spotLight;
		}

		void setTimeOfDay(std::string t) 
		{
			timeOfDay = Time::fromString(t.c_str());

			params->setTime(timeOfDay);
			adjust();
		}

		void setGeoLatitude(float l)
		{
			latitude = l;
			params->setLatitude(latitude);
			adjust();
		}

		std::string getTimeOfDay() { return Time::toString(timeOfDay); }
		Color3 getSpotLight() { return spotLight; }
		Color3 getTopAmbient() { return ambientTop; }
		Color3 getBottomAmbient() { return ambientBottom; }
		Color3 getClearColor() { return clear_color; }

		float getGeoLatitude() { return latitude; }

		static Lighting* get();

		Lighting();
		RTTR_ENABLE(RBX::Instance);
	};
}