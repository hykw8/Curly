/*!
@file StringComponents.h
@brief ������`��R���|�[�l���g
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	�p�r: StringSprite�R���|�[�l���g
	//	������\���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~StringSprite();
		//�A�N�Z�T
		enum TextAlignment{
			m_Left,		//�����킹
			m_Center,	//�������킹
			m_Right		//�E���킹
		};
		//�A�N�Z�T
		void SetFont(const wstring& FontName, float FontSize);

		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		const wstring& GetText() const;
		void SetText(const wstring& str);
		void AddText(const wstring& str);


		const bsm::Col4& GetFontColor() const;
		void SetFontColor(const bsm::Col4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const bsm::Col4& GetBackColor() const;
		void SetBackColor(const bsm::Col4& Col);

		Point2D<float> GetBackTextMargin() const;
		void SetBackTextMargin(Point2D<float> p);

		float GetTextBlockWidth() const;
		void SetTextBlockWidth(float f);
		float GetTextBlockHeight() const;
		void SetTextBlockHeight(float f);

		ComPtr<IDWriteTextLayout>& GetTextLayout()const;
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics() const;

		const Point2D<float>& GetStartPosition() const;
		void SetStartPosition(const Point2D<float>& pos);

		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		//����
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}

//end basecross
