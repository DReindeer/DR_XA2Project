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
 54;
 0.00753;-0.55466;-0.69088;,
 0.57553;-0.52481;-0.45665;,
 0.00772;-0.52639;-0.13678;,
 0.00753;-0.55466;-0.69088;,
 0.30710;-0.63960;-1.16390;,
 0.00753;-0.55466;-0.69088;,
 -0.30710;-0.63960;-1.16390;,
 0.00753;-0.55466;-0.69088;,
 -0.57553;-0.52481;-0.45665;,
 0.00753;-0.55466;-0.69088;,
 0.00772;-0.52639;-0.13678;,
 -0.62423;-0.67538;-1.82965;,
 0.62423;-0.67538;-1.82965;,
 0.00577;-0.49236;0.66587;,
 0.69141;-0.34953;0.63857;,
 1.06397;0.50863;-0.01744;,
 -0.00657;0.43874;0.55068;,
 0.62423;-0.67538;-1.82965;,
 0.49775;-0.11153;-1.97866;,
 -0.62423;-0.67538;-1.82965;,
 -0.49775;-0.11153;-1.97866;,
 -0.69141;-0.34953;0.63857;,
 -1.06397;0.50863;-0.01744;,
 0.00577;-0.49236;0.66587;,
 -0.00657;0.43874;0.55068;,
 0.49775;-0.11153;-1.97866;,
 -0.49775;-0.11153;-1.97866;,
 -0.41849;0.01816;-1.74044;,
 0.41849;0.01816;-1.74044;,
 -0.32119;0.35372;-0.92466;,
 0.32119;0.35372;-0.92466;,
 -0.00657;0.43874;0.55068;,
 1.06397;0.50863;-0.01744;,
 -0.00770;0.62784;-0.24639;,
 -0.00770;0.62784;-0.24639;,
 -0.00770;0.62784;-0.24639;,
 -1.06397;0.50863;-0.01744;,
 -0.00770;0.62784;-0.24639;,
 -0.00657;0.43874;0.55068;,
 -0.00770;0.62784;-0.24639;,
 1.06397;0.50863;-0.01744;,
 1.06397;0.50863;-0.01744;,
 -1.06397;0.50863;-0.01744;,
 -1.06397;0.50863;-0.01744;,
 0.57553;-0.52481;-0.45665;,
 0.00577;-0.49236;0.66587;,
 -0.57553;-0.52481;-0.45665;,
 0.00577;-0.49236;0.66587;,
 0.62423;-0.67538;-1.82965;,
 0.57553;-0.52481;-0.45665;,
 -0.62423;-0.67538;-1.82965;,
 -0.57553;-0.52481;-0.45665;,
 0.00577;-0.49236;0.66587;,
 0.00577;-0.49236;0.66587;;
 
 30;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 4;4,6,11,12;,
 4;13,14,15,16;,
 4;14,17,18,15;,
 4;17,19,20,18;,
 4;19,21,22,20;,
 4;21,23,24,22;,
 4;25,26,27,28;,
 4;28,27,29,30;,
 3;31,32,33;,
 3;32,30,34;,
 3;30,29,35;,
 3;29,36,37;,
 3;36,38,39;,
 3;40,28,30;,
 3;41,25,28;,
 3;42,29,27;,
 3;43,27,26;,
 3;44,45,2;,
 3;44,4,12;,
 3;46,11,6;,
 3;46,10,47;,
 3;48,14,49;,
 3;50,51,21;,
 3;52,49,14;,
 3;53,21,51;;
 
 MeshMaterialList {
  6;
  30;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/shepherd/shepherd_tongue.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/shepherd/shepherd_body.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/shepherd/shepherd_body2.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/shepherd/shepherd_tume.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/shepherd/shepherd_eye.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/shepherd/shepherd_eye.png";
   }
  }
 }
 MeshNormals {
  28;
  0.000062;-0.992471;0.122483;,
  0.000399;-0.998911;0.046657;,
  0.118893;-0.987423;0.104214;,
  0.027134;-0.991442;0.127699;,
  -0.027262;-0.991444;0.127651;,
  -0.118155;-0.987509;0.104236;,
  -0.001236;0.299295;0.954160;,
  0.154487;-0.983463;0.094524;,
  -0.154487;-0.983463;0.094524;,
  0.213604;0.293045;0.931932;,
  -0.979576;-0.132995;-0.150806;,
  -0.000941;0.735985;0.676997;,
  0.241883;0.969622;0.036406;,
  0.979576;-0.132995;-0.150806;,
  0.000000;-0.255501;-0.966809;,
  0.140017;0.906686;-0.397889;,
  -0.140017;0.906686;-0.397889;,
  0.100923;0.930580;-0.351902;,
  -0.101621;0.930305;-0.352428;,
  -0.000751;0.994230;-0.107267;,
  -0.216018;0.291571;0.931838;,
  -0.158584;0.726459;0.668662;,
  0.162579;0.888306;-0.429512;,
  -0.162579;0.888306;-0.429512;,
  -0.242154;0.909036;-0.339138;,
  0.001027;-0.996036;0.088941;,
  0.271023;-0.954541;0.124090;,
  -0.269408;-0.954967;0.124329;;
  30;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,1,5;,
  4;3,4,8,7;,
  4;6,9,12,11;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;10,10,10,10;,
  4;20,6,11,21;,
  4;22,23,16,15;,
  4;15,16,18,17;,
  3;11,12,19;,
  3;12,17,19;,
  3;17,18,19;,
  3;18,24,19;,
  3;21,11,19;,
  3;12,15,17;,
  3;12,22,15;,
  3;24,18,16;,
  3;24,16,23;,
  3;2,25,1;,
  3;2,3,7;,
  3;5,8,4;,
  3;5,1,25;,
  3;7,26,2;,
  3;8,5,27;,
  3;25,2,26;,
  3;25,27,5;;
 }
 MeshTextureCoords {
  54;
  0.100000;0.000000;,
  0.200000;0.125000;,
  0.000000;0.125000;,
  0.300000;0.000000;,
  0.400000;0.125000;,
  0.500000;0.000000;,
  0.600000;0.125000;,
  0.700000;0.000000;,
  0.800000;0.125000;,
  0.900000;0.000000;,
  1.000000;0.125000;,
  0.600000;0.250000;,
  0.400000;0.250000;,
  0.000000;0.375000;,
  0.200000;0.375000;,
  0.200000;0.500000;,
  0.000000;0.500000;,
  0.400000;0.375000;,
  0.400000;0.500000;,
  0.600000;0.375000;,
  0.600000;0.500000;,
  0.800000;0.375000;,
  0.800000;0.500000;,
  1.000000;0.375000;,
  1.000000;0.500000;,
  0.400000;0.625000;,
  0.600000;0.625000;,
  0.600000;0.750000;,
  0.400000;0.750000;,
  0.600000;0.875000;,
  0.400000;0.875000;,
  0.000000;0.875000;,
  0.200000;0.875000;,
  0.100000;1.000000;,
  0.300000;1.000000;,
  0.500000;1.000000;,
  0.800000;0.875000;,
  0.700000;1.000000;,
  1.000000;0.875000;,
  0.900000;1.000000;,
  0.200000;0.812500;,
  0.200000;0.687500;,
  0.800000;0.812500;,
  0.800000;0.687500;,
  0.200000;0.187500;,
  0.000000;0.250000;,
  0.800000;0.187500;,
  1.000000;0.250000;,
  0.400000;0.312500;,
  0.200000;0.250000;,
  0.600000;0.312500;,
  0.800000;0.250000;,
  0.000000;0.312500;,
  1.000000;0.312500;;
 }
 MeshVertexColors {
  54;
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
  53;1.000000;1.000000;1.000000;1.000000;;
 }
}
