#include "brickcolor.h"

RBX::BrickColor::BrickMap* brickMap;

RBX::BrickColor::BrickMap::BrickMap()
{
	insert("White", 1, Color3(0.94902, 0.952941, 0.94902));
	insert("Grey", 2, Color3(0.631373, 0.647059, 0.635294));
	insert("Light yellow", 3, Color3(0.976471, 0.913726, 0.6));
	insert("Brick yellow", 5, Color3(0.843137, 0.772549, 0.6));
	insert("Light green (Mint)", 6, Color3(0.760784, 0.854902, 0.721569));
	insert("Light reddish violet", 9, Color3(0.909804, 0.729412, 0.780392));
	insert("Pastel Blue", 11, Color3(0.501961, 0.733333, 0.858824));
	insert("Light orange brown", 12, Color3(0.796079, 0.517647, 0.258824));
	insert("Nougat", 18, Color3(0.8, 0.556863, 0.407843));
	insert("Bright red", 21, Color3(0.768628, 0.156863, 0.105882));
	insert("Med. reddish violet", 22, Color3(0.768628, 0.439216, 0.627451));
	insert("Bright blue", 23, Color3(0.0509804, 0.411765, 0.670588));
	insert("Bright yellow", 24, Color3(0.960784, 0.803922, 0.184314));
	insert("Earth orange", 25, Color3(0.384314, 0.278431, 0.196078));
	insert("Black", 26, Color3(0.105882, 0.164706, 0.203922));
	insert("Dark grey", 27, Color3(0.427451, 0.431373, 0.423529));
	insert("Dark green", 28, Color3(0.156863, 0.498039, 0.27451));
	insert("Medium green", 29, Color3(0.631373, 0.768628, 0.545098));
	insert("Lig. Yellowich orange", 36, Color3(0.952941, 0.811765, 0.607843));
	insert("Bright green", 37, Color3(0.294118, 0.592157, 0.290196));
	insert("Dark orange", 38, Color3(0.627451, 0.372549, 0.203922));
	insert("Light bluish violet", 39, Color3(0.756863, 0.792157, 0.870588));
	insert("Transparent", 40, Color3(0.92549, 0.92549, 0.92549));
	insert("Tr. Red", 41, Color3(0.803922, 0.329412, 0.294118));
	insert("Tr. Lg blue", 42, Color3(0.756863, 0.87451, 0.941177));
	insert("Tr. Blue", 43, Color3(0.482353, 0.713726, 0.909804));
	insert("Tr. Yellow", 44, Color3(0.968628, 0.945098, 0.552941));
	insert("Light blue", 45, Color3(0.705882, 0.823529, 0.890196));
	insert("Tr. Flu. Reddish orange", 47, Color3(0.85098, 0.521569, 0.423529));
	insert("Tr. Green", 48, Color3(0.517647, 0.713726, 0.552941));
	insert("Tr. Flu. Green", 49, Color3(0.972549, 0.945098, 0.517647));
	insert("Phosph. White", 50, Color3(0.92549, 0.909804, 0.870588));
	insert("Light red", 100, Color3(0.933333, 0.768628, 0.713726));
	insert("Medium red", 101, Color3(0.854902, 0.52549, 0.47451));
	insert("Medium blue", 102, Color3(0.431373, 0.6, 0.788235));
	insert("Light grey", 103, Color3(0.780392, 0.756863, 0.717647));
	insert("Bright violet", 104, Color3(0.419608, 0.196078, 0.482353));
	insert("Br. yellowish orange", 105, Color3(0.886275, 0.607843, 0.247059));
	insert("Bright orange", 106, Color3(0.854902, 0.521569, 0.25098));
	insert("Bright bluish green", 107, Color3(0, 0.560784, 0.607843));
	insert("Earth yellow", 108, Color3(0.407843, 0.360784, 0.262745));
	insert("Bright bluish violet", 110, Color3(0.262745, 0.329412, 0.576471));
	insert("Tr. Brown", 111, Color3(0.74902, 0.717647, 0.694118));
	insert("Medium bluish violet", 112, Color3(0.407843, 0.454902, 0.67451));
	insert("Tr. Medi. reddish violet", 113, Color3(0.894118, 0.678431, 0.784314));
	insert("Med. yellowish green", 115, Color3(0.780392, 0.823529, 0.235294));
	insert("Med. bluish green", 116, Color3(0.333333, 0.647059, 0.686275));
	insert("Light bluish green", 118, Color3(0.717647, 0.843137, 0.835294));
	insert("Br. yellowish green", 119, Color3(0.643137, 0.741176, 0.27451));
	insert("Lig. yellowish green", 120, Color3(0.85098, 0.894118, 0.654902));
	insert("Med. yellowish orange", 121, Color3(0.905882, 0.67451, 0.345098));
	insert("Br. reddish orange", 123, Color3(0.827451, 0.435294, 0.298039));
	insert("Bright reddish violet", 124, Color3(0.572549, 0.223529, 0.470588));
	insert("Light orange", 125, Color3(0.917647, 0.721569, 0.568627));
	insert("Tr. Bright bluish violet", 126, Color3(0.647059, 0.647059, 0.796079));
	insert("Gold", 127, Color3(0.862745, 0.737255, 0.505882));
	insert("Dark nougat", 128, Color3(0.682353, 0.478431, 0.34902));
	insert("Silver", 131, Color3(0.611765, 0.639216, 0.658824));
	insert("Neon orange", 133, Color3(0.835294, 0.45098, 0.239216));
	insert("Neon green", 134, Color3(0.847059, 0.866667, 0.337255));
	insert("Sand blue", 135, Color3(0.454902, 0.52549, 0.611765));
	insert("Sand violet", 136, Color3(0.529412, 0.486275, 0.564706));
	insert("Medium orange", 137, Color3(0.878431, 0.596078, 0.392157));
	insert("Sand yellow", 138, Color3(0.584314, 0.541176, 0.45098));
	insert("Earth blue", 140, Color3(0.12549, 0.227451, 0.337255));
	insert("Earth green", 141, Color3(0.152941, 0.27451, 0.172549));
	insert("Tr. Flu. Blue", 143, Color3(0.811765, 0.886275, 0.968628));
	insert("Sand blue metallic", 145, Color3(0.47451, 0.533333, 0.631373));
	insert("Sand violet metallic", 146, Color3(0.584314, 0.556863, 0.639216));
	insert("Sand yellow metallic", 147, Color3(0.576471, 0.529412, 0.403922));
	insert("Dark grey metallic", 148, Color3(0.341176, 0.345098, 0.341176));
	insert("Black metallic", 149, Color3(0.0862745, 0.113725, 0.196078));
	insert("Light grey metallic", 150, Color3(0.670588, 0.678431, 0.67451));
	insert("Sand green", 151, Color3(0.470588, 0.564706, 0.505882));
	insert("Sand red", 153, Color3(0.584314, 0.47451, 0.462745));
	insert("Dark red", 154, Color3(0.482353, 0.180392, 0.184314));
	insert("Tr. Flu. Yellow", 157, Color3(1, 0.964706, 0.482353));
	insert("Tr. Flu. Red", 158, Color3(0.882353, 0.643137, 0.760784));
	insert("Gun metallic", 168, Color3(0.458824, 0.423529, 0.384314));
	insert("Red flip/flop", 176, Color3(0.592157, 0.411765, 0.356863));
	insert("Yellow flip/flop", 178, Color3(0.705882, 0.517647, 0.333333));
	insert("Silver flip/flop", 179, Color3(0.537255, 0.529412, 0.533333));
	insert("Curry", 180, Color3(0.843137, 0.662745, 0.294118));
	insert("Fire Yellow", 190, Color3(0.976471, 0.839216, 0.180392));
	insert("Flame yellowish orange", 191, Color3(0.909804, 0.670588, 0.176471));
	insert("Reddish brown", 192, Color3(0.411765, 0.25098, 0.152941));
	insert("Flame reddish orange", 193, Color3(0.811765, 0.376471, 0.141176));
	insert("Medium stone grey", 194, Color3(0.639216, 0.635294, 0.643137));
	insert("Royal blue", 195, Color3(0.27451, 0.403922, 0.643137));
	insert("Dark Royal blue", 196, Color3(0.137255, 0.278431, 0.545098));
	insert("Bright reddish lilac", 198, Color3(0.556863, 0.258824, 0.521569));
	insert("Dark stone grey", 199, Color3(0.388235, 0.372549, 0.380392));
	insert("Lemon metalic", 200, Color3(0.509804, 0.541176, 0.364706));
	insert("Light stone grey", 208, Color3(0.898039, 0.894118, 0.870588));
	insert("Dark Curry", 209, Color3(0.690196, 0.556863, 0.266667));
	insert("Faded green", 210, Color3(0.439216, 0.584314, 0.470588));
	insert("Turquoise", 211, Color3(0.47451, 0.709804, 0.709804));
	insert("Light Royal blue", 212, Color3(0.623529, 0.764706, 0.913726));
	insert("Medium Royal blue", 213, Color3(0.423529, 0.505882, 0.717647));
	insert("Rust", 216, Color3(0.560784, 0.298039, 0.164706));
	insert("Brown", 217, Color3(0.486275, 0.360784, 0.270588));
	insert("Reddish lilac", 218, Color3(0.588235, 0.439216, 0.623529));
	insert("Lilac", 219, Color3(0.419608, 0.384314, 0.607843));
	insert("Light lilac", 220, Color3(0.654902, 0.662745, 0.807843));
	insert("Bright purple", 221, Color3(0.803922, 0.384314, 0.596078));
	insert("Light purple", 222, Color3(0.894118, 0.678431, 0.784314));
	insert("Light pink", 223, Color3(0.862745, 0.564706, 0.584314));
	insert("Light brick yellow", 224, Color3(0.941177, 0.835294, 0.627451));
	insert("Warm yellowish orange", 225, Color3(0.921569, 0.721569, 0.498039));
	insert("Cool yellow", 226, Color3(0.992157, 0.917647, 0.54902));
	insert("Dove blue", 232, Color3(0.490196, 0.733333, 0.866667));
	insert("Medium lilac", 268, Color3(0.203922, 0.168627, 0.458824));
}

RBX::BrickColor::BrickMap* RBX::BrickColor::BrickMap::get()
{
	if (!brickMap) brickMap = new BrickMap();
	return brickMap;
}

void RBX::BrickColor::BrickMap::insert(std::string name, int number, Color3 color)
{
	Number brickColorNumber;
	brickColorNumber.number = number;
	brickColorNumber.name = name;
	colors[brickColorNumber] = color;
}

Color3 RBX::BrickColor::BrickMap::fromNumber(int number)
{
	std::map<Number, Color3>::iterator it;

	for (it = colors.begin(); it != colors.end(); it++)
	{
		Number n = it->first;
		if (n.number == number) return it->second;
	}
	return Color3::gray();
}

Color3 RBX::BrickColor::BrickMap::fromName(std::string name)
{
	std::map<Number, Color3>::iterator it;

	for (it = colors.begin(); it != colors.end(); it++)
	{
		Number n = it->first;
		if (n.name == name) return it->second;
	}
	return Color3::gray();
}
