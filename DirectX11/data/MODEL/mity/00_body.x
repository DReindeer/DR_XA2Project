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
 132;
 0.00000;6.93581;0.66025;,
 0.64598;6.69669;-0.91265;,
 0.00000;6.69669;-1.10753;,
 0.00000;6.48233;1.32478;,
 0.64598;6.37936;1.23727;,
 -0.64598;6.37936;1.23727;,
 -0.64598;6.69669;-0.91265;,
 1.05188;6.23336;-1.55896;,
 0.00000;6.23336;-1.83457;,
 1.40873;6.69669;-0.34155;,
 2.03151;6.69669;0.13926;,
 2.76112;6.23336;-0.26664;,
 2.22654;6.23336;-1.10769;,
 2.22638;6.69669;0.66025;,
 2.92090;6.23336;0.66025;,
 2.03151;6.69669;0.87724;,
 2.76112;6.23336;1.28314;,
 1.40537;6.43358;1.34734;,
 2.20910;6.23580;1.54985;,
 0.00000;6.12599;1.84947;,
 1.05335;6.10442;1.83820;,
 -1.05335;6.10442;1.83820;,
 -2.03151;6.69669;0.87724;,
 -2.22638;6.69669;0.66025;,
 -2.92090;6.23336;0.66025;,
 -2.76112;6.23336;1.28314;,
 -2.03151;6.69669;0.13926;,
 -2.76112;6.23336;-0.26664;,
 -1.40873;6.69669;-0.34155;,
 -2.22654;6.23336;-1.10769;,
 -1.05188;6.23336;-1.55896;,
 1.22000;5.43581;-1.96485;,
 0.00000;5.43581;-2.27391;,
 3.16702;5.43581;-0.43477;,
 2.53720;5.43581;-1.41835;,
 3.39476;5.43581;0.66025;,
 3.16702;5.43581;1.45127;,
 -3.39476;5.43581;0.66025;,
 -3.16702;5.43581;1.45127;,
 -3.16702;5.43581;-0.43477;,
 -2.53720;5.43581;-1.41835;,
 -1.22000;5.43581;-1.96485;,
 3.16702;1.59918;1.45127;,
 3.47608;1.59918;0.66025;,
 2.53720;5.43581;1.74296;,
 2.53720;1.59918;1.74296;,
 -2.53720;5.43581;1.74296;,
 -3.16702;1.59918;1.45127;,
 -2.53720;1.59918;1.74296;,
 -3.47608;1.59918;0.66025;,
 3.16702;1.59918;-0.43477;,
 3.03674;0.53852;0.66025;,
 2.76112;0.53852;-0.26664;,
 2.76112;0.53852;1.28314;,
 2.22654;0.53852;1.43230;,
 -2.76112;0.53852;1.28314;,
 -2.22654;0.53852;1.43230;,
 -3.03674;0.53852;0.66025;,
 -3.16702;1.59918;-0.43477;,
 -2.76112;0.53852;-0.26664;,
 2.22654;0.53852;-1.10769;,
 2.03151;0.09918;0.13926;,
 1.36743;0.09918;-0.63542;,
 2.22638;0.09918;0.66025;,
 2.03151;0.09918;0.87724;,
 1.41126;0.09918;1.34951;,
 1.05188;0.53852;1.73937;,
 0.00000;0.53852;2.01498;,
 0.00000;0.09918;1.22340;,
 0.64598;0.09918;1.23727;,
 -1.05188;0.53852;1.73937;,
 -0.64598;0.09918;1.23727;,
 -2.03151;0.09918;0.87724;,
 -1.41126;0.09918;1.34951;,
 -2.22638;0.09918;0.66025;,
 -2.03151;0.09918;0.13926;,
 -2.22654;0.53852;-1.10769;,
 -1.36743;0.09918;-0.63542;,
 0.00000;-0.01003;0.66025;,
 1.65138;0.53852;1.58265;,
 -1.65138;0.53852;1.58265;,
 1.35876;0.09918;0.31179;,
 1.36916;0.09918;0.66025;,
 1.37354;0.09918;0.80696;,
 -1.37354;0.09918;0.80696;,
 -1.36916;0.09918;0.66025;,
 -1.35876;0.09918;0.31179;,
 1.38441;6.76515;0.30521;,
 1.38345;6.76515;0.66025;,
 1.38306;6.76515;0.80798;,
 -1.40537;6.43358;1.34734;,
 -1.38306;6.76515;0.80798;,
 -1.38345;6.76515;0.66025;,
 -1.38441;6.76515;0.30521;,
 1.64241;6.23336;-1.33209;,
 1.40537;6.43358;1.34734;,
 -1.69450;6.18996;1.76595;,
 -1.64241;6.23336;-1.33209;,
 2.21380;5.43581;-1.55253;,
 -2.21380;5.43581;-1.55253;,
 -0.64598;0.09918;-0.91265;,
 0.64598;0.09918;-0.91265;,
 2.23574;1.55624;1.81759;,
 1.14354;1.61082;2.15179;,
 -2.23574;1.55624;1.81759;,
 -1.14354;1.61082;2.15179;,
 2.18091;5.43581;1.85178;,
 -2.18091;5.43581;1.85178;,
 0.85129;0.32141;-1.23957;,
 0.00000;0.09918;-1.05117;,
 -0.85129;0.32141;-1.23957;,
 1.56642;0.53734;-1.35912;,
 -1.56642;0.53734;-1.35912;,
 -2.53720;1.59918;-1.41835;,
 -2.17499;1.59918;-1.56863;,
 -1.07372;0.54141;-1.58518;,
 2.17499;1.59918;-1.56863;,
 2.53720;1.59918;-1.41835;,
 1.07372;0.54141;-1.58518;,
 0.00000;0.59425;-1.85765;,
 0.00000;1.59918;-2.27391;,
 1.22000;1.59918;-1.96485;,
 1.22000;5.43581;2.14526;,
 0.00000;5.43581;2.24932;,
 0.00000;1.59918;2.40668;,
 0.00000;0.21714;3.23552;,
 -1.22000;5.43581;2.14526;,
 -1.22000;1.59918;-1.96485;,
 1.69450;6.18996;1.76595;,
 -2.20910;6.23580;1.54985;,
 2.63534;1.59918;-1.26509;,
 -2.63534;1.59918;-1.26509;;
 
 160;
 3;0,1,2;,
 3;0,3,4;,
 3;0,5,3;,
 3;0,2,6;,
 4;2,1,7,8;,
 4;9,10,11,12;,
 4;10,13,14,11;,
 4;13,15,16,14;,
 4;15,17,18,16;,
 4;4,3,19,20;,
 4;3,5,21,19;,
 4;22,23,24,25;,
 4;23,26,27,24;,
 4;26,28,29,27;,
 4;6,2,8,30;,
 4;8,7,31,32;,
 4;12,11,33,34;,
 4;11,14,35,33;,
 4;14,16,36,35;,
 4;25,24,37,38;,
 4;24,27,39,37;,
 4;27,29,40,39;,
 4;30,8,32,41;,
 4;35,36,42,43;,
 4;36,44,45,42;,
 4;46,38,47,48;,
 4;38,37,49,47;,
 4;50,43,51,52;,
 4;43,42,53,51;,
 4;42,45,54,53;,
 4;48,47,55,56;,
 4;47,49,57,55;,
 4;49,58,59,57;,
 4;60,52,61,62;,
 4;52,51,63,61;,
 4;51,53,64,63;,
 4;53,54,65,64;,
 4;66,67,68,69;,
 4;67,70,71,68;,
 4;56,55,72,73;,
 4;55,57,74,72;,
 4;57,59,75,74;,
 4;59,76,77,75;,
 3;69,68,78;,
 3;68,71,78;,
 4;79,66,69,65;,
 4;73,71,70,80;,
 3;81,78,62;,
 3;82,78,81;,
 4;81,61,63,82;,
 3;83,78,82;,
 4;82,63,64,83;,
 3;65,78,83;,
 3;84,78,73;,
 3;85,78,84;,
 4;84,72,74,85;,
 3;86,78,85;,
 4;85,74,75,86;,
 3;77,78,86;,
 3;9,0,87;,
 4;88,13,10,87;,
 3;87,0,88;,
 4;89,15,13,88;,
 3;88,0,89;,
 3;89,0,17;,
 3;90,0,91;,
 4;92,23,22,91;,
 3;91,0,92;,
 4;93,26,23,92;,
 3;92,0,93;,
 3;93,0,28;,
 4;94,7,1,9;,
 4;17,4,20,95;,
 4;96,21,5,90;,
 4;28,6,30,97;,
 4;94,12,34,98;,
 4;99,40,29,97;,
 4;97,30,41,99;,
 4;98,31,7,94;,
 3;97,29,28;,
 3;9,12,94;,
 3;28,26,93;,
 3;0,6,28;,
 3;9,1,0;,
 3;87,10,9;,
 3;95,18,17;,
 3;90,5,0;,
 3;91,22,90;,
 3;17,15,89;,
 3;0,4,17;,
 3;86,75,77;,
 3;77,100,78;,
 3;78,101,62;,
 3;62,61,81;,
 3;65,54,79;,
 3;80,56,73;,
 3;83,64,65;,
 3;65,69,78;,
 3;78,71,73;,
 3;73,72,84;,
 4;102,103,66,79;,
 4;79,54,45,102;,
 4;104,48,56,80;,
 4;80,70,105,104;,
 4;106,102,45,44;,
 4;46,48,104,107;,
 3;108,101,109;,
 3;109,100,110;,
 4;111,62,101,108;,
 4;110,100,77,112;,
 3;77,76,112;,
 3;111,60,62;,
 4;76,113,114,112;,
 3;112,115,110;,
 4;111,116,117,60;,
 3;108,118,111;,
 4;110,115,119,109;,
 4;109,119,118,108;,
 3;78,100,109;,
 3;109,101,78;,
 4;119,120,121,118;,
 4;103,122,123,124;,
 4;103,102,106,122;,
 4;125,67,66,103;,
 4;126,107,104,105;,
 3;105,125,124;,
 4;124,123,126,105;,
 4;125,105,70,67;,
 3;103,124,125;,
 4;121,116,111,118;,
 4;115,112,114,127;,
 4;127,120,119,115;,
 4;128,106,44,18;,
 4;44,36,16,18;,
 3;18,95,128;,
 4;129,25,38,46;,
 3;128,95,20;,
 4;96,107,126,21;,
 4;126,123,19,21;,
 4;122,106,128,20;,
 4;20,19,123,122;,
 4;90,22,25,129;,
 3;96,90,129;,
 4;129,46,107,96;,
 3;52,60,50;,
 3;60,117,50;,
 3;117,130,50;,
 3;59,58,76;,
 3;76,58,113;,
 3;58,131,113;,
 4;35,43,50,33;,
 4;33,50,117,34;,
 4;37,39,58,49;,
 4;39,40,113,58;,
 4;40,99,114,113;,
 4;99,41,127,114;,
 4;41,32,120,127;,
 4;32,31,121,120;,
 4;31,98,116,121;,
 4;98,34,117,116;;
 
 MeshMaterialList {
  4;
  160;
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
  0,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
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
  1,
  1,
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
  0,
  1,
  0,
  1,
  1,
  1,
  0,
  1,
  0,
  1,
  1,
  0,
  1,
  0,
  1,
  1,
  1,
  0,
  1,
  0,
  1,
  1,
  1,
  0,
  0,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  0,
  0,
  1,
  0,
  1,
  1,
  0,
  0,
  0,
  0,
  1,
  1,
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
  0,
  0,
  0,
  1,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  1,
  0,
  1,
  0,
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
  1,
  1,
  1,
  1,
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
  134;
  -0.000000;0.979319;0.202320;,
  0.000000;0.941546;-0.336883;,
  0.128058;0.943667;-0.305113;,
  0.162988;0.957055;-0.239752;,
  0.256473;0.956815;-0.136846;,
  0.298177;0.953960;0.032427;,
  0.193999;0.918456;0.344678;,
  -0.009692;0.817726;0.575526;,
  0.007713;0.810045;0.586318;,
  0.000000;0.851761;0.523930;,
  -0.017744;0.806453;0.591031;,
  -0.016715;0.829730;0.557914;,
  -0.193999;0.918456;0.344678;,
  -0.298177;0.953960;0.032427;,
  -0.256473;0.956815;-0.136846;,
  -0.162988;0.957055;-0.239752;,
  -0.128058;0.943667;-0.305113;,
  0.000000;0.691832;-0.722058;,
  0.239401;0.693235;-0.679789;,
  0.416117;0.737969;-0.531270;,
  0.639811;0.716602;-0.277711;,
  0.704268;0.708505;0.045029;,
  0.509670;0.697964;0.503073;,
  0.258224;0.644264;0.719892;,
  0.029757;0.718285;0.695113;,
  0.000000;0.700061;0.714083;,
  -0.061686;0.673646;0.736475;,
  -0.258224;0.644264;0.719892;,
  -0.509670;0.697964;0.503073;,
  -0.704268;0.708505;0.045029;,
  -0.639811;0.716602;-0.277711;,
  -0.416117;0.737969;-0.531270;,
  -0.239401;0.693235;-0.679789;,
  0.000000;0.252359;-0.967634;,
  0.303602;0.245611;-0.920598;,
  0.618922;0.250865;-0.744313;,
  0.893165;0.255644;-0.370004;,
  0.963203;0.265822;0.039736;,
  0.728088;0.242691;0.641084;,
  0.355024;0.177205;0.917909;,
  0.168349;0.216099;0.961748;,
  0.000000;0.251921;0.967748;,
  -0.168349;0.216099;0.961748;,
  -0.355024;0.177205;0.917909;,
  -0.728088;0.242691;0.641084;,
  -0.963203;0.265822;0.039736;,
  -0.893165;0.255644;-0.370004;,
  -0.618922;0.250865;-0.744313;,
  -0.303602;0.245611;-0.920598;,
  0.000000;-0.190845;-0.981620;,
  0.311522;-0.186320;-0.931794;,
  0.626707;-0.195909;-0.754227;,
  0.878567;-0.230320;-0.418416;,
  0.979743;-0.194283;0.048566;,
  0.705330;-0.187296;0.683688;,
  0.322702;-0.168607;0.931362;,
  0.329145;-0.116961;0.937008;,
  0.000000;0.275907;0.961185;,
  -0.329145;-0.116961;0.937008;,
  -0.322702;-0.168607;0.931362;,
  -0.705330;-0.187296;0.683688;,
  -0.979743;-0.194283;0.048566;,
  -0.878567;-0.230320;-0.418416;,
  -0.626707;-0.195909;-0.754227;,
  -0.311522;-0.186320;-0.931794;,
  0.000000;-0.653577;-0.756860;,
  0.233739;-0.655295;-0.718300;,
  0.533684;-0.627425;-0.567026;,
  0.665484;-0.677597;-0.313039;,
  0.722017;-0.689463;0.057731;,
  0.512840;-0.648460;0.562578;,
  0.274504;-0.517418;0.810510;,
  0.252315;-0.665819;0.702155;,
  0.000000;-0.838852;0.544360;,
  -0.252315;-0.665819;0.702155;,
  -0.274504;-0.517418;0.810510;,
  -0.512840;-0.648460;0.562578;,
  -0.722017;-0.689463;0.057731;,
  -0.665484;-0.677597;-0.313039;,
  -0.533684;-0.627425;-0.567026;,
  -0.233739;-0.655295;-0.718300;,
  0.000000;-0.928443;-0.371476;,
  0.084316;-0.954884;-0.284759;,
  0.141212;-0.965812;-0.217408;,
  0.208913;-0.972125;-0.106431;,
  0.225240;-0.973883;0.028622;,
  0.178177;-0.961420;0.209582;,
  0.106099;-0.899611;0.423607;,
  0.023435;-0.894500;0.446453;,
  0.000000;-0.929264;0.369417;,
  -0.023435;-0.894500;0.446453;,
  -0.106099;-0.899611;0.423607;,
  -0.178177;-0.961420;0.209582;,
  -0.225240;-0.973883;0.028622;,
  -0.208913;-0.972125;-0.106431;,
  -0.141212;-0.965812;-0.217408;,
  -0.084316;-0.954884;-0.284759;,
  0.000000;-0.999387;0.034997;,
  0.377221;-0.184595;-0.907540;,
  0.269206;-0.180898;0.945941;,
  -0.269206;-0.180898;0.945941;,
  -0.377221;-0.184595;-0.907540;,
  0.269238;-0.704836;-0.656291;,
  0.194496;-0.478067;0.856518;,
  -0.194496;-0.478067;0.856518;,
  -0.269238;-0.704836;-0.656291;,
  0.039986;-0.999200;-0.000410;,
  0.039787;-0.999207;-0.001188;,
  0.039872;-0.999203;-0.001983;,
  -0.039872;-0.999203;-0.001983;,
  -0.039787;-0.999207;-0.001188;,
  -0.039986;-0.999200;-0.000410;,
  0.366474;0.239779;-0.899001;,
  0.262518;0.172555;0.949373;,
  -0.262518;0.172555;0.949373;,
  -0.366474;0.239779;-0.899001;,
  0.271835;0.713445;-0.645835;,
  -0.026215;0.871937;0.488916;,
  -0.131291;0.654796;0.744315;,
  -0.271835;0.713445;-0.645835;,
  -0.077478;0.956830;0.280130;,
  -0.107754;0.994162;0.005521;,
  -0.097725;0.993718;-0.054535;,
  0.097725;0.993718;-0.054535;,
  0.107754;0.994162;0.005521;,
  0.077478;0.956830;0.280130;,
  0.144823;-0.863838;-0.482503;,
  -0.144823;-0.863838;-0.482503;,
  0.581071;-0.677256;0.451311;,
  0.141344;0.649496;0.747112;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;0.000000;,
  0.000000;0.514321;0.857598;,
  -0.581071;-0.677256;0.451311;;
  160;
  3;0,2,1;,
  3;0,9,8;,
  3;0,10,9;,
  3;0,1,16;,
  4;1,2,18,17;,
  4;3,4,20,19;,
  4;4,5,21,20;,
  4;5,6,22,21;,
  4;6,7,23,22;,
  4;8,9,25,24;,
  4;9,10,26,25;,
  4;12,13,29,28;,
  4;13,14,30,29;,
  4;14,15,31,30;,
  4;16,1,17,32;,
  4;17,18,34,33;,
  4;19,20,36,35;,
  4;20,21,37,36;,
  4;21,22,38,37;,
  4;28,29,45,44;,
  4;29,30,46,45;,
  4;30,31,47,46;,
  4;32,17,33,48;,
  4;37,38,54,53;,
  4;38,39,55,54;,
  4;43,44,60,59;,
  4;44,45,61,60;,
  4;52,53,69,68;,
  4;53,54,70,69;,
  4;54,55,71,70;,
  4;59,60,76,75;,
  4;60,61,77,76;,
  4;61,62,78,77;,
  4;67,68,84,83;,
  4;68,69,85,84;,
  4;69,70,86,85;,
  4;70,71,87,86;,
  4;72,73,89,88;,
  4;73,74,90,89;,
  4;75,76,92,91;,
  4;76,77,93,92;,
  4;77,78,94,93;,
  4;78,79,95,94;,
  3;88,89,97;,
  3;89,90,97;,
  4;103,72,88,87;,
  4;91,90,74,104;,
  3;106,97,83;,
  3;107,97,106;,
  4;106,84,85,107;,
  3;108,97,107;,
  4;107,85,86,108;,
  3;87,97,108;,
  3;109,97,91;,
  3;110,97,109;,
  4;109,92,93,110;,
  3;111,97,110;,
  4;110,93,94,111;,
  3;95,97,111;,
  3;3,0,123;,
  4;124,5,4,123;,
  3;123,0,124;,
  4;125,6,5,124;,
  3;124,0,125;,
  3;125,0,7;,
  3;11,0,120;,
  4;121,13,12,120;,
  3;120,0,121;,
  4;122,14,13,121;,
  3;121,0,122;,
  3;122,0,15;,
  4;116,18,2,3;,
  4;7,8,24,117;,
  4;118,26,10,11;,
  4;15,16,32,119;,
  4;116,19,35,112;,
  4;115,47,31,119;,
  4;119,32,48,115;,
  4;112,34,18,116;,
  3;119,31,15;,
  3;3,19,116;,
  3;15,14,122;,
  3;0,16,15;,
  3;3,2,0;,
  3;123,4,3;,
  3;131,131,131;,
  3;11,10,0;,
  3;120,12,11;,
  3;7,6,125;,
  3;0,8,7;,
  3;111,94,95;,
  3;95,96,97;,
  3;97,82,83;,
  3;83,84,106;,
  3;87,71,103;,
  3;104,75,91;,
  3;108,86,87;,
  3;87,88,97;,
  3;97,90,91;,
  3;91,92,109;,
  4;99,56,72,103;,
  4;103,71,55,99;,
  4;100,59,75,104;,
  4;104,74,58,100;,
  4;113,99,55,39;,
  4;43,59,100,114;,
  3;126,82,81;,
  3;81,96,127;,
  4;102,83,82,126;,
  4;127,96,95,105;,
  3;95,79,105;,
  3;102,67,83;,
  4;79,63,101,105;,
  3;105,80,127;,
  4;102,98,51,67;,
  3;126,66,102;,
  4;127,80,65,81;,
  4;81,65,66,126;,
  3;97,96,81;,
  3;81,82,97;,
  4;65,49,50,66;,
  4;56,40,41,57;,
  4;56,99,113,40;,
  4;128,73,72,56;,
  4;42,114,100,58;,
  3;58,132,57;,
  4;57,41,42,58;,
  4;133,58,74,73;,
  3;56,57,132;,
  4;50,98,102,66;,
  4;80,105,101,64;,
  4;64,49,65,80;,
  4;129,113,39,23;,
  4;39,38,22,23;,
  3;23,117,129;,
  4;27,28,44,43;,
  3;129,117,24;,
  4;118,114,42,26;,
  4;42,41,25,26;,
  4;40,113,129,24;,
  4;24,25,41,40;,
  4;11,12,28,27;,
  3;118,11,27;,
  4;27,43,114,118;,
  3;68,67,52;,
  3;67,51,52;,
  3;130,130,130;,
  3;78,62,79;,
  3;79,62,63;,
  3;130,130,130;,
  4;37,53,52,36;,
  4;36,52,51,35;,
  4;45,46,62,61;,
  4;46,47,63,62;,
  4;47,115,101,63;,
  4;115,48,64,101;,
  4;48,33,49,64;,
  4;33,34,50,49;,
  4;34,112,98,50;,
  4;112,35,51,98;;
 }
 MeshTextureCoords {
  132;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
 MeshVertexColors {
  132;
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
  103;1.000000;1.000000;1.000000;1.000000;,
  104;1.000000;1.000000;1.000000;1.000000;,
  105;1.000000;1.000000;1.000000;1.000000;,
  106;1.000000;1.000000;1.000000;1.000000;,
  107;1.000000;1.000000;1.000000;1.000000;,
  108;1.000000;1.000000;1.000000;1.000000;,
  109;1.000000;1.000000;1.000000;1.000000;,
  110;1.000000;1.000000;1.000000;1.000000;,
  111;1.000000;1.000000;1.000000;1.000000;,
  112;1.000000;1.000000;1.000000;1.000000;,
  113;1.000000;1.000000;1.000000;1.000000;,
  114;1.000000;1.000000;1.000000;1.000000;,
  115;1.000000;1.000000;1.000000;1.000000;,
  116;1.000000;1.000000;1.000000;1.000000;,
  117;1.000000;1.000000;1.000000;1.000000;,
  118;1.000000;1.000000;1.000000;1.000000;,
  119;1.000000;1.000000;1.000000;1.000000;,
  120;1.000000;1.000000;1.000000;1.000000;,
  121;1.000000;1.000000;1.000000;1.000000;,
  122;1.000000;1.000000;1.000000;1.000000;,
  123;1.000000;1.000000;1.000000;1.000000;,
  124;1.000000;1.000000;1.000000;1.000000;,
  125;1.000000;1.000000;1.000000;1.000000;,
  126;1.000000;1.000000;1.000000;1.000000;,
  127;1.000000;1.000000;1.000000;1.000000;,
  128;1.000000;1.000000;1.000000;1.000000;,
  129;1.000000;1.000000;1.000000;1.000000;,
  130;1.000000;1.000000;1.000000;1.000000;,
  131;1.000000;1.000000;1.000000;1.000000;;
 }
}