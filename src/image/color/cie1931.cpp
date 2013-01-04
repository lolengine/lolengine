//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

namespace lol
{

/* CIE 1931 Standard Colorimetric Observer, aka. 2º Observer
 * for wavelengths 360 to 830. */
static float cie_1931_xyz[] =
{
    0.0001299f, 3.917e-06f, 0.0006061f, /* 360 nm */
    0.0001458f, 4.394e-06f, 0.0006809f,
    0.0001638f, 4.93e-06f, 0.0007651f,
    0.000184f, 5.532e-06f, 0.00086f,
    0.0002067f, 6.208e-06f, 0.0009666f,
    0.0002321f, 6.965e-06f, 0.001086f,
    0.0002607f, 7.813e-06f, 0.0012206f,
    0.0002931f, 8.767e-06f, 0.0013727f,
    0.0003294f, 9.84e-06f, 0.0015436f,
    0.0003699f, 1.104e-05f, 0.0017343f,
    0.0004149f, 1.239e-05f, 0.001946f,
    0.0004642f, 1.389e-05f, 0.0021778f,
    0.000519f, 1.556e-05f, 0.0024358f,
    0.0005819f, 1.744e-05f, 0.002732f,
    0.0006552f, 1.958e-05f, 0.0030781f,
    0.0007416f, 2.202e-05f, 0.003486f,
    0.000845f, 2.484e-05f, 0.0039752f,
    0.0009645f, 2.804e-05f, 0.0045409f,
    0.0010949f, 3.153e-05f, 0.0051583f,
    0.0012312f, 3.522e-05f, 0.0058029f,
    0.001368f, 0.000039f, 0.00645f,
    0.0015021f, 4.283e-05f, 0.0070832f,
    0.0016423f, 4.691e-05f, 0.0077455f,
    0.0018024f, 5.159e-05f, 0.0085012f,
    0.0019958f, 5.718e-05f, 0.0094145f,
    0.002236f, 0.000064f, 0.01055f,
    0.0025354f, 7.234e-05f, 0.0119658f,
    0.0028926f, 8.221e-05f, 0.0136559f,
    0.0033008f, 9.351e-05f, 0.0155881f,
    0.0037532f, 0.0001061f, 0.0177302f,
    0.004243f, 0.00012f, 0.02005f,
    0.0047624f, 0.000135f, 0.0225114f,
    0.00533f, 0.0001515f, 0.0252029f,
    0.0059787f, 0.0001702f, 0.0282797f,
    0.0067411f, 0.0001918f, 0.031897f,
    0.00765f, 0.000217f, 0.03621f,
    0.0087514f, 0.0002469f, 0.0414377f,
    0.0100289f, 0.0002812f, 0.0475037f,
    0.0114217f, 0.0003185f, 0.0541199f,
    0.012869f, 0.0003573f, 0.060998f,
    0.01431f, 0.000396f, 0.06785f, /* 400 nm */
    0.0157044f, 0.0004337f, 0.0744863f,
    0.0171474f, 0.000473f, 0.0813616f,
    0.0187812f, 0.0005179f, 0.0891536f,
    0.020748f, 0.0005722f, 0.0985405f,
    0.02319f, 0.00064f, 0.1102f,
    0.0262074f, 0.0007246f, 0.1246133f,
    0.0297825f, 0.0008255f, 0.1417017f,
    0.0338809f, 0.0009412f, 0.1613035f,
    0.0384682f, 0.0010699f, 0.1832568f,
    0.04351f, 0.00121f, 0.2074f,
    0.0489956f, 0.0013621f, 0.2336921f,
    0.0550226f, 0.0015308f, 0.2626114f,
    0.0617188f, 0.0017204f, 0.2947746f,
    0.069212f, 0.0019353f, 0.3307985f,
    0.07763f, 0.00218f, 0.3713f,
    0.0869581f, 0.0024548f, 0.4162091f,
    0.0971767f, 0.002764f, 0.4654642f,
    0.1084063f, 0.0031178f, 0.5196948f,
    0.1207672f, 0.0035264f, 0.5795303f,
    0.13438f, 0.004f, 0.6456f,
    0.1493582f, 0.0045462f, 0.7184838f,
    0.1653957f, 0.0051593f, 0.7967133f,
    0.1819831f, 0.0058293f, 0.8778459f,
    0.198611f, 0.0065462f, 0.959439f,
    0.21477f, 0.0073f, 1.0390501f,
    0.2301868f, 0.0080865f, 1.1153673f,
    0.2448797f, 0.0089087f, 1.1884971f,
    0.2587773f, 0.0097677f, 1.2581233f,
    0.2718079f, 0.0106644f, 1.3239296f,
    0.2839f, 0.0116f, 1.3856f,
    0.2949438f, 0.0125732f, 1.4426352f,
    0.3048965f, 0.0135827f, 1.4948035f,
    0.3137873f, 0.0146297f, 1.5421903f,
    0.3216454f, 0.0157151f, 1.5848807f,
    0.3285f, 0.01684f, 1.62296f,
    0.3343513f, 0.0180074f, 1.6564048f,
    0.3392101f, 0.0192145f, 1.6852959f,
    0.3431213f, 0.0204539f, 1.7098745f,
    0.3461296f, 0.0217182f, 1.7303821f,
    0.34828f, 0.023f, 1.74706f, /* 440 nm */
    0.3495999f, 0.0242946f, 1.7600446f,
    0.3501474f, 0.0256102f, 1.7696233f,
    0.350013f, 0.0269586f, 1.7762637f,
    0.349287f, 0.0283513f, 1.7804334f,
    0.34806f, 0.0298f, 1.7826f,
    0.3463733f, 0.0313108f, 1.7829682f,
    0.3442624f, 0.0328837f, 1.7816998f,
    0.3418088f, 0.0345211f, 1.7791982f,
    0.3390941f, 0.0362257f, 1.7758671f,
    0.3362f, 0.038f, 1.77211f,
    0.3331977f, 0.0398467f, 1.7682589f,
    0.3300411f, 0.041768f, 1.764039f,
    0.3266357f, 0.043766f, 1.7589438f,
    0.3228868f, 0.0458427f, 1.7524663f,
    0.3187f, 0.048f, 1.7441f,
    0.3140251f, 0.0502437f, 1.7335595f,
    0.308884f, 0.052573f, 1.7208581f,
    0.3032904f, 0.0549806f, 1.7059369f,
    0.2972579f, 0.0574587f, 1.6887372f,
    0.2908f, 0.06f, 1.6692f,
    0.2839701f, 0.062602f, 1.6475287f,
    0.2767214f, 0.0652775f, 1.6234127f,
    0.2689178f, 0.0680421f, 1.5960223f,
    0.2604227f, 0.0709111f, 1.564528f,
    0.2511f, 0.0739f, 1.5281f,
    0.2408475f, 0.077016f, 1.4861114f,
    0.2298512f, 0.0802664f, 1.4395215f,
    0.2184072f, 0.0836668f, 1.3898799f,
    0.2068115f, 0.0872328f, 1.3387362f,
    0.19536f, 0.09098f, 1.28764f,
    0.1842136f, 0.0949176f, 1.2374223f,
    0.1733273f, 0.0990458f, 1.1878243f,
    0.1626881f, 0.1033674f, 1.1387611f,
    0.1522833f, 0.1078846f, 1.090148f,
    0.1421f, 0.1126f, 1.0419f,
    0.1321786f, 0.117532f, 0.9941976f,
    0.1225696f, 0.1226744f, 0.9473473f,
    0.1132752f, 0.1279928f, 0.9014531f,
    0.1042979f, 0.1334528f, 0.8566193f,
    0.09564f, 0.13902f, 0.8129501f, /* 480 nm */
    0.0872996f, 0.1446764f, 0.7705173f,
    0.079308f, 0.1504693f, 0.7294448f,
    0.0717178f, 0.1564619f, 0.6899136f,
    0.064581f, 0.1627177f, 0.6521049f,
    0.05795f, 0.1693f, 0.6162f,
    0.0518621f, 0.1762431f, 0.5823286f,
    0.0462815f, 0.1835581f, 0.5504162f,
    0.0411509f, 0.1912735f, 0.5203376f,
    0.0364128f, 0.199418f, 0.4919673f,
    0.03201f, 0.20802f, 0.46518f,
    0.0279172f, 0.2171199f, 0.4399246f,
    0.0241444f, 0.2267345f, 0.4161836f,
    0.020687f, 0.2368571f, 0.3938822f,
    0.0175404f, 0.2474812f, 0.3729459f,
    0.0147f, 0.2586f, 0.3533f,
    0.0121618f, 0.2701849f, 0.3348578f,
    0.00992f, 0.2822939f, 0.3175521f,
    0.0079672f, 0.2950505f, 0.3013375f,
    0.0062963f, 0.308578f, 0.2861686f,
    0.0049f, 0.323f, 0.272f,
    0.0037772f, 0.3384021f, 0.2588171f,
    0.0029453f, 0.3546858f, 0.2464838f,
    0.0024249f, 0.3716986f, 0.2347718f,
    0.0022363f, 0.3892875f, 0.2234533f,
    0.0024f, 0.4073f, 0.2123f,
    0.0029255f, 0.4256299f, 0.2011692f,
    0.0038366f, 0.4443096f, 0.1901196f,
    0.0051748f, 0.4633944f, 0.1792254f,
    0.0069821f, 0.4829395f, 0.1685608f,
    0.0093f, 0.503f, 0.1582f,
    0.0121495f, 0.5235693f, 0.1481383f,
    0.0155359f, 0.544512f, 0.1383758f,
    0.0194775f, 0.56569f, 0.1289942f,
    0.0239928f, 0.5869653f, 0.1200751f,
    0.0291f, 0.6082f, 0.1117f,
    0.0348149f, 0.6293456f, 0.1039048f,
    0.0411202f, 0.6503068f, 0.0966675f,
    0.047985f, 0.6708752f, 0.0899827f,
    0.0553786f, 0.6908424f, 0.0838453f,
    0.06327f, 0.71f, 0.07825f, /* 520 nm */
    0.071635f, 0.7281852f, 0.073209f,
    0.0804622f, 0.7454636f, 0.0686782f,
    0.08974f, 0.7619694f, 0.0645678f,
    0.0994565f, 0.7778368f, 0.0607884f,
    0.1096f, 0.7932f, 0.05725f,
    0.1201674f, 0.8081104f, 0.0539044f,
    0.1311145f, 0.8224962f, 0.0507466f,
    0.1423679f, 0.8363068f, 0.0477528f,
    0.1538542f, 0.8494916f, 0.0448986f,
    0.1655f, 0.862f, 0.04216f,
    0.1772571f, 0.8738108f, 0.0395073f,
    0.18914f, 0.8849624f, 0.0369356f,
    0.2011694f, 0.8954936f, 0.0344584f,
    0.2133658f, 0.9054432f, 0.0320887f,
    0.2257499f, 0.9148501f, 0.02984f,
    0.2383209f, 0.9237348f, 0.0277118f,
    0.2510668f, 0.9320924f, 0.0256944f,
    0.2639922f, 0.9399226f, 0.0237872f,
    0.2771017f, 0.9472252f, 0.0219893f,
    0.2904f, 0.954f, 0.0203f,
    0.3038912f, 0.9602561f, 0.0187181f,
    0.3175726f, 0.9660074f, 0.0172404f,
    0.3314384f, 0.9712606f, 0.0158636f,
    0.3454828f, 0.9760225f, 0.0145846f,
    0.3597f, 0.9803f, 0.0134f,
    0.3740839f, 0.9840924f, 0.0123072f,
    0.3886396f, 0.9874182f, 0.0113019f,
    0.4033784f, 0.9903128f, 0.0103779f,
    0.4183115f, 0.9928116f, 0.0095293f,
    0.4334499f, 0.9949501f, 0.00875f,
    0.4487953f, 0.9967108f, 0.0080352f,
    0.464336f, 0.9980983f, 0.0073816f,
    0.480064f, 0.999112f, 0.0067854f,
    0.4959713f, 0.9997482f, 0.0062428f,
    0.5120501f, 1.f, 0.00575f,
    0.5282959f, 0.9998567f, 0.0053036f,
    0.5446916f, 0.9993046f, 0.0048998f,
    0.5612094f, 0.9983255f, 0.0045342f,
    0.5778215f, 0.9968987f, 0.0042024f,
    0.5945f, 0.995f, 0.0039f, /* 560 nm */
    0.6112209f, 0.9926005f, 0.0036232f,
    0.6279758f, 0.9897426f, 0.0033706f,
    0.6447602f, 0.9864444f, 0.0031414f,
    0.6615697f, 0.9827241f, 0.0029348f,
    0.6784f, 0.9786f, 0.00275f,
    0.6952392f, 0.9740837f, 0.0025852f,
    0.7120586f, 0.9691712f, 0.0024386f,
    0.7288284f, 0.9638568f, 0.0023094f,
    0.7455188f, 0.9581349f, 0.0021968f,
    0.7621f, 0.952f, 0.0021f,
    0.7785432f, 0.9454504f, 0.0020177f,
    0.7948256f, 0.9384992f, 0.0019482f,
    0.8109264f, 0.9311628f, 0.0018898f,
    0.8268248f, 0.9234576f, 0.0018409f,
    0.8425f, 0.9154f, 0.0018f,
    0.8579325f, 0.9070064f, 0.0017663f,
    0.8730816f, 0.8982772f, 0.0017378f,
    0.8878944f, 0.8892048f, 0.0017112f,
    0.9023181f, 0.8797816f, 0.0016831f,
    0.9163f, 0.87f, 0.00165f,
    0.9297995f, 0.8598613f, 0.0016101f,
    0.9427984f, 0.849392f, 0.0015644f,
    0.9552776f, 0.838622f, 0.0015136f,
    0.9672179f, 0.8275813f, 0.0014585f,
    0.9786f, 0.8163f, 0.0014f,
    0.9893856f, 0.8047947f, 0.0013367f,
    0.9995488f, 0.793082f, 0.00127f,
    1.0090892f, 0.781192f, 0.001205f,
    1.0180064f, 0.7691547f, 0.0011467f,
    1.0263f, 0.757f, 0.0011f,
    1.0339827f, 0.7447541f, 0.0010688f,
    1.040986f, 0.7324224f, 0.0010494f,
    1.047188f, 0.7200036f, 0.0010356f,
    1.0524667f, 0.7074965f, 0.0010212f,
    1.0567f, 0.6949f, 0.001f,
    1.0597944f, 0.6822192f, 0.0009686f,
    1.0617992f, 0.6694716f, 0.0009299f,
    1.0628068f, 0.6566744f, 0.0008869f,
    1.0629096f, 0.6438448f, 0.0008426f,
    1.0622f, 0.631f, 0.0008f, /* 600 nm */
    1.0607352f, 0.6181555f, 0.000761f,
    1.0584436f, 0.6053144f, 0.0007237f,
    1.0552244f, 0.5924756f, 0.0006859f,
    1.0509768f, 0.5796379f, 0.0006454f,
    1.0456f, 0.5668f, 0.0006f,
    1.0390369f, 0.5539611f, 0.0005479f,
    1.0313608f, 0.5411372f, 0.0004916f,
    1.0226662f, 0.5283528f, 0.0004354f,
    1.0130477f, 0.5156323f, 0.0003835f,
    1.0026f, 0.503f, 0.00034f,
    0.9913675f, 0.4904688f, 0.0003073f,
    0.9793314f, 0.4780304f, 0.0002832f,
    0.9664916f, 0.4656776f, 0.0002654f,
    0.9528479f, 0.4534032f, 0.0002518f,
    0.9384f, 0.4412f, 0.00024f,
    0.923194f, 0.42908f, 0.0002295f,
    0.907244f, 0.417036f, 0.0002206f,
    0.890502f, 0.405032f, 0.000212f,
    0.87292f, 0.393032f, 0.0002022f,
    0.8544499f, 0.381f, 0.00019f,
    0.835084f, 0.3689184f, 0.0001742f,
    0.814946f, 0.3568272f, 0.0001556f,
    0.794186f, 0.3447768f, 0.000136f,
    0.772954f, 0.3328176f, 0.0001169f,
    0.7514f, 0.321f, 0.0001f,
    0.7295836f, 0.3093381f, 8.613e-05f,
    0.7075888f, 0.2978504f, 0.0000746f,
    0.6856022f, 0.2865936f, 0.000065f,
    0.6638104f, 0.2756245f, 5.693e-05f,
    0.6424f, 0.265f, 5e-05f,
    0.6215149f, 0.2547632f, 4.416e-05f,
    0.6011138f, 0.2448896f, 3.948e-05f,
    0.5811052f, 0.2353344f, 3.572e-05f,
    0.5613977f, 0.2260528f, 3.264e-05f,
    0.5419f, 0.217f, 0.00003f,
    0.5225995f, 0.2081616f, 2.765e-05f,
    0.5035464f, 0.1995488f, 2.556e-05f,
    0.4847436f, 0.1911552f, 2.364e-05f,
    0.4661939f, 0.1829744f, 2.181e-05f,
    0.4479f, 0.175f, 0.00002f, /* 640 nm */
    0.4298613f, 0.1672235f, 1.813e-05f,
    0.412098f, 0.1596464f, 0.0000162f,
    0.394644f, 0.1522776f, 0.0000142f,
    0.3775333f, 0.1451259f, 1.213e-05f,
    0.3608f, 0.1382f, 0.00001f,
    0.3444563f, 0.1315003f, 7.733e-06f,
    0.3285168f, 0.1250248f, 0.0000054f,
    0.3130192f, 0.1187792f, 0.0000032f,
    0.2980011f, 0.1127691f, 1.333e-06f,
    0.2835f, 0.107f, 0.0f,
    0.2695448f, 0.1014762f, 0.0f,
    0.2561184f, 0.0961886f, 0.0f,
    0.2431896f, 0.091123f, 0.0f,
    0.2307272f, 0.0862649f, 0.0f,
    0.2187f, 0.0816f, 0.0f,
    0.2070971f, 0.0771206f, 0.0f,
    0.1959232f, 0.0728255f, 0.0f,
    0.1851708f, 0.0687101f, 0.0f,
    0.1748323f, 0.0647698f, 0.0f,
    0.1649f, 0.061f, 0.0f,
    0.1553667f, 0.0573962f, 0.0f,
    0.14623f, 0.053955f, 0.0f,
    0.13749f, 0.0506738f, 0.0f,
    0.1291467f, 0.0475497f, 0.0f,
    0.1212f, 0.04458f, 0.0f,
    0.1136397f, 0.0417587f, 0.0f,
    0.106465f, 0.039085f, 0.0f,
    0.0996904f, 0.0365638f, 0.0f,
    0.0933306f, 0.0342005f, 0.0f,
    0.0874f, 0.032f, 0.0f,
    0.081901f, 0.0299626f, 0.0f,
    0.0768043f, 0.0280766f, 0.0f,
    0.0720771f, 0.0263294f, 0.0f,
    0.0676866f, 0.0247081f, 0.0f,
    0.0636f, 0.0232f, 0.0f,
    0.0598069f, 0.0218008f, 0.0f,
    0.0562822f, 0.0205011f, 0.0f,
    0.052971f, 0.0192811f, 0.0f,
    0.0498186f, 0.0181207f, 0.0f,
    0.04677f, 0.017f, 0.0f, /* 680 nm */
    0.0437841f, 0.0159038f, 0.0f,
    0.0408754f, 0.0148372f, 0.0f,
    0.0380726f, 0.0138107f, 0.0f,
    0.0354046f, 0.0128348f, 0.0f,
    0.0329f, 0.01192f, 0.0f,
    0.0305642f, 0.0110683f, 0.0f,
    0.0283806f, 0.0102734f, 0.0f,
    0.0263448f, 0.0095333f, 0.0f,
    0.0244528f, 0.0088462f, 0.0f,
    0.0227f, 0.00821f, 0.0f,
    0.0210843f, 0.0076238f, 0.0f,
    0.0195999f, 0.0070854f, 0.0f,
    0.0182373f, 0.0065915f, 0.0f,
    0.0169872f, 0.0061385f, 0.0f,
    0.01584f, 0.005723f, 0.0f,
    0.0147906f, 0.0053431f, 0.0f,
    0.0138313f, 0.0049958f, 0.0f,
    0.0129487f, 0.0046764f, 0.0f,
    0.0121292f, 0.0043801f, 0.0f,
    0.0113592f, 0.004102f, 0.0f,
    0.0106294f, 0.0038385f, 0.0f,
    0.0099388f, 0.0035891f, 0.0f,
    0.0092884f, 0.0033542f, 0.0f,
    0.0086789f, 0.0031341f, 0.0f,
    0.0081109f, 0.002929f, 0.0f,
    0.0075824f, 0.0027381f, 0.0f,
    0.0070887f, 0.0025599f, 0.0f,
    0.0066273f, 0.0023932f, 0.0f,
    0.0061954f, 0.0022373f, 0.0f,
    0.0057903f, 0.002091f, 0.0f,
    0.0054098f, 0.0019536f, 0.0f,
    0.0050526f, 0.0018246f, 0.0f,
    0.0047175f, 0.0017036f, 0.0f,
    0.0044035f, 0.0015902f, 0.0f,
    0.0041095f, 0.001484f, 0.0f,
    0.0038339f, 0.0013845f, 0.0f,
    0.0035757f, 0.0012913f, 0.0f,
    0.0033343f, 0.0012041f, 0.0f,
    0.0031091f, 0.0011227f, 0.0f,
    0.0028993f, 0.001047f, 0.0f, /* 720 nm */
    0.0027043f, 0.0009766f, 0.0f,
    0.002523f, 0.0009111f, 0.0f,
    0.0023542f, 0.0008501f, 0.0f,
    0.0021966f, 0.0007932f, 0.0f,
    0.0020492f, 0.00074f, 0.0f,
    0.001911f, 0.0006901f, 0.0f,
    0.0017814f, 0.0006433f, 0.0f,
    0.0016601f, 0.0005995f, 0.0f,
    0.0015465f, 0.0005585f, 0.0f,
    0.00144f, 0.00052f, 0.0f,
    0.00134f, 0.0004839f, 0.0f,
    0.0012463f, 0.0004501f, 0.0f,
    0.0011585f, 0.0004183f, 0.0f,
    0.0010764f, 0.0003887f, 0.0f,
    0.0009999f, 0.0003611f, 0.0f,
    0.0009287f, 0.0003354f, 0.0f,
    0.0008624f, 0.0003114f, 0.0f,
    0.0008008f, 0.0002892f, 0.0f,
    0.0007434f, 0.0002685f, 0.0f,
    0.0006901f, 0.0002492f, 0.0f,
    0.0006405f, 0.0002313f, 0.0f,
    0.0005945f, 0.0002147f, 0.0f,
    0.0005519f, 0.0001993f, 0.0f,
    0.0005124f, 0.000185f, 0.0f,
    0.000476f, 0.0001719f, 0.0f,
    0.0004425f, 0.0001598f, 0.0f,
    0.0004115f, 0.0001486f, 0.0f,
    0.000383f, 0.0001383f, 0.0f,
    0.0003566f, 0.0001288f, 0.0f,
    0.0003323f, 0.00012f, 0.0f,
    0.0003098f, 0.0001119f, 0.0f,
    0.0002889f, 0.0001043f, 0.0f,
    0.0002695f, 9.734e-05f, 0.0f,
    0.0002516f, 9.085e-05f, 0.0f,
    0.0002348f, 0.0000848f, 0.0f,
    0.0002192f, 7.915e-05f, 0.0f,
    0.0002045f, 7.386e-05f, 0.0f,
    0.0001908f, 6.892e-05f, 0.0f,
    0.0001781f, 6.43e-05f, 0.0f,
    0.0001662f, 0.00006f, 0.0f, /* 760 nm */
    0.000155f, 5.598e-05f, 0.0f,
    0.0001446f, 5.223e-05f, 0.0f,
    0.0001349f, 4.872e-05f, 0.0f,
    0.0001259f, 4.545e-05f, 0.0f,
    0.0001174f, 0.0000424f, 0.0f,
    0.0001096f, 3.956e-05f, 0.0f,
    0.0001022f, 3.692e-05f, 0.0f,
    9.539e-05f, 3.445e-05f, 0.0f,
    8.902e-05f, 3.215e-05f, 0.0f,
    8.308e-05f, 0.00003f, 0.0f,
    7.751e-05f, 2.799e-05f, 0.0f,
    7.231e-05f, 2.611e-05f, 0.0f,
    6.746e-05f, 2.436e-05f, 0.0f,
    6.293e-05f, 2.272e-05f, 0.0f,
    5.871e-05f, 0.0000212f, 0.0f,
    5.477e-05f, 1.978e-05f, 0.0f,
    5.11e-05f, 1.845e-05f, 0.0f,
    4.768e-05f, 1.722e-05f, 0.0f,
    4.449e-05f, 1.606e-05f, 0.0f,
    4.151e-05f, 1.499e-05f, 0.0f,
    3.873e-05f, 1.399e-05f, 0.0f,
    3.614e-05f, 1.305e-05f, 0.0f,
    3.372e-05f, 1.218e-05f, 0.0f,
    3.146e-05f, 1.136e-05f, 0.0f,
    2.935e-05f, 0.0000106f, 0.0f,
    2.738e-05f, 9.886e-06f, 0.0f,
    2.552e-05f, 9.217e-06f, 0.0f,
    2.379e-05f, 8.592e-06f, 0.0f,
    2.218e-05f, 8.009e-06f, 0.0f,
    2.067e-05f, 7.466e-06f, 0.0f,
    1.927e-05f, 6.96e-06f, 0.0f,
    1.797e-05f, 6.488e-06f, 0.0f,
    1.675e-05f, 6.049e-06f, 0.0f,
    1.562e-05f, 5.639e-06f, 0.0f,
    1.456e-05f, 5.258e-06f, 0.0f,
    1.357e-05f, 4.902e-06f, 0.0f,
    1.265e-05f, 4.57e-06f, 0.0f,
    1.18e-05f, 4.26e-06f, 0.0f,
    1.1e-05f, 3.972e-06f, 0.0f,
    1.025e-05f, 3.703e-06f, 0.0f, /* 800 nm */
    9.56e-06f, 3.452e-06f, 0.0f,
    8.912e-06f, 3.218e-06f, 0.0f,
    8.308e-06f, 3e-06f, 0.0f,
    7.746e-06f, 2.797e-06f, 0.0f,
    7.221e-06f, 2.608e-06f, 0.0f,
    6.732e-06f, 2.431e-06f, 0.0f,
    6.276e-06f, 2.267e-06f, 0.0f,
    5.851e-06f, 2.113e-06f, 0.0f,
    5.455e-06f, 1.97e-06f, 0.0f,
    5.086e-06f, 1.837e-06f, 0.0f,
    4.741e-06f, 1.712e-06f, 0.0f,
    4.42e-06f, 1.596e-06f, 0.0f,
    4.121e-06f, 1.488e-06f, 0.0f,
    3.842e-06f, 1.387e-06f, 0.0f,
    3.582e-06f, 1.293e-06f, 0.0f,
    3.339e-06f, 1.206e-06f, 0.0f,
    3.113e-06f, 1.124e-06f, 0.0f,
    2.902e-06f, 1.048e-06f, 0.0f,
    2.706e-06f, 9.771e-07f, 0.0f,
    2.523e-06f, 9.109e-07f, 0.0f,
    2.352e-06f, 8.493e-07f, 0.0f,
    2.192e-06f, 7.917e-07f, 0.0f,
    2.044e-06f, 7.381e-07f, 0.0f,
    1.905e-06f, 6.881e-07f, 0.0f,
    1.777e-06f, 6.415e-07f, 0.0f,
    1.656e-06f, 5.981e-07f, 0.0f,
    1.544e-06f, 5.576e-07f, 0.0f,
    1.439e-06f, 5.198e-07f, 0.0f,
    1.342e-06f, 4.846e-07f, 0.0f,
    1.251e-06f, 4.518e-07f, 0.0f, /* 830 nm */
};

/* CIE 1964 Colorimetric Observer, aka. 10º Observer
 * for wavelengths 360 to 830. */
static float cie_1964_xyz[] =
{
    1.222e-07f, 1.34e-08f, 5.35e-07f, /* 360 nm */
    1.851e-07f, 2.029e-08f, 8.107e-07f,
    2.788e-07f, 3.056e-08f, 1.221e-06f,
    4.175e-07f, 4.574e-08f, 1.829e-06f,
    6.213e-07f, 6.805e-08f, 2.722e-06f,
    9.193e-07f, 1.007e-07f, 4.028e-06f,
    1.352e-06f, 1.48e-07f, 5.926e-06f,
    1.977e-06f, 2.163e-07f, 8.665e-06f,
    2.873e-06f, 3.142e-07f, 1.26e-05f,
    4.15e-06f, 4.537e-07f, 1.82e-05f,
    5.959e-06f, 6.511e-07f, 2.614e-05f,
    8.506e-06f, 9.288e-07f, 3.733e-05f,
    1.207e-05f, 1.318e-06f, 5.299e-05f,
    1.702e-05f, 1.857e-06f, 7.476e-05f,
    2.387e-05f, 2.602e-06f, 0.0001049f,
    3.327e-05f, 3.625e-06f, 0.0001462f,
    4.609e-05f, 5.019e-06f, 0.0002027f,
    6.347e-05f, 6.907e-06f, 0.0002792f,
    8.689e-05f, 9.449e-06f, 0.0003825f,
    0.0001182f, 1.285e-05f, 0.0005207f,
    0.00016f, 1.736e-05f, 0.0007048f,
    0.0002151f, 2.333e-05f, 0.0009482f,
    0.0002875f, 3.115e-05f, 0.0012682f,
    0.000382f, 4.135e-05f, 0.0016861f,
    0.0005046f, 5.456e-05f, 0.0022285f,
    0.0006624f, 7.156e-05f, 0.0029278f,
    0.0008645f, 0.0000933f, 0.0038237f,
    0.0011215f, 0.0001209f, 0.0049642f,
    0.0014462f, 0.0001556f, 0.0064067f,
    0.0018536f, 0.0001992f, 0.0082193f,
    0.0023616f, 0.0002534f, 0.0104822f,
    0.0029906f, 0.0003202f, 0.013289f,
    0.0037645f, 0.0004024f, 0.016747f,
    0.0047102f, 0.0005023f, 0.02098f,
    0.0058581f, 0.0006232f, 0.026127f,
    0.0072423f, 0.0007685f, 0.032344f,
    0.0088996f, 0.0009417f, 0.039802f,
    0.0108709f, 0.0011478f, 0.048691f,
    0.0131989f, 0.0013903f, 0.05921f,
    0.0159292f, 0.001674f, 0.071576f,
    0.0191097f, 0.0020044f, 0.0860109f, /* 400 nm */
    0.022788f, 0.002386f, 0.10274f,
    0.027011f, 0.002822f, 0.122f,
    0.031829f, 0.003319f, 0.14402f,
    0.037278f, 0.00388f, 0.16899f,
    0.0434f, 0.004509f, 0.19712f,
    0.050223f, 0.005209f, 0.22857f,
    0.057764f, 0.005985f, 0.26347f,
    0.066038f, 0.006833f, 0.3019f,
    0.075033f, 0.007757f, 0.34387f,
    0.084736f, 0.008756f, 0.389366f,
    0.095041f, 0.009816f, 0.43797f,
    0.105836f, 0.010918f, 0.48922f,
    0.117066f, 0.012058f, 0.5429f,
    0.128682f, 0.013237f, 0.59881f,
    0.140638f, 0.014456f, 0.65676f,
    0.152893f, 0.015717f, 0.71658f,
    0.165416f, 0.017025f, 0.77812f,
    0.178191f, 0.018399f, 0.84131f,
    0.191214f, 0.019848f, 0.90611f,
    0.204492f, 0.021391f, 0.972542f,
    0.21765f, 0.022992f, 1.0389f,
    0.230267f, 0.024598f, 1.1031f,
    0.242311f, 0.026213f, 1.1651f,
    0.253793f, 0.027841f, 1.2249f,
    0.264737f, 0.029497f, 1.2825f,
    0.275195f, 0.031195f, 1.3382f,
    0.285301f, 0.032927f, 1.3926f,
    0.295143f, 0.034738f, 1.4461f,
    0.304869f, 0.036654f, 1.4994f,
    0.314679f, 0.038676f, 1.55348f,
    0.324355f, 0.040792f, 1.6072f,
    0.33357f, 0.042946f, 1.6589f,
    0.342243f, 0.045114f, 1.7082f,
    0.350312f, 0.047333f, 1.7548f,
    0.357719f, 0.049602f, 1.7985f,
    0.364482f, 0.051934f, 1.8392f,
    0.370493f, 0.054337f, 1.8766f,
    0.375727f, 0.056822f, 1.9105f,
    0.380158f, 0.059399f, 1.9408f,
    0.383734f, 0.062077f, 1.96728f, /* 440 nm */
    0.386327f, 0.064737f, 1.9891f,
    0.387858f, 0.067285f, 2.0057f,
    0.388396f, 0.069764f, 2.0174f,
    0.387978f, 0.072218f, 2.0244f,
    0.386726f, 0.074704f, 2.0273f,
    0.384696f, 0.077272f, 2.0264f,
    0.382006f, 0.079979f, 2.0223f,
    0.378709f, 0.082874f, 2.0153f,
    0.374915f, 0.086f, 2.006f,
    0.370702f, 0.089456f, 1.9948f,
    0.366089f, 0.092947f, 1.9814f,
    0.361045f, 0.096275f, 1.9653f,
    0.355518f, 0.099535f, 1.9464f,
    0.349486f, 0.102829f, 1.9248f,
    0.342957f, 0.106256f, 1.9007f,
    0.335893f, 0.109901f, 1.8741f,
    0.328284f, 0.113835f, 1.8451f,
    0.32015f, 0.118167f, 1.8139f,
    0.311475f, 0.122932f, 1.7806f,
    0.302273f, 0.128201f, 1.74537f,
    0.292858f, 0.133457f, 1.7091f,
    0.283502f, 0.138323f, 1.6723f,
    0.274044f, 0.143042f, 1.6347f,
    0.264263f, 0.147787f, 1.5956f,
    0.254085f, 0.152761f, 1.5549f,
    0.243392f, 0.158102f, 1.5122f,
    0.232187f, 0.163941f, 1.4673f,
    0.220488f, 0.170362f, 1.4199f,
    0.208198f, 0.177425f, 1.37f,
    0.195618f, 0.18519f, 1.31756f,
    0.183034f, 0.193025f, 1.2624f,
    0.170222f, 0.200313f, 1.205f,
    0.157348f, 0.207156f, 1.1466f,
    0.14465f, 0.213644f, 1.088f,
    0.132349f, 0.21994f, 1.0302f,
    0.120584f, 0.22617f, 0.97383f,
    0.109456f, 0.232467f, 0.91943f,
    0.099042f, 0.239025f, 0.86746f,
    0.089388f, 0.245997f, 0.81828f,
    0.080507f, 0.253589f, 0.772125f, /* 480 nm */
    0.072034f, 0.261876f, 0.72829f,
    0.06371f, 0.270643f, 0.68604f,
    0.055694f, 0.279645f, 0.64553f,
    0.048117f, 0.288694f, 0.60685f,
    0.041072f, 0.297665f, 0.57006f,
    0.034642f, 0.306469f, 0.53522f,
    0.028896f, 0.315035f, 0.50234f,
    0.023876f, 0.323335f, 0.4714f,
    0.019628f, 0.331366f, 0.44239f,
    0.016172f, 0.339133f, 0.415254f,
    0.0133f, 0.34786f, 0.390024f,
    0.010759f, 0.358326f, 0.366399f,
    0.008542f, 0.370001f, 0.344015f,
    0.006661f, 0.382464f, 0.322689f,
    0.005132f, 0.395379f, 0.302356f,
    0.003982f, 0.408482f, 0.283036f,
    0.003239f, 0.421588f, 0.264816f,
    0.002934f, 0.434619f, 0.247848f,
    0.003114f, 0.447601f, 0.232318f,
    0.003816f, 0.460777f, 0.218502f,
    0.005095f, 0.47434f, 0.205851f,
    0.006936f, 0.4882f, 0.193596f,
    0.009299f, 0.50234f, 0.181736f,
    0.012147f, 0.51674f, 0.170281f,
    0.015444f, 0.53136f, 0.159249f,
    0.019156f, 0.54619f, 0.148673f,
    0.02325f, 0.56118f, 0.138609f,
    0.02769f, 0.57629f, 0.129096f,
    0.032444f, 0.5915f, 0.120215f,
    0.037465f, 0.606741f, 0.112044f,
    0.042956f, 0.62215f, 0.10471f,
    0.049114f, 0.63783f, 0.098196f,
    0.05592f, 0.65371f, 0.092361f,
    0.063349f, 0.66968f, 0.087088f,
    0.071358f, 0.68566f, 0.082248f,
    0.079901f, 0.70155f, 0.077744f,
    0.088909f, 0.71723f, 0.073456f,
    0.098293f, 0.73257f, 0.069268f,
    0.107949f, 0.74746f, 0.06506f,
    0.117749f, 0.761757f, 0.060709f, /* 520 nm */
    0.127839f, 0.77534f, 0.056457f,
    0.13845f, 0.78822f, 0.052609f,
    0.149516f, 0.80046f, 0.049122f,
    0.161041f, 0.81214f, 0.045954f,
    0.172953f, 0.82333f, 0.04305f,
    0.185209f, 0.83412f, 0.040368f,
    0.197755f, 0.8446f, 0.037839f,
    0.210538f, 0.85487f, 0.035384f,
    0.22346f, 0.86504f, 0.032949f,
    0.236491f, 0.875211f, 0.030451f,
    0.249633f, 0.88537f, 0.028029f,
    0.262972f, 0.89537f, 0.025862f,
    0.276515f, 0.90515f, 0.02392f,
    0.290269f, 0.91465f, 0.022174f,
    0.304213f, 0.92381f, 0.020584f,
    0.318361f, 0.93255f, 0.019127f,
    0.332705f, 0.94081f, 0.01774f,
    0.347232f, 0.94852f, 0.016403f,
    0.361926f, 0.9556f, 0.015064f,
    0.376772f, 0.961988f, 0.013676f,
    0.391683f, 0.96754f, 0.012308f,
    0.406594f, 0.97223f, 0.011056f,
    0.421539f, 0.97617f, 0.009915f,
    0.436517f, 0.97946f, 0.008872f,
    0.451584f, 0.9822f, 0.007918f,
    0.466782f, 0.98452f, 0.00703f,
    0.482147f, 0.98652f, 0.006223f,
    0.497738f, 0.98832f, 0.005453f,
    0.513606f, 0.99002f, 0.004714f,
    0.529826f, 0.991761f, 0.003988f,
    0.54644f, 0.99353f, 0.003289f,
    0.563426f, 0.99523f, 0.002646f,
    0.580726f, 0.99677f, 0.002063f,
    0.59829f, 0.99809f, 0.001533f,
    0.616053f, 0.99911f, 0.001091f,
    0.633948f, 0.99977f, 0.000711f,
    0.651901f, 1.f, 0.000407f,
    0.669824f, 0.99971f, 0.000184f,
    0.687632f, 0.99885f, 0.000047f,
    0.705224f, 0.99734f, 0.0f, /* 560 nm */
    0.722773f, 0.99526f, 0.0f,
    0.740483f, 0.99274f, 0.0f,
    0.758273f, 0.98975f, 0.0f,
    0.776083f, 0.9863f, 0.0f,
    0.793832f, 0.98238f, 0.0f,
    0.811436f, 0.97798f, 0.0f,
    0.828822f, 0.97311f, 0.0f,
    0.845879f, 0.96774f, 0.0f,
    0.862525f, 0.96189f, 0.0f,
    0.878655f, 0.955552f, 0.0f,
    0.894208f, 0.948601f, 0.0f,
    0.909206f, 0.940981f, 0.0f,
    0.923672f, 0.932798f, 0.0f,
    0.937638f, 0.924158f, 0.0f,
    0.951162f, 0.915175f, 0.0f,
    0.964283f, 0.905954f, 0.0f,
    0.977068f, 0.896608f, 0.0f,
    0.98959f, 0.887249f, 0.0f,
    1.00191f, 0.877986f, 0.0f,
    1.01416f, 0.868934f, 0.0f,
    1.0265f, 0.860164f, 0.0f,
    1.0388f, 0.851519f, 0.0f,
    1.051f, 0.842963f, 0.0f,
    1.0629f, 0.834393f, 0.0f,
    1.0743f, 0.825623f, 0.0f,
    1.0852f, 0.816764f, 0.0f,
    1.0952f, 0.807544f, 0.0f,
    1.1042f, 0.797947f, 0.0f,
    1.112f, 0.787893f, 0.0f,
    1.11852f, 0.777405f, 0.0f,
    1.1238f, 0.76649f, 0.0f,
    1.128f, 0.755309f, 0.0f,
    1.1311f, 0.743845f, 0.0f,
    1.1332f, 0.73219f, 0.0f,
    1.1343f, 0.720353f, 0.0f,
    1.1343f, 0.708281f, 0.0f,
    1.1333f, 0.696055f, 0.0f,
    1.1312f, 0.683621f, 0.0f,
    1.1281f, 0.671048f, 0.0f,
    1.12399f, 0.658341f, 0.0f, /* 600 nm */
    1.1189f, 0.645545f, 0.0f,
    1.1129f, 0.632718f, 0.0f,
    1.1059f, 0.619815f, 0.0f,
    1.098f, 0.606887f, 0.0f,
    1.0891f, 0.593878f, 0.0f,
    1.0792f, 0.580781f, 0.0f,
    1.0684f, 0.567653f, 0.0f,
    1.0567f, 0.55449f, 0.0f,
    1.044f, 0.541228f, 0.0f,
    1.03048f, 0.527963f, 0.0f,
    1.016f, 0.514634f, 0.0f,
    1.0008f, 0.501363f, 0.0f,
    0.98479f, 0.488124f, 0.0f,
    0.96808f, 0.474935f, 0.0f,
    0.95074f, 0.461834f, 0.0f,
    0.9328f, 0.448823f, 0.0f,
    0.91434f, 0.435917f, 0.0f,
    0.89539f, 0.423153f, 0.0f,
    0.87603f, 0.410526f, 0.0f,
    0.856297f, 0.398057f, 0.0f,
    0.83635f, 0.385835f, 0.0f,
    0.81629f, 0.373951f, 0.0f,
    0.79605f, 0.362311f, 0.0f,
    0.77561f, 0.350863f, 0.0f,
    0.75493f, 0.339554f, 0.0f,
    0.73399f, 0.328309f, 0.0f,
    0.71278f, 0.317118f, 0.0f,
    0.69129f, 0.305936f, 0.0f,
    0.66952f, 0.294737f, 0.0f,
    0.647467f, 0.283493f, 0.0f,
    0.62511f, 0.272222f, 0.0f,
    0.60252f, 0.26099f, 0.0f,
    0.57989f, 0.249877f, 0.0f,
    0.55737f, 0.238946f, 0.0f,
    0.53511f, 0.228254f, 0.0f,
    0.51324f, 0.217853f, 0.0f,
    0.49186f, 0.20778f, 0.0f,
    0.47108f, 0.198072f, 0.0f,
    0.45096f, 0.188748f, 0.0f,
    0.431567f, 0.179828f, 0.0f, /* 640 nm */
    0.41287f, 0.171285f, 0.0f,
    0.39475f, 0.163059f, 0.0f,
    0.37721f, 0.155151f, 0.0f,
    0.36019f, 0.147535f, 0.0f,
    0.34369f, 0.140211f, 0.0f,
    0.32769f, 0.13317f, 0.0f,
    0.31217f, 0.1264f, 0.0f,
    0.29711f, 0.119892f, 0.0f,
    0.2825f, 0.11364f, 0.0f,
    0.268329f, 0.107633f, 0.0f,
    0.25459f, 0.10187f, 0.0f,
    0.2413f, 0.096347f, 0.0f,
    0.22848f, 0.091063f, 0.0f,
    0.21614f, 0.08601f, 0.0f,
    0.2043f, 0.081187f, 0.0f,
    0.19295f, 0.076583f, 0.0f,
    0.18211f, 0.072198f, 0.0f,
    0.17177f, 0.068024f, 0.0f,
    0.16192f, 0.064052f, 0.0f,
    0.152568f, 0.060281f, 0.0f,
    0.14367f, 0.056697f, 0.0f,
    0.1352f, 0.053292f, 0.0f,
    0.12713f, 0.050059f, 0.0f,
    0.11948f, 0.046998f, 0.0f,
    0.11221f, 0.044096f, 0.0f,
    0.10531f, 0.041345f, 0.0f,
    0.098786f, 0.0387507f, 0.0f,
    0.09261f, 0.0362978f, 0.0f,
    0.086773f, 0.0339832f, 0.0f,
    0.0812606f, 0.0318004f, 0.0f,
    0.076048f, 0.0297395f, 0.0f,
    0.071114f, 0.0277918f, 0.0f,
    0.066454f, 0.0259551f, 0.0f,
    0.062062f, 0.0242263f, 0.0f,
    0.05793f, 0.0226017f, 0.0f,
    0.05405f, 0.0210779f, 0.0f,
    0.050412f, 0.0196505f, 0.0f,
    0.047006f, 0.0183153f, 0.0f,
    0.043823f, 0.0170686f, 0.0f,
    0.0408508f, 0.0159051f, 0.0f, /* 680 nm */
    0.038072f, 0.0148183f, 0.0f,
    0.035468f, 0.0138008f, 0.0f,
    0.033031f, 0.0128495f, 0.0f,
    0.030753f, 0.0119607f, 0.0f,
    0.028623f, 0.0111303f, 0.0f,
    0.026635f, 0.0103555f, 0.0f,
    0.024781f, 0.0096332f, 0.0f,
    0.023052f, 0.0089599f, 0.0f,
    0.021441f, 0.0083324f, 0.0f,
    0.0199413f, 0.0077488f, 0.0f,
    0.018544f, 0.0072046f, 0.0f,
    0.017241f, 0.0066975f, 0.0f,
    0.016027f, 0.0062251f, 0.0f,
    0.014896f, 0.005785f, 0.0f,
    0.013842f, 0.0053751f, 0.0f,
    0.012862f, 0.0049941f, 0.0f,
    0.011949f, 0.0046392f, 0.0f,
    0.0111f, 0.0043093f, 0.0f,
    0.010311f, 0.0040028f, 0.0f,
    0.0095769f, 0.0037177f, 0.0f,
    0.008894f, 0.0034526f, 0.0f,
    0.0082581f, 0.0032058f, 0.0f,
    0.0076664f, 0.0029762f, 0.0f,
    0.0071163f, 0.0027628f, 0.0f,
    0.0066052f, 0.0025646f, 0.0f,
    0.0061306f, 0.0023805f, 0.0f,
    0.0056903f, 0.0022097f, 0.0f,
    0.0052819f, 0.0020513f, 0.0f,
    0.0049033f, 0.0019045f, 0.0f,
    0.0045526f, 0.0017685f, 0.0f,
    0.0042275f, 0.0016424f, 0.0f,
    0.0039258f, 0.0015254f, 0.0f,
    0.0036457f, 0.0014167f, 0.0f,
    0.0033859f, 0.001316f, 0.0f,
    0.0031447f, 0.0012224f, 0.0f,
    0.0029208f, 0.0011356f, 0.0f,
    0.002713f, 0.0010549f, 0.0f,
    0.0025202f, 0.0009801f, 0.0f,
    0.0023411f, 0.0009107f, 0.0f,
    0.002175f, 0.0008462f, 0.0f, /* 720 nm */
    0.0020206f, 0.0007863f, 0.0f,
    0.0018773f, 0.0007307f, 0.0f,
    0.0017441f, 0.000679f, 0.0f,
    0.0016205f, 0.000631f, 0.0f,
    0.0015057f, 0.0005864f, 0.0f,
    0.0013992f, 0.0005451f, 0.0f,
    0.0013004f, 0.0005067f, 0.0f,
    0.0012087f, 0.0004711f, 0.0f,
    0.0011236f, 0.0004381f, 0.0f,
    0.0010448f, 0.0004074f, 0.0f,
    0.0009716f, 0.000379f, 0.0f,
    0.0009036f, 0.0003525f, 0.0f,
    0.0008405f, 0.000328f, 0.0f,
    0.0007819f, 0.0003052f, 0.0f,
    0.0007275f, 0.000284f, 0.0f,
    0.0006769f, 0.0002644f, 0.0f,
    0.00063f, 0.0002461f, 0.0f,
    0.0005864f, 0.0002291f, 0.0f,
    0.0005459f, 0.0002134f, 0.0f,
    0.0005083f, 0.0001987f, 0.0f,
    0.0004733f, 0.0001851f, 0.0f,
    0.0004408f, 0.0001725f, 0.0f,
    0.0004106f, 0.0001607f, 0.0f,
    0.0003825f, 0.0001497f, 0.0f,
    0.0003564f, 0.0001396f, 0.0f,
    0.0003321f, 0.0001301f, 0.0f,
    0.0003096f, 0.0001213f, 0.0f,
    0.0002886f, 0.0001131f, 0.0f,
    0.0002691f, 0.0001055f, 0.0f,
    0.000251f, 9.843e-05f, 0.0f,
    0.0002341f, 9.185e-05f, 0.0f,
    0.0002185f, 8.574e-05f, 0.0f,
    0.000203910f, 8.005e-05f, 0.0f,
    0.0001904f, 7.475e-05f, 0.0f,
    0.0001777f, 6.982e-05f, 0.0f,
    0.000166f, 6.522e-05f, 0.0f,
    0.000155f, 6.094e-05f, 0.0f,
    0.0001448f, 5.694e-05f, 0.0f,
    0.0001353f, 5.322e-05f, 0.0f,
    0.0001264f, 4.974e-05f, 0.0f, /* 760 nm */
    0.0001181f, 4.649e-05f, 0.0f,
    0.0001104f, 4.346e-05f, 0.0f,
    0.0001032f, 4.064e-05f, 0.0f,
    9.643e-05f, 0.000038f, 0.0f,
    9.015e-05f, 3.554e-05f, 0.0f,
    8.429e-05f, 3.324e-05f, 0.0f,
    7.883e-05f, 3.11e-05f, 0.0f,
    7.373e-05f, 2.91e-05f, 0.0f,
    6.897e-05f, 2.723e-05f, 0.0f,
    6.453e-05f, 2.549e-05f, 0.0f,
    6.038e-05f, 2.386e-05f, 0.0f,
    0.0000565f, 2.233e-05f, 0.0f,
    5.288e-05f, 2.091e-05f, 0.0f,
    4.95e-05f, 1.958e-05f, 0.0f,
    4.634e-05f, 1.834e-05f, 0.0f,
    4.339e-05f, 1.718e-05f, 0.0f,
    4.063e-05f, 1.609e-05f, 0.0f,
    3.806e-05f, 1.508e-05f, 0.0f,
    3.566e-05f, 1.413e-05f, 0.0f,
    3.341e-05f, 1.325e-05f, 0.0f,
    3.132e-05f, 1.242e-05f, 0.0f,
    2.936e-05f, 1.165e-05f, 0.0f,
    2.752e-05f, 1.093e-05f, 0.0f,
    2.581e-05f, 1.025e-05f, 0.0f,
    2.421e-05f, 9.62e-06f, 0.0f,
    2.271e-05f, 9.028e-06f, 0.0f,
    2.131e-05f, 8.474e-06f, 0.0f,
    1.999e-05f, 7.955e-06f, 0.0f,
    1.876e-05f, 7.469e-06f, 0.0f,
    1.761e-05f, 7.013e-06f, 0.0f,
    1.653e-05f, 6.586e-06f, 0.0f,
    1.552e-05f, 6.186e-06f, 0.0f,
    1.457e-05f, 5.811e-06f, 0.0f,
    1.369e-05f, 5.459e-06f, 0.0f,
    1.286e-05f, 5.13e-06f, 0.0f,
    1.208e-05f, 4.821e-06f, 0.0f,
    1.135e-05f, 4.531e-06f, 0.0f,
    1.066e-05f, 4.259e-06f, 0.0f,
    1.002e-05f, 4.004e-06f, 0.0f,
    9.414e-06f, 3.765e-06f, 0.0f, /* 800 nm */
    8.848e-06f, 3.54e-06f, 0.0f,
    8.317e-06f, 3.329e-06f, 0.0f,
    7.819e-06f, 3.131e-06f, 0.0f,
    7.352e-06f, 2.945e-06f, 0.0f,
    6.913e-06f, 2.771e-06f, 0.0f,
    6.502e-06f, 2.607e-06f, 0.0f,
    6.115e-06f, 2.453e-06f, 0.0f,
    5.753e-06f, 2.309e-06f, 0.0f,
    5.413e-06f, 2.173e-06f, 0.0f,
    5.093e-06f, 2.046e-06f, 0.0f,
    4.794e-06f, 1.927e-06f, 0.0f,
    4.513e-06f, 1.814e-06f, 0.0f,
    4.248e-06f, 1.709e-06f, 0.0f,
    4e-06f, 1.61e-06f, 0.0f,
    3.767e-06f, 1.517e-06f, 0.0f,
    3.548e-06f, 1.429e-06f, 0.0f,
    3.342e-06f, 1.347e-06f, 0.0f,
    3.149e-06f, 1.269e-06f, 0.0f,
    2.967e-06f, 1.197e-06f, 0.0f,
    2.795e-06f, 1.128e-06f, 0.0f,
    2.635e-06f, 1.064e-06f, 0.0f,
    2.483e-06f, 1.003e-06f, 0.0f,
    2.341e-06f, 9.462e-07f, 0.0f,
    2.208e-06f, 8.926e-07f, 0.0f,
    2.082e-06f, 8.422e-07f, 0.0f,
    1.964e-06f, 7.946e-07f, 0.0f,
    1.852e-06f, 7.498e-07f, 0.0f,
    1.747e-06f, 7.074e-07f, 0.0f,
    1.647e-06f, 6.675e-07f, 0.0f,
    1.553e-06f, 6.297e-07f, 0.0f, /* 830 nm */
};

vec3 Color::WavelengthToCIExyY(float nm)
{
    nm -= 360.f;

    int i = (int)nm;
    if (i < 0 || i > 830 - 360)
        return vec3(0.0f);

    float t = nm - i, s = 1.0 - t;
    float x = s * cie_1931_xyz[i * 3 + 0] + t * cie_1931_xyz[i * 3 + 3];
    float y = s * cie_1931_xyz[i * 3 + 1] + t * cie_1931_xyz[i * 3 + 4];
    float z = s * cie_1931_xyz[i * 3 + 2] + t * cie_1931_xyz[i * 3 + 5];
    float normalize = 1.f / (x + y + z);

    return vec3(x * normalize, y * normalize, 100.0f);
}

} /* namespace lol */

