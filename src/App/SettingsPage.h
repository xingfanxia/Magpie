#pragma once

#include "Settings.h"
#include "SettingsPage.g.h"

namespace winrt::Magpie::implementation {

struct SettingsPage : SettingsPageT<SettingsPage> {
	SettingsPage();

	void ThemeComboBox_SelectionChanged(IInspectable const&, Controls::SelectionChangedEventArgs const& args);

	void PortableModeToggleSwitch_Toggled(IInspectable const&, RoutedEventArgs const&);

	void ComboBox_DropDownOpened(IInspectable const&, IInspectable const&);

private:
	Magpie::Settings _settings{ nullptr };
};

}

namespace winrt::Magpie::factory_implementation {

struct SettingsPage : SettingsPageT<SettingsPage, implementation::SettingsPage> {
};

}
