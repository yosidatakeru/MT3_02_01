#include <Novice.h>
#include <Function.h>
#include <Vector3.h>
#include <cstdint>
#include <imgui.h>

const char kWindowTitle[] = "LE2B_29_ヨシダ_タケル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };



	//Affine
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };

	//カメラ
	//Vector3 cameraPosition = { 0.0f,0.0f,-1.0f };
	Vector3 cameraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };


	//ローカル
	Vector3 LocalVertics[2] = {};
	LocalVertics[0] = { -0.2f,0.0f,0.0f };
	LocalVertics[1] = { 0.2f,0.0f,0.0f };



	unsigned int sphrecolor = WHITE;
	Sphere sphere1LocalCoodinate = { {0.0f,0.0f,0.0f},0.2f };
	Sphere sphere2LocalCoodinate = { {1.0f,0.0f,0.5f},0.3f };


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///


		//レンダリングパイプライン(グラフィックスパイプライン)の流れ
		//      
		//ローカル座標系
		//      ↓
		//ワールド座標系
		//      ↓
		//ビュー座標系
		//      ↓
		//正規化デバイス座標系
		//      ↓
		//スクリーン座標系



		//計算
		Matrix4x4 cameraMatrix = MakeAffineMatrix(scale, cameraRotate, cameraTranslate);
		////ビュー(カメラ)
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		//射影
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WINDOW_SIZE_WIDTH) / float(WINDOW_SIZE_HEIGHT), 0.1f, 100.0f);


		//ビューポート
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(WINDOW_SIZE_WIDTH), float(WINDOW_SIZE_HEIGHT), 0.0f, 1.0f);



		if (IsCollision(sphere1LocalCoodinate, sphere2LocalCoodinate) == true) {
			sphrecolor = RED;
		}
		else {
			sphrecolor = WHITE;;
		}


		///
		/// ↑更新処理ここまで
		///




		///
		/// ↓描画処理ここから
		///


		//Grid
		DrawGrid(viewMatrix, projectionMatrix, viewportMatrix);




		//Proj
		//Localを入れるよ
		DrawSphere({ sphere1LocalCoodinate.center,sphere1LocalCoodinate.radius }, viewMatrix, projectionMatrix, viewportMatrix, sphrecolor);
		DrawSphere({ sphere2LocalCoodinate.center,sphere2LocalCoodinate.radius }, viewMatrix, projectionMatrix, viewportMatrix, sphrecolor);

		ImGui::Begin("Sphere");
		ImGui::DragFloat3("Sphere1", &sphere1LocalCoodinate.center.x, 0.01f);
		ImGui::DragFloat("Sphere1Radius", &sphere1LocalCoodinate.radius, 0.01f);
		ImGui::SliderFloat3("Sphere1Center", &sphere1LocalCoodinate.center.x, -2.0f, 2.0f);
		ImGui::SliderFloat("Sphere1Radius", &sphere1LocalCoodinate.radius, 0.0f, 0.5f);


		ImGui::DragFloat3("Sphere2", &sphere2LocalCoodinate.center.x, 0.01f);
		ImGui::DragFloat("Sphere2Radius", &sphere2LocalCoodinate.radius, 0.01f);
		ImGui::SliderFloat3("Sphere2Coodenate", &sphere2LocalCoodinate.center.x, -2.0f, 2.0f);
		ImGui::SliderFloat("Sphere2Radius", &sphere2LocalCoodinate.radius, 0.0f, 0.5f);


		ImGui::End();




		ImGui::Begin("Camera");
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);

		ImGui::End();





		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}