#pragma once

#include "SettingItem.g.h"


namespace winrt::Magpie::implementation
{
	struct SettingItem : SettingItem_base<SettingItem>
	{
		SettingItem();

		void Title(const hstring& value);

		hstring Title() const;

		void Description(Windows::Foundation::IInspectable value);

		Windows::Foundation::IInspectable Description() const;

		void Icon(Windows::Foundation::IInspectable value);

		Windows::Foundation::IInspectable Icon() const;

		void ActionContent(Windows::Foundation::IInspectable value);

		Windows::Foundation::IInspectable ActionContent() const;

		void OnApplyTemplate();

		static Windows::UI::Xaml::DependencyProperty TitleProperty;
		static Windows::UI::Xaml::DependencyProperty DescriptionProperty;
		static Windows::UI::Xaml::DependencyProperty IconProperty;
		static Windows::UI::Xaml::DependencyProperty ActionContentProperty;

	private:
		static void _OnPropertyChanged(Windows::UI::Xaml::DependencyObject const& sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs const&);

		void _Setting_IsEnabledChanged(Windows::Foundation::IInspectable const&, Windows::UI::Xaml::DependencyPropertyChangedEventArgs const&);

		void _Update();

		void _SetEnabledState();

		Windows::UI::Xaml::Controls::ContentPresenter _iconPresenter{ nullptr };
		Windows::UI::Xaml::Controls::ContentPresenter _descriptionPresenter{ nullptr };

		winrt::event_token _isEnabledChangedToken{};

		static constexpr const wchar_t* _PartIconPresenter = L"IconPresenter";
		static constexpr const wchar_t* _PartDescriptionPresenter = L"DescriptionPresenter";
	};
}

namespace winrt::Magpie::factory_implementation
{
	struct SettingItem : SettingItemT<SettingItem, implementation::SettingItem>
	{
	};
}
