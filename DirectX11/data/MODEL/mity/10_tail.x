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
 104;
 -0.01361;-3.47688;6.29724;,
 0.30923;-2.79110;5.99998;,
 -0.05072;-2.68277;6.19937;,
 -0.01361;-3.47688;6.29724;,
 0.01361;-3.47688;6.29724;,
 -0.01361;-3.47688;6.29724;,
 0.45690;-3.17351;5.77558;,
 -0.01361;-3.47688;6.29724;,
 0.32593;-3.58826;5.57565;,
 -0.01361;-3.47688;6.29724;,
 0.05665;-3.69351;5.79213;,
 -0.01361;-3.47688;6.29724;,
 -0.32593;-3.58826;5.57565;,
 0.05665;-3.69351;5.79213;,
 -0.01361;-3.47688;6.29724;,
 -0.56782;-3.25919;5.61906;,
 -0.01361;-3.47688;6.29724;,
 -0.45690;-3.17351;5.77558;,
 -0.01361;-3.47688;6.29724;,
 -0.30923;-2.79110;5.99998;,
 -0.01361;-3.47688;6.29724;,
 -0.05072;-2.68277;6.19937;,
 0.51639;-1.33992;5.09669;,
 -0.07989;-1.06970;5.23772;,
 0.94738;-2.22648;4.85145;,
 1.03195;-3.08379;4.82909;,
 1.06415;-3.34439;4.34690;,
 0.36988;-3.85849;4.15479;,
 -1.06415;-3.34439;4.34690;,
 -0.36988;-3.85849;4.15479;,
 -1.03195;-3.08379;4.82909;,
 -0.94738;-2.22648;4.85145;,
 -0.51639;-1.33992;5.09669;,
 -0.07989;-1.06970;5.23772;,
 0.84518;-0.76932;4.34193;,
 -0.09668;-0.48379;4.49114;,
 1.55630;-1.53271;4.18999;,
 1.85681;-2.46080;3.91215;,
 1.56148;-2.99266;3.05882;,
 0.31319;-3.77539;2.72137;,
 -0.31319;-3.77539;2.72137;,
 -1.56148;-2.99266;3.05882;,
 -1.85681;-2.46080;3.91215;,
 -1.55630;-1.53271;4.18999;,
 -0.84518;-0.76932;4.34193;,
 -0.09668;-0.48379;4.49114;,
 0.96468;-0.51173;3.21487;,
 -0.09853;-0.25272;3.34963;,
 1.68124;-1.03559;3.13448;,
 2.12720;-1.94143;2.75000;,
 1.74221;-2.47893;2.05871;,
 0.38886;-3.08464;1.70193;,
 -0.38886;-3.08464;1.70193;,
 -1.74221;-2.47893;2.05871;,
 -2.12720;-1.94143;2.75000;,
 -1.68124;-1.03559;3.13448;,
 -0.96468;-0.51173;3.21487;,
 -0.09853;-0.25272;3.34963;,
 0.85669;-0.23318;2.08972;,
 -0.08516;0.07917;2.25350;,
 1.43132;-0.83465;1.79423;,
 1.79323;-1.21693;1.60772;,
 1.57504;-1.52332;1.30787;,
 0.32471;-2.04693;0.99779;,
 -0.32471;-2.04693;0.99779;,
 -1.57504;-1.52332;1.30787;,
 -1.79323;-1.21693;1.60772;,
 -1.43132;-0.83465;1.79423;,
 -0.85669;-0.23318;2.08972;,
 -0.08516;0.07917;2.25350;,
 0.47955;-0.09576;0.80512;,
 -0.05334;0.14492;0.98510;,
 0.77840;-0.26158;0.68582;,
 1.03775;-0.48141;0.54879;,
 1.09770;-0.84010;0.60373;,
 0.07667;-1.15391;0.40910;,
 -0.07667;-1.15391;0.40910;,
 -1.09770;-0.84010;0.60373;,
 -1.03775;-0.48141;0.54879;,
 -0.77840;-0.26158;0.68582;,
 -0.47955;-0.09576;0.80512;,
 -0.05334;0.14492;0.98510;,
 0.02291;0.14652;0.45832;,
 -0.02291;0.14652;0.45832;,
 0.31683;0.00078;0.38210;,
 0.59562;-0.22481;0.24229;,
 0.35373;-0.29973;0.18780;,
 0.00698;-0.60513;0.00962;,
 -0.35373;-0.29973;0.18780;,
 0.00698;-0.60513;0.00962;,
 -0.59562;-0.22481;0.24229;,
 -0.48470;-0.05280;0.34825;,
 -0.31683;0.00078;0.38210;,
 -0.02291;0.14652;0.45832;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;,
 0.01649;0.07383;-0.01305;;
 
 86;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,13;,
 3;14,15,12;,
 3;16,17,15;,
 3;18,19,17;,
 3;20,21,19;,
 4;2,1,22,23;,
 4;1,4,24,22;,
 4;4,6,25,24;,
 4;6,8,26,25;,
 4;8,10,27,26;,
 4;13,12,28,29;,
 4;12,15,30,28;,
 4;15,17,31,30;,
 4;17,19,32,31;,
 4;19,21,33,32;,
 4;23,22,34,35;,
 4;22,24,36,34;,
 4;24,25,37,36;,
 4;25,26,38,37;,
 4;26,27,39,38;,
 4;27,29,40,39;,
 4;29,28,41,40;,
 4;28,30,42,41;,
 4;30,31,43,42;,
 4;31,32,44,43;,
 4;32,33,45,44;,
 4;35,34,46,47;,
 4;34,36,48,46;,
 4;36,37,49,48;,
 4;37,38,50,49;,
 4;38,39,51,50;,
 4;39,40,52,51;,
 4;40,41,53,52;,
 4;41,42,54,53;,
 4;42,43,55,54;,
 4;43,44,56,55;,
 4;44,45,57,56;,
 4;47,46,58,59;,
 4;46,48,60,58;,
 4;48,49,61,60;,
 4;49,50,62,61;,
 4;50,51,63,62;,
 4;51,52,64,63;,
 4;52,53,65,64;,
 4;53,54,66,65;,
 4;54,55,67,66;,
 4;55,56,68,67;,
 4;56,57,69,68;,
 4;59,58,70,71;,
 4;58,60,72,70;,
 4;60,61,73,72;,
 4;61,62,74,73;,
 4;62,63,75,74;,
 4;63,64,76,75;,
 4;64,65,77,76;,
 4;65,66,78,77;,
 4;66,67,79,78;,
 4;67,68,80,79;,
 4;68,69,81,80;,
 4;71,70,82,83;,
 4;70,72,84,82;,
 4;72,73,85,84;,
 4;73,74,86,85;,
 4;74,75,87,86;,
 4;76,77,88,89;,
 4;77,78,90,88;,
 4;78,79,91,90;,
 4;79,80,92,91;,
 4;80,81,93,92;,
 3;83,82,94;,
 3;82,84,95;,
 3;84,85,96;,
 3;85,86,97;,
 3;86,87,98;,
 3;89,88,99;,
 3;88,90,100;,
 3;90,91,101;,
 3;91,92,102;,
 3;92,93,103;,
 3;29,27,10;,
 3;87,75,76;;
 
 MeshMaterialList {
  4;
  86;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
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
  89;
  -0.044665;0.226658;0.972950;,
  0.495449;0.285028;0.820542;,
  0.503201;0.263549;0.823001;,
  0.397237;0.350675;0.848074;,
  0.000000;-0.864450;-0.502718;,
  0.679699;-0.641685;0.355316;,
  0.387956;-0.891944;0.232221;,
  -0.599506;-0.737125;0.311833;,
  -0.815596;-0.305975;0.491103;,
  -0.816326;0.014255;0.577415;,
  -0.723718;0.133357;0.677088;,
  0.140017;0.666154;0.732553;,
  0.630591;0.381018;0.676151;,
  0.756500;0.149747;0.636618;,
  0.818107;-0.218031;0.532131;,
  0.680221;-0.706912;0.193844;,
  0.285672;-0.957280;0.044801;,
  -0.269144;-0.962012;0.045774;,
  -0.658810;-0.730821;0.178520;,
  -0.833227;-0.276936;0.478581;,
  -0.773901;0.140554;0.617513;,
  -0.658206;0.377784;0.651187;,
  -0.043228;0.890975;0.451989;,
  0.528854;0.697646;0.483326;,
  0.781651;0.342267;0.521416;,
  0.921926;-0.211967;0.324226;,
  0.715808;-0.695086;-0.066889;,
  0.290019;-0.914307;-0.282722;,
  -0.290019;-0.914307;-0.282722;,
  -0.715808;-0.695086;-0.066889;,
  -0.921926;-0.211967;0.324226;,
  -0.781651;0.342267;0.521416;,
  -0.565538;0.673449;0.476060;,
  -0.031868;0.971063;0.236688;,
  0.492601;0.844917;0.208472;,
  0.792615;0.587182;0.164252;,
  0.997390;0.050441;-0.051659;,
  0.730007;-0.526194;-0.436129;,
  0.243640;-0.702285;-0.668906;,
  -0.243640;-0.702285;-0.668906;,
  -0.730007;-0.526194;-0.436129;,
  -0.997390;0.050441;-0.051659;,
  -0.792615;0.587182;0.164252;,
  -0.520186;0.829351;0.203921;,
  -0.033448;0.990913;0.130279;,
  0.493674;0.867107;0.066419;,
  0.698165;0.715927;-0.003801;,
  0.905836;0.319257;-0.278454;,
  0.666017;-0.310396;-0.678289;,
  0.195057;-0.536637;-0.820959;,
  -0.195057;-0.536637;-0.820959;,
  -0.666017;-0.310396;-0.678289;,
  -0.905836;0.319257;-0.278454;,
  -0.698165;0.715927;-0.003801;,
  -0.522440;0.850726;0.057640;,
  -0.084291;0.996344;0.013881;,
  0.417877;0.908483;0.006085;,
  0.585672;0.808836;-0.052654;,
  0.772120;0.572342;-0.276143;,
  0.546619;-0.261024;-0.795659;,
  0.214577;-0.510951;-0.832397;,
  -0.211737;-0.510181;-0.833597;,
  -0.543967;-0.260648;-0.797598;,
  -0.776216;0.385769;-0.498670;,
  -0.599115;0.799308;-0.046561;,
  -0.479925;0.877255;0.009800;,
  -0.170083;0.982545;-0.075341;,
  0.280866;0.957762;-0.061686;,
  0.499627;0.855220;-0.137738;,
  0.552600;0.806113;-0.211695;,
  0.425424;-0.204364;-0.881618;,
  0.007648;-0.312387;-0.949924;,
  -0.415839;-0.196226;-0.888016;,
  -0.470390;0.103025;-0.876424;,
  -0.499043;0.846591;-0.185039;,
  -0.396823;0.917131;-0.037453;,
  -0.054484;0.968656;-0.242359;,
  -0.438159;-0.895328;0.080028;,
  0.691370;-0.590403;0.416451;,
  0.813406;-0.254340;0.523147;,
  -0.436122;-0.865721;0.245611;,
  -0.782440;-0.081780;0.617333;,
  -0.614954;0.227456;0.755047;,
  0.872274;0.008716;0.488940;,
  -0.586899;0.382472;0.713628;,
  0.433417;-0.166594;-0.885662;,
  0.360073;-0.205241;-0.910068;,
  -0.663449;0.716975;-0.213970;,
  0.007466;-0.143626;-0.989604;;
  86;
  3;0,2,1;,
  3;0,3,2;,
  3;77,4,4;,
  3;78,5,79;,
  3;78,6,5;,
  3;77,7,80;,
  3;77,8,7;,
  3;81,9,8;,
  3;81,10,9;,
  3;0,82,10;,
  4;1,2,12,11;,
  4;2,83,13,12;,
  4;3,79,14,13;,
  4;79,5,15,14;,
  4;5,6,16,15;,
  4;80,7,18,17;,
  4;7,8,19,18;,
  4;8,9,20,19;,
  4;9,10,21,20;,
  4;10,82,84,21;,
  4;11,12,23,22;,
  4;12,13,24,23;,
  4;13,14,25,24;,
  4;14,15,26,25;,
  4;15,16,27,26;,
  4;16,17,28,27;,
  4;17,18,29,28;,
  4;18,19,30,29;,
  4;19,20,31,30;,
  4;20,21,32,31;,
  4;21,11,22,32;,
  4;22,23,34,33;,
  4;23,24,35,34;,
  4;24,25,36,35;,
  4;25,26,37,36;,
  4;26,27,38,37;,
  4;27,28,39,38;,
  4;28,29,40,39;,
  4;29,30,41,40;,
  4;30,31,42,41;,
  4;31,32,43,42;,
  4;32,22,33,43;,
  4;33,34,45,44;,
  4;34,35,46,45;,
  4;35,36,47,46;,
  4;36,37,48,47;,
  4;37,38,49,48;,
  4;38,39,50,49;,
  4;39,40,51,50;,
  4;40,41,52,51;,
  4;41,42,53,52;,
  4;42,43,54,53;,
  4;43,33,44,54;,
  4;44,45,56,55;,
  4;45,46,57,56;,
  4;46,47,58,57;,
  4;47,48,59,58;,
  4;48,49,60,59;,
  4;49,50,61,60;,
  4;50,51,62,61;,
  4;51,52,63,62;,
  4;52,53,64,63;,
  4;53,54,65,64;,
  4;54,44,55,65;,
  4;55,56,67,66;,
  4;56,57,68,67;,
  4;57,58,69,68;,
  4;85,59,70,86;,
  4;59,60,71,70;,
  4;61,62,72,71;,
  4;62,63,73,72;,
  4;63,64,74,87;,
  4;64,65,75,74;,
  4;65,55,66,75;,
  3;66,67,76;,
  3;67,68,76;,
  3;68,69,76;,
  3;86,70,88;,
  3;70,71,88;,
  3;71,72,88;,
  3;72,73,88;,
  3;73,74,76;,
  3;74,75,76;,
  3;75,66,76;,
  3;17,16,6;,
  3;71,60,61;;
 }
 MeshTextureCoords {
  104;
  0.045450;0.000000;,
  0.090910;0.125000;,
  0.000000;0.125000;,
  0.136360;0.000000;,
  0.181820;0.125000;,
  0.227270;0.000000;,
  0.272730;0.125000;,
  0.318180;0.000000;,
  0.363640;0.125000;,
  0.409090;0.000000;,
  0.454540;0.125000;,
  0.590910;0.000000;,
  0.636360;0.125000;,
  0.545450;0.125000;,
  0.681820;0.000000;,
  0.727270;0.125000;,
  0.772730;0.000000;,
  0.818180;0.125000;,
  0.863640;0.000000;,
  0.909090;0.125000;,
  0.954550;0.000000;,
  1.000000;0.125000;,
  0.090910;0.250000;,
  0.000000;0.250000;,
  0.181820;0.250000;,
  0.272730;0.250000;,
  0.363640;0.250000;,
  0.454540;0.250000;,
  0.636360;0.250000;,
  0.545450;0.250000;,
  0.727270;0.250000;,
  0.818180;0.250000;,
  0.909090;0.250000;,
  1.000000;0.250000;,
  0.090910;0.375000;,
  0.000000;0.375000;,
  0.181820;0.375000;,
  0.272730;0.375000;,
  0.363640;0.375000;,
  0.454540;0.375000;,
  0.545450;0.375000;,
  0.636360;0.375000;,
  0.727270;0.375000;,
  0.818180;0.375000;,
  0.909090;0.375000;,
  1.000000;0.375000;,
  0.090910;0.500000;,
  0.000000;0.500000;,
  0.181820;0.500000;,
  0.272730;0.500000;,
  0.363640;0.500000;,
  0.454540;0.500000;,
  0.545450;0.500000;,
  0.636360;0.500000;,
  0.727270;0.500000;,
  0.818180;0.500000;,
  0.909090;0.500000;,
  1.000000;0.500000;,
  0.090910;0.625000;,
  0.000000;0.625000;,
  0.181820;0.625000;,
  0.272730;0.625000;,
  0.363640;0.625000;,
  0.454540;0.625000;,
  0.545450;0.625000;,
  0.636360;0.625000;,
  0.727270;0.625000;,
  0.818180;0.625000;,
  0.909090;0.625000;,
  1.000000;0.625000;,
  0.090910;0.750000;,
  0.000000;0.750000;,
  0.181820;0.750000;,
  0.272730;0.750000;,
  0.363640;0.750000;,
  0.454540;0.750000;,
  0.545450;0.750000;,
  0.636360;0.750000;,
  0.727270;0.750000;,
  0.818180;0.750000;,
  0.909090;0.750000;,
  1.000000;0.750000;,
  0.090910;0.875000;,
  0.000000;0.875000;,
  0.181820;0.875000;,
  0.272730;0.875000;,
  0.363640;0.875000;,
  0.454540;0.875000;,
  0.636360;0.875000;,
  0.545450;0.875000;,
  0.727270;0.875000;,
  0.818180;0.875000;,
  0.909090;0.875000;,
  1.000000;0.875000;,
  0.045450;1.000000;,
  0.136360;1.000000;,
  0.227270;1.000000;,
  0.318180;1.000000;,
  0.409090;1.000000;,
  0.590910;1.000000;,
  0.681820;1.000000;,
  0.772730;1.000000;,
  0.863640;1.000000;,
  0.954550;1.000000;;
 }
 MeshVertexColors {
  104;
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
  69;1.000000;1.000000;1.000000;1.000000;,
  70;1.000000;1.000000;1.000000;1.000000;,
  71;1.000000;1.000000;1.000000;1.000000;,
  72;1.000000;1.000000;1.000000;1.000000;,
  73;1.000000;1.000000;1.000000;1.000000;,
  74;1.000000;1.000000;1.000000;1.000000;,
  75;1.000000;1.000000;1.000000;1.000000;,
  76;1.000000;1.000000;1.000000;1.000000;,
  77;1.000000;1.000000;1.000000;1.000000;,
  78;1.000000;1.000000;1.000000;1.000000;,
  79;1.000000;1.000000;1.000000;1.000000;,
  80;1.000000;1.000000;1.000000;1.000000;,
  81;1.000000;1.000000;1.000000;1.000000;,
  82;1.000000;1.000000;1.000000;1.000000;,
  83;1.000000;1.000000;1.000000;1.000000;,
  84;1.000000;1.000000;1.000000;1.000000;,
  85;1.000000;1.000000;1.000000;1.000000;,
  86;1.000000;1.000000;1.000000;1.000000;,
  87;1.000000;1.000000;1.000000;1.000000;,
  88;1.000000;1.000000;1.000000;1.000000;,
  89;1.000000;1.000000;1.000000;1.000000;,
  90;1.000000;1.000000;1.000000;1.000000;,
  91;1.000000;1.000000;1.000000;1.000000;,
  92;1.000000;1.000000;1.000000;1.000000;,
  93;1.000000;1.000000;1.000000;1.000000;,
  94;1.000000;1.000000;1.000000;1.000000;,
  95;1.000000;1.000000;1.000000;1.000000;,
  96;1.000000;1.000000;1.000000;1.000000;,
  97;1.000000;1.000000;1.000000;1.000000;,
  98;1.000000;1.000000;1.000000;1.000000;,
  99;1.000000;1.000000;1.000000;1.000000;,
  100;1.000000;1.000000;1.000000;1.000000;,
  101;1.000000;1.000000;1.000000;1.000000;,
  102;1.000000;1.000000;1.000000;1.000000;,
  103;1.000000;1.000000;1.000000;1.000000;;
 }
}