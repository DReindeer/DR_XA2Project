xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 70;
 0.00199;0.25506;0.10518;,
 0.09474;-0.01982;0.26607;,
 0.27276;-0.02126;0.14569;,
 0.00199;0.25506;0.10518;,
 -0.01510;-0.02040;0.26796;,
 0.00199;0.25506;0.10518;,
 -0.09905;-0.02137;0.25305;,
 0.00199;0.25506;0.10518;,
 -0.32463;-0.01759;0.10114;,
 0.00199;0.25506;0.10518;,
 -0.12796;-0.01848;-0.16857;,
 0.00199;0.25506;0.10518;,
 0.01814;-0.01897;-0.16938;,
 0.00199;0.25506;0.10518;,
 0.12996;-0.01809;-0.15037;,
 0.00199;0.25506;0.10518;,
 0.27276;-0.02126;0.14569;,
 0.20956;-0.51975;0.35444;,
 0.40594;-0.52023;0.15278;,
 -0.03648;-0.52017;0.39640;,
 -0.26954;-0.52023;0.32760;,
 -0.42579;-0.51805;0.09229;,
 -0.26991;-0.51754;-0.15711;,
 0.01671;-0.51740;-0.27410;,
 0.28096;-0.51840;-0.11735;,
 0.40594;-0.52023;0.15278;,
 0.22562;-1.24055;0.34408;,
 0.35352;-1.23938;0.14458;,
 -0.03767;-1.24169;0.37157;,
 -0.27309;-1.24290;0.31252;,
 -0.39757;-1.24185;0.09227;,
 -0.28705;-1.24020;-0.13873;,
 0.01065;-1.23870;-0.33036;,
 0.24901;-1.22105;-0.08781;,
 0.35352;-1.23938;0.14458;,
 0.20871;-2.04130;0.34546;,
 0.33791;-2.06105;0.13974;,
 -0.04760;-2.04249;0.36017;,
 -0.29147;-2.04344;0.28285;,
 -0.39417;-2.04294;0.08906;,
 -0.27495;-2.04144;-0.14124;,
 0.00015;-2.03953;-0.33469;,
 0.22687;-2.05324;-0.09322;,
 0.33791;-2.06105;0.13974;,
 0.11831;-2.76244;0.32584;,
 0.27610;-2.76228;0.13253;,
 -0.05712;-2.76398;0.34663;,
 -0.27777;-2.76416;0.25978;,
 -0.39212;-2.76307;0.08526;,
 -0.27927;-2.76178;-0.16763;,
 -0.01271;-2.76127;-0.28623;,
 0.20592;-2.76145;-0.13317;,
 0.27610;-2.76228;0.13253;,
 0.03286;-3.26180;0.21733;,
 0.11047;-3.26158;0.11847;,
 -0.05761;-3.26290;0.25042;,
 -0.16460;-3.26321;0.19217;,
 -0.21697;-3.26281;0.09575;,
 -0.17991;-3.26150;-0.06930;,
 -0.03387;-3.26144;-0.08955;,
 0.09873;-3.26148;-0.04943;,
 0.11047;-3.26158;0.11847;,
 -0.05027;-3.44049;0.10658;,
 -0.05027;-3.44049;0.10658;,
 -0.05027;-3.44049;0.10658;,
 -0.05027;-3.44049;0.10658;,
 -0.05027;-3.44049;0.10658;,
 -0.05027;-3.44049;0.10658;,
 -0.05027;-3.44049;0.10658;,
 -0.05027;-3.44049;0.10658;;
 
 56;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;36,35,44,45;,
 4;35,37,46,44;,
 4;37,38,47,46;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 3;54,53,62;,
 3;53,55,63;,
 3;55,56,64;,
 3;56,57,65;,
 3;57,58,66;,
 3;58,59,67;,
 3;59,60,68;,
 3;60,61,69;;
 
 MeshMaterialList {
  4;
  56;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   8.750000;
   0.025000;0.025000;0.025000;;
   0.112000;0.112000;0.112000;;
   TextureFilename {
    "data/TEXTURE/mity/mity_hada.png";
   }
  }
  Material {
   0.625000;0.625000;0.625000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.063000;0.063000;0.063000;;
   TextureFilename {
    "data/TEXTURE/mity/mity_ke.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   8.750000;
   0.025000;0.025000;0.025000;;
   0.112000;0.112000;0.112000;;
   TextureFilename {
    "data/TEXTURE/mity/eye_mity.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   8.750000;
   0.025000;0.025000;0.025000;;
   0.112000;0.112000;0.112000;;
   TextureFilename {
    "data/TEXTURE/mity/mouth_mity.png";
   }
  }
 }
 MeshNormals {
  62;
  -0.037008;0.565072;0.824211;,
  0.556033;0.402483;0.727210;,
  0.315061;0.402842;0.859334;,
  -0.070846;0.379599;0.922434;,
  -0.398886;0.397947;0.826153;,
  -0.587453;0.395740;0.705896;,
  -0.446997;0.492970;-0.746441;,
  0.069343;0.459887;-0.885266;,
  0.569645;0.484263;-0.664074;,
  0.713721;0.105361;0.692461;,
  0.438297;0.110938;0.891957;,
  -0.066952;0.108501;0.991839;,
  -0.542203;0.115138;0.832322;,
  -0.782766;0.108429;0.612798;,
  -0.627469;0.097051;-0.772569;,
  0.083742;0.091483;-0.992279;,
  0.730507;0.085267;-0.677561;,
  0.814483;-0.021381;0.579793;,
  0.505557;-0.020157;0.862558;,
  -0.081277;-0.019398;0.996503;,
  -0.611662;-0.017975;0.790915;,
  -0.978147;-0.010519;0.207647;,
  -0.726044;0.003884;-0.687638;,
  -0.510747;0.018153;-0.859540;,
  0.805307;-0.023943;-0.592374;,
  0.993660;-0.050500;0.100448;,
  0.504181;-0.041521;0.862599;,
  -0.111867;-0.020374;0.993514;,
  -0.629625;-0.017561;0.776701;,
  -0.999510;-0.004910;0.030912;,
  -0.741189;0.000384;-0.671296;,
  -0.526113;-0.000822;-0.850415;,
  0.823619;-0.022489;-0.566698;,
  0.962903;-0.190811;0.190811;,
  0.507465;-0.159963;0.846694;,
  -0.112899;-0.115966;0.986816;,
  -0.646631;-0.130785;0.751508;,
  -0.984510;-0.158695;0.074534;,
  -0.703437;-0.163090;-0.691793;,
  -0.123946;-0.228386;-0.965649;,
  0.800809;-0.175965;-0.572487;,
  0.836934;-0.491380;0.241013;,
  0.503150;-0.441591;0.742857;,
  -0.078532;-0.430014;0.899400;,
  -0.614651;-0.441192;0.653876;,
  -0.855389;-0.506646;0.107798;,
  -0.566219;-0.575988;-0.589605;,
  0.069039;-0.556995;-0.827642;,
  0.659934;-0.568660;-0.491032;,
  -0.047870;-0.718734;0.693635;,
  0.066299;0.784708;-0.616310;,
  -0.725088;0.465693;-0.507324;,
  0.809687;0.445911;-0.381538;,
  -0.850035;0.090841;-0.518834;,
  0.903443;0.087579;-0.419667;,
  -0.876264;-0.014276;-0.481619;,
  0.670262;-0.007736;-0.742084;,
  0.907208;-0.039136;-0.418859;,
  0.688257;-0.012558;-0.725358;,
  0.653699;-0.014944;-0.756607;,
  -0.227538;-0.799031;-0.556575;,
  0.744577;-0.665472;-0.052459;;
  56;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;50,6,51;,
  3;50,7,6;,
  3;50,8,7;,
  3;50,52,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;51,6,14,53;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,52,54,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;53,14,22,55;,
  4;14,15,23,22;,
  4;15,16,24,56;,
  4;16,54,57,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;56,24,32,58;,
  4;24,57,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;58,32,40,59;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  3;41,42,49;,
  3;42,43,49;,
  3;43,44,49;,
  3;44,45,49;,
  3;45,46,60;,
  3;46,47,60;,
  3;47,48,60;,
  3;48,41,61;;
 }
 MeshTextureCoords {
  70;
  0.062500;0.000000;,
  0.125000;0.142860;,
  0.000000;0.142860;,
  0.187500;0.000000;,
  0.250000;0.142860;,
  0.312500;0.000000;,
  0.375000;0.142860;,
  0.437500;0.000000;,
  0.500000;0.142860;,
  0.562500;0.000000;,
  0.625000;0.142860;,
  0.687500;0.000000;,
  0.750000;0.142860;,
  0.812500;0.000000;,
  0.875000;0.142860;,
  0.937500;0.000000;,
  1.000000;0.142860;,
  0.125000;0.285710;,
  0.000000;0.285710;,
  0.250000;0.285710;,
  0.375000;0.285710;,
  0.500000;0.285710;,
  0.625000;0.285710;,
  0.750000;0.285710;,
  0.875000;0.285710;,
  1.000000;0.285710;,
  0.125000;0.428570;,
  0.000000;0.428570;,
  0.250000;0.428570;,
  0.375000;0.428570;,
  0.500000;0.428570;,
  0.625000;0.428570;,
  0.750000;0.428570;,
  0.875000;0.428570;,
  1.000000;0.428570;,
  0.125000;0.571430;,
  0.000000;0.571430;,
  0.250000;0.571430;,
  0.375000;0.571430;,
  0.500000;0.571430;,
  0.625000;0.571430;,
  0.750000;0.571430;,
  0.875000;0.571430;,
  1.000000;0.571430;,
  0.125000;0.714290;,
  0.000000;0.714290;,
  0.250000;0.714290;,
  0.375000;0.714290;,
  0.500000;0.714290;,
  0.625000;0.714290;,
  0.750000;0.714290;,
  0.875000;0.714290;,
  1.000000;0.714290;,
  0.125000;0.857140;,
  0.000000;0.857140;,
  0.250000;0.857140;,
  0.375000;0.857140;,
  0.500000;0.857140;,
  0.625000;0.857140;,
  0.750000;0.857140;,
  0.875000;0.857140;,
  1.000000;0.857140;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
 MeshVertexColors {
  70;
  0;1.000000;1.000000;1.000000;1.000000;,
  1;1.000000;1.000000;1.000000;1.000000;,
  2;1.000000;1.000000;1.000000;1.000000;,
  3;1.000000;1.000000;1.000000;1.000000;,
  4;1.000000;1.000000;1.000000;1.000000;,
  5;1.000000;1.000000;1.000000;1.000000;,
  6;1.000000;1.000000;1.000000;1.000000;,
  7;1.000000;1.000000;1.000000;1.000000;,
  8;1.000000;1.000000;1.000000;1.000000;,
  9;1.000000;1.000000;1.000000;1.000000;,
  10;1.000000;1.000000;1.000000;1.000000;,
  11;1.000000;1.000000;1.000000;1.000000;,
  12;1.000000;1.000000;1.000000;1.000000;,
  13;1.000000;1.000000;1.000000;1.000000;,
  14;1.000000;1.000000;1.000000;1.000000;,
  15;1.000000;1.000000;1.000000;1.000000;,
  16;1.000000;1.000000;1.000000;1.000000;,
  17;1.000000;1.000000;1.000000;1.000000;,
  18;1.000000;1.000000;1.000000;1.000000;,
  19;1.000000;1.000000;1.000000;1.000000;,
  20;1.000000;1.000000;1.000000;1.000000;,
  21;1.000000;1.000000;1.000000;1.000000;,
  22;1.000000;1.000000;1.000000;1.000000;,
  23;1.000000;1.000000;1.000000;1.000000;,
  24;1.000000;1.000000;1.000000;1.000000;,
  25;1.000000;1.000000;1.000000;1.000000;,
  26;1.000000;1.000000;1.000000;1.000000;,
  27;1.000000;1.000000;1.000000;1.000000;,
  28;1.000000;1.000000;1.000000;1.000000;,
  29;1.000000;1.000000;1.000000;1.000000;,
  30;1.000000;1.000000;1.000000;1.000000;,
  31;1.000000;1.000000;1.000000;1.000000;,
  32;1.000000;1.000000;1.000000;1.000000;,
  33;1.000000;1.000000;1.000000;1.000000;,
  34;1.000000;1.000000;1.000000;1.000000;,
  35;1.000000;1.000000;1.000000;1.000000;,
  36;1.000000;1.000000;1.000000;1.000000;,
  37;1.000000;1.000000;1.000000;1.000000;,
  38;1.000000;1.000000;1.000000;1.000000;,
  39;1.000000;1.000000;1.000000;1.000000;,
  40;1.000000;1.000000;1.000000;1.000000;,
  41;1.000000;1.000000;1.000000;1.000000;,
  42;1.000000;1.000000;1.000000;1.000000;,
  43;1.000000;1.000000;1.000000;1.000000;,
  44;1.000000;1.000000;1.000000;1.000000;,
  45;1.000000;1.000000;1.000000;1.000000;,
  46;1.000000;1.000000;1.000000;1.000000;,
  47;1.000000;1.000000;1.000000;1.000000;,
  48;1.000000;1.000000;1.000000;1.000000;,
  49;1.000000;1.000000;1.000000;1.000000;,
  50;1.000000;1.000000;1.000000;1.000000;,
  51;1.000000;1.000000;1.000000;1.000000;,
  52;1.000000;1.000000;1.000000;1.000000;,
  53;1.000000;1.000000;1.000000;1.000000;,
  54;1.000000;1.000000;1.000000;1.000000;,
  55;1.000000;1.000000;1.000000;1.000000;,
  56;1.000000;1.000000;1.000000;1.000000;,
  57;1.000000;1.000000;1.000000;1.000000;,
  58;1.000000;1.000000;1.000000;1.000000;,
  59;1.000000;1.000000;1.000000;1.000000;,
  60;1.000000;1.000000;1.000000;1.000000;,
  61;1.000000;1.000000;1.000000;1.000000;,
  62;1.000000;1.000000;1.000000;1.000000;,
  63;1.000000;1.000000;1.000000;1.000000;,
  64;1.000000;1.000000;1.000000;1.000000;,
  65;1.000000;1.000000;1.000000;1.000000;,
  66;1.000000;1.000000;1.000000;1.000000;,
  67;1.000000;1.000000;1.000000;1.000000;,
  68;1.000000;1.000000;1.000000;1.000000;,
  69;1.000000;1.000000;1.000000;1.000000;;
 }
}
