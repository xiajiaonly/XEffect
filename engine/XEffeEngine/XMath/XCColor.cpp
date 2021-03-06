#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XCColor.h"
#include "XFColor.h"
namespace XE {
XCColor::XCColor(const XFColor& temp)
{
	r = temp.r * 255.0f;
	g = temp.g * 255.0f;
	b = temp.b * 255.0f;
	a = temp.a * 255.0f;
}
XCColor& XCColor::operator = (const XFColor& temp)
{
	r = temp.r * 255.0f;
	g = temp.g * 255.0f;
	b = temp.b * 255.0f;
	a = temp.a * 255.0f;
	return *this;
}
const XCColor XCColor::gray(255.0f / 2, 255.0f / 2, 255.0f / 2);
const XCColor XCColor::white(255.0f, 255.0f, 255.0f);
const XCColor XCColor::red(255.0f, 0, 0);
const XCColor XCColor::green(0, 255.0f, 0);
const XCColor XCColor::blue(0, 0, 255.0f);
const XCColor XCColor::cyan(0, 255.0f, 255.0f);
const XCColor XCColor::magenta(255.0f, 0, 255.0f);
const XCColor XCColor::yellow(255.0f, 255.0f, 0);
const XCColor XCColor::black(0, 0, 0);
const XCColor XCColor::aliceBlue(0.941176*255.0f, 0.972549*255.0f, 1 * 255.0f);
const XCColor XCColor::antiqueWhite(0.980392*255.0f, 0.921569*255.0f, 0.843137*255.0f);
const XCColor XCColor::aqua(0 * 255.0f, 1 * 255.0f, 1 * 255.0f);
const XCColor XCColor::aquamarine(0.498039*255.0f, 1 * 255.0f, 0.831373*255.0f);
const XCColor XCColor::azure(0.941176*255.0f, 1 * 255.0f, 1 * 255.0f);
const XCColor XCColor::beige(0.960784*255.0f, 0.960784*255.0f, 0.862745*255.0f);
const XCColor XCColor::bisque(1 * 255.0f, 0.894118*255.0f, 0.768627*255.0f);
const XCColor XCColor::blanchedAlmond(1 * 255.0f, 0.921569*255.0f, 0.803922*255.0f);
const XCColor XCColor::blueViolet(0.541176*255.0f, 0.168627*255.0f, 0.886275*255.0f);
const XCColor XCColor::brown(0.647059*255.0f, 0.164706*255.0f, 0.164706*255.0f);
const XCColor XCColor::burlyWood(0.870588*255.0f, 0.721569*255.0f, 0.529412*255.0f);
const XCColor XCColor::cadetBlue(0.372549*255.0f, 0.619608*255.0f, 0.627451*255.0f);
const XCColor XCColor::chartreuse(0.498039*255.0f, 1 * 255.0f, 0 * 255.0f);
const XCColor XCColor::chocolate(0.823529*255.0f, 0.411765*255.0f, 0.117647*255.0f);
const XCColor XCColor::coral(1 * 255.0f, 0.498039*255.0f, 0.313726*255.0f);
const XCColor XCColor::cornflowerBlue(0.392157*255.0f, 0.584314*255.0f, 0.929412*255.0f);
const XCColor XCColor::cornsilk(1 * 255.0f, 0.972549*255.0f, 0.862745*255.0f);
const XCColor XCColor::crimson(0.862745*255.0f, 0.0784314*255.0f, 0.235294*255.0f);
const XCColor XCColor::darkBlue(0 * 255.0f, 0 * 255.0f, 0.545098*255.0f);
const XCColor XCColor::darkCyan(0 * 255.0f, 0.545098*255.0f, 0.545098*255.0f);
const XCColor XCColor::darkGoldenRod(0.721569*255.0f, 0.52549*255.0f, 0.0431373*255.0f);
const XCColor XCColor::darkGray(0.662745*255.0f, 0.662745*255.0f, 0.662745*255.0f);
const XCColor XCColor::darkGrey(0.662745*255.0f, 0.662745*255.0f, 0.662745*255.0f);
const XCColor XCColor::darkGreen(0 * 255.0f, 0.392157*255.0f, 0 * 255.0f);
const XCColor XCColor::darkKhaki(0.741176*255.0f, 0.717647*255.0f, 0.419608*255.0f);
const XCColor XCColor::darkMagenta(0.545098*255.0f, 0 * 255.0f, 0.545098*255.0f);
const XCColor XCColor::darkOliveGreen(0.333333*255.0f, 0.419608*255.0f, 0.184314*255.0f);
const XCColor XCColor::darkorange(1 * 255.0f, 0.54902*255.0f, 0 * 255.0f);
const XCColor XCColor::darkOrchid(0.6*255.0f, 0.196078*255.0f, 0.8*255.0f);
const XCColor XCColor::darkRed(0.545098*255.0f, 0 * 255.0f, 0 * 255.0f);
const XCColor XCColor::darkSalmon(0.913725*255.0f, 0.588235*255.0f, 0.478431*255.0f);
const XCColor XCColor::darkSeaGreen(0.560784*255.0f, 0.737255*255.0f, 0.560784*255.0f);
const XCColor XCColor::darkSlateBlue(0.282353*255.0f, 0.239216*255.0f, 0.545098*255.0f);
const XCColor XCColor::darkSlateGray(0.184314*255.0f, 0.309804*255.0f, 0.309804*255.0f);
const XCColor XCColor::darkSlateGrey(0.184314*255.0f, 0.309804*255.0f, 0.309804*255.0f);
const XCColor XCColor::darkTurquoise(0 * 255.0f, 0.807843*255.0f, 0.819608*255.0f);
const XCColor XCColor::darkViolet(0.580392*255.0f, 0 * 255.0f, 0.827451*255.0f);
const XCColor XCColor::deepPink(1 * 255.0f, 0.0784314*255.0f, 0.576471*255.0f);
const XCColor XCColor::deepSkyBlue(0 * 255.0f, 0.74902*255.0f, 1 * 255.0f);
const XCColor XCColor::dimGray(0.411765*255.0f, 0.411765*255.0f, 0.411765*255.0f);
const XCColor XCColor::dimGrey(0.411765*255.0f, 0.411765*255.0f, 0.411765*255.0f);
const XCColor XCColor::dodgerBlue(0.117647*255.0f, 0.564706*255.0f, 1 * 255.0f);
const XCColor XCColor::fireBrick(0.698039*255.0f, 0.133333*255.0f, 0.133333*255.0f);
const XCColor XCColor::floralWhite(1 * 255.0f, 0.980392*255.0f, 0.941176*255.0f);
const XCColor XCColor::forestGreen(0.133333*255.0f, 0.545098*255.0f, 0.133333*255.0f);
const XCColor XCColor::fuchsia(1 * 255.0f, 0 * 255.0f, 1 * 255.0f);
const XCColor XCColor::gainsboro(0.862745*255.0f, 0.862745*255.0f, 0.862745*255.0f);
const XCColor XCColor::ghostWhite(0.972549*255.0f, 0.972549*255.0f, 1 * 255.0f);
const XCColor XCColor::gold(1 * 255.0f, 0.843137*255.0f, 0 * 255.0f);
const XCColor XCColor::goldenRod(0.854902*255.0f, 0.647059*255.0f, 0.12549*255.0f);
const XCColor XCColor::grey(0.501961*255.0f, 0.501961*255.0f, 0.501961*255.0f);
const XCColor XCColor::greenYellow(0.678431*255.0f, 1 * 255.0f, 0.184314*255.0f);
const XCColor XCColor::honeyDew(0.941176*255.0f, 1 * 255.0f, 0.941176*255.0f);
const XCColor XCColor::hotPink(1 * 255.0f, 0.411765*255.0f, 0.705882*255.0f);
const XCColor XCColor::indianRed(0.803922*255.0f, 0.360784*255.0f, 0.360784*255.0f);
const XCColor XCColor::indigo(0.294118*255.0f, 0 * 255.0f, 0.509804*255.0f);
const XCColor XCColor::ivory(1 * 255.0f, 1 * 255.0f, 0.941176*255.0f);
const XCColor XCColor::khaki(0.941176*255.0f, 0.901961*255.0f, 0.54902*255.0f);
const XCColor XCColor::lavender(0.901961*255.0f, 0.901961*255.0f, 0.980392*255.0f);
const XCColor XCColor::lavenderBlush(1 * 255.0f, 0.941176*255.0f, 0.960784*255.0f);
const XCColor XCColor::lawnGreen(0.486275*255.0f, 0.988235*255.0f, 0 * 255.0f);
const XCColor XCColor::lemonChiffon(1 * 255.0f, 0.980392*255.0f, 0.803922*255.0f);
const XCColor XCColor::lightBlue(0.678431*255.0f, 0.847059*255.0f, 0.901961*255.0f);
const XCColor XCColor::lightCoral(0.941176*255.0f, 0.501961*255.0f, 0.501961*255.0f);
const XCColor XCColor::lightCyan(0.878431*255.0f, 1 * 255.0f, 1 * 255.0f);
const XCColor XCColor::lightGoldenRodYellow(0.980392*255.0f, 0.980392*255.0f, 0.823529*255.0f);
const XCColor XCColor::lightGray(0.827451*255.0f, 0.827451*255.0f, 0.827451*255.0f);
const XCColor XCColor::lightGrey(0.827451*255.0f, 0.827451*255.0f, 0.827451*255.0f);
const XCColor XCColor::lightGreen(0.564706*255.0f, 0.933333*255.0f, 0.564706*255.0f);
const XCColor XCColor::lightPink(1 * 255.0f, 0.713726*255.0f, 0.756863*255.0f);
const XCColor XCColor::lightSalmon(1 * 255.0f, 0.627451*255.0f, 0.478431*255.0f);
const XCColor XCColor::lightSeaGreen(0.12549*255.0f, 0.698039*255.0f, 0.666667*255.0f);
const XCColor XCColor::lightSkyBlue(0.529412*255.0f, 0.807843*255.0f, 0.980392*255.0f);
const XCColor XCColor::lightSlateGray(0.466667*255.0f, 0.533333*255.0f, 0.6*255.0f);
const XCColor XCColor::lightSlateGrey(0.466667*255.0f, 0.533333*255.0f, 0.6*255.0f);
const XCColor XCColor::lightSteelBlue(0.690196*255.0f, 0.768627*255.0f, 0.870588*255.0f);
const XCColor XCColor::lightYellow(1 * 255.0f, 1 * 255.0f, 0.878431*255.0f);
const XCColor XCColor::lime(0 * 255.0f, 1 * 255.0f, 0 * 255.0f);
const XCColor XCColor::limeGreen(0.196078*255.0f, 0.803922*255.0f, 0.196078*255.0f);
const XCColor XCColor::linen(0.980392*255.0f, 0.941176*255.0f, 0.901961*255.0f);
const XCColor XCColor::maroon(0.501961*255.0f, 0 * 255.0f, 0 * 255.0f);
const XCColor XCColor::mediumAquaMarine(0.4*255.0f, 0.803922*255.0f, 0.666667*255.0f);
const XCColor XCColor::mediumBlue(0 * 255.0f, 0 * 255.0f, 0.803922*255.0f);
const XCColor XCColor::mediumOrchid(0.729412*255.0f, 0.333333*255.0f, 0.827451*255.0f);
const XCColor XCColor::mediumPurple(0.576471*255.0f, 0.439216*255.0f, 0.858824*255.0f);
const XCColor XCColor::mediumSeaGreen(0.235294*255.0f, 0.701961*255.0f, 0.443137*255.0f);
const XCColor XCColor::mediumSlateBlue(0.482353*255.0f, 0.407843*255.0f, 0.933333*255.0f);
const XCColor XCColor::mediumSpringGreen(0 * 255.0f, 0.980392*255.0f, 0.603922*255.0f);
const XCColor XCColor::mediumTurquoise(0.282353*255.0f, 0.819608*255.0f, 0.8*255.0f);
const XCColor XCColor::mediumVioletRed(0.780392*255.0f, 0.0823529*255.0f, 0.521569*255.0f);
const XCColor XCColor::midnightBlue(0.0980392*255.0f, 0.0980392*255.0f, 0.439216*255.0f);
const XCColor XCColor::mintCream(0.960784*255.0f, 1 * 255.0f, 0.980392*255.0f);
const XCColor XCColor::mistyRose(1 * 255.0f, 0.894118*255.0f, 0.882353*255.0f);
const XCColor XCColor::moccasin(1 * 255.0f, 0.894118*255.0f, 0.709804*255.0f);
const XCColor XCColor::navajoWhite(1 * 255.0f, 0.870588*255.0f, 0.678431*255.0f);
const XCColor XCColor::navy(0 * 255.0f, 0 * 255.0f, 0.501961*255.0f);
const XCColor XCColor::oldLace(0.992157*255.0f, 0.960784*255.0f, 0.901961*255.0f);
const XCColor XCColor::olive(0.501961*255.0f, 0.501961*255.0f, 0 * 255.0f);
const XCColor XCColor::oliveDrab(0.419608*255.0f, 0.556863*255.0f, 0.137255*255.0f);
const XCColor XCColor::orange(1 * 255.0f, 0.647059*255.0f, 0 * 255.0f);
const XCColor XCColor::orangeRed(1 * 255.0f, 0.270588*255.0f, 0 * 255.0f);
const XCColor XCColor::orchid(0.854902*255.0f, 0.439216*255.0f, 0.839216*255.0f);
const XCColor XCColor::paleGoldenRod(0.933333*255.0f, 0.909804*255.0f, 0.666667*255.0f);
const XCColor XCColor::paleGreen(0.596078*255.0f, 0.984314*255.0f, 0.596078*255.0f);
const XCColor XCColor::paleTurquoise(0.686275*255.0f, 0.933333*255.0f, 0.933333*255.0f);
const XCColor XCColor::paleVioletRed(0.858824*255.0f, 0.439216*255.0f, 0.576471*255.0f);
const XCColor XCColor::papayaWhip(1 * 255.0f, 0.937255*255.0f, 0.835294*255.0f);
const XCColor XCColor::peachPuff(1 * 255.0f, 0.854902*255.0f, 0.72549*255.0f);
const XCColor XCColor::peru(0.803922*255.0f, 0.521569*255.0f, 0.247059*255.0f);
const XCColor XCColor::pink(1 * 255.0f, 0.752941*255.0f, 0.796078*255.0f);
const XCColor XCColor::plum(0.866667*255.0f, 0.627451*255.0f, 0.866667*255.0f);
const XCColor XCColor::powderBlue(0.690196*255.0f, 0.878431*255.0f, 0.901961*255.0f);
const XCColor XCColor::purple(0.501961*255.0f, 0 * 255.0f, 0.501961*255.0f);
const XCColor XCColor::rosyBrown(0.737255*255.0f, 0.560784*255.0f, 0.560784*255.0f);
const XCColor XCColor::royalBlue(0.254902*255.0f, 0.411765*255.0f, 0.882353*255.0f);
const XCColor XCColor::saddleBrown(0.545098*255.0f, 0.270588*255.0f, 0.0745098*255.0f);
const XCColor XCColor::salmon(0.980392*255.0f, 0.501961*255.0f, 0.447059*255.0f);
const XCColor XCColor::sandyBrown(0.956863*255.0f, 0.643137*255.0f, 0.376471*255.0f);
const XCColor XCColor::seaGreen(0.180392*255.0f, 0.545098*255.0f, 0.341176*255.0f);
const XCColor XCColor::seaShell(1 * 255.0f, 0.960784*255.0f, 0.933333*255.0f);
const XCColor XCColor::sienna(0.627451*255.0f, 0.321569*255.0f, 0.176471*255.0f);
const XCColor XCColor::silver(0.752941*255.0f, 0.752941*255.0f, 0.752941*255.0f);
const XCColor XCColor::skyBlue(0.529412*255.0f, 0.807843*255.0f, 0.921569*255.0f);
const XCColor XCColor::slateBlue(0.415686*255.0f, 0.352941*255.0f, 0.803922*255.0f);
const XCColor XCColor::slateGray(0.439216*255.0f, 0.501961*255.0f, 0.564706*255.0f);
const XCColor XCColor::slateGrey(0.439216*255.0f, 0.501961*255.0f, 0.564706*255.0f);
const XCColor XCColor::snow(1 * 255.0f, 0.980392*255.0f, 0.980392*255.0f);
const XCColor XCColor::springGreen(0 * 255.0f, 1 * 255.0f, 0.498039*255.0f);
const XCColor XCColor::steelBlue(0.27451*255.0f, 0.509804*255.0f, 0.705882*255.0f);
const XCColor XCColor::tan(0.823529*255.0f, 0.705882*255.0f, 0.54902*255.0f);
const XCColor XCColor::teal(0 * 255.0f, 0.501961*255.0f, 0.501961*255.0f);
const XCColor XCColor::thistle(0.847059*255.0f, 0.74902*255.0f, 0.847059*255.0f);
const XCColor XCColor::tomato(1 * 255.0f, 0.388235*255.0f, 0.278431*255.0f);
const XCColor XCColor::turquoise(0.25098*255.0f, 0.878431*255.0f, 0.815686*255.0f);
const XCColor XCColor::violet(0.933333*255.0f, 0.509804*255.0f, 0.933333*255.0f);
const XCColor XCColor::wheat(0.960784*255.0f, 0.870588*255.0f, 0.701961*255.0f);
const XCColor XCColor::whiteSmoke(0.960784*255.0f, 0.960784*255.0f, 0.960784*255.0f);
const XCColor XCColor::yellowGreen(0.603922*255.0f, 0.803922*255.0f, 0.196078*255.0f);

#if !WITH_INLINE_FILE
#include "XCColor.inl"
#endif
}