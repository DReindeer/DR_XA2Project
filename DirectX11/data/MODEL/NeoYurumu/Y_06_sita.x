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
 81;
 7.00000;1.00000;0.00000;,
 7.43934;7.62247;0.00000;,
 5.26041;7.62247;-5.26041;,
 4.94975;1.00000;-4.94975;,
 9.91866;10.96999;0.00000;,
 6.01041;6.97490;-6.01041;,
 13.84909;5.78124;0.00000;,
 8.96536;8.68834;-9.95011;,
 11.50963;1.00000;0.00000;,
 8.37809;1.00000;-7.07107;,
 9.56066;-0.06066;-0.00000;,
 6.76041;-0.06066;-6.76041;,
 8.50000;-0.50000;-0.00000;,
 6.01041;-0.50000;-6.01041;,
 7.43934;-0.06066;-0.00000;,
 5.26041;-0.06066;-5.26041;,
 7.00000;1.00000;0.00000;,
 4.94975;1.00000;-4.94975;,
 -0.00000;4.44187;-7.43934;,
 -0.00000;1.00000;-7.00000;,
 -0.00000;5.85866;-8.50000;,
 -0.00000;4.33708;-14.51007;,
 -0.00000;1.00000;-11.35487;,
 -0.00000;-0.06066;-9.56066;,
 -0.00000;-0.50000;-8.50000;,
 -0.00000;-0.06066;-7.43934;,
 -0.00000;1.00000;-7.00000;,
 -5.26041;7.62247;-5.26041;,
 -4.94975;1.00000;-4.94975;,
 -6.01041;6.97490;-6.01041;,
 -8.96536;8.68834;-9.95011;,
 -8.37809;1.00000;-7.07107;,
 -6.76041;-0.06066;-6.76041;,
 -6.01041;-0.50000;-6.01041;,
 -5.26041;-0.06066;-5.26041;,
 -4.94975;1.00000;-4.94975;,
 -7.43934;7.62247;0.00000;,
 -7.00000;1.00000;0.00000;,
 -9.91866;10.96999;0.00000;,
 -13.84909;5.78124;0.00000;,
 -11.50963;1.00000;0.00000;,
 -9.56066;-0.06066;0.00000;,
 -8.50000;-0.50000;0.00000;,
 -7.43934;-0.06066;0.00000;,
 -7.00000;1.00000;0.00000;,
 -5.26041;4.67864;5.26041;,
 -4.94975;1.00000;4.94975;,
 -6.01041;4.79119;6.01041;,
 -8.41010;7.43976;13.28901;,
 -9.41376;1.00000;8.39494;,
 -6.76041;-0.06066;6.76041;,
 -6.01041;-0.50000;6.01041;,
 -5.26041;-0.06066;5.26041;,
 -4.94975;1.00000;4.94975;,
 0.00000;2.06066;7.43934;,
 0.00000;1.00000;7.00000;,
 0.00000;2.50000;8.50000;,
 0.00000;2.06066;9.56066;,
 0.00000;-0.22467;12.73879;,
 0.00000;-0.52077;9.56066;,
 0.00000;-0.54191;8.50000;,
 0.00000;-0.06066;7.43934;,
 0.00000;1.00000;7.00000;,
 5.26041;4.67864;5.26041;,
 4.94975;1.00000;4.94975;,
 6.01041;4.79119;6.01041;,
 8.41010;7.43976;13.28901;,
 9.41376;1.00000;8.39494;,
 6.76041;-0.06066;6.76041;,
 6.01041;-0.50000;6.01041;,
 5.26041;-0.06066;5.26041;,
 4.94975;1.00000;4.94975;,
 7.43934;7.62247;0.00000;,
 7.00000;1.00000;0.00000;,
 9.91866;10.96999;0.00000;,
 13.84909;5.78124;0.00000;,
 11.50963;1.00000;0.00000;,
 9.56066;-0.06066;-0.00000;,
 8.50000;-0.50000;-0.00000;,
 7.43934;-0.06066;-0.00000;,
 7.00000;1.00000;0.00000;;
 
 64;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;28,27,36,37;,
 4;27,29,38,36;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;34,35,44,43;,
 4;37,36,45,46;,
 4;36,38,47,45;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;43,44,53,52;,
 4;46,45,54,55;,
 4;45,47,56,54;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;52,53,62,61;,
 4;55,54,63,64;,
 4;54,56,65,63;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 4;61,62,71,70;,
 4;64,63,72,73;,
 4;63,65,74,72;,
 4;65,66,75,74;,
 4;66,67,76,75;,
 4;67,68,77,76;,
 4;68,69,78,77;,
 4;69,70,79,78;,
 4;70,71,80,79;;
 
 MeshMaterialList {
  5;
  64;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/NeoYurumu/yuruBody.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/NeoYurumu/yuruBody2.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/NeoYurumu/yuruEye.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/NeoYurumu/leye.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/NeoYurumu/reye.png";
   }
  }
 }
 MeshNormals {
  83;
  -0.987608;-0.156883;0.004296;,
  -0.937781;0.337472;0.081726;,
  -0.167515;0.984998;0.041447;,
  0.249981;0.966015;0.065756;,
  0.738550;-0.671684;-0.058189;,
  0.414432;-0.909815;-0.021966;,
  0.000000;-1.000000;-0.000000;,
  -0.698872;-0.715247;-0.000000;,
  -0.701758;-0.155860;0.695157;,
  -0.711176;0.081288;0.698298;,
  -0.200417;0.974688;-0.099081;,
  0.008297;0.991686;-0.128417;,
  0.563204;-0.643712;-0.518108;,
  0.330572;-0.881433;-0.337340;,
  0.000000;-1.000000;-0.000000;,
  -0.494177;-0.715247;0.494177;,
  0.000000;-0.149078;0.988825;,
  0.000000;0.097643;0.995222;,
  0.000000;0.999810;0.019501;,
  0.000000;0.999944;-0.010589;,
  -0.000000;-0.687784;-0.725915;,
  -0.000000;-0.870362;-0.492412;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.715247;0.698872;,
  0.701758;-0.155860;0.695157;,
  0.711176;0.081288;0.698298;,
  0.200417;0.974688;-0.099081;,
  -0.008297;0.991686;-0.128417;,
  -0.563204;-0.643712;-0.518108;,
  -0.330572;-0.881433;-0.337340;,
  0.000000;-1.000000;-0.000000;,
  0.494177;-0.715247;0.494177;,
  0.987608;-0.156883;0.004296;,
  0.937781;0.337472;0.081726;,
  0.167515;0.984998;0.041447;,
  -0.249981;0.966015;0.065756;,
  -0.738550;-0.671684;-0.058188;,
  -0.414432;-0.909815;-0.021966;,
  0.000000;-1.000000;-0.000000;,
  0.698872;-0.715247;-0.000000;,
  0.712324;-0.109932;-0.693188;,
  0.723311;0.597268;-0.346545;,
  0.383853;0.912424;0.141914;,
  0.201545;0.953009;0.226171;,
  -0.543303;-0.809830;0.221354;,
  -0.266627;-0.950029;0.162344;,
  0.010668;-0.998568;-0.052425;,
  0.494593;-0.712128;-0.498248;,
  0.000000;-0.068859;-0.997626;,
  0.000000;0.257620;-0.966246;,
  0.000000;0.999873;-0.015931;,
  0.531270;0.844974;0.061405;,
  0.000000;0.143667;0.989626;,
  0.000000;-0.982789;0.184732;,
  0.000000;-0.994682;-0.102995;,
  -0.000000;-0.708377;-0.705834;,
  -0.712324;-0.109933;-0.693188;,
  -0.714499;0.174257;-0.677589;,
  -0.383853;0.912424;0.141914;,
  -0.201545;0.953009;0.226171;,
  0.137418;0.142304;0.980238;,
  0.266627;-0.950029;0.162344;,
  -0.010668;-0.998568;-0.052425;,
  -0.494593;-0.712128;-0.498248;,
  -0.431802;0.883815;-0.180051;,
  -0.390676;0.918083;-0.067060;,
  0.939547;-0.336264;-0.064633;,
  0.694454;-0.405975;-0.594069;,
  0.000000;0.998748;0.050021;,
  0.509311;-0.461147;-0.726599;,
  0.390676;0.918083;-0.067060;,
  -0.509311;-0.461147;-0.726599;,
  -0.694454;-0.405975;-0.594069;,
  0.431802;0.883815;-0.180051;,
  -0.939547;-0.336264;-0.064633;,
  -0.903830;-0.296126;0.308869;,
  0.000000;0.995120;-0.098672;,
  -0.137418;0.142304;0.980238;,
  0.000000;-0.985884;0.167432;,
  -0.541197;0.838969;0.056898;,
  -0.531270;0.844974;0.061405;,
  0.281445;-0.948627;0.144555;,
  0.903830;-0.296126;0.308869;;
  64;
  4;0,1,9,8;,
  4;64,2,10,65;,
  4;2,3,11,10;,
  4;66,4,12,67;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;65,10,18,68;,
  4;10,11,19,18;,
  4;67,12,20,69;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;68,18,26,70;,
  4;18,19,27,26;,
  4;71,20,28,72;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  4;24,25,33,32;,
  4;70,26,34,73;,
  4;26,27,35,34;,
  4;72,28,36,74;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,24,32,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;74,36,44,75;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,32,40,47;,
  4;40,41,49,48;,
  4;41,42,50,76;,
  4;42,43,51,50;,
  4;77,77,52,52;,
  4;44,45,53,78;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,40,48,55;,
  4;48,49,57,56;,
  4;76,50,58,79;,
  4;50,80,59,58;,
  4;52,52,60,60;,
  4;78,53,61,81;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,48,56,63;,
  4;56,57,1,0;,
  4;79,58,2,1;,
  4;58,59,3,2;,
  4;82,82,4,66;,
  4;81,61,5,4;,
  4;61,62,6,5;,
  4;62,63,7,6;,
  4;63,56,0,7;;
 }
 MeshTextureCoords {
  81;
  0.000000;0.000000;,
  0.000000;0.125000;,
  0.125000;0.125000;,
  0.125000;0.000000;,
  0.000000;0.250000;,
  0.125000;0.250000;,
  0.000000;0.375000;,
  0.125000;0.375000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.000000;0.625000;,
  0.125000;0.625000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.000000;0.875000;,
  0.125000;0.875000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.250000;0.125000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.250000;0.375000;,
  0.250000;0.500000;,
  0.250000;0.625000;,
  0.250000;0.750000;,
  0.250000;0.875000;,
  0.250000;1.000000;,
  0.375000;0.125000;,
  0.375000;0.000000;,
  0.375000;0.250000;,
  0.375000;0.375000;,
  0.375000;0.500000;,
  0.375000;0.625000;,
  0.375000;0.750000;,
  0.375000;0.875000;,
  0.375000;1.000000;,
  0.500000;0.125000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.500000;0.375000;,
  0.500000;0.500000;,
  0.500000;0.625000;,
  0.500000;0.750000;,
  0.500000;0.875000;,
  0.500000;1.000000;,
  0.625000;0.125000;,
  0.625000;0.000000;,
  0.625000;0.250000;,
  0.625000;0.375000;,
  0.625000;0.500000;,
  0.625000;0.625000;,
  0.625000;0.750000;,
  0.625000;0.875000;,
  0.625000;1.000000;,
  0.750000;0.125000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  0.750000;0.375000;,
  0.750000;0.500000;,
  0.750000;0.625000;,
  0.750000;0.750000;,
  0.750000;0.875000;,
  0.750000;1.000000;,
  0.875000;0.125000;,
  0.875000;0.000000;,
  0.875000;0.250000;,
  0.875000;0.375000;,
  0.875000;0.500000;,
  0.875000;0.625000;,
  0.875000;0.750000;,
  0.875000;0.875000;,
  0.875000;1.000000;,
  1.000000;0.125000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  1.000000;0.375000;,
  1.000000;0.500000;,
  1.000000;0.625000;,
  1.000000;0.750000;,
  1.000000;0.875000;,
  1.000000;1.000000;;
 }
 MeshVertexColors {
  81;
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
  80;1.000000;1.000000;1.000000;1.000000;;
 }
}