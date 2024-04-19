#include <Novice.h>
#include <cmath>

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 resultMultiply = {};

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			resultMultiply.m[y][x] = m1.m[y][0] * m2.m[0][x] + m1.m[y][1] * m2.m[1][x] + m1.m[y][2] * m2.m[2][x] + m1.m[y][3] * m2.m[3][x];
		}
	}

	return resultMultiply;
}

// 3次元回転行列
Matrix4x4 MakeRotateMatrix(Vector3 radian) {

	Matrix4x4 rotateX = { 0.0f };

	rotateX.m[0][0] = 1.0f;
	rotateX.m[1][1] = cosf(radian.x);
	rotateX.m[1][2] = sinf(radian.x);
	rotateX.m[2][1] = -sinf(radian.x);
	rotateX.m[2][2] = cosf(radian.x);
	rotateX.m[3][3] = 1.0f;

	Matrix4x4 rotateY = { 0.0f };

	rotateY.m[0][0] = cosf(radian.y);
	rotateY.m[0][2] = -sinf(radian.y);
	rotateY.m[1][1] = 1.0f;
	rotateY.m[2][0] = sinf(radian.y);
	rotateY.m[2][2] = cosf(radian.y);
	rotateY.m[3][3] = 1.0f;

	Matrix4x4 rotateZ = { 0.0f };

	rotateZ.m[0][0] = cosf(radian.z);
	rotateZ.m[0][1] = sinf(radian.z);
	rotateZ.m[1][0] = -sinf(radian.z);
	rotateZ.m[1][1] = cosf(radian.z);
	rotateZ.m[2][2] = 1.0f;
	rotateZ.m[3][3] = 1.0f;

	Matrix4x4 resultRotate = { 0.0f };

	resultRotate = Multiply(rotateX, Multiply(rotateY, rotateZ));

	return resultRotate;
}

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 rotateMatrix = { 0.0f };

	rotateMatrix = MakeRotateMatrix(rotate);

	Matrix4x4 resultAffine = { 0.0f };

	resultAffine.m[0][0] = scale.x * rotateMatrix.m[0][0];
	resultAffine.m[0][1] = scale.x * rotateMatrix.m[0][1];
	resultAffine.m[0][2] = scale.x * rotateMatrix.m[0][2];
	resultAffine.m[1][0] = scale.y * rotateMatrix.m[1][0];
	resultAffine.m[1][1] = scale.y * rotateMatrix.m[1][1];
	resultAffine.m[1][2] = scale.y * rotateMatrix.m[1][2];
	resultAffine.m[2][0] = scale.z * rotateMatrix.m[2][0];
	resultAffine.m[2][1] = scale.z * rotateMatrix.m[2][1];
	resultAffine.m[2][2] = scale.z * rotateMatrix.m[2][2];

	resultAffine.m[3][0] = translate.x;
	resultAffine.m[3][1] = translate.y;
	resultAffine.m[3][2] = translate.z;
	resultAffine.m[3][3] = 1.0f;

	return resultAffine;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

// 4x4行列の数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x, y, " %s", label);
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

const char kWindowTitle[] = "LE2B_01_アキモト_ワタル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 scale = { 1.2f,0.79f,-2.1f };
	Vector3 rotate = { 0.4f,1.43f,-0.8f };
	Vector3 translate = { 2.7f,-4.15f,1.57f };

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, worldMatrix, "worldMatrix");

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
