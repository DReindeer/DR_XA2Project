# DR_XA2Project
<h1>Let's enjoy a SoundProgramming ↑↑</h1>

<h2>[導入過程]</h2>
<ol>
<li><b>XAudio2を導入</b></li>
<li>リソースとソースボイスを分ける</li>
<li>ソースボイスを2D,3Dで分ける</li>
<li>waveをcpu全のせ,Streaming再生で分ける</li>
<li>Streaming再生を別スレッドで行う機構を作成</li>
<li>同じSEを複数同時に鳴らせるようにする</li>
<li>不要なSEを消去する機構を作成</li>
<li><b>X3DAudioを導入</b></li>
<li>Streaming再生のポーリング処理を修正</li>
<li>ogg→waveへのデコード</li>
<li>3D音源再生時の自動モノラル変換           ← 今ココ！</li>
</ol>

<h2>[発生中のバグ報告など]</h2>
<ul>
<li>特になし</li>
</ul>

<h2>[目的]</h2>
<ul>
<li>サウンドプログラミングの知識を深める</li>
</ul>

<h2>[サウンド系API]</h2>
<ul>
<li>XAudio2</li>
<li>X3DAudio</li>
<li>XAPO</li>
</ul>

<h2>[ライブラリ]</h2>
<ul>
<li>DirectX11 --- 計算ライブラリはDirectXMath</li>
<li>DirectXTex</li>
<li>libogg_static, libvorbis_static, libvorbisfile_static(oggデコード用ライブラリ)</li>
</ul>

<h2>[言語]</h2>
<ul>
<li>c++</li>
</ul>
