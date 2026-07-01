#include "DirectX2DManager.h"
#include "DirectX3DManager.h"

#pragma comment(lib, "d2d1.lib") 
#pragma comment(lib, "dwrite.lib")

namespace DirectX2DManager {

	inline ID2D1Factory* factory = nullptr;
	inline IDWriteFactory* writeFactory = nullptr;
	inline ID2D1RenderTarget* renderTarget = nullptr;

	inline const WCHAR* DEFAULT_FONT = L"メイリオ";
	inline int DEFAULT_FONT_SIZE = 30;
	inline D2D1_COLOR_F DEFAULT_COLOR = D2D1::ColorF(0.0, 0.0, 0.0, 1.0);
	IDWriteTextFormat* textFormat = nullptr;
	ID2D1SolidColorBrush* brush = nullptr;

	using namespace DirectX3DManager;

	void Init() {

		//初期処理
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown**>(&writeFactory));
		IDXGISurface* surface = nullptr;
		hr = GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&surface));
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_UNKNOWN,
				D2D1_ALPHA_MODE_IGNORE
			)
		);
		hr = factory->CreateDxgiSurfaceRenderTarget(surface, &props, &renderTarget); 
		surface->Release();

		//フォント処理
		writeFactory->CreateTextFormat(
			DEFAULT_FONT,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			DEFAULT_FONT_SIZE,
			L"",
			&textFormat
		);

		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		renderTarget->CreateSolidColorBrush(DEFAULT_COLOR, &brush);
	}

	void DrawFontText(int x, int y, std::wstring text) {
		D2D1_SIZE_F size = renderTarget->GetSize();
		renderTarget->BeginDraw();
		D2D1_MATRIX_3X2_F transform_ = D2D1::Matrix3x2F::Scale(1.0, 1.0) * D2D1::Matrix3x2F::Translation(x, y);
		renderTarget->SetTransform(transform_);

		renderTarget->DrawTextA(
			text.c_str(),
			text.size(),
			textFormat,
			D2D1::RectF(x, y, size.width, size.height),
			brush
		);

		renderTarget->EndDraw();
	}
}